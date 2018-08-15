#ifndef _FlashMemory_H_
#define _FlashMemory_H_

//#include "emulator_types.h"
//#include "util.h"

#ifdef EMOTE
#define NUMBER_OF_FLASHBLOCKS 256
#else
#define NUMBER_OF_FLASHBLOCKS 128
#endif

#define BITWISE_AND &
#define NUMBER_OF_CLUSTERS 1


/* Pragma to disable any possible padding that would be critical problem */
//#pragma pack(push, 1)

class FlashMemory
{
private:
	int data;

    /* Generic write function that is being used by all the other operator functions */
	void writeOperator(const int data);
public:
    /* Constructor (that is never ever expected to be called :) ) */
	FlashMemory();

    /* Operator overloading to overload all possible assignment operations with
       FlashMemory on RHS */
	FlashMemory* operator=   (const FlashMemory &inputData);
	FlashMemory* operator-=  (const FlashMemory &inputData);
	FlashMemory* operator+=  (const FlashMemory &inputData);
	FlashMemory* operator*=  (const FlashMemory &inputData);
	FlashMemory* operator/=  (const FlashMemory &inputData);
    FlashMemory* operator<<= (const FlashMemory &inputData);
	FlashMemory* operator>>= (const FlashMemory &inputData);
	FlashMemory* operator&=  (const FlashMemory &inputData);
	FlashMemory* operator^=  (const FlashMemory &inputData);
	FlashMemory* operator|=  (const FlashMemory &inputData);
	FlashMemory* operator%=  (const FlashMemory &inputData);

    /* Operator overloading to overload all possible assignment operations with
       Type T on RHS */
    FlashMemory* operator=   (const int &inputData);
	FlashMemory* operator-=  (const int &inputData);
	FlashMemory* operator+=  (const int &inputData);
	FlashMemory* operator*=  (const int &inputData);
	FlashMemory* operator/=  (const int &inputData);
    FlashMemory* operator<<= (const int &inputData);
	FlashMemory* operator>>= (const int &inputData);
	FlashMemory* operator&=  (const int &inputData);
	FlashMemory* operator^=  (const int &inputData);
	FlashMemory* operator|=  (const int &inputData);
	FlashMemory* operator%=  (const int &inputData);

    /* Defining the explicit typecasting - This will ease the coding
       greatly-Believe me :) */
	// Native data type now removing templates from the equation
	// The template aspect will never be used
#if 0
    operator T (){
        return data;
    }
#endif
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
FlashMemory::FlashMemory()
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
void FlashMemory::writeOperator(const int data)
{
	register char *writePtr; //= (char*)lookUpAddress(this);      /* Char* because I want to handle data one-byte at a time */
	char *inputDataCharPtr = (char*)&data;

    //EMULATOR_ASSERT( writePtr != NULL , "Failed to map ReadOnlyPTR to writeable ptr!");

    for(int index = 0; index < sizeof(int); index++){
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
FlashMemory* FlashMemory::operator=(const FlashMemory &inputData)
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
FlashMemory* FlashMemory::operator-=(const FlashMemory &inputData)
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
FlashMemory* FlashMemory::operator+=(const FlashMemory &inputData)
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
FlashMemory* FlashMemory::operator*=(const FlashMemory &inputData)
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
FlashMemory* FlashMemory::operator/=(const FlashMemory &inputData)
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

FlashMemory* FlashMemory::operator<<=(const FlashMemory &inputData)
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

FlashMemory* FlashMemory::operator>>=(const FlashMemory &inputData)
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

FlashMemory* FlashMemory::operator&=(const FlashMemory &inputData)
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

FlashMemory* FlashMemory::operator^=(const FlashMemory &inputData)
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

FlashMemory* FlashMemory::operator|=(const FlashMemory &inputData)
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

FlashMemory* FlashMemory::operator%=(const FlashMemory &inputData)
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

FlashMemory* FlashMemory::operator=(const int &inputData)
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

FlashMemory* FlashMemory::operator-=  (const int &inputData)
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

FlashMemory* FlashMemory::operator+=  (const int &inputData)
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

FlashMemory* FlashMemory::operator*=  (const int &inputData)
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

FlashMemory* FlashMemory::operator/=  (const int &inputData)
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

FlashMemory* FlashMemory::operator<<= (const int &inputData)
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

FlashMemory* FlashMemory::operator>>= (const int &inputData)
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

FlashMemory* FlashMemory::operator&=  (const int &inputData)
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
FlashMemory* FlashMemory::operator^=  (const int &inputData)
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
FlashMemory* FlashMemory::operator|=  (const int &inputData)
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

FlashMemory* FlashMemory::operator%=  (const int &inputData)
{
    writeOperator(data % inputData);
    return this;
}

#endif

