#ifndef _FLASH_MEMORY_H_
#define _FLASH_MEMORY_H_

#include "emulator_types.h"
#include "util.h"

#ifdef EMOTE
#define NUMBER_OF_FLASHBLOCKS 256
#else
#define NUMBER_OF_FLASHBLOCKS 128
#endif

#define NUMBER_OF_CLUSTERS 1
/* Just for readability */
#define BITWISE_AND &


/* Pragma to disable any possible padding that would be critical problem */
#pragma pack(push, 1)
template <class T>
class Flash_Memory
{
private:
	T data;

    /* Generic write function that is being used by all the other operator functions */
	void writeOperator(const T data);
public:
    /* Constructor (that is never ever expected to be called :) ) */
	Flash_Memory<T>();

    /* Operator overloading to overload all possible assignment operations with
       Flash_Memory<T> on RHS */
	Flash_Memory<T>* operator=   (const Flash_Memory &inputData);
	Flash_Memory<T>* operator-=  (const Flash_Memory &inputData);
	Flash_Memory<T>* operator+=  (const Flash_Memory &inputData);
	Flash_Memory<T>* operator*=  (const Flash_Memory &inputData);
	Flash_Memory<T>* operator/=  (const Flash_Memory &inputData);
    Flash_Memory<T>* operator<<= (const Flash_Memory &inputData);
	Flash_Memory<T>* operator>>= (const Flash_Memory &inputData);
	Flash_Memory<T>* operator&=  (const Flash_Memory &inputData);
	Flash_Memory<T>* operator^=  (const Flash_Memory &inputData);
	Flash_Memory<T>* operator|=  (const Flash_Memory &inputData);
	Flash_Memory<T>* operator%=  (const Flash_Memory &inputData);

    /* Operator overloading to overload all possible assignment operations with
       Type T on RHS */
    Flash_Memory<T>* operator=   (const T &inputData);
	Flash_Memory<T>* operator-=  (const T &inputData);
	Flash_Memory<T>* operator+=  (const T &inputData);
	Flash_Memory<T>* operator*=  (const T &inputData);
	Flash_Memory<T>* operator/=  (const T &inputData);
    Flash_Memory<T>* operator<<= (const T &inputData);
	Flash_Memory<T>* operator>>= (const T &inputData);
	Flash_Memory<T>* operator&=  (const T &inputData);
	Flash_Memory<T>* operator^=  (const T &inputData);
	Flash_Memory<T>* operator|=  (const T &inputData);
	Flash_Memory<T>* operator%=  (const T &inputData);

    /* Defining the explicit typecasting - This will ease the coding
       greatly-Believe me :) */
    operator T (){
        return data;
    }
};
#pragma pack(pop)


/****************************************************************************
*
*  Function Name : Constructor (thats never called :) )
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>::Flash_Memory()
{
	data = 0;
}


/****************************************************************************
*
*  Function Name :  writeOperator
*
******************************************************************************/
/*!
*  \brief          Write operation that writes to the required location after
*                  applying required mask.
*
******************************************************************************/
template <class T>
void Flash_Memory<T>::writeOperator(const T data)
{
	register char *writePtr = (char*)lookUpAddress(this);      /* Char* because I want to handle data one-byte at a time */
	char *inputDataCharPtr = (char*)&data;

    EMULATOR_ASSERT( writePtr != NULL , "Failed to map ReadOnlyPTR to writeable ptr!");

    for(int index = 0; index < sizeof(T); index++){
        writePtr[index] = (writePtr[index]) BITWISE_AND (inputDataCharPtr[index]);

#ifdef EMULATOR_DO_VERIFIED_WRITE
	/* Optionally assert if there is memory overwrites are detected */
		EMULATOR_ASSERT( writePtr[index] == inputDataCharPtr[index], "ERROR : Memory over-writes detected!!!" );

		if(writePtr[index] != inputDataCharPtr[index]){
			/* Something more to be done? */
		}
#endif

    }
}


/****************************************************************************
*
*  Function Name : Operator =
*
******************************************************************************/
/*!
*  \brief
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator=(const Flash_Memory &inputData)
{
	writeOperator(inputData.data);
	return this;
}


/****************************************************************************
*
*  Function Name : Operator -=
*
******************************************************************************/
/*!
*  \brief
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator-=(const Flash_Memory &inputData)
{
	writeOperator(data - inputData.data);
	return this;
}


/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator+=(const Flash_Memory &inputData)
{
	writeOperator(data+inputData.data);
	return this;
}


/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator*=(const Flash_Memory &inputData)
{
	writeOperator(data * (inputData.data));
	return this;
}


/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator/=(const Flash_Memory &inputData)
{
	writeOperator(data / inputData.data);
	return this;
}


/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator<<=(const Flash_Memory &inputData)
{
	writeOperator(data << inputData.data);
	return this;
}


/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator>>=(const Flash_Memory &inputData)
{
	writeOperator(data >> inputData.data);
	return this;
}


/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator&=(const Flash_Memory &inputData)
{
	writeOperator((data) BITWISE_AND (inputData.data));
    return this;
}



/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator^=(const Flash_Memory &inputData)
{
	writeOperator(data ^ inputData.data);
	return this;
}



/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator|=(const Flash_Memory &inputData)
{
	writeOperator(data | inputData.data);
	return this;
}



/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator%=(const Flash_Memory &inputData)
{
	writeOperator( data % inputData.data);
	return this;
}




/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator=(const T &inputData)
{
    writeOperator(inputData);
    return this;
}



/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator-=  (const T &inputData)
{
    writeOperator(data - inputData);
    return this;
}


/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator+=  (const T &inputData)
{
    writeOperator(data + inputData);
    return this;
}


/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator*=  (const T &inputData)
{
    writeOperator(data * inputData);
    return this;
}


/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator/=  (const T &inputData)
{
    writeOperator(data / inputData);
    return this;
}

/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator<<= (const T &inputData)
{
    writeOperator(data << inputData);
    return this;
}

/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator>>= (const T &inputData)
{
    writeOperator(data >> inputData);
    return this;
}

/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator&=  (const T &inputData)
{
    writeOperator(data & inputData);
    return this;
}

/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator^=  (const T &inputData)
{
    writeOperator(data ^ inputData);
    return this;
}


/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator|=  (const T &inputData)
{
    writeOperator(data | inputData);
    return this;
}

/****************************************************************************
*
*  Function Name : Operator
*
******************************************************************************/
/*!
*  \brief          Creates/Opens a flash emulation object by name flashName.
*
******************************************************************************/
template <class T>
Flash_Memory<T>* Flash_Memory<T>::operator%=  (const T &inputData)
{
    writeOperator(data % inputData);
    return this;
}

#endif

