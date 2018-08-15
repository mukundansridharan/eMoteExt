/*
 * DataTransmissionHandler.h
 *
 *  Created on: Apr 30, 2013
 *      Author: Mukundan
 */

#ifndef DATATRANSMISSIONHANDLER_H_
#define DATATRANSMISSIONHANDLER_H_

#include <tinyhal.h>
#include <Samraksh/Message.h>
//#include <Samraksh/HALTimer.h>
#include "OMACConstants.h"
#include "Handlers.h"

#define OMAC_DTH_TIMER_TARGET_TIME_CORRECTION 1

#define OMAC_DTH_DEBUG_PRINTF_PACKET_SEND 0

#define OMAC_DTH_DEBUG_ReceiveDATAACK 0
#define OMAC_DTH_DEBUG_ReceiveDATAACK_PRINTOUT 0
#define OMAC_DTH_DEBUG_SendACKHandler 0
#define OMAC_DTH_DEBUG_CCA 0

#define OMAC_DTH_DEBUG_UNEXPECTED_POST_EX 0
#define OMAC_DTH_DEBUG_LATEWAKEUP 0
#define OMAC_DTH_DEBUG_LATEWAKEUP_PIN_TOGGLING 0
#define OMAC_DTH_DELAY_FROM_SEND_TO_RADIO_DRIVER_SEND_IN_TICKS 8*1116
#if OMAC_DTH_DEBUG_LATEWAKEUP
#define OMAC_DTH_DEBUG_LATEWAKEUP_ALLOWANCE_IN_TICKS 259*8
#endif

#define OMAC_SEND_DEBUGGING_FOR_MF 0

#define OMAC_DTH_PRINT_NEIGHBOR_TABLE_AFTER_SEND 0

//#include "Scheduler.h"
enum DataTransmissionHandlerStates{
	DTS_EXECUTE_START,
	DTS_RADIO_START_SUCCESS,
	DTS_RADIO_START_FAILED,
	DTS_BACKOFF,
	DTS_DEST_NOT_AVAILABLE,
	DTS_SEND_INITIATION_SUCCESS,
	DTS_SEND_INITIATION_FAIL,
	DTS_CCA_CLEAR,
	DTS_CCA_BUSY,
	DTS_SEND_FINISHED,
	DTS_WAITING_FOR_ACKS,
	DTS_RECEIVEDDATAACK,
	DTS_POSTEXECUTION,
	DTS_SEND_INITIATED,
	DTS_SEND_PACKET_NOT_READY,
	DTS_PACKET_SELECTED,
	DTS_PACKET_NOTSELECTED
};

//page 70 in RF231 datasheet
enum TRAC_STATUS
{
	TRAC_STATUS_SUCCESS = 0x00,
	TRAC_STATUS_SUCCESS_DATA_PENDING = 0x20,	//0010 0000 (32)
	TRAC_STATUS_SUCCESS_WAIT_FOR_ACK = 0x40,	//0100 0000 (64)
	TRAC_STATUS_CHANNEL_ACCESS_FAILURE = 0x60,	//0110 0000 (96)
	TRAC_STATUS_FAIL_TO_SEND = 0x80,			//1000 0000 (128)
	TRAC_STATUS_NO_ACK = 0xA0,					//1010 0000 (160)
	TRAC_STATUS_INVALID	= 0xE0,					//1110 0000 (224)
};


/*
 *
 */
class DataTransmissionHandler: public EventHandler {

	//Message_15_4_t m_piggybackBeaconCopy;
	Message_15_4_t* m_outgoingEntryPtr; //Pointer to the packet to be sent next. Set by schedule data packet
	//UINT8 m_outgoingEntryPtr_pos; //Position of the packet in the buffer to be sent next set by next_event
	UINT16 m_outgoingEntryPtr_dest;

	//Message_15_4_t m_TXMsgBuffer;
	//DataMsg_t *m_TXMsg;
	BOOL isDataPacketScheduled;
	UINT8 m_currentSlotRetryAttempt;

#if OMAC_DTH_DEBUG_LATEWAKEUP
	UINT64 m_scheduledFUTime_in_own_clock_micro;

	UINT64 m_scheduledTXTime_in_neigh_clock_ticks;
	UINT64 m_scheduledTXTime_in_own_clock_ticks;
#endif
#if OMAC_DTH_TIMER_TARGET_TIME_CORRECTION||OMAC_DTH_DEBUG_UNEXPECTED_POST_EX
public:
	UINT64 m_scheduledTimer_in_ticks;
	UINT64 m_scheduledTimer_in_ticks2;
	UINT64 m_curTime_in_ticks;
	UINT64 m_TimeDiff_in_micros;
#endif
private:
	BOOL m_RANDOM_BACKOFF;
	UINT16 m_backoff_seed;
	UINT16 m_backoff_mask;

	DataTransmissionHandlerStates txhandler_state;
	UINT64 CalculateNextTxMicro(UINT16 dest);
	void SelectRetrySlotNumForNeighborBackOff();
public:
	void SendACKToUpperLayers(Message_15_4_t* msg, UINT16 Size, NetOpStatus status, UINT8 radioAckStatus);
private:
	Message_15_4_t* SelectPacketForDest(UINT16 m_outgoingEntryPtr_dest_);


public:
	void Initialize();
	UINT64 NextEvent();
	//UINT64 NextEvent(UINT64 currentTicks);
	void ExecuteEvent();
	void ExecuteEventHelper();
	//UINT8 ExecuteEventDone();
	void PostExecuteEvent();

	//BOOL ScheduleDataPacket(UINT8 _skipperiods);

	void DropPacket();
	bool Send();
	void SendRetry();
	void SendACKHandler(Message_15_4_t* rcv_msg, UINT8 radioAckStatus);
	void ReceiveDATAACK(UINT16 sourceaddress);
	void FailsafeStop();

	UINT64 CalculateNextRXOpp(UINT16 dest);
	BOOL UpdateNeighborsWakeUpSlot(UINT16 dest, UINT8 _skipperiods);
};


#endif /* DATATRANSMISSIONHANDLER_H_ */
