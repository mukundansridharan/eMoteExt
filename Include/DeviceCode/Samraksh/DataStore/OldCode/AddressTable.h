#ifndef _ADDRESS_TABLE_H_INCLUDED_
#define _ADDRESS_TABLE_H_INCLUDED_

//#include <vector>
#include <tinyhal.h>
#include "types.h"
#include "datastore_int.h"

#define MAX_NUM_TABLE_ENTRIES       (256)

using namespace std;

typedef struct
{
    RECORD_ID  recordID;           /* Unique allocation identifier */
    LPVOID     givenPtr;           /* Smart pointer that was given to the application */
    DWORD      allocationSize;     /* Size of the allocation */
    LPVOID     currentLoc;         /* Current location of the corresponding data on the flash */
}DATASTORE_ADDR_TBL_ENTRY;


class myVector
{
private:
    /* Static, because I don't want it to come into the stack and I am sure that
       there will be only one instance of this class */
    //static DATASTORE_ADDR_TBL_ENTRY intTable[MAX_NUM_TABLE_ENTRIES];
	DATASTORE_ADDR_TBL_ENTRY intTable[MAX_NUM_TABLE_ENTRIES];;
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
public:
    DATASTORE_AddrTable();
    DATASTORE_STATUS addEntry(DATASTORE_ADDR_TBL_ENTRY *entry);
    LPVOID           getCurrentLoc(RECORD_ID recordID);
    LPVOID           getCurrentLoc(LPVOID givenPtr, LPVOID startPtr, LPVOID endPtr);
    LPVOID           getGivenAddress(RECORD_ID recordID);
    LPVOID			 getGivenPointer(LPVOID currentPtr);
    RECORD_ID        getRecordID(LPVOID givenAddr);
    uint32           getAllocationSize(RECORD_ID recordID);
    uint32           getMaxWriteSize(LPVOID givenAddr);      /* Tells number of bytes that can be written from the given point */
    DATASTORE_STATUS updateCurrentLocation(RECORD_ID recordID, LPVOID newLoc);
    DATASTORE_STATUS removeEntry(RECORD_ID recordID);
    DATASTORE_STATUS removeEntry(LPVOID givenPtr);
    DATASTORE_STATUS copyEntry(myVector *table);
     ~DATASTORE_AddrTable();
};

#endif
