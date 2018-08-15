#ifndef _DATASTORE_INT_H_INCLUDED_
#define _DATASTORE_INT_H_INCLUDED_


#define FLAG_RECORD_ACTIVE      (1)     /* Because of flash properties, this has to be 1 */
#define FLAG_RECORD_INACTIVE    (0)

#ifdef DATASTORE_DEBUG_DEFINED
#define DATASTORE_ASSERT(condition, messageStr)     do{ASSERT(condition)}while(0);
#else
#define DATASTORE_ASSERT(condition, messageStr)
#endif

#define SKIP_TO_NEXT_SECTOR_FLAG    (unsigned char)(0xAA)    /* Header.zero will be kept 1 here intentially */

typedef void* LPVOID;

typedef enum _datastore_state
{
    DATASTORE_STATE_UNINIT,
    DATASTORE_STATE_READY,
    DATASTORE_STATE_INT_ERROR
}DATASTORE_STATE;

/* For the persistence function */
typedef enum _persistence_direction
{
	GO_LEFT,
	GO_RIGHT
}PERSISTENCE_DIRECTION;


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


/* Describes a cluster of blocks, used in DATASTORE_PROPERTIES, a cluster is a group
   of blocks that have different block sizes.
   This is to support the requirement of support flashes with blocks of multiple
   sizes */
typedef struct _datastore_cluster
{
    uint32 blockSize;  /* Block size in this cluster */
    uint32 numBlocks;  /* Number of blocks in this cluster */
}DATASTORE_CLUSTER;


/* Properties of the flash device that needs to be simulated */
typedef struct _datastore_properties
{
    DATASTORE_CLUSTER *clusters;      /* Number of clusters */
    int               numClusters;    /* Number of elements in Array BLOCK_CLUSTER *clusters */
    /* Till here its identical to the flash_properties */

    LPVOID            addressRangeStart;
    LPVOID            addressRangeEnd;
}DATASTORE_PROPERTIES;



typedef struct _record_header
{
    char zero:1;
    uint32 version:2;
    uint32 activeFlag:1;
    uint32 size:28;   /* sizeof(version+isActive+size) = 32bits */

    uint32 recordID;
    void*  nextLink;
}RECORD_HEADER;


class uniquePtr
{
private:
	LPVOID start, end;
	LPVOID lastVal;
public:
	uniquePtr()
	{
		start = (LPVOID) 0x0;
		end = (LPVOID) (1 << 31);
		lastVal = (LPVOID) 1;
	}

	void init()
	{
		start = (LPVOID) 0x0;
		end = (LPVOID) (1 << 31);
		lastVal = (LPVOID) 1;
	}

	uniquePtr(LPVOID start, LPVOID end);
	LPVOID getUniquePtr(uint32 numBytes);
};

class hiddenPersistanceMetaData
{/* class that handles storing and managing  */
private:
    uint32 curLoc;

public:
    /*  */
    hiddenPersistanceMetaData();
    DATASTORE_STATUS storeClearPtrOffset();
};

/* Prototypes */
int datastore_abs(int val);


#endif

