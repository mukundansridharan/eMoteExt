#ifndef _ADDRESS_TABLE_H_INCLUDED_
#define _ADDRESS_TABLE_H_INCLUDED_

//#include <vector>
#include <tinyhal.h>
#include "Types.h"
//#include "myVector.h"
//#include "Datastore.h"
//#include "DatastoreInt.h"

#define MAX_NUM_TABLE_ENTRIES       (128)

typedef void* LPVOID;

using namespace std;


typedef struct PACK
{
    RECORD_ID  recordID;           /* Unique allocation identifier */
    //LPVOID     givenPtr;           /* Smart pointer that is given to the user of datastore */
    UINT32     allocationSize;     /* Size of the allocation */
    LPVOID     currentLoc;         /* Current location of the corresponding data on the flash */
}DATASTORE_ADDR_TBL_ENTRY;


/*
    Definition of different API status for the datastore APIs
*/
typedef enum _datastore_status
{
    DATASTORE_STATUS_OK,
    DATASTORE_STATUS_NOT_OK,
    DATASTORE_STATUS_INVALID_PARAM,
    DATASTORE_STATUS_RECORD_ALREADY_EXISTS,
    DATASTORE_STATUS_INT_ERROR,
    DATASTORE_STATUS_OUT_OF_MEM,
    DATASTORE_STATUS_OVERLAPPING_ADDRESS_SPACE,      /* Detected overlapping Address space between datastores */
    DATASTORE_STATUS_NOT_FOUND
}DATASTORE_STATUS;


class myVector
{
private:
    /* Static, because I don't want it to come into the stack and I am sure that
       there will be only one instance of this class */
    //static DATASTORE_ADDR_TBL_ENTRY intTable[MAX_NUM_TABLE_ENTRIES];
	DATASTORE_ADDR_TBL_ENTRY intTable[MAX_NUM_TABLE_ENTRIES];
    int vectSize;
public:
	myVector(){vectSize = 0;};
    int     size(void){return vectSize;};
    int     begin(void){return 0;};
    int     insert ( int position, const DATASTORE_ADDR_TBL_ENTRY& x );
    int     erase(int position);
    void    clear(void);

    DATASTORE_ADDR_TBL_ENTRY& operator[] (const int index);
};


/*
    The table is optimized for the Lookup using given_address
*/
class DATASTORE_AddrTable
{
private:
    //vector<DATASTORE_ADDR_TBL_ENTRY> table;
	myVector table;
    /* Add anything required later */
    DATASTORE_STATUS search(LPVOID givenAddr, DATASTORE_ADDR_TBL_ENTRY *entry);
    BOOL 			 sortAddressTable();
    BOOL			 eraseAddressTable();
public:
    ////myVector table;
    DATASTORE_AddrTable();
    DATASTORE_STATUS addEntry(DATASTORE_ADDR_TBL_ENTRY *entry);
    LPVOID			 getCurrentLoc(RECORD_ID recordID, int &index);
    LPVOID           getCurrentLoc(RECORD_ID recordID);
    LPVOID           getCurrentLoc(LPVOID givenPtr, LPVOID startPtr, LPVOID endPtr);
    LPVOID           getCurrentLoc(LPVOID givenPtr, LPVOID startPtr, LPVOID endPtr, DATASTORE_ADDR_TBL_ENTRY &tblEntry);
    LPVOID           getGivenAddress(RECORD_ID recordID);
    LPVOID           getCurrentLocation(RECORD_ID recordID);
    LPVOID			 getGivenPointer(LPVOID currentPtr);
    RECORD_ID        getRecordID(LPVOID givenAddr);
    uint32           getAllocationSize(RECORD_ID recordID);
    uint32           getMaxWriteSize(LPVOID givenAddr);      /* Tells number of bytes that can be written from the given point */
    DATASTORE_STATUS updateCurrentLocation(RECORD_ID recordID, LPVOID newLoc);
    DATASTORE_STATUS removeEntry(RECORD_ID recordID);
    DATASTORE_STATUS removeEntry(LPVOID givenPtr);
    DATASTORE_STATUS copyEntry(myVector *table);
     ~DATASTORE_AddrTable();
     friend class Data_Store;
};

#endif
