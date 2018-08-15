#ifndef HAL_UTIL_H_
#define HAL_UTIL_H_

enum DeviceStatus{
DS_Success,  //TODO: change mapping to DS_Fail=0, DS_Success, ... so BOOL would map correctly
DS_Fail,
DS_Ready,
DS_Busy,
DS_Timeout,
DS_Bug,
};


enum NetOpStatus{
NetworkOperations_Success,
NetworkOperations_Fail,
NetworkOperations_Busy,
NetworkOperations_Full,
NetworkOperations_Ready,
NetworkOperations_BadPacket,
NetworkOperations_Collision,
NetworkOperations_SendInitiated,
NetworkOperations_SendACKed,
NetworkOperations_SendNACKed,
NetworkOperations_SendFailed
};




enum InteropNetOpStatus{
/// <summary>
        /// Radio initialization failed
        /// </summary>
        E_RadioInit,
        /// <summary>
        /// 
        /// </summary>
        E_RadioSync,
        /// <summary>
        /// Radio configuration failed
        /// </summary>
        E_RadioConfig,
        /// <summary>
        /// Mac layer initialization failed
        /// </summary>
        E_MacInit,
        /// <summary>
        /// Mac configuration failed
        /// </summary>
        E_MacConfig,
        /// <summary>
        /// Mac layer send failed
        /// </summary>
        E_MacSendError,
        /// <summary>
        /// Mac layer bufferis full
        /// </summary>
        E_MacBufferFull,
        /// <summary>
        /// Success
        /// </summary>
        S_Success
};



#if defined(SAM_USE_ATOMIC_LOCK)
// Implementing atomic class akin to linux
// you can now do thread safe atomic increments and decrements

typedef volatile int atomic_t;

BOOL atomic_init(atomic_t *lock, int val)
{
	int result = 0;
#if 0
	__asm__(
			"LDREX r0, %[lockin]\n\t"
			 "MOV r0,%[value]\n\t"
			 "STREX r1, r0, %[lockout]\n\t"
			 "MOV r1, %[result]\n\t"
			:[lockout] "=r" (*lock),[result]"=r" (result)
			:[lockin] "r" (*lock), [value] "r" (val)
			: /* No clobbers */
			 );

	if(result)
		return TRUE;
	else
		return FALSE;
#endif
	return TRUE;

}

BOOL atomic_incr(atomic_t *lock)
{
	int result = 0;
#if 0
	__asm__("LDREX r0, %[lockin]\n\t"
			"ADD r0,r0,1\n\t"
			"STREX %[result], r0, %[lockout]\n\t"
			:[lockout] "=r" (*lock),[result]"=r"(result)
			:[lockin] "r" (*lock)
			:);

	if(result)
		return TRUE;
	else
		return FALSE;
#endif

	return TRUE;
}

BOOL atomic_decr(atomic_t *lock)
{
	int result = 0;
#if 0
	__asm__("LDREX r0,%[lockin]\n\t"
			"SUB r0,r0,1\n\t"
			"STREX %[result],r0,%[lockout]\n\t"
			:[lockout] "=r" (*lock),[result]"=r"(result)
			:[lockin] "r" (*lock)
			:);

	if(result)
		return TRUE;
	else
		return FALSE;
#endif
	return TRUE;
}

BOOL atomic_test(atomic_t *lock, BOOL *resultOfTest)
{
	int result = 0;

#if 0
	__asm__("LDREX r0,%[lockin]\n\t"
			"CMP r0,0\n\t"
			"IT EQ\n\t"
			"MOVEQ %[resultOfTest],#1\n\t"
			"STREX %[result],r0,%[lockout]\n\t"
			:[lockout]"=r" (*lock),[result]"=r"(result),[resultOfTest]"=r"(*resultOfTest)
			:[lockin]"r"(*lock)
			:);

	if(result)
		return TRUE;
	else
		return FALSE;
#endif
	return TRUE;
}

BOOL atomic_decr_and_test(atomic_t *lock, BOOL *resultOfTest)
{
	int result = 0;
#if 0
	__asm__("LDREX r0,%[lockin]\n\t"
			"SUB r0, r0, 1\n\t"
			"CMP r0,0\n\t"
			"IT EQ\n\t"
			"MOVEQ %[resultOfTest],#1\n\t"
			"STREX %[result],r0,%[lockout]\n\t"
			:[lockout]"=r" (*lock),[result]"=r"(result),[resultOfTest]"=r"(*resultOfTest)
			:[lockin]"r"(*lock)
			:);

	if(result)
		return TRUE;
	else
		return FALSE;
#endif
	return TRUE;

}


class LOCK
{
	volatile int refCounter;

public:
	BOOL init()
	{
		if(!atomic_init(&refCounter,0))
			return FALSE;

		return TRUE;
	}

	// If a lock is acquired while a lock is already held this
	// function ensures that the lock is not released at the
	// end
	BOOL acquire_and_save()
	{
		if(!atomic_incr(&refCounter))
			return FALSE;

		__asm volatile("cpsid i");

		return TRUE;
	}

	BOOL acquire()
	{
		if(!atomic_incr(&refCounter))
			return FALSE;

		__asm volatile("cpsid i");

		return TRUE;

	}

	BOOL release_and_restore()
	{
		BOOL result = FALSE;

		return force_release();

		if(!atomic_test(&refCounter, &result))
			return FALSE;


		if(result)
		{
			__asm volatile("cpsie i");
			return TRUE;
		}

		if(!atomic_decr_and_test(&refCounter, &result))
		{
			return FALSE;
		}

		if(result)
		{
			__asm volatile("cpsie i");
		}
		return TRUE;
	}


	BOOL force_release()
	{
		__asm volatile("cpsie i");
		return  TRUE;
	}

	BOOL release()
	{
		BOOL result = FALSE;

		if(!atomic_test(&refCounter, &result))
			return FALSE;


		if(result)
		{
			__asm volatile("cpsie i");
			return TRUE;
		}

		if(!atomic_decr_and_test(&refCounter, &result))
		{
			return FALSE;
		}


		if(result)
		{
			__asm volatile("cpsie i");
		}

		return TRUE;
	}
};
#endif


#endif /* HAL_UTIL_H_ */
