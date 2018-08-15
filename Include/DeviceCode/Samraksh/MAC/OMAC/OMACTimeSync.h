#ifndef OMACTIMESYNC_H_
#define OMACTIMESYNC_H_

#include <Samraksh\PacketTimeSync_15_4.h>
//#include "PacketTimeSync_15_4.h"
#include <Samraksh/Message.h>
#include <Samraksh/Hal_util.h>
#include <Samraksh/GlobalTime.h>

#define OMAC_TSYNC_DEBUG_PRINTF_PACKET_ENQUE 0

#define DEBUG_TSYNC_PIN
/*
 *
 */
//typedef class OMACTimeSync : public TimeSyncMessage_15_4  {
class OMACTimeSync : public EventHandler{
	UINT8 RadioID;
	UINT8 MacID;
	Message_15_4_t m_timeSyncMsgBuffer;
	TimeSyncMsg *m_timeSyncMsg;
	UINT64 m_messagePeriod;
	//PacketTimeSync_15_4 m_packetTimeSync;
	UINT32 m_seqNo;

	UINT64 m_inter_clock_offset;

public:
	GlobalTime m_globalTime;

	void Initialize(UINT8 radioID, UINT8 macID);
	UINT64 NextEvent();
  	UINT16 NextEventinSlots();
	//UINT64 NextEvent(UINT64 currentTicks);
	void ExecuteEvent();
	UINT8 ExecuteEventDone();
	void PostExecuteEvent();
	void CreateMessage(TimeSyncMsg* timeSyncMsg, UINT64 curticks, bool request_TimeSync = false);
	void FailsafeStop();
	//UINT32 GetSyncPoint();
	DeviceStatus Receive(RadioAddress_t msg_src, TimeSyncMsg* rcv_msg, UINT64 SenderDelay, UINT64 ReceiveTS);
	DeviceStatus ReceiveTSReq(RadioAddress_t msg_src, TimeSyncRequestMsg* rcv_msg);

	BOOL Send(RadioAddress_t address);



};


#endif /* OMACTIMESYNC_H_ */
