#ifndef OMACCLOK_H_
#define OMACCLOK_H_

#include <Samraksh/Hal_util.h>
#include <Samraksh/VirtualTimer.h>

#define DEBUG_TSYNC_PIN

class OMACClock{
	//UINT64 m_inter_clock_offset;
	UINT64 m_first_clock_reading;
public:
	OMACClock(){
//		m_inter_clock_offset = 0;
		m_first_clock_reading = GetCurrentTimeinTicks();
	}
	UINT64 GetCurrentTimeinTicks(){ //This function gets the time ticks required for OMAC
		if(OMACClockSpecifier == LFCLOCKID){
			return VirtTimer_GetTicks(VIRT_TIMER_OMAC_SCHEDULER) * OMACClocktoSystemClockFreqRatio - m_first_clock_reading; //+ m_inter_clock_offset;
		}
		else{
			return HAL_Time_CurrentTicks() - m_first_clock_reading;
		}
	};
	UINT64 ConvertTickstoMicroSecs(const UINT64& ticks){ //This function gets the time ticks required for OMAC
		//return VirtTimer_GetTicks(VIRT_TIMER_OMAC_SCHEDULER) * OMACClocktoSystemClockFreqRatio;
		return ticks/8;
	};
	UINT64 ConvertMicroSecstoTicks(const UINT64& microsecs){ //This function gets the time ticks required for OMAC
		//return VirtTimer_GetTicks(VIRT_TIMER_OMAC_SCHEDULER) * OMACClocktoSystemClockFreqRatio;
		return microsecs*8;
	};
	UINT64 ConvertSlotstoTicks(const UINT64& slots){ //This function gets the time ticks required for OMAC
		//return VirtTimer_GetTicks(VIRT_TIMER_OMAC_SCHEDULER) * OMACClocktoSystemClockFreqRatio;
		return slots*16000*8;
	};

	void CreateSyncPointBetweenClocks(){
//		if(0 && OMACClockSpecifier == LFCLOCKID){
//			m_inter_clock_offset = HAL_Time_CurrentTicks() - VirtTimer_GetTicks(VIRT_TIMER_OMAC_SCHEDULER) * OMACClocktoSystemClockFreqRatio ;
//		}
	};
	inline UINT64 AddTicks(const UINT64& ticks1, const UINT64& ticks2){
		return (ticks1 + ticks2);
	};
	inline UINT64 SubstractTicks(const UINT64& ticks1, const UINT64& ticks2){
		ASSERT(ticks1 >= ticks2);
		return ( (ticks1 > ticks2) ? (ticks1 - ticks2) : 0 );
	};
	inline UINT64 SubstractMicroSeconds(const UINT64& ticks1, const UINT64& ticks2){
		ASSERT(ticks1 >= ticks2);
		return ( (ticks1 > ticks2) ? (ticks1 - ticks2) : 0);
	};
	inline UINT64 AddMicroSeconds(const UINT64& ticks1, const UINT64& ticks2){
		return (ticks1 + ticks2);
	};


};

/*
OMACClock::OMACClock(){
	m_inter_clock_offset = 0;
}

UINT64 OMACClock::CreateSyncPointBetweenClocks(){
	if(0 && OMACClockSpecifier == LFCLOCKID){
		m_inter_clock_offset = HAL_Time_CurrentTicks() - VirtTimer_GetTicks(VIRT_TIMER_OMAC_SCHEDULER) * OMACClocktoSystemClockFreqRatio ;
	}
}

UINT64 OMACClock::GetCurrentTimeinTicks(){ //This function gets the time ticks required for OMAC
	if(OMACClockSpecifier == LFCLOCKID){
		return VirtTimer_GetTicks(VIRT_TIMER_OMAC_SCHEDULER) * OMACClocktoSystemClockFreqRatio + m_inter_clock_offset;
	}
	else{
		return HAL_Time_CurrentTicks();
	}
}

UINT64 OMACClock::ConvertTickstoMicroSecs(const UINT64& ticks){ //This function gets the time ticks required for OMAC
	//return VirtTimer_GetTicks(VIRT_TIMER_OMAC_SCHEDULER) * OMACClocktoSystemClockFreqRatio;
	return HAL_Time_TicksToTime(ticks);
}
*/

#endif /* OMACCLOK_H_ */
