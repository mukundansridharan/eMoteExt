#ifndef _FLASHDEVICE_H_
#define _FLASHDEVICE_H_

#include<tinyhal.h>
//#include "flash_memory.h"
#include "FlashMemory.h"

#define FLASHDEVICE_FILE_FORMAT_VERSION    (1)

typedef void* LPVOID;
typedef UINT16 USHORT;

/* Describes a cluster of blocks, used in FLASH_PROPERTIES, a cluster is a group
   of blocks that have
   This is to support the requirement of support flashes with blocks of multiple
   sizes */
typedef struct _block_cluster
{
    int blockSize;  /* Block size in this cluster */
    int numBlocks;  /* Number of blocks in this cluster */
}BLOCK_CLUSTER;


/* Properties of the flash device that needs to be simulated */
typedef struct _flash_properties
{
    BLOCK_CLUSTER *clusters;      /* Number of clusters */
    int           numClusters;    /* Number of elements in Array BLOCK_CLUSTER *clusters */
    /* Add any more properties here */
}FLASH_PROPERTIES;


/* Block statistics, now just has number of erases, add any more required
   properties into this */
typedef struct _flash_block_statistics
{
    int numErases;           /* Number of erases done to given Block */
    int numWritesAfterErase; /* Number of writes after previous erase */

    /* NOT AVAILABLE YET!!! */
    int totalWrites;         /* Absolute total number of writes since creating this flash file */
    /* Do we need any more statistics for each block apart from num of erases??? */
}FLASH_BLOCK_STATISTICS;


/* Structure used for blockID and Address Conversion table.
   This conversion can be don't without a lookup table also, but its efficient this way */
typedef struct _addr_mapping
{
    int  blockStartID, blockEndID;
    char *blockStartAddr, *blockEndAddr;    /* Char Ptr for easy pointer arithmetic */
    int blockSize;
}ADDR_MAPPING;


typedef enum _flashdevice_state
{
    FLASHDEVICE_STATE_UNINITIALIZED,
    FLASHDEVICE_STATE_READY,
    FLASHDEVICE_STATE_FAILED_TO_LOAD,
    FLASHDEVICE_STATE_CLOSED
    /* Do we need more states here? */
}FLASHDEVICE_STATE;

typedef enum _flashdevice_status
{
	FLASHDEVICE_STATUS_OK,
	FLASHDEVICE_STATUS_NOT_OK
    /* Add more errors here */
}FLASHDEVICE_STATUS;

class FlashDevice
{
private:
	char* flashDeviceName;
    /* Private data */
    BlockStorageDevice *device;
    const BlockDeviceInfo    *deviceInfo;
    //FLASH_PROPERTIES flashProperties;     /* Properties of flash file */
    DWORD            flashSize;           /* Size of the emulated flash device */
    USHORT           headerSize;          /* Size of the header */
    FLASHDEVICE_STATE   state;               /* Current state of the Emulator */
    UINT32 			 currAddress;

    ADDR_MAPPING     conversionTbl[NUMBER_OF_CLUSTERS];      /* Conversion b/w addr and blockID */
    int              conversionTblLen;    /* Length of conversion table - Will be equal to numClusters */

    LPVOID           readOnlyPtr;         /* Read-only file mapped pointer returned to the user */
    LPVOID           writePtr;            /* File mapping ptr with write permissions */



    /* PRIVATE FUNCTIONS */
    BOOL createNewFlashDeviceFile( char *flashName);
    BOOL loadExistingFlashDevice ( char *flashName);

    /* Updates the block statistics on each erase, write events on given block */
    FLASHDEVICE_STATUS updateBlockStatistics(int blockID, FLASH_BLOCK_STATISTICS *newVal);

    /* initFlashEmulator() will initialize everything for Constructor */
    void initFlashDevice( char *flashName);

    /* Function that build the conversion table that is used in the conversion between
       blockID and Address */
    void buildConversionTbl( void );

    /* Function that creates memory mapping of file, one that is exposed to user with
       READ-ONLY permission and internal mapping with READ-WRITE permission */
    BOOL createMemoryMapping();

public:
	void init()
	{
	  	this->flashDeviceName = "NOR";
	  	initFlashDevice("NOR");
	}

	FlashDevice();
    /* Constructor - Opens an existing flash by name flashName or creates one with
       given FLASH_PROPERTIES if it doesn't exist */
    FlashDevice( char *flashName);

    /* Constructor - Opens an existing flash by name flashName or creates one with
       given properties given in configFIleName */
    //FlashDevice( char *flashName, char *configFileName );

    /* Writes data into flash at given offset
       Return Value : Number of bytes actually written. Return value ZERO means error */
    int writeData( int *dataIn, int dataCount, void* addr );

    /* Writes any datatype to flash - Flash with WordSize=Int wants to write some string
       Return Value : Number of bytes actually written. Return value ZERO means error */
    int writeRawData( void *dataIn, int dataSize, void* addr );

    /* Returns the base address of the device from which user can read the data */
    //LPVOID getDeviceBaseAddress();
    FlashMemory* getDeviceBaseAddress();

    /* Converts the given absolute address into the blockID */
    int getBlockIDFromAddress(LPVOID address);

    /* Converts the given blockID into the block start address */
    FlashMemory* getBlockAddressFromBlockID(int blockID);

    /* Erase a block given by eraseBlockID
       Return Value : Appropriate error status defined FLASHDEVICE_STATUS */
    FLASHDEVICE_STATUS eraseBlock(int eraseBlockID);

    /* Get number of clusters in the device */
    int getNumberofClusters();

    /* Read the device properties - Esp useful when already exosting flash image is opened
           Return value : Appropriate error status defined FLASHDEVICE_STATUS  */
    FLASHDEVICE_STATUS getDeviceProperties(const BlockDeviceInfo *deviceInfo);


    FLASHDEVICE_STATUS getDeviceProperties(FLASH_PROPERTIES *properties);

    /* Reads the statistics for given blockID
       Return value : Appropriate error status defined FLASHDEVICE_STATUS  */
    FLASHDEVICE_STATUS getBlockStatistic(int blockID, FLASH_BLOCK_STATISTICS *statistic);

    /* get block size of any block */
    int getBlockSize( int blockID );

    /* get device size */
    unsigned int getDeviceSize(){return flashSize;}

    /* Gives the current state of the Emulator */
    FLASHDEVICE_STATE getDeviceState(){return state;}

    /* Close the device handle */
    FLASHDEVICE_STATUS close();

    /* Delete the emulator */
    /* NOT sure if this has to be member function! */
    /* Closes the device if it is open, and deletes the emulation file */
    BOOL doesFileExist()
    {

    	return FALSE;

    	if(flashDeviceName == NULL)
    		return FALSE;
    	else
    		return TRUE;
    }

    BOOL updateCurrAddress(int size)
    {
    	currAddress += size;
    }

    BOOL goToBeginning()
    {
    	currAddress = deviceInfo->Regions->Start;
    }
    /* Destructor for the emulator */
    ~FlashDevice();

    //friend int writeFlashProperties(FlashDevice<T>* );

    //friend void writeFlashRegion(FlashDevice<T> &,USHORT headerSize, DWORD fileSize );

    //friend bool writeFlashStatisticsArea(FlashDevice<T> &);

    //friend bool doesFileExist(FlashDevice<T> &);

    int writeFlashProperties(int deviceWordSize);

    BOOL loadFlashProperties(int deviceWordSize);

};





#endif //_FLASHDEVICE_H_
