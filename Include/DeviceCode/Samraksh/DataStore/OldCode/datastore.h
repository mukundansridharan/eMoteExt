#ifndef _DATASTORE_H_INCLUDED_
#define _DATASTORE_H_INCLUDED_


/* Intermodule includes */
#include "flash_device.h"

/* Include intra-module headers */
#include "types.h"
#include "datastore_int.h"
#include "datastore_reg.h"
#include "address_table.h"

#define DEBUG_DATASTORE

#ifdef DEBUG_DATASTORE
#define PRINT_DEBUG(x) hal_printf(x)
#else
#define PRINT_DEBUG(x)
#endif


/* Constants */
/* Threshold that needs to be always maintained*/
#define MIN_SPACE_REQUIRED_FOR_COMPACTION	    (1024 * 128)

#define ENABLE_TEST

/* Defines where the data-store begins in the flash */
#define DATA_STORE_OFFSET                       (8763)

/* Default properties of device */
static DATASTORE_CLUSTER cluster[] = { { 512,        1024  },
                                       { 4*1024,     256 },
                                       { 10*1024,    128 }};

static DATASTORE_PROPERTIES defaultProperty = { cluster,
                                                sizeof(cluster)/sizeof(DATASTORE_CLUSTER),
                                                (LPVOID)0x1,
                                                (LPVOID)(((uint32)~0)>>2)
                                              };

/* Error values */
typedef enum _datastore_errors
{
    DATASTORE_ERROR_NONE,
    DATASTORE_ERROR_INVALID_PARAM,
    DATASTORE_ERROR_OUT_OF_MEMORY,
    DATASTORE_ERROR_INVALID_RECORD_ID,
    DATASTORE_ERROR_INVALID_GIVEN_ADDR,
    DATASTORE_ERROR_OUT_OF_BOUND_ACCESS,
    DATASTORE_ERROR_OUT_OF_FLASH_MEMORY,
    DATASTORE_ERROR_RECORD_ID_ALREADY_EXISTS,
    DATASTORE_ERROR_UNEXPECTED_ERROR
}DATASTORE_ERROR;


/* Data store class */
template<class T>
class Data_Store
{
private:
    /* Flash device */
    flash_device<T> flashDevice;


    /* Current state of the device */
    DATASTORE_STATE state;
    /* Last error status - Stores any error that occured in previous API
       This value can be read by the Application using getLastErrorValue()
    */
    DATASTORE_ERROR lastErrorVal;

    /* This is to find the log, error and erase pointers */
#if ENABLE_PERSISTENCE
    PERSISTENCE_DIRECTION per_dir;
#endif
    /* Internal pointers that maintain the circular buffer locations */
    uint32 logPointByteOffset;
    uint32 erasePointByteOffset;
    uint32 clearLogPointByOffset;   /* Tells where the next record that can be checked for clearing is */

    /* Offsets telling start and end of flash data region - For ease of use */
    uint32 dataStoreStartByteOffset;
    uint32 dataStoreEndByteOffset;

    /* Address translation table */
    class DATASTORE_AddrTable addressTable;

    /* Unique Pointer generator */
    uniquePtr myUniquePtrGen;

    /* Offset where clearPtrOffset needs to be written for persistance */
       /* Not sure if its a good idea to abstract out this as a separate small class
          or keep this info here like this! - Chethan */
    UINT32 hiddenClearPtrOffsetStore;

    /* Private methods */
    /* Locks */
    DATASTORE_STATUS lockDataStore();
    DATASTORE_STATUS unlockDataStore();

    /* PRIVATE FUNCTIONS */
    DATASTORE_STATUS initDataStore( char *datastoreName,
                                    DATASTORE_PROPERTIES *property );
    /* Compact function */
    DATASTORE_STATUS compactLog();

    /* Helper function to increment log-pointer cyclically */
    void incrementLogPointer(uint32 amount);

    /* Helper function to increment erase-point cyclically */
    void incrementErasePoint(uint32 amount);

    /* Helper function to increment erase-point cyclically */
    void incrementClearPoint(uint32 amount);

    /* Helper function to increment pointers so that it doesn't run out of circular buffer */
    LPVOID incrementPointer(LPVOID inputPtr, int amount);

    /* Helper function to increment pointers so that it doesn't run out of circular buffer */
    int calculateNumBytes(LPVOID fromAddr, LPVOID toAddr);

    /* Helper function to calculate Log-head room */
    uint32 calculateLogHeadRoom();

    /* Create new allocation */
    LPVOID createAllocation( RECORD_ID recordID, LPVOID givenPtr, uint32 numBytes);

    /* Detect overwrites - Not sure if this needs to be a member function */
    bool detectOverWrite( void *addr, void *data, int dataLen, uint32* conflictStartLoc );

    /* Reads record header from the flash handling cyclic copy */
    uint32 cyclicDataRead( LPVOID buff,
                           LPVOID currentLoc,
                           uint32 numBytes );

    /* Reads record header from the flash handling cyclic copy */
    uint32 cyclicDataWrite( LPVOID buff,
                            LPVOID currentLoc,
                            uint32 numBytes );


    /* Function that stores the recent info of the clearLogPointByOffset in hidden region */
    DATASTORE_STATUS storeClearPtrOffset();

    /* Helper function for scanFLashDevice - used for getting the erase, log and clear pointers */
    LPVOID traversePointer(PERSISTENCE_DIRECTION per_dir, char* address);

    	/* Helper function for scanFLashDevice - used for scanning through the sectors */
    LPVOID readPointers();

   	/* Helper function for scanFLashDevice - used for scanning through the sectors */
   	int readRecordinBlock(int blockID);

    /* Helper function for scanFlashDevice */
    uint32 decideWhichOneisRecent(RECORD_HEADER tempHeader, RECORD_HEADER header);

    /* Fetches the the start point for scan */
    uint32 returnScanAddress();

    /* Scan the flash device and load all the records in the Flash Device
       Could use this at bootup to load all the records in the flash */
    LPVOID scanFlashDevice();

    /* create a dummy filler allocation in the datastore to make sure that records
           don't overlap between sectors */
    bool createDummyAllocation(int nextAllocationSize);

    //static Data_Store<T> instance;

    //static BOOL instanceFlag;
    static BOOL initialized;

public:
    // privatising constructors to ensure there is only one instance of data store in the
    // system
    Data_Store()
    {
    	Data_Store("NOR");
    }

    /* Helper function for the addressTable. Gets a table, copies it to a new table and returns it */
    	//LPVOID copyAddressTable(vector<DATASTORE_ADDR_TBL_ENTRY> *table);
    	LPVOID copyAddressTable(myVector *table);


        /* Load with default settings */
     Data_Store( char *flashDeviceName );

          /* Create a Datastore with specific property - Only used with emulator */
     Data_Store( char *flashDeviceName,
                      DATASTORE_PROPERTIES *property );


     void init()
     {
    	 if(initialized == TRUE)
    		 return;

    	 myUniquePtrGen.init();
    	 flashDevice.init();

    	 state = DATASTORE_STATE_UNINIT;
    	 DATASTORE_STATUS status;

    	 lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
    	 status = initDataStore( "NOR",
    	                             &defaultProperty );

    	 state = (status == DATASTORE_STATUS_OK)?(DATASTORE_STATE_READY):(DATASTORE_STATE_INT_ERROR);
    	 initialized = TRUE;
     }
	//static Data_Store<T> getInstance()
	//{
	//	return instance;
	//}

#ifdef ENABLE_PERSISTENCE
	/* Helper function for the addressTable. Gets a table, copies it to a new table and returns it */
	LPVOID copyAddressTable(vector<DATASTORE_ADDR_TBL_ENTRY> *table);
#endif
    /* Create a record in the data store */
    LPVOID createRecord( RECORD_ID id, uint32 numBytes );

    /* Delete record */
    DATASTORE_STATUS deleteRecord( RECORD_ID id );

    /* Get base address for given Record_id */
    LPVOID getAddress( RECORD_ID id );

    /* Symmetric API to lookup ID using PTR */
    RECORD_ID getRecordID(LPVOID givenPtr);

    /* Write data to the store */
    uint32 writeData( LPVOID dest, T *data, uint32 count );

    /* Write raw data - Can be of any word-size */
    uint32 writeRawData(LPVOID dest, void* data, uint32 numBytes);

    uint32 readData(LPVOID src, T* data, uint32 count);

    /* Read raw data */
    uint32 readRawData(LPVOID src, void *data, uint32 numBytes);

    /* Returns the error code of any error in the previous call */
    DATASTORE_ERROR getLastError();


	/* Function that returns the current value of the Log point */
	uint32 returnLogPoint();

	/* Function that returns the current value of the Clear point */
	uint32 returnClearPoint();

	/* Function that returns the current value of the Erase point */
	uint32 returnErasePoint();

    /* Destructor */
    ~Data_Store();
};

/*
template<class T>
Data_Store<T> Data_Store<T>::instance("NOR");
*/
template<class T>
BOOL Data_Store<T>::initialized = FALSE;


/* Helper function for the addressTable. Gets a table, copies it to a new table and returns it */
template<class T>
LPVOID Data_Store<T>::copyAddressTable(myVector *table)
{
	addressTable.copyEntry(table);
	return NULL;
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
void Data_Store<T>::incrementLogPointer( uint32 amount )
{
    logPointByteOffset += amount;
    if(logPointByteOffset > dataStoreEndByteOffset){
        /* Going out of the range - correct it */
        logPointByteOffset = (logPointByteOffset - dataStoreEndByteOffset) + dataStoreStartByteOffset - 1;
    }
    DATASTORE_ASSERT( (logPointByteOffset >= dataStoreStartByteOffset && logPointByteOffset <= dataStoreEndByteOffset),
                      "logPointByteOffset went out of range!" );
    if((logPointByteOffset < dataStoreStartByteOffset ||
        logPointByteOffset > dataStoreEndByteOffset)){
        assert(false);
    }
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
void Data_Store<T>::incrementErasePoint(uint32 amount)
{
    erasePointByteOffset += amount;
    if(erasePointByteOffset > dataStoreEndByteOffset){
        /* Going out of the range - correct it */
        erasePointByteOffset = (erasePointByteOffset - dataStoreEndByteOffset) + dataStoreStartByteOffset - 1;
    }
    DATASTORE_ASSERT( (erasePointByteOffset >= dataStoreStartByteOffset && erasePointByteOffset <= dataStoreEndByteOffset),
                      "erasePointByteOffset went out of range!" );
    if((erasePointByteOffset < dataStoreStartByteOffset ||
        erasePointByteOffset > dataStoreEndByteOffset)){
        assert(false);
    }
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
void Data_Store<T>::incrementClearPoint(uint32 amount)
{
    clearLogPointByOffset += amount;
    if(clearLogPointByOffset > dataStoreEndByteOffset){
        /* Going out of the range - correct it */
        clearLogPointByOffset = (clearLogPointByOffset - dataStoreEndByteOffset) + dataStoreStartByteOffset - 1;
    }
    DATASTORE_ASSERT( (clearLogPointByOffset >= dataStoreStartByteOffset && clearLogPointByOffset <= dataStoreEndByteOffset),
                      "erasePointByteOffset went out of range!" );
    if((clearLogPointByOffset < dataStoreStartByteOffset ||
        clearLogPointByOffset > dataStoreEndByteOffset)){
        assert(false);
    }
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
LPVOID Data_Store<T>::incrementPointer(LPVOID inputPtr, int numBytes)
{
    char *lPtr = (char*)inputPtr;
    char *lDataStoreEndPtr = (char*)flashDevice.getDeviceBaseAddress() + dataStoreEndByteOffset;
    char *lDataStoreStartPtr = (char*)flashDevice.getDeviceBaseAddress()+dataStoreStartByteOffset;

    lPtr = lPtr + numBytes;
    if(lPtr > lDataStoreEndPtr){
        lPtr = lDataStoreStartPtr + (lPtr - lDataStoreEndPtr - 1);
    }else if(lPtr < lDataStoreStartPtr){
        lPtr = lDataStoreEndPtr - (lDataStoreStartPtr - lPtr - 1);
    }
    return lPtr;
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
/* Inclusive of pointers passed - So, subtract 1 from the value if appropriate */
template<class T>
int Data_Store<T>::calculateNumBytes(LPVOID fromAddr, LPVOID toAddr)
{
    int retVal = 0;

    DATASTORE_ASSERT( (NULL != fromAddr && NULL != toAddr),
                       "Invalid parameters being passed!" );
    do{
        if( NULL == fromAddr ||
            NULL == toAddr ){
            retVal = 0;
            break;
        }

        if( fromAddr <= toAddr ){
            /* There is no wrapping of pointers - Direct substraction is enough */
            retVal = (int)((char*)toAddr - (char*)fromAddr) + 1;
        }else{
            char *startAddr = (char*)flashDevice.getDeviceBaseAddress() + dataStoreStartByteOffset ;
            char *endAddr   = (char*)flashDevice.getDeviceBaseAddress() + dataStoreEndByteOffset ;

            retVal = (int)((endAddr - (char*)fromAddr + 1) + ((char*)toAddr - startAddr + 1) );
        }
    }
    while(0);
    return retVal;
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
uint32 Data_Store<T>::calculateLogHeadRoom()
{
    uint32 retVal = 0;
    if(erasePointByteOffset > logPointByteOffset){
        /* The free space doesn't overlap between end and beginning of flash */
        retVal = erasePointByteOffset - logPointByteOffset + 1;
    }else{
        retVal = (dataStoreEndByteOffset - logPointByteOffset + 1) +
                    (erasePointByteOffset - dataStoreStartByteOffset + 1);
    }
	return retVal;
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
LPVOID Data_Store<T>::createAllocation( RECORD_ID recordID, LPVOID givenPtr, uint32 numBytes )
{
    LPVOID retVal = NULL;
    uint32 allocationSize = numBytes + sizeof(RECORD_HEADER);
    RECORD_HEADER header = { 0 };

    lastErrorVal = DATASTORE_ERROR_NONE;
    do{
    	/* Create a dummy/Skip allocation if required to make sure that this allocation
    	           doesn't fall in sectors -  return value tells if an allocation was done or not
    	           We don't care to know the status here */
	    createDummyAllocation(numBytes);

        if(calculateLogHeadRoom() < (MIN_SPACE_REQUIRED_FOR_COMPACTION + allocationSize)){
            /* Free memory is below the minimum threshold, try creating some free
               space by moving active records, clearing inactive hence compacting flash */
            int prevHeadRoom = 0;
            int currentHeadRoom = calculateLogHeadRoom();

            while(prevHeadRoom != currentHeadRoom){
            /* Run till we are making some free space on each iteration */
                compactLog();                               /* Clear some memory */
                prevHeadRoom = currentHeadRoom;
                currentHeadRoom = calculateLogHeadRoom();   /* Check free space available now */
                if(currentHeadRoom >= (MIN_SPACE_REQUIRED_FOR_COMPACTION+ allocationSize)){
                    /* Come out if we have enough space */
                    break;
                }
            }
        }
        if(calculateLogHeadRoom() < (MIN_SPACE_REQUIRED_FOR_COMPACTION + allocationSize)){
            /* If this is the case, its impossible to create more headRoom (is it?) */
            lastErrorVal = DATASTORE_ERROR_OUT_OF_FLASH_MEMORY;
            break;
        }

        /* Now that we have created enough free space, continue with the allocation */
        retVal = incrementPointer(flashDevice.getDeviceBaseAddress(), logPointByteOffset);
        incrementLogPointer(allocationSize);

        /* Now search for old allocation if any and mark them inactive - If this is the first record,
           then no harm done :)
           CHETHAN :: Should I pass a flag controlling whether this should be done or not, as in some
           cases its obviously unnecessary but still we would be doing it and lose some performance.
        */
        LPVOID oldRecAddr = addressTable.getCurrentLoc(recordID);         /* O(n) :( */
        if( NULL != oldRecAddr ){
            /* There is an old record already, mark it inactive */
            oldRecAddr = incrementPointer( oldRecAddr, -1*sizeof(RECORD_HEADER));
            cyclicDataRead( &header,
                            oldRecAddr,
                            sizeof(RECORD_HEADER) );

            header.activeFlag = FLAG_RECORD_INACTIVE;
            header.nextLink = incrementPointer(retVal, sizeof(RECORD_HEADER));

            cyclicDataWrite( &header,
                             oldRecAddr,
                             sizeof(RECORD_HEADER) );

            /* Now, update the header struct and write it back */
            header.activeFlag = FLAG_RECORD_ACTIVE;
            header.nextLink   = (LPVOID)~0;
            header.version++;                 /* Move to next value */
            header.zero       = 0;

            /* Writing at the end of the function */
            addressTable.updateCurrentLocation( recordID, incrementPointer(retVal, sizeof(RECORD_HEADER)));
        }else{
            /* This is the first time this record is being created - So init the header struct */
            DATASTORE_ADDR_TBL_ENTRY entry;

            entry.allocationSize = numBytes;
            entry.currentLoc     = (char*)incrementPointer(retVal , sizeof(RECORD_HEADER));
            entry.givenPtr       = givenPtr;
            entry.recordID       = recordID;

            header.activeFlag = FLAG_RECORD_ACTIVE;
            header.nextLink   = (LPVOID)~0;
            header.recordID   = recordID;
            header.size       = numBytes;
            header.version    = 0;
            header.zero       = 0;

            addressTable.addEntry(&entry);
        }
        /* Now, write the header to flash  */
        cyclicDataWrite( &header,
                         retVal,
                         sizeof(RECORD_HEADER) );
    }while(0);
    return retVal;
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
bool Data_Store<T>::detectOverWrite( void *addr, void *data, int dataLen, uint32 *conflictStartLoc )
{
    char *flashData = (char*)addr;
    char *givenData = (char*)data;
    bool retVal = false;

    *conflictStartLoc = dataLen;

    for(int index = 0; index < dataLen; index++){
        if((flashData[index] & givenData[index]) != givenData[index]){
            /* Overwrite detected */
            retVal = true;
            *conflictStartLoc = index;
            break;
        }
    }
    return retVal;
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
/* Get base address for given Record_id */
template <class T>
LPVOID Data_Store<T>::getAddress(RECORD_ID id)
{
    LPVOID retVal = NULL;
    lastErrorVal = DATASTORE_ERROR_NONE;

    if( NULL == (retVal = addressTable.getGivenAddress(id)) ){
        lastErrorVal = DATASTORE_ERROR_INVALID_RECORD_ID;
    }
    return retVal;
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
/* Get base address for given Record_id */
template <class T>
RECORD_ID Data_Store<T>::getRecordID(LPVOID givenPtr)
{
    return addressTable.getRecordID(givenPtr);
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
DATASTORE_STATUS Data_Store<T>::initDataStore( char *datastoreName,
                                               DATASTORE_PROPERTIES *property )
{
    DATASTORE_STATUS status = DATASTORE_STATUS_INVALID_PARAM;
    FLASH_PROPERTIES flashProp = { 0 };
    do{
        if( NULL == datastoreName ||
            NULL == property ){
            lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
            break;
        }
#ifdef EMULATOR_MODE
        /* Starting of Flash Emulator creation */
        /* Fill flash emulator structures and initialize the flash emulator */
        flashProp.numClusters = property->numClusters;
        flashProp.clusters = (BLOCK_CLUSTER *)malloc(sizeof(BLOCK_CLUSTER)*flashProp.numClusters);
        DATASTORE_ASSERT( flashProp.clusters != NULL,
                          "Failed to malloc memory for flashProp.clusters" );
        if( NULL == flashProp.clusters){
            lastErrorVal = DATASTORE_ERROR_OUT_OF_MEMORY;
            status = DATASTORE_STATUS_OUT_OF_MEM;
            break;
        }

        for(int index = 0; index < flashProp.numClusters; index++){
            flashProp.clusters[index].blockSize = property->clusters[index].blockSize;
            flashProp.clusters[index].numBlocks = property->clusters[index].numBlocks;
        }

        flashDevice = new flash_emulator<T>(datastoreName, &flashProp);

        DATASTORE_ASSERT( flashDevice != NULL,
                                  "Creating Flash Device, new failed!" );
                if( NULL == flashDevice ){
                    lastErrorVal = DATASTORE_ERROR_UNEXPECTED_ERROR;
                    status = DATASTORE_STATUS_INT_ERROR;
                    break;
        }
#else
        //flash_device<int> flashDevice(datastoreName);
        //this.flashDevice = flashDevice;
#endif


        DATASTORE_ASSERT( flashDevice.getDeviceState()== EMULATOR_STATE_READY,
                          "Created flash device not in ready state!" );
        if( FLASHDEVICE_STATE_READY != flashDevice.getDeviceState()){
            lastErrorVal = DATASTORE_ERROR_UNEXPECTED_ERROR;
            status       = DATASTORE_STATUS_INT_ERROR;
            break;
        }
        /* End of Flash Emulator creation */

        /* Initialize the Unique Pointer class */
        //myUniquePtrGen = new uniquePtr( property->addressRangeStart,
        //                                property->addressRangeEnd );
        /*
        DATASTORE_ASSERT( myUniquePtrGen != NULL,
                          "new failed to create uniquePtr Object" );
        if( NULL == myUniquePtrGen ){
            status       = DATASTORE_STATUS_OUT_OF_MEM;
            lastErrorVal = DATASTORE_ERROR_OUT_OF_MEMORY;
            break;
        }
        */

        /* Initialize the internal variables */
        int lDataStoreStartBlockID = 0;      /* Will calculate BlockID of first block of datastore */
        LPVOID lDataStoreStartPtr = NULL;
        /* This represents the block where the given offset starts */
        lDataStoreStartBlockID = flashDevice.getBlockIDFromAddress( (char*)flashDevice.getDeviceBaseAddress() +
                                                                     DATA_STORE_OFFSET );
        /* Datastore should start at the beginning of next block */
        lDataStoreStartBlockID = lDataStoreStartBlockID + 1;

        /* Now We have the address of the first block of datastore - Calculate byte offset from the
           beginning of the flash */
        lDataStoreStartPtr = flashDevice.getBlockAddressFromBlockID(lDataStoreStartBlockID);

        dataStoreStartByteOffset = (uint32)((char*)lDataStoreStartPtr - (char*)flashDevice.getDeviceBaseAddress());
        dataStoreEndByteOffset   = flashDevice.getDeviceSize() - 1;

        /* Now, that the datastore is ready, we need to register it with the global
           registration table which is used for address translations later */
        DATASTORE_REG_ENTRY regEntry;
        regEntry.addrRangeStart  = (LPVOID)property->addressRangeStart;
        regEntry.addrRangeEnd    = (LPVOID)property->addressRangeEnd;
        regEntry.dataStoreHandle = (LPVOID)this;

        status = DATASTORE_REG_CreateEntry(&regEntry);
        DATASTORE_ASSERT( DATASTORE_STATUS_OK == status,
                          "Registration Failed!" );
        if(DATASTORE_STATUS_OK != status){
            lastErrorVal = DATASTORE_ERROR_UNEXPECTED_ERROR;
            status       = DATASTORE_STATUS_INT_ERROR;
            break;
        }

        /* Now, scan the device for previously stored records and this should update
           my logPtr, erasePtr, cleanPtr */
        scanFlashDevice();

        lastErrorVal = DATASTORE_ERROR_NONE;
        status       = DATASTORE_STATUS_OK;
        state        = DATASTORE_STATE_READY;
    }while(0);
    return status;
}

/****************************************************************************
*
*  Function Name : cyclicDataRead
*
******************************************************************************/
/*!
*  \brief    Reads data from the flash
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
bool Data_Store<T>::createDummyAllocation(int nextAllocationSize)
{
    char *logPtr     = NULL;
    int  currBlockID = 0;
    char *currBlockStartAddr = NULL;
    char *currBlockEndAddr   = NULL;
    int byteLeftInBlock = 0;

    bool retVal = false;

    /* Add the record header as this is the actual size of the allocation */
    nextAllocationSize = nextAllocationSize + sizeof(RECORD_HEADER);


    logPtr = (char*)flashDevice.getDeviceBaseAddress() + logPointByteOffset;

    currBlockID = flashDevice.getBlockIDFromAddress(logPtr);
    currBlockStartAddr = (char*)flashDevice.getBlockAddressFromBlockID(currBlockID);
    currBlockEndAddr   = currBlockStartAddr + flashDevice.getBlockSize(currBlockID) - 1;

    byteLeftInBlock = currBlockEndAddr - logPtr + 1;
    if(byteLeftInBlock == 0){
        /* I am sure this is impossible - Chethan */
        retVal = false;
    }
    else if(byteLeftInBlock < nextAllocationSize){
        /* This particular sector has less than required number of bytes, hence we
           need to fill this by a filler allocation using SKIP_TO_NEXT_SECTOR_FLAG */
		unsigned char lflag = SKIP_TO_NEXT_SECTOR_FLAG;

		flashDevice.writeRawData( &lflag, 1, logPtr);
        incrementLogPointer(byteLeftInBlock);   /* Now move to the next sector */
        retVal = true;
    }else{
        /* Nothing to be done I guess */
        retVal = false;
    }
    return retVal;
}



/****************************************************************************
*
*  Function Name : cyclicDataRead
*
******************************************************************************/
/*!
*  \brief    Reads data from the flash
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
uint32 Data_Store<T>::cyclicDataRead( LPVOID buff,
                                      LPVOID currentLoc,
                                      uint32 numBytes )
{
    uint32 retVal = numBytes;
    uint32 offset = 0;

    if( NULL == currentLoc ||
        NULL == buff ||
        (char*)currentLoc < (char*)flashDevice.getDeviceBaseAddress() ||
        numBytes > flashDevice.getDeviceSize() ){

        DATASTORE_ASSERT( false, "Error!!!");
        assert(false);  //This should never happen!
        //return 0;
    }

    offset = (uint32)((char*)currentLoc - (char*)flashDevice.getDeviceBaseAddress());
    do{
        DATASTORE_ASSERT( ((offset >= dataStoreStartByteOffset) && (offset <= dataStoreEndByteOffset)),
                           "Trying to read out of bound region!"  );

        if( offset < dataStoreStartByteOffset ||
            offset > dataStoreEndByteOffset ){
            /* Trying to read out of bound region */
            retVal       = 0;
            lastErrorVal = DATASTORE_ERROR_OUT_OF_BOUND_ACCESS;
            break;
        }

        if(offset + numBytes <= dataStoreEndByteOffset){

            /* Common case - Just read */

            memcpy( buff,
                    currentLoc,
                    numBytes );

        }else{
            /* Need to read in pieces */
            int byteTillEnd = dataStoreEndByteOffset - offset + 1;


            /* Copy first piece */
            memcpy( buff,
                    currentLoc,
                    byteTillEnd );

            /* Copy second piece */
            memcpy( (char*)buff+byteTillEnd,
                    (char*)flashDevice.getDeviceBaseAddress()+dataStoreStartByteOffset,
                    numBytes - byteTillEnd );
        }
    }while(0);
    return retVal;
}


/****************************************************************************
*
*  Function Name : cyclicDataRead
*
******************************************************************************/
/*!
*  \brief    Reads data from the flash
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
uint32 Data_Store<T>::cyclicDataWrite( LPVOID buff,
                                       LPVOID currentLoc,
                                       uint32 numBytes )
{
    uint32 retVal = numBytes;
    uint32 offset = 0;

    if(numBytes == 0)
    	return 0;
    //LPVOID targetLoc = (char*)flashDevice.getDeviceBaseAddress()+4156;

    if( NULL == currentLoc ||
        NULL == buff ||
        (char*)currentLoc < (char*)flashDevice.getDeviceBaseAddress() ||
        numBytes > flashDevice.getDeviceSize() ){

        DATASTORE_ASSERT( false, "Error!!!");
        assert(false);  //This should never happen!
        //return 0;
    }
    offset = (uint32)((char*)currentLoc - (char*)flashDevice.getDeviceBaseAddress());
    do{
        DATASTORE_ASSERT( ((offset >= dataStoreStartByteOffset) && (offset <= dataStoreEndByteOffset)),
                           "Trying to Write out of bound region!"  );

        if( offset < dataStoreStartByteOffset ||
            offset > dataStoreEndByteOffset ){
            /* Trying to read out of bound region */
            retVal       = 0;
            lastErrorVal = DATASTORE_ERROR_OUT_OF_BOUND_ACCESS;
            break;
        }

        if(offset + numBytes <= dataStoreEndByteOffset){
            /* Common case - Just Write */
            flashDevice.writeRawData( buff,
                                       numBytes,
                                       currentLoc );
        }else{
            /* Need to read in pieces */
            int byteTillEnd = dataStoreEndByteOffset - offset + 1;

            /* Copy first piece */
            flashDevice.writeRawData( buff,
                                       byteTillEnd,
                                       currentLoc );
            /* Copy second piece */
            flashDevice.writeRawData( (char*)buff+byteTillEnd,
                                       numBytes - byteTillEnd,
                                       (char*)flashDevice.getDeviceBaseAddress()+dataStoreStartByteOffset );
        }
    }while(0);
    return retVal;
}

/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
DATASTORE_STATUS Data_Store<T>::storeClearPtrOffset()
{
    DATASTORE_STATUS status = DATASTORE_STATUS_NOT_OK;
    do{

    }while(0);
    return status;
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
DATASTORE_STATUS Data_Store<T>::compactLog()
{
    uint32 initialClearPtrBlockID;
    uint32 currClearPtrBlockID;
    DATASTORE_STATUS status = DATASTORE_STATUS_NOT_OK;
    RECORD_HEADER recHeader = { 0 };
    char *clearPtr = NULL;

    clearPtr = (char*)flashDevice.getDeviceBaseAddress()+ clearLogPointByOffset;

    initialClearPtrBlockID = currClearPtrBlockID = flashDevice.getBlockIDFromAddress( clearPtr );
    while(initialClearPtrBlockID == currClearPtrBlockID){
        /* Compact one block */
        cyclicDataRead( &recHeader,
                        clearPtr,
                        sizeof(RECORD_HEADER) );
        if(*((unsigned char*)&recHeader) == SKIP_TO_NEXT_SECTOR_FLAG){
                   /* Checking for the flag indicating to skip to the next sector */
                   int  currentBlockID = flashDevice.getBlockIDFromAddress(clearPtr);
                   char *sectorEndByte = (char*)flashDevice.getBlockAddressFromBlockID(currentBlockID) +
                                           flashDevice.getBlockSize(currentBlockID) - 1;
                   int numBytesToIncrement = sectorEndByte - clearPtr + 1;

                   incrementClearPoint(numBytesToIncrement);
                   clearPtr = (char*)flashDevice.getDeviceBaseAddress() + clearLogPointByOffset;
        }
        else if( 0 != recHeader.zero ){
            assert(false);  /* This should never happen */
            break;  //When assert is removed
        }
        else if( 0 == recHeader.zero &&
            FLAG_RECORD_ACTIVE == recHeader.activeFlag ){
            /* Record that we are seeing is active, so we need to move it to the
               end of log - Can't use write method directly because current call
               could be coming from there :) */

        	  /* Create a dummy/skip allocation if required */
        	createDummyAllocation(recHeader.size);


            /* First mark the current location as inactive */
            recHeader.activeFlag = FLAG_RECORD_INACTIVE;
            recHeader.nextLink   = (char*)flashDevice.getDeviceBaseAddress() + \
                                        logPointByteOffset + \
                                        sizeof(RECORD_HEADER);
            cyclicDataWrite( (LPVOID)&recHeader,
                             clearPtr,
                             sizeof(RECORD_HEADER) );

            /* Old record is marked inactive, now migrate the record to the end of log */
            recHeader.activeFlag = FLAG_RECORD_ACTIVE;
            recHeader.nextLink   = (LPVOID)~0u;

            cyclicDataWrite( &recHeader,
                             ((char*)flashDevice.getDeviceBaseAddress()) + logPointByteOffset,
                             sizeof(RECORD_HEADER));
            incrementLogPointer(sizeof(RECORD_HEADER));
            incrementClearPoint(sizeof(RECORD_HEADER));
            clearPtr = (char*)flashDevice.getDeviceBaseAddress()+ clearLogPointByOffset;
            /* Now, clearPtr is pointing to the data_part of the record binging migrated */

            /* Copy data of the record being migrated */
			LPVOID lCurrLoc = ((char*)flashDevice.getDeviceBaseAddress()) + logPointByteOffset;
            cyclicDataWrite( clearPtr,
                             lCurrLoc,
                             recHeader.size );

            incrementLogPointer(recHeader.size);
            incrementClearPoint(recHeader.size);

            clearPtr = (char*)flashDevice.getDeviceBaseAddress()+ clearLogPointByOffset;
			addressTable.updateCurrentLocation( recHeader.recordID, lCurrLoc );
        }else{
            /* Inactive record, just move the clear pointer to the next record */
            incrementClearPoint(recHeader.size + sizeof(RECORD_HEADER));
            clearPtr = (char*)flashDevice.getDeviceBaseAddress()+ clearLogPointByOffset;
        }
        /* Now, check the blockID where my clearPointer is now */
        currClearPtrBlockID = flashDevice.getBlockIDFromAddress(clearPtr);

        status = DATASTORE_STATUS_OK;   //In a loop :)
    }

    /* Now check if there is a complete block of inactive records that can be cleared */
    uint32 logPtrBlockID   = flashDevice.getBlockIDFromAddress( (char*)flashDevice.getDeviceBaseAddress() + logPointByteOffset);
    uint32 clearPtrBlockID = flashDevice.getBlockIDFromAddress( (char*)flashDevice.getDeviceBaseAddress() + clearLogPointByOffset);

    if(datastore_abs((int)clearPtrBlockID - (int)logPtrBlockID ) >= 2){
        /* There is a gap of atleast 1 block and initialClearPtrBlockID could be safely
           erased */
    	   /* Before erase, update the hidden sector with the new clearPtrOffset, which can be used while
    	           scanning the device on the reboot */

        flashDevice.eraseBlock(initialClearPtrBlockID);
        incrementErasePoint(flashDevice.getBlockSize(initialClearPtrBlockID));
    }
    return status;
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template <class T>
Data_Store<T>::Data_Store(char *flashDeviceName)
{
    state = DATASTORE_STATE_UNINIT;
    DATASTORE_STATUS status;

    lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
    status = initDataStore( flashDeviceName,
                            &defaultProperty );

    state = (status == DATASTORE_STATUS_OK)?(DATASTORE_STATE_READY):(DATASTORE_STATE_INT_ERROR);
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template <class T>
Data_Store<T>::Data_Store( char *flashDeviceName,
                           DATASTORE_PROPERTIES *property )
{
    state = DATASTORE_STATE_UNINIT;
    DATASTORE_STATUS status;

    lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
    status = initDataStore( flashDeviceName,
                            property );

    state = (status == DATASTORE_STATUS_OK)?(DATASTORE_STATE_READY):(DATASTORE_STATE_INT_ERROR);
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
#if 0
template<class T>
DATASTORE_STATUS Data_Store<T>::scanFlashDevice()
{/* Need to scan the physical device first */

    /* For now, jsut reset the flash and use it as a new device, will implement
       the persistance later */
    int numBlocks = 0;
#ifdef EMULATOR

    FLASH_PROPERTIES property;
    property.numClusters = flashDevice.getNumberofClusters();
    property.clusters  = (BLOCK_CLUSTER *)malloc(sizeof(BLOCK_CLUSTER)*property.numClusters);

    flashDevice.getDeviceProperties(&property);
    for(int index = 0; index < property.numClusters; index++){
        numBlocks += property.clusters[index].numBlocks;
    }

    for(int index = 0; index < numBlocks; index++){
        flashDevice.eraseBlock(index);
    }

#else
    const BlockDeviceInfo* deviceInfo;

   	flashDevice.getDeviceProperties(deviceInfo);

   	numBlocks = deviceInfo->Regions[0].NumBlocks;

   	//for(int index = 0; index < numBlocks; index++){
   	//    flashDevice.eraseBlock(index);
   	//}

#endif
	logPointByteOffset    = dataStoreStartByteOffset;
    clearLogPointByOffset = logPointByteOffset;  //for now
	erasePointByteOffset  = dataStoreEndByteOffset;
    return DATASTORE_STATUS_OK;
}
#endif
/****************************************************************************
*
*  Function Name : traversePointer(PERSISTENCE_DIRECTION per_dir, char* address)
*
******************************************************************************/
/*!
*  \brief:	Helper for persistence. This function is for those situations where the
*			entire allocated block size is not used for storing the record. The rest
*			of the allocated region could be all FFs. The logPoint is the end of this
*			block and this function helps in finding that.
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
LPVOID Data_Store<T>::traversePointer(PERSISTENCE_DIRECTION per_dir, char* address)
{
	int recordID;
		int blockID;
		LPVOID givenPtr;
		RECORD_HEADER header = { 0 };
		LPVOID dataStoreStartAddr = (char*)flashDevice.getDeviceBaseAddress()+dataStoreStartByteOffset;
		LPVOID dataStoreEndAddr = (char*)flashDevice.getDeviceBaseAddress()+dataStoreEndByteOffset;

		int lfirstBlockID = flashDevice.getBlockIDFromAddress(dataStoreStartAddr);
		int llastBlockID = flashDevice.getBlockIDFromAddress(dataStoreEndAddr);

		/* Traverse left of the address passed as parameter */
		if(per_dir == GO_LEFT){
			while(*address == (char)0xFF){
				blockID = flashDevice.getBlockIDFromAddress(address);
				blockID--;
				if(blockID < lfirstBlockID){
					blockID = llastBlockID;
				}
				address = (char*)flashDevice.getBlockAddressFromBlockID(blockID);
			}

			/* Jump record at a time */
			cyclicDataRead(&header, address, sizeof(RECORD_HEADER));
			while(*((char*)&header) != (char)0xFF){
				if( SKIP_TO_NEXT_SECTOR_FLAG == *((unsigned char*)&header)){
					blockID = flashDevice.getBlockIDFromAddress(address);
					blockID++;
					blockID = (blockID >llastBlockID)?(lfirstBlockID):(blockID);

					address = (char*)flashDevice.getBlockAddressFromBlockID(blockID);
				}else{
					address = (char*)incrementPointer( address,
												header.size+sizeof(RECORD_HEADER));
				}
				cyclicDataRead(&header, address, sizeof(RECORD_HEADER));
			}
		}

		/* Traverse right of the address passed as parameter */
		else if(per_dir == GO_RIGHT){
			while(*address == (char)0xFF){
				blockID = flashDevice.getBlockIDFromAddress(address);
				blockID++;
				if(blockID > llastBlockID){
					blockID = lfirstBlockID;
				}
				address = (char*)flashDevice.getBlockAddressFromBlockID(blockID);
			}
			address = (char*)incrementPointer(address, -1);
		}
		return address;
}


/****************************************************************************
*
*  Function Name : readPointers()
*
******************************************************************************/
/*!
*  \brief:	Helper function for persistence. To get the accurate locations of the
*			log, erase and clear points. While traversing the dataStore, the pointer
*			could end up either in between a region of FFs or in between data.
*			Depending on the location, we traverse left and right of the pointer.
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template <class T>
LPVOID Data_Store<T>::readPointers()
{
	char* returnAddress;
	char* tempAddress1;
	char* tempAddress2;
	int numBytes = 0;
	int tempBlockID = 0;
	PERSISTENCE_DIRECTION per_dir;
	RECORD_HEADER header;

	LPVOID dataStoreStartAddr = (char*)flashDevice.getDeviceBaseAddress()+dataStoreStartByteOffset;
	LPVOID dataStoreEndAddr = (char*)flashDevice.getDeviceBaseAddress()+dataStoreEndByteOffset;
	int firstBlock = flashDevice.getBlockIDFromAddress(dataStoreStartAddr);
	int lastBlock  = flashDevice.getBlockIDFromAddress(dataStoreEndAddr);

	/* Traverse through the entire data store and search for the FF regions */
	for(int index = firstBlock;index <= lastBlock; index++){
		returnAddress = (char *)flashDevice.getBlockAddressFromBlockID(index);
		cyclicDataRead(&header, returnAddress, sizeof(RECORD_HEADER));

		if(*returnAddress == (char)0xFF){
			per_dir = GO_LEFT;
			tempAddress1 = (char *)traversePointer(per_dir, returnAddress);
			per_dir = GO_RIGHT;
			tempAddress2 = (char *)traversePointer(per_dir, returnAddress);
			break;
		}
	}

	logPointByteOffset = (int)(char*)(tempAddress1 - (char*)flashDevice.getDeviceBaseAddress());
	erasePointByteOffset = (int)(char*)(tempAddress2 - (char*)flashDevice.getDeviceBaseAddress());
	clearLogPointByOffset = (uint32)((char*)incrementPointer(tempAddress2,1) - (char*)flashDevice.getDeviceBaseAddress());

	return NULL;
}

/****************************************************************************
*
*  Function Name : readRecordinBlock(int blockID)
*
******************************************************************************/
/*!
*  \brief:	Helper function for persistence. Checks the versions of 2 headers and
*			either marks one for deletion or marks it as inactive.
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
int Data_Store<T>::readRecordinBlock(int blockID)
{
	char* addr = (char *)flashDevice.getBlockAddressFromBlockID(blockID);
	char *endAddr = addr + flashDevice.getBlockSize(blockID);
	int tempCount = 0;

	PRINT_DEBUG("Check Point 1.1 : In Function Read In Block");

	DATASTORE_ADDR_TBL_ENTRY entry;
	DATASTORE_STATUS status;
	int count = 0;
	RECORD_HEADER header;
	while(*addr != (char)0xFF && addr < (endAddr))
	{
		tempCount++;

		//hal_printf("Count Number %d\n", tempCount);

		cyclicDataRead(&header, addr, sizeof(RECORD_HEADER));
		if(*((unsigned char*)&header) == SKIP_TO_NEXT_SECTOR_FLAG){
					break;
		}

		if(FLAG_RECORD_ACTIVE == header.activeFlag )
		{
		entry.allocationSize = header.size;
		entry.currentLoc     = addr+sizeof(RECORD_HEADER);
		entry.givenPtr       = myUniquePtrGen.getUniquePtr(header.size);
		entry.recordID       = header.recordID;

		hal_printf("Record Number Block Id : %d  Record Id: %d Record Size : %d\n", blockID, header.recordID, header.size);

		/* Change the condition to check for the STATUS duplicate recordID */
		status = addressTable.addEntry(&entry);
		if(DATASTORE_STATUS_RECORD_ALREADY_EXISTS == status){
			int oldEntryVer;
			int curEntryVer;
			LPVOID temp;
			RECORD_HEADER tempHeader;

			temp = (Flash_Memory<char> *)addressTable.getCurrentLoc(header.recordID);
			memcpy( &tempHeader,
					(char*)temp-sizeof(RECORD_HEADER),
					sizeof(RECORD_HEADER) );

			oldEntryVer = tempHeader.version;
			curEntryVer = header.version;
			/* Check which header is recent */
			if(decideWhichOneisRecent(tempHeader, header) == curEntryVer){       /*If loop 3*/
				deleteRecord(header.recordID);
				addressTable.addEntry(&entry);
			}else{
				temp = entry.currentLoc;
				tempHeader.activeFlag = FLAG_RECORD_INACTIVE;
				cyclicDataWrite( &tempHeader,(char*)temp-sizeof(RECORD_HEADER),
											sizeof(RECORD_HEADER));
			}       /*End of if loop 3*/
		}else if(status == DATASTORE_STATUS_OK){
			//count++;
		}
		else{
			DATASTORE_ASSERT( false, "Address Table failed!");
		}
		}

		count++;
		addr = (char*)incrementPointer(addr, sizeof(RECORD_HEADER)+header.size);
	}
	PRINT_DEBUG("Check Point 1.2 : Exiting Function Read In Block");
	return count;
}





/****************************************************************************
*
*  Function Name : decideWhichOneisRecent(RECORD_HEADER tempHeader, RECORD_HEADER header)
*
******************************************************************************/
/*!
*  \brief:	Helper function for persistence. Returns which of the two parameters passed
*			as parameters has the most recent version of the header
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
uint32 Data_Store<T>::decideWhichOneisRecent(RECORD_HEADER tempHeader, RECORD_HEADER header)
{
    return((tempHeader.version+1)>header.version?tempHeader.version:header.version);
}


/****************************************************************************
*
*  Function Name : scanFlashDevice()
*
******************************************************************************/
/*!
*  \brief:	Function for persistence. This is called during the device initialization
*			to get the log, erase and clear pointers.
*
*  \param
*  \param
*  \return
*
******************************************************************************/

template<class T>
LPVOID Data_Store<T>::scanFlashDevice()
{
	LPVOID dataStoreStartAddr = (char*)flashDevice.getDeviceBaseAddress()+dataStoreStartByteOffset;
	LPVOID dataStoreEndAddr = (char*)flashDevice.getDeviceBaseAddress()+dataStoreEndByteOffset;

	int firstBlock = flashDevice.getBlockIDFromAddress(dataStoreStartAddr);
	int lastBlock  = flashDevice.getBlockIDFromAddress(dataStoreEndAddr);
	int numOfEntries = 0;
	char* returnAddress;

	for(int index = firstBlock;index <= lastBlock; index++){
		numOfEntries += readRecordinBlock(index);
	}

	/* The device is new */
	if(numOfEntries == 0){
		logPointByteOffset = clearLogPointByOffset = dataStoreStartByteOffset;
		erasePointByteOffset = dataStoreEndByteOffset;
	}
	/* If the device has already some data and crashed during use, call the below function */
	else{
		readPointers();
	}
	return NULL;
}



/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template <class T>
DATASTORE_STATUS Data_Store<T>::deleteRecord(RECORD_ID id)
{
    RECORD_HEADER header;
    DATASTORE_STATUS status = DATASTORE_STATUS_NOT_OK;
    lastErrorVal = DATASTORE_ERROR_NONE;
    do{
        LPVOID currLoc = addressTable.getCurrentLoc(id);
        if(currLoc == NULL){
            lastErrorVal = DATASTORE_ERROR_INVALID_RECORD_ID;
            break;
        }

        cyclicDataRead( (void*)&header,
                        (char*)currLoc - sizeof(RECORD_HEADER),
                        sizeof(RECORD_HEADER) );
        /* We need to mark this as inactive so that the garbage collector can clear
           this */
        header.activeFlag = FLAG_RECORD_INACTIVE;

        cyclicDataWrite( (LPVOID)&header,
                         (char*)currLoc - sizeof(RECORD_HEADER),
                         sizeof(RECORD_HEADER) );
        addressTable.removeEntry(id);
        status = DATASTORE_STATUS_OK;
    }while(0);
    return status;
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
LPVOID Data_Store<T>::createRecord( RECORD_ID recordID, uint32 numBytes )
{
    RECORD_HEADER header;
    LPVOID lGivenPtr = NULL;
    LPVOID allocLoc  = NULL;
    lastErrorVal = DATASTORE_ERROR_NONE;

    // The STM Driver accepts half words as inputs so all writes are in half word sizes
    numBytes = numBytes + numBytes % 2;

    do{
        if(NULL != addressTable.getGivenAddress(recordID)){
            /* This record id is already present in the flash. So, can't proceed */
            lastErrorVal = DATASTORE_ERROR_RECORD_ID_ALREADY_EXISTS;
            break;
        }

        /* Generate an unique pointer and register it with the address_table */
        lGivenPtr = myUniquePtrGen.getUniquePtr(numBytes);
        allocLoc  = createAllocation( recordID, lGivenPtr, numBytes );
        if( NULL == allocLoc ){
            break;
        }
        /* Address table will be updated by createAllocation, so need not to bother here */
    }while(0);
    return lGivenPtr;
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
uint32 Data_Store<T>::readRawData(LPVOID src, void *data, uint32 numBytes)
{
    uint32 lNumBytes = 0;
    LPVOID curLoc = NULL;

    LPVOID lDataStoreStartByteAddr = (char*)flashDevice.getDeviceBaseAddress() + dataStoreStartByteOffset;
    LPVOID lDataStoreEndByteAddr   = (char*)flashDevice.getDeviceBaseAddress() + dataStoreEndByteOffset;

    lastErrorVal = DATASTORE_ERROR_NONE;
    do{
        if(NULL == src || NULL == data){
            lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
            lNumBytes = numBytes = 0;
            break;
        }
        if( NULL == (curLoc = addressTable.getCurrentLoc(src, lDataStoreStartByteAddr, lDataStoreEndByteAddr))){
            lastErrorVal = DATASTORE_ERROR_INVALID_GIVEN_ADDR;
            lNumBytes = numBytes = 0;
            break;
        }

        lNumBytes = addressTable.getMaxWriteSize(src);
        lNumBytes = (numBytes < lNumBytes)?numBytes:lNumBytes;

        cyclicDataRead( data, curLoc, lNumBytes);
    }while(0);
    return lNumBytes;
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
uint32 Data_Store<T>::readData(LPVOID src, T* data, uint32 count)
{
    return readRawData( src, data, count*sizeof(T));
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
uint32 Data_Store<T>::writeData( LPVOID dest, T *data, uint32 count )
{
    return writeRawData( dest, data, count*sizeof(T));
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
uint32 Data_Store<T>::writeRawData(LPVOID dest, void* data, uint32 numBytes)
{
    LPVOID recBaseAddr       = NULL;    /* Base address of the current allocation */
    LPVOID recWriteStartAddr = NULL;    /* Address that correspond to the address where give data needs to be written */
    LPVOID recWriteEndAddr   = NULL;    /* Address of corresponding location in flash where last byte of data needs to be written */
    LPVOID recEndAddr        = NULL;    /* Address of the last byte of the current allocation */

    RECORD_ID recordID     = 0;         /* Record ID of the record where we are trying to write */
    uint32    recordSize   = 0;         /* Allocation size of this record */
    uint32 numBytesWritten = 0;         /* Actual number of bytes that can be written */

    uint32 overWriteStartOffset = 0;    /* Doesn't matter much in new implementation */
    bool   isOverWriteDetected  = false;

    lastErrorVal = DATASTORE_ERROR_NONE;
    do{
        DATASTORE_ASSERT( dest != NULL && data != NULL, "Invalid parameters being passed" );
        if(dest == NULL || data == NULL){
            lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
            numBytesWritten = 0;
            break;
        }

        /* Initialize all the pointers */
        recBaseAddr = addressTable.getCurrentLoc((recordID = addressTable.getRecordID(dest)));
        if(NULL == recBaseAddr){
            lastErrorVal = DATASTORE_ERROR_INVALID_GIVEN_ADDR;
            DATASTORE_ASSERT( NULL != recBaseAddr,
                              "Invalid address being passed for write" );
            break;
        }

        recordSize        = addressTable.getMaxWriteSize(addressTable.getGivenAddress(addressTable.getRecordID(dest)));

        recWriteStartAddr = addressTable.getCurrentLoc( dest,
                                                        (char*)flashDevice.getDeviceBaseAddress() + dataStoreStartByteOffset,
                                                        (char*)flashDevice.getDeviceBaseAddress() + dataStoreEndByteOffset );
        numBytesWritten   = addressTable.getMaxWriteSize(dest);    /* Number of bytes till the end of record */
        /* Minimum of these two sizes */
        numBytesWritten   = (numBytes < numBytesWritten)?numBytes:numBytesWritten;

        recWriteEndAddr   = incrementPointer(recWriteStartAddr , numBytesWritten - 1);              /* corresponding End address on flash */
        recEndAddr        = incrementPointer(recBaseAddr , recordSize - 1);     /* Last byte address of the record on flash */

        /* Now check if there is a overwrite possiblility */
        isOverWriteDetected = detectOverWrite( recWriteStartAddr,
                                               data,
                                               numBytesWritten,
                                               &overWriteStartOffset );
        if(false == isOverWriteDetected){
            /* No overwrites are detected, so its safe to just write data */
            numBytesWritten = cyclicDataWrite( data,
                                               recWriteStartAddr ,
                                               numBytesWritten );
        }else{
            /* Possibility for Overwrite is detected.
               We need to create a new allocation and move the data.
                Now we have three regions to write
               1. Region before overwrite
               2. Overwrite region
               3. Region after overwrite */

            /* Get the given pointer base value from the destination */
            LPVOID lGivenPtr = addressTable.getGivenAddress(addressTable.getRecordID(dest));

            LPVOID newRecBaseAddr       = NULL;
            LPVOID newRecWriteStartAddr = NULL;

            newRecBaseAddr = createAllocation( recordID, lGivenPtr, recordSize );
            if( NULL == newRecBaseAddr ){
                /* Allocation failed! */
                lastErrorVal = DATASTORE_ERROR_OUT_OF_FLASH_MEMORY;
                numBytesWritten = 0;
                break;
            }

            /* createAllocation returns the pointer to the header location */
            newRecBaseAddr = incrementPointer( newRecBaseAddr , sizeof(RECORD_HEADER));

            newRecWriteStartAddr = addressTable.getCurrentLoc(dest,
                                                              (char*)flashDevice.getDeviceBaseAddress()+dataStoreStartByteOffset,
                                                              (char*)flashDevice.getDeviceBaseAddress()+dataStoreEndByteOffset );

            /* Temporary pointers that point to the write location and length at each step below */
            LPVOID lCurrentWriteLoc = newRecBaseAddr;
            uint32 lCurrentWriteLen = calculateNumBytes(newRecBaseAddr, newRecWriteStartAddr) - 1;

            /* Write region-1 */
            cyclicDataWrite( recBaseAddr,
                             lCurrentWriteLoc,
                             lCurrentWriteLen );

            /* Move the current location pointer */
            lCurrentWriteLoc = incrementPointer(lCurrentWriteLoc, lCurrentWriteLen);
            lCurrentWriteLen = numBytesWritten;

            /* Write region-2 */
            cyclicDataWrite( data,
                             lCurrentWriteLoc,
                             lCurrentWriteLen );

            /* Move the current location pointer */
            lCurrentWriteLoc = incrementPointer(lCurrentWriteLoc , lCurrentWriteLen);
            lCurrentWriteLen = calculateNumBytes(recWriteEndAddr, recEndAddr) - 1;

            /* Write region-3 */
            cyclicDataWrite( incrementPointer(recWriteEndAddr, 1),
                             lCurrentWriteLoc,
                             lCurrentWriteLen );

            /* Congrats, thats all you had to do, write completed successfully :) */
        }
    }while(0);
    return numBytesWritten;
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
DATASTORE_ERROR Data_Store<T>::getLastError()
{
    return lastErrorVal;
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
Data_Store<T>::~Data_Store()
{
    DATASTORE_REG_RemoveEntry(this);
#ifdef _EMULATOR_MODE
    delete flashDevice;
    delete myUniquePtrGen;
#endif
}

/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
#ifdef ENABLE_TEST
template<class T>
uint32 Data_Store<T>::returnLogPoint()
{
	return logPointByteOffset;
}

template<class T>
uint32 Data_Store<T>::returnClearPoint()
{
    return clearLogPointByOffset;
}

template<class T>
uint32 Data_Store<T>::returnErasePoint()
{
	return erasePointByteOffset;
}
#endif

#endif
