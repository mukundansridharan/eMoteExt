/*
 * Scheduler.h
 *
 *  Created on: Aug 30, 2012
 *      Author: Mukundan
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

//#include <Samraksh/Hal_util.h>
//#include <Samraksh/HALTimer.h>

#include "OMACConstants.h"
#include "Handlers.h"
#include "RadioControl.h"
#include "DiscoveryHandler.h"
#include "DataReceptionHandler.h"
#include "DataTransmissionHandler.h"
//#include "GlobalTime.h"
#include "OMACTimeSync.h"
#include "SeedGenerator.h"

//extern HALTimerManager gHalTimerManagerObject;


#define OMAC_SCHEDULER_TIMER_TARGET_TIME_CORRECTION 1
#define OMAC_SCEHDULER_EASY_DEBUG 0

#define MAXSCHEDULERUPDATE 5000000
#define DISCO_SLOT_GUARD 10


/*
 *
 */
class OMACScheduler{
#if OMAC_SCHEDULER_TIMER_TARGET_TIME_CORRECTION
	public:
	UINT64 m_scheduledTimer_in_ticks;
	UINT64 m_curTime_in_ticks;
	UINT8 m_num_rescheduled;
#endif


private:

	UINT8 radioID;
	UINT8 macID;

	HandlerType_t m_lastHandler;
	UINT8 m_num_sleep_retry_attempts;

	UINT8 m_num_FailsafeStop;
	bool SchedulerINUse;



public:

	UINT64 m_scheduledFailSafeTimer_in_ticks;
	UINT8 m_num_FailsafeTimerFiring;


	OMACSchedulerState_t m_state;		//The state of the scheduler
	bool m_execution_started;

	DiscoveryHandler m_DiscoveryHandler;
	DataReceptionHandler m_DataReceptionHandler;
	DataTransmissionHandler m_DataTransmissionHandler;
	OMACTimeSync m_TimeSyncHandler;
	SeedGenerator m_seedGenerator;

#if OMAC_SCEHDULER_EASY_DEBUG
	UINT64 nextWakeupTimeInMicSec;
#endif
	//UINT64 m_InitializationTimeinTicks; //The time val


	//BK The methods from this point on is in use for sure. The rest can be deleted.
	//TODO: Clean up the code by removing unused variables and methods
	//Define public methods
	void Initialize(UINT8 radioID, UINT8 macID);
	void UnInitialize();
	UINT64 GetSlotNumber();
	UINT64 GetSlotNumberfromTicks(const UINT64 &y);
	UINT32 GetSlotNumberfromMicroSec(const UINT64 &y);
	UINT32 GetTimeTillTheEndofSlot();
	void ScheduleNextEvent();


	//Main Tasks
	bool RunEventTask(); // BK: This is the main event for the slot. Queries all the modules and executes one if needed
	void FailsafeStop();
	void PostExecution();
	void PostPostExecution();
	bool EnsureStopRadio();

};




#endif /* SCHEDULER_H_ */
