/*
 *  Name : MemoryPtr.h
 *
 *	Desc : Interfaces the C# object and the datastore instance
 *
 */
#ifndef _MEMORY_PTR_H
#define _MEMORY_PTR_H

#include<tinyhal.h>


#ifdef DEBUG_DATASTORE
#define PRINT_DEBUG(x) hal_printf(x)
#else
#define PRINT_DEBUG(x)
#endif


#include <datastore.h>


typedef INT32 INT;

Data_Store<UINT8> ds;

template<class T> class MemPtr
{
	T* ptr;
	INT recordId;
	UINT32 numOfBytes;
	//Data_Store<T> ds;

	// Explicit assignment and copy constructors are blocked to avoid scenarios where two instances of
	// C# objects point to the same record. Disposing of one object may create dangling pointers.
	MemPtr(MemPtr<T> const&);
	MemPtr<T>& operator=(MemPtr<T> const&);


public:

	T& operator*()  { return *ptr;}

	T* operator->()   { return ptr;}


	// Called by the C# object constructor
	MemPtr(INT id, UINT32 size) : recordId(id),numOfBytes(size)
	{
		//PRINT_DEBUG("Check Point 1 : Constructor Called");

		ds.init();
		//Data_Store<T> ds = Data_Store<T>::getInstance();
		// create a record in datastore with record id and size
		//PRINT_DEBUG("Check Point 2 : Data Store Initialized");

		ptr = (T*) ds.createRecord(recordId,size);

		//PRINT_DEBUG("Check Point 3 : Record Created");

		// record already exists
		if(ptr == NULL)
		{
			//PRINT_DEBUG("Check Point 4 : Record already exists");
			ptr = (T*) ds.getAddress(recordId);
		}

		// Call the instance of datastore

	}

	// called during other invocations from the c# object. The C# object maintains the record id
	MemPtr(INT Id) : recordId(Id)
	{

		//Data_Store<T> ds = Data_Store<T>::getInstance();
		ptr = (T*) ds.getAddress(recordId);
	}

	MemPtr& operator=(CLR_RT_TypedArray_UINT8 buffer)
	{

		//Data_Store<T> ds = Data_Store<T>::getInstance();
		CLR_RT_TypedArray_UINT8 localBuffer = buffer;
		UINT8* bufferPointer = localBuffer.GetBuffer();
		hal_printf("Writing Record %d of size %d\n",recordId, localBuffer.GetSize());
		ds.writeRawData(ptr, localBuffer.GetBuffer(),localBuffer.GetSize());
		hal_printf("Write Complete\n");
	}

	void read(void *buffer, int len)
	{
		ds.readRawData(ptr,buffer,len);
	}
	// returns the id of the newly created pointer
	UINT32 getId()
	{
		return recordId;
	}

	T* getPointer()
	{
		return ptr;
	}


	~MemPtr()
	{

	}

	void dispose()
	{
		//Data_Store<T> ds = Data_Store<T>::getInstance();
		ds.deleteRecord(recordId);
	}

};



#endif
