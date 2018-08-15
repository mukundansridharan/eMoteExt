/*
 * Neighbors.h : Defines and implements the MAC/Link layer Neighbor Table
 *
 *Created on: Oct 25, 2012
 *Author: Mukundan Sridharan
 *
 */

#ifndef NEIGHBORS_H_
#define NEIGHBORS_H_

#include <Samraksh/Mac_decl.h>
#include <Samraksh/Buffer.h>

#define NeighborChanged 1
#define Received 0

//#include "TinyCLR_Runtime.h"

#define Data_Send_Buffer_15_4_t_SIZE		2
#define TimeSync_Send_Buffer_15_4_t_SIZE 	1

extern UINT8 MacName;
#define MAX_NEIGHBORS 12
#define INVALID_NEIGHBOR_INDEX 255
#define INVALID_MACADDRESS 0
#define INVALID_MACADDRESS2 65535
#define ISMAC_VALID(x) ( (x==INVALID_MACADDRESS2 || x==INVALID_MACADDRESS) ? false : true)

#define Total_Buffer_SIZE 18


#define NUM_ENFORCED_TSR_PCKTS_BEFORE_DATA_PCKTS 2


#define NEIGHBORS_DEBUG_QUEUE_INSERT 0
//extern void  ManagedCallback(UINT16 arg1, UINT16 arg2);
//#define DEBUG_NEIGHBORTABLE

#if defined(DEBUG_NEIGHBORTABLE)
#define ENABLE_PIN_NB(x,y) CPU_GPIO_EnableOutputPin(x,y)
#define SET_PIN_NB(x,y) CPU_GPIO_SetPinState(x,y)
#define DEBUG_PRINTF_NB(...) hal_printf(__VA_ARGS__)
#else
#define ENABLE_PIN_NB(x,y)
#define SET_PIN_NB(x,y)
#define DEBUG_PRINTF_NB(...) (void)0
#endif

struct PACK Link_t {
	/* AvgRSSI
	 * Exponentially smoothed (with lambda of 0.2) average of   SINR code as reported by the radio.
	 * 		For RF231 has conversion (-91 + AvgRSSI) provides the true value.
	 * 		For SI radio the conversion is already done at the driver and is cast into uint.
	 *	  	  Hence the conversion to get signed value is -1*( (0xFF - AvgRSSI)+1 ).
	 */
	UINT8 AvgRSSI;
	/*LinkQuality:
	 * As reported by the radio.
	 * For RF231, represent a measure of the BER for the corresponding SINR.
	 * For SI radio, not available. Hence a value of 0 is reported.
	 * Exponentially smoothed with lambda of 0.2.
	 */
	UINT8 LinkQuality;
	/* AveDelay
	 * : Has units of 65.535 (=10^3/(2^19/2^8)) ms. Exponentially smoothed
	 */
	UINT8 AveDelay;

	UINT8 Link_reliability_bitmap;

public:
	Link_t() : AvgRSSI(0), LinkQuality(0), AveDelay(0), Link_reliability_bitmap(0xFF)  {}
	void Initialize(){
		AvgRSSI = 0;
		LinkQuality = 0;
		AveDelay = 0;
		Link_reliability_bitmap = 0x00;
	}
	void RecordPacketSuccess(bool s){
		Link_reliability_bitmap = Link_reliability_bitmap>>1;
		if(s){
			Link_reliability_bitmap = Link_reliability_bitmap | 0x80;
		}
		else{
			Link_reliability_bitmap = Link_reliability_bitmap & 0x7F;
		}
	}
//	bool IsReliable(){
//		if(Link_reliability_bitmap <= 0x0F ) return false;
//		else return true;
//	}
};

enum NeighborStatus {
	Alive = 0,
	Dead = 1,
	//Suspect = 2,
	NbrStatusError = 0xF
};

typedef struct {
	UINT16 MACAddress;
	UINT8 NumTimeSyncMessagesSent;	//Count of timesync packets sent per neighbor
	bool IsInitializationTimeSamplesNeeded(){
		if(NumTimeSyncMessagesSent < NUM_ENFORCED_TSR_PCKTS_BEFORE_DATA_PCKTS) return true;
		else return false;
	}
	void IncrementNumTimeSyncMessagesSent(){
		if( IsInitializationTimeSamplesNeeded() ) {
			++(NumTimeSyncMessagesSent);
			if(NumTimeSyncMessagesSent == NUM_ENFORCED_TSR_PCKTS_BEFORE_DATA_PCKTS){
				++(NumTimeSyncMessagesSent);
			}
		}

	}
	//Send (formerly forward) link details between current and neighbor node
	Link_t SendLink;
	//Receive (formerly reverse) link details between current and neighbor node
	Link_t ReceiveLink;
	NeighborStatus neighborStatus;
	bool IsAvailableForUpperLayers;
	UINT16 CountOfPacketsReceived;
	UINT64 LastHeardTime;
	UINT8 ReceiveDutyCycle; //percentage
	UINT16 FrameLength;

	UINT16  nextSeed; //the seed we have from in the control message last received
	UINT16 mask;
	UINT64  nextwakeupSlot;
	UINT32  seedUpdateIntervalinSlots;

	UINT64  LastTimeSyncRecvTime;			// Lasst time a time sync message is received
	UINT64  LastTimeSyncRequestTime;	// Last time instant a time sync request is sent
	UINT64  LastTimeSyncSendTime;	// Last time instant a time sync is sent(piggbacked)

	UINT8 random_back_off_slot;
	UINT8 random_back_off_window_size;
	//UINT8   numErrors;
	//UINT8   size;
	//BOOL    isInTransition;
	//UINT32  localAvg;
	//INT32   offsetAvg;
	//float   skew;
	//TODO: BK: DELETE THESE NOT USED BUT KEPT FOR THE TIME BEIGN
	UINT16  radioStartDelay;
	UINT16  counterOffset;

	//	Buffer_15_4<Data_Send_Buffer_15_4_t_SIZE> send_buffer;
	//	Buffer_15_4<TimeSync_Send_Buffer_15_4_t_SIZE> tsr_send_buffer;

	//	Buffer_15_4_T<Data_Send_Buffer_15_4_t_SIZE, Message_15_4_t*> send_buffer;
	//	Buffer_15_4_T<TimeSync_Send_Buffer_15_4_t_SIZE, Message_15_4_t*> tsr_send_buffer;

	void Clear(){
		//		send_buffer.Initialize();
		//		tsr_send_buffer.Initialize();

		MACAddress = INVALID_MACADDRESS;
		NumTimeSyncMessagesSent = 0;
		//		SendLink.AvgRSSI = 0;
		//		SendLink.LinkQuality = 0;
		//		SendLink.AveDelay = 0;
		//		ReceiveLink.AvgRSSI = 0;
		//		ReceiveLink.LinkQuality = 0;
		//		ReceiveLink.AveDelay = 0;
		SendLink.Initialize();
		ReceiveLink.Initialize();

		neighborStatus = Dead;
		IsAvailableForUpperLayers = false;
		CountOfPacketsReceived = 0;
		LastHeardTime = 0;
		ReceiveDutyCycle = 0;
		FrameLength = 0;

		nextSeed = 0;
		mask = 0;
		nextwakeupSlot = 0;
		seedUpdateIntervalinSlots = 0;

		LastTimeSyncRecvTime = 0;			// Lasst time a time sync message is received
		LastTimeSyncRequestTime = 0;	// Last time instant a time sync request is sent
		LastTimeSyncSendTime = 0;	// Last time instant a time sync is sent(piggbacked)

		UINT8 random_back_off_slot = 0;
		UINT8 random_back_off_window_size = 1;
		//UINT8   numErrors;
		//UINT8   size;
		//BOOL    isInTransition;
		//UINT32  localAvg;
		//INT32   offsetAvg;
		//float   skew;
		//		//TODO: BK: DELETE THESE NOT USED BUT KEPT FOR THE TIME BEIGN
		//		UINT16  radioStartDelay;
		//		UINT16  counterOffset;



	}
}Neighbor_t;

class NeighborTableCommonParameters_One_t{
public:
	UINT16 MACAddress;
	NeighborStatus status;
	UINT64 lastHeardTime;
	Link_t linkQualityMetrics;
	NeighborTableCommonParameters_One_t(){
		MACAddress = 0;
		status = NbrStatusError;
		lastHeardTime = 0;
		linkQualityMetrics.LinkQuality = 0;
		linkQualityMetrics.AvgRSSI = 0;
		linkQualityMetrics.AveDelay = 0;
	}
};

class NeighborTableCommonParameters_Two_t{
public:
	UINT16  nextSeed; //the seed we have from in the control message last received
	UINT16 	mask;
	UINT64  nextwakeupSlot;
	UINT32  seedUpdateIntervalinSlots;
	NeighborTableCommonParameters_Two_t(){
		nextSeed = 0;
		mask = 0;
		nextwakeupSlot = 0;
		seedUpdateIntervalinSlots = 0;
	}
};

class NeighborTable {
public:
	UINT8 NumberValidNeighbor;
	UINT8 PreviousNumberValidNeighbor;
	Neighbor_t Neighbor[MAX_NEIGHBORS];
	ArrayBuffer<Total_Buffer_SIZE> send_buffer;


	// neighbor table util functions
	DeviceStatus GetFreeIdx(UINT8* index);
	DeviceStatus ClearNeighbor(UINT16 MACAddress);
	DeviceStatus ClearNeighborwIndex(UINT8 tableIndex);
	DeviceStatus FindIndexEvenDead(const UINT16 MACAddress, UINT8* index);
	DeviceStatus FindIndex(const UINT16 MACAddress, UINT8* index);
	void ClearTable();
	UINT8 BringOutYourDead(UINT32 delay);
	Neighbor_t* GetNeighborPtr(UINT16 address);
	UINT8 NumberOfNeighbors();
	UINT8 NumberOfNeighborsTotal();
	UINT8 PreviousNumberOfNeighbors();
	void SetPreviousNumberOfNeighbors(UINT8 previousNeighborCnt);
	DeviceStatus InsertNeighbor(const NeighborTableCommonParameters_One_t *neighborTableCommonParameters_One_t, const NeighborTableCommonParameters_Two_t *neighborTableCommonParameters_Two_t);
	DeviceStatus FindOrInsertNeighbor(const UINT16 address, UINT8* index);
	DeviceStatus UpdateLink(UINT16 address, Link_t *forwardLink, Link_t *reverseLink, UINT8* index);
	DeviceStatus UpdateFrameLength(UINT16 address, NeighborStatus status, UINT16 frameLength, UINT8* index);
	DeviceStatus UpdateDutyCycle(UINT16 address, UINT8 dutyCycle, UINT8* index);
	DeviceStatus UpdateNeighbor(const NeighborTableCommonParameters_One_t *neighborTableCommonParameters_One_t, const NeighborTableCommonParameters_Two_t *neighborTableCommonParameters_Two_t);
	//DeviceStatus UpdateNeighbor(UINT16 address, NeighborStatus status, UINT64 currTime, UINT16  lastSeed, UINT16  dataInterval, UINT16  radioStartDelay, UINT16  counterOffset, UINT8* index);
	DeviceStatus UpdateNeighbor(const NeighborTableCommonParameters_One_t *neighborTableCommonParameters_One_t);
	UINT8  UpdateNeighborTable(UINT64 livelinessDelayInTicks, UINT64 currentTime);
	UINT8  UpdateNeighborTable(UINT32 NeighborLivenessDelay);

	DeviceStatus RecordTimeSyncRequestSent(UINT16 address, UINT64 _LastTimeSyncTime);
	DeviceStatus RecordTimeSyncSent(UINT16 address, UINT64 _LastTimeSyncTime);
	DeviceStatus RecordTimeSyncRecv(UINT16 address, UINT64 _lastTimeSyncRecv);
	UINT64 GetLastTimeSyncRecv(UINT16 address);
	Neighbor_t* GetMostObsoleteTimeSyncNeighborPtr(NeighborStatus ns = Alive);
	//Neighbor_t* GetNeighborWOldestSyncPtr(const UINT64& curticks, const UINT64& request_limit);
	Neighbor_t* GetCritalSyncNeighborWOldestSyncPtr(const UINT64& curticks, const UINT64& request_limit,const UINT64& forcererequest_limit, const UINT64& fast_disco_request_interval);
	void DegradeLinks();
	UINT16 GetMaxNeighbors();

	DeviceStatus RecordLastHeardTime(UINT16 MACAddress, UINT64 currTime);
	DeviceStatus RecordSenderDelayIncoming(UINT16 MACAddress, const UINT8& delay);


	UINT8 NumberofNeighborsWithNoPacketinQueue(){
		UINT8 numneigh = 0;
		for (UINT8 tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
			if(ISMAC_VALID(Neighbor[tableIndex].MACAddress)){
				if(!IsThereAPacketWithDest(Neighbor[tableIndex].MACAddress)){
					++numneigh;
				}
			}
			else{
				++numneigh;
			}
		}
		return numneigh;
	}

	bool DeletePacket(Message_15_4_t* msg_carrier){
		//		for(UINT8 i = 0; i < MAX_NEIGHBORS ; ++i){
		//			if(Neighbor[i].MACAddress == msg_carrier->GetHeader()->dest){
		//				if(msg_carrier == FindTSRPacketForNeighbor(Neighbor[i].MACAddress)){
		//					Neighbor[i].tsr_send_buffer.DropOldest(1);
		//				}
		//				else if(msg_carrier == FindDataPacketForNeighbor(Neighbor[i].MACAddress)){
		//					Neighbor[i].send_buffer.DropOldest(1);
		//				}
		//			}
		//		}
		return send_buffer.DeletePacket(msg_carrier);
	}
	Message_15_4_t* FindStalePacketWithRetryAttemptsGreaterThan(UINT8 retryLimit,  BufferOwner n_buf_ow){
		return send_buffer.FindStalePacketWithRetryAttemptsGreaterThan(retryLimit,  n_buf_ow);
	}

	bool ChangeOwnerShipOfElementwIndex(PacketID_T index, BufferOwner n_buf_ow){
		return send_buffer.ChangeOwnerShipOfElementwIndex(index, n_buf_ow);
	}

	/*void FindAndRemoveStalePackets(UINT8 retryLimit){
		send_buffer.RemovePacketsWithRetryAttemptsGreaterThan(retryLimit);
		//		for(UINT8 i = 0; i < MAX_NEIGHBORS ; ++i){
		//			if(Neighbor[i].neighborStatus != Dead){
		////				if( g_OMAC.m_omac_scheduler.m_TimeSyncHandler.m_globalTime.IsNeighborTimeAvailable(Neighbor[i].MACAddress)){
		//					while(Neighbor[i].tsr_send_buffer.GetNumberMessagesInBuffer() > 0
		//							&& Neighbor[i].tsr_send_buffer.GetOldestwithoutRemoval()->GetMetaData()->GetRetryAttempts() > retryLimit // This can be handled more gracefully
		//					){
		//						Message_15_4_t* msg = Neighbor[i].tsr_send_buffer.GetOldestwithoutRemoval();
		//
		//						ClearMsgContents(msg);
		//						Neighbor[i].tsr_send_buffer.DropOldest(1);
		//						Neighbor[i].SendLink.RecordPacketSuccess(false);
		//					}
		//					while(Neighbor[i].send_buffer.GetNumberMessagesInBuffer() > 0
		//							&& Neighbor[i].send_buffer.GetOldestwithoutRemoval()->GetMetaData()->GetRetryAttempts() > retryLimit
		//					){
		//						Message_15_4_t* msg = Neighbor[i].send_buffer.GetOldestwithoutRemoval();
		//
		//						ClearMsgContents(Neighbor[i].send_buffer.GetOldestwithoutRemoval());
		//						Neighbor[i].send_buffer.DropOldest(1);
		//						Neighbor[i].SendLink.RecordPacketSuccess(false);
		//					}
		////				}
		//			}
		//		}
	}*/
	bool IsThereAPacketWithDest(const UINT16 address){
		if(IsThereADataPacketWithDest(address) || IsThereATSRPacketWithDest(address)){
			return true;
		}
		return false;
	}
	bool IsThereADataPacketWithDest(const UINT16 address){
		//		for(UINT8 i = 0; i < MAX_NEIGHBORS ; ++i){
		//			if(Neighbor[i].MACAddress == address){
		//				if(Neighbor[i].send_buffer.GetNumberMessagesInBuffer() > 0){
		//					return true;
		//				}
		//				return false;
		//			}
		//		}
		//		return false;
		return send_buffer.IsThereADataPacketWithDest(address);
	}
	bool IsThereATSRPacketWithDest(const UINT16 address){
		//		for(UINT8 i = 0; i < MAX_NEIGHBORS ; ++i){
		//			if(Neighbor[i].MACAddress == address){
		//				if(Neighbor[i].tsr_send_buffer.GetNumberMessagesInBuffer() > 0){
		//					return true;
		//				}
		//				return false;
		//			}
		//		}
		//		return false;
		return send_buffer.IsThereATSRPacketWithDest(address);
	}
	Message_15_4_t* FindTSRPacketForNeighbor(const UINT16 neigh){
		//		Neighbor_t* neigh_ptr = GetNeighborPtr(neigh);
		//		if(neigh_ptr == NULL){
		//			return NULL;
		//		}
		//		else{
		//			return(neigh_ptr->tsr_send_buffer.GetOldestwithoutRemoval());
		//		}
		return send_buffer.FindTSRPacketForNeighbor(neigh);
	}
	Message_15_4_t* FindDataPacketForNeighbor(const UINT16 neigh){
		//		Neighbor_t* neigh_ptr = GetNeighborPtr(neigh);
		//		if(neigh_ptr == NULL){
		//			return NULL;
		//		}
		//		else{
		//			return(neigh_ptr->send_buffer.GetOldestwithoutRemoval());
		//		}
		return send_buffer.FindDataPacketForNeighbor(neigh);
	}
	bool InsertMessage(Message_15_4_t* msg_carrier){
		if(ISPACKET_ID_VALID(InsertMessageGetIndex(msg_carrier)) ) return true;
		else return false;
	}
	PacketID_T InsertMessageGetIndex(Message_15_4_t* msg_carrier){ //TODO: BK: This needs to be revisited. The criteria should not be whether there is a packet for a destination but whether there is a packet for a destination that piggybacks time info

		if(msg_carrier){ // If the msg is valid
			if(IsThereAPacketWithDest(msg_carrier->GetHeader()->dest)){//Preserve space for nodes that do not have any packets in the buffer
				UINT8 numneigh = NumberofNeighborsWithNoPacketinQueue();
				if(numneigh < Total_Buffer_SIZE - send_buffer.GetNumberofElements()){ //If the remaining space is larger than the amount of space that needs to be preserved
					while(IsThereATSRPacketWithDest(msg_carrier->GetHeaderConst()->GetDestConst())) {
						DeletePacket(FindTSRPacketForNeighbor(msg_carrier->GetHeaderConst()->GetDestConst()));
					}
	#if NEIGHBORS_DEBUG_QUEUE_INSERT
					hal_printf("\n There is enough space in queue attempting to insert");
	#endif
					return send_buffer.InsertMessageGetIndex(msg_carrier);
				}
				else{ // Refuse packet If we need to preserve empty space
	#if NEIGHBORS_DEBUG_QUEUE_INSERT
					hal_printf("\nRefuse packet due to need for preserving space numneigh = %u ,  nuel = %u \n", numneigh, send_buffer.GetNumberofElements());
	#endif
					return INVALID_PACKET_ID;
				}

			}
			else{ //This dest has no packets directly insert
#if NEIGHBORS_DEBUG_QUEUE_INSERT
				hal_printf("\n There might be critical space in queue but inserting since this since no other packets exists for this dest");
#endif
				return send_buffer.InsertMessageGetIndex(msg_carrier);
//				if(msg_carrier->GetHeader()->flags & (TIMESTAMPED_FLAG)){
//#if NEIGHBORS_DEBUG_QUEUE_INSERT
//				hal_printf("\n There might be critical space in queue but inserting since this since no other packets exists and this is a timesync packet");
//#endif
//				return send_buffer.InsertMessageGetIndex(msg_carrier);
//				}
//				else{
//					UINT8 numneigh = NumberofNeighborsWithNoPacketinQueue(); //Preserve space for nodes that do not have any packets in the buffer
//					if(numneigh < Total_Buffer_SIZE - send_buffer.GetNumberofElements()){ //If the remaining space is larger than the amount of space that needs to be preserved
//						while(IsThereATSRPacketWithDest(msg_carrier->GetHeaderConst()->GetDestConst())) {
//							DeletePacket(FindTSRPacketForNeighbor(msg_carrier->GetHeaderConst()->GetDestConst()));
//						}
//		#if NEIGHBORS_DEBUG_QUEUE_INSERT
//						hal_printf("\n There is enough space in queue attempting to insert");
//		#endif
//						return send_buffer.InsertMessageGetIndex(msg_carrier);
//					}
//					else{ // Refuse packet If we need to preserve empty space
//		#if NEIGHBORS_DEBUG_QUEUE_INSERT
//						hal_printf("\nRefuse packet due to need for preserving space numneigh = %u ,  nuel = %u \n", numneigh, send_buffer.GetNumberofElements());
//		#endif
//						return INVALID_PACKET_ID;
//					}
//				}
			}
		}
		else{  // Refuse packet If no packet exists
#if NEIGHBORS_DEBUG_QUEUE_INSERT
			hal_printf("\nRefuse packet due to pointer null\n");
#endif

			return INVALID_PACKET_ID;
		}
	}
	Message_15_4_t* GetPacketPtrWithIndex(PacketID_T index){
		return send_buffer.GetPacketPtrWithIndex(index);
	}
	PacketID_T GetIndexWithPtr(Message_15_4_t* msg_carrier){
		if(msg_carrier){
			return send_buffer.GetIndex(*msg_carrier);
		}
		return INVALID_PACKET_ID;
	}

	void FlushDataPacketQueueForNeighbor(const UINT16 address){
		Message_15_4_t* msg_carrier = FindDataPacketForNeighbor(address);
		while(msg_carrier){
			DeletePacket(msg_carrier);
		}
	}
	void FlushTSRPacketQueueForNeighbor(const UINT16 address){
		Message_15_4_t* msg_carrier = FindTSRPacketForNeighbor(address);
		while(msg_carrier){
			DeletePacket(msg_carrier);
		}
	}

	void FlushAllPacketQueueForNeighbor(const UINT16 address){
		FlushTSRPacketQueueForNeighbor(address);
		FlushDataPacketQueueForNeighbor(address);
	}
};


DeviceStatus NeighborTable::RecordSenderDelayIncoming(UINT16 MACAddress, const UINT8& delay){
	UINT8 index;
	DeviceStatus retValue = FindOrInsertNeighbor(MACAddress, &index);

	if ( (retValue==DS_Success) && ISMAC_VALID(MACAddress)){
		if(Neighbor[index].ReceiveLink.AveDelay == 0) {
			Neighbor[index].ReceiveLink.AveDelay = (UINT8)delay;
		}
		else{
			Neighbor[index].ReceiveLink.AveDelay = (UINT8)((float)Neighbor[index].ReceiveLink.AveDelay*0.8 + (float)delay*0.2);
		}

		return DS_Success;
	}
	else {
		return DS_Fail;
	}
}


DeviceStatus NeighborTable::RecordLastHeardTime(UINT16 MACAddress, UINT64 currTime){
	UINT8 index;
	DeviceStatus retValue = FindOrInsertNeighbor(MACAddress, &index);

	if ( (retValue==DS_Success) && ISMAC_VALID(MACAddress)){
		Neighbor[index].LastHeardTime = currTime;
		return DS_Success;
	}
	else {
		return DS_Fail;
	}
}

UINT16 NeighborTable::GetMaxNeighbors(void){
	return MAX_NEIGHBORS;
}

UINT8 NeighborTable::UpdateNeighborTable(UINT64 livelinessDelayInTicks, UINT64 currentTime)
{
	//	UINT8 deadNeighbors = 0;
	//
	////	UINT64 livelinessDelayInTicks = CPU_MillisecondsToTicks(NeighborLivenessDelay * 1000);
	////
	//
	//	//if (Neighbor[0].Status == Alive)
	//	//	hal_printf("neighbor 0 last time: %lld\tcurrent time: %lld\tlivelinessDelayinticks: %lld\r\n", Neighbor[0].LastHeardTime,  currentTime, livelinessDelayInTicks);
	//
	//	for(UINT16 i = 0; i < MAX_NEIGHBORS; i++)
	//	{
	//		if((Neighbor[i].neighborStatus == Alive) && ((currentTime - Neighbor[i].LastHeardTime) > livelinessDelayInTicks) && (Neighbor[i].LastHeardTime != 0))
	//		{
	//
	//			DEBUG_PRINTF_NB("[NATIVE] Neighbors.h : Removing inactive neighbor %hu \n", Neighbor[i].MACAddress);
	//			Neighbor[i].neighborStatus = Dead;
	//			++deadNeighbors;
	//			--NumberValidNeighbor;
	//		}
	//	}
	//
	//	return deadNeighbors;

	return 0;
}

Neighbor_t* NeighborTable::GetMostObsoleteTimeSyncNeighborPtr(NeighborStatus ns){
	Neighbor_t* rn = NULL;
	int tableIndex;
	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if (Neighbor[tableIndex].neighborStatus == ns){
			if(rn == NULL) {
				rn = &Neighbor[tableIndex];
			}
			else if( Neighbor[tableIndex].LastHeardTime != 0 && rn->neighborStatus <= Neighbor[tableIndex].LastHeardTime){
				rn = &Neighbor[tableIndex];
			}
		}
	}
	return rn;
}


UINT8 NeighborTable::UpdateNeighborTable(UINT32 NeighborLivenessDelay)
{
	return BringOutYourDead(NeighborLivenessDelay);
}

DeviceStatus NeighborTable::FindIndexEvenDead(const UINT16 MACAddress, UINT8* index){
	int tableIndex;

	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if ( (Neighbor[tableIndex].MACAddress == MACAddress)) {
			*index = tableIndex;
			return DS_Success;
		}
	}
	return DS_Fail;
}

DeviceStatus NeighborTable::FindIndex(const UINT16 MACAddress, UINT8* index){
	int tableIndex;

	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if ( (Neighbor[tableIndex].MACAddress == MACAddress) && (Neighbor[tableIndex].neighborStatus == Alive)) {
			*index = tableIndex;
			return DS_Success;
		}
	}
	return DS_Fail;
}

UINT8 NeighborTable::BringOutYourDead(UINT32 delay){

	////	GLOBAL_LOCK(irq);
	//
	//	UINT8 deadNeighbors = 0;
	//
	//	UINT64 livelinessDelayInTicks = CPU_MillisecondsToTicks(delay * 1000);
	//
	//	UINT64 currentTime = HAL_Time_CurrentTicks();
	//
	//
	//	//if (Neighbor[0].Status == Alive)
	//	//	hal_printf("neighbor 0 last time: %lld\tcurrent time: %lld\tlivelinessDelayinticks: %lld\r\n", Neighbor[0].LastHeardTime,  currentTime, livelinessDelayInTicks);
	//
	//	for(UINT16 i = 0; i < MAX_NEIGHBORS; i++)
	//	{
	//		if((Neighbor[i].neighborStatus == Alive) && ((currentTime - Neighbor[i].LastHeardTime) > livelinessDelayInTicks) && (Neighbor[i].LastHeardTime != 0))
	//		{
	//			DEBUG_PRINTF_NB("[NATIVE] Neighbors.h : Removing Neighbor %hu due to inactivity\n", Neighbor[i].MACAddress);
	//			Neighbor[i].neighborStatus = Dead;
	//			deadNeighbors++;
	//			NumberValidNeighbor--;
	//		}
	//	}
	//
	//	return deadNeighbors;

	return 0;
}

DeviceStatus NeighborTable::ClearNeighbor(UINT16 nodeId){
	UINT8 tableIndex;
	if (FindIndex(nodeId, &tableIndex) == DS_Success){
		return ClearNeighborwIndex(tableIndex);
	} else {
		return DS_Fail;
	}
}

DeviceStatus NeighborTable::ClearNeighborwIndex(UINT8 index){
	Message_15_4_t* msg_carrier;
	msg_carrier = FindDataPacketForNeighbor(Neighbor[index].MACAddress);
	while(msg_carrier != NULL){
		DeletePacket(msg_carrier);
		msg_carrier = FindDataPacketForNeighbor(Neighbor[index].MACAddress);
	}
	msg_carrier = FindTSRPacketForNeighbor(Neighbor[index].MACAddress);
	while(msg_carrier != NULL){
		DeletePacket(msg_carrier);
		msg_carrier = FindDataPacketForNeighbor(Neighbor[index].MACAddress);
	}

	Neighbor[index].Clear();

	NumberValidNeighbor--;
	return DS_Success;
}
void NeighborTable::ClearTable(){
	int tableIndex;
	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		ClearNeighborwIndex(tableIndex);
	}
	NumberValidNeighbor = 0;
	send_buffer.Initialize();
}

// neighbor table util functions
DeviceStatus NeighborTable::GetFreeIdx(UINT8* index){
	DeviceStatus rv = DS_Fail;
	int tableIndex;
	*index = INVALID_NEIGHBOR_INDEX;

	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if( !ISMAC_VALID(Neighbor[tableIndex].MACAddress) ){
			*index = tableIndex;
			rv = DS_Success;
			break;
		}
		else if (Neighbor[tableIndex].neighborStatus != Alive){
			*index = tableIndex;
			rv = DS_Success;
		}
	}
	if(rv == DS_Success){
		if(ISMAC_VALID(Neighbor[*index].MACAddress)){
			ClearNeighborwIndex(*index);
			rv = DS_Success;
		}
	}
	return rv;
}

Neighbor_t* NeighborTable::GetNeighborPtr(UINT16 address){
	UINT8 index;

	if (FindIndex(address, &index) != DS_Success){
		return NULL;
	}else {
		return &Neighbor[index];
	}
}

UINT8 NeighborTable::NumberOfNeighbors(){
	//return NumberValidNeighbor;
	UINT8 numneigh = 0;
	for (UINT8 tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if (Neighbor[tableIndex].neighborStatus == Alive){
			++numneigh;
		}
	}
	return numneigh;
}

UINT8 NeighborTable::NumberOfNeighborsTotal(){
	//return NumberValidNeighbor;
	UINT8 numneigh = 0;
	for (UINT8 tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if (ISMAC_VALID(Neighbor[tableIndex].MACAddress)){
			++numneigh;
		}
	}
	return numneigh;
}

UINT8 NeighborTable::PreviousNumberOfNeighbors(){
	return PreviousNumberValidNeighbor;
}

void NeighborTable::SetPreviousNumberOfNeighbors(UINT8 previousNeighborCnt){
	PreviousNumberValidNeighbor = previousNeighborCnt;
}

DeviceStatus NeighborTable::FindOrInsertNeighbor(const UINT16 address, UINT8* index){
	DeviceStatus retValue = DS_Fail;
	if(ISMAC_VALID(address)){
		retValue = FindIndexEvenDead(address, index);
		if(retValue == DS_Fail) {
			retValue = GetFreeIdx(index);
			if(retValue == DS_Success) {
				Neighbor[*index].MACAddress = address;
				Neighbor[*index].NumTimeSyncMessagesSent = 0;
				DEBUG_PRINTF_NB("[NATIVE] Neighbors.h : Inserting Neighbor %hu.\n", address);
			}
		}
		else{
			if(Neighbor[*index].neighborStatus != Alive){
				Neighbor[*index].NumTimeSyncMessagesSent = 0;
			}
		}
	}
	return retValue;
}

DeviceStatus NeighborTable::InsertNeighbor(const NeighborTableCommonParameters_One_t *neighborTableCommonParameters_One_t, const NeighborTableCommonParameters_Two_t *neighborTableCommonParameters_Two_t){
	UINT16 address = neighborTableCommonParameters_One_t->MACAddress;
	UINT8 index;
	DeviceStatus retValue = FindOrInsertNeighbor(address, &index);

	if (retValue == DS_Success && ISMAC_VALID(address)){
		NumberValidNeighbor++;
		Neighbor[index].ReceiveLink.AvgRSSI =  0;
		Neighbor[index].ReceiveLink.LinkQuality =  0;
		Neighbor[index].ReceiveLink.AveDelay =  0;
		Neighbor[index].SendLink.AvgRSSI =  0;
		Neighbor[index].SendLink.LinkQuality =  0;
		Neighbor[index].SendLink.AveDelay =  0;
		retValue = UpdateNeighbor(neighborTableCommonParameters_One_t, neighborTableCommonParameters_Two_t);
	}
	return retValue;
}

DeviceStatus NeighborTable::UpdateLink(UINT16 address, Link_t *forwardLink, Link_t *reverseLink, UINT8* index){
	DeviceStatus retValue = FindIndex(address, index);

	if ((retValue!=DS_Success) && ISMAC_VALID(address)){
		if(forwardLink != NULL){
			Neighbor[*index].SendLink.AveDelay = forwardLink->AveDelay;
			Neighbor[*index].SendLink.AvgRSSI = forwardLink->AvgRSSI;
			Neighbor[*index].SendLink.LinkQuality = forwardLink->LinkQuality;
		}
		if(reverseLink != NULL){
			Neighbor[*index].ReceiveLink.AveDelay = reverseLink->AveDelay;
			Neighbor[*index].ReceiveLink.AvgRSSI = reverseLink->AvgRSSI;
			Neighbor[*index].ReceiveLink.LinkQuality = reverseLink->LinkQuality;
		}
	}
	return retValue;
}

DeviceStatus NeighborTable::UpdateFrameLength(UINT16 address, NeighborStatus status, UINT16 frameLength, UINT8* index){
	DeviceStatus retValue = FindIndex(address, index);

	if ((retValue!=DS_Success) && ISMAC_VALID(address)){
		Neighbor[*index].FrameLength = frameLength;
	}
	return retValue;
}

DeviceStatus NeighborTable::UpdateDutyCycle(UINT16 address, UINT8 dutyCycle, UINT8* index){
	DeviceStatus retValue = FindIndex(address, index);

	if ((retValue!=DS_Success) && ISMAC_VALID(address)){
		Neighbor[*index].ReceiveDutyCycle = dutyCycle;
	}
	return retValue;
}

DeviceStatus NeighborTable::UpdateNeighbor(const NeighborTableCommonParameters_One_t *neighborTableCommonParameters_One_t, const NeighborTableCommonParameters_Two_t *neighborTableCommonParameters_Two_t){
	UINT16 address = neighborTableCommonParameters_One_t->MACAddress;
	NeighborStatus status = neighborTableCommonParameters_One_t->status;
	UINT64 LastHeardTime = neighborTableCommonParameters_One_t->lastHeardTime;
	UINT8 lqi = neighborTableCommonParameters_One_t->linkQualityMetrics.LinkQuality;
	UINT8 rssi = neighborTableCommonParameters_One_t->linkQualityMetrics.AvgRSSI;
	UINT8 delay = neighborTableCommonParameters_One_t->linkQualityMetrics.AveDelay;

	if(status == NbrStatusError){
		return DS_Fail;
	}
	if(LastHeardTime == 0){
		return DS_Fail;
	}

	UINT16 nextSeed = neighborTableCommonParameters_Two_t->nextSeed;
	UINT16 mask = neighborTableCommonParameters_Two_t->mask;
	UINT64 nextwakeupSlot = neighborTableCommonParameters_Two_t->nextwakeupSlot;
	UINT32 seedUpdateIntervalinSlots = neighborTableCommonParameters_Two_t->seedUpdateIntervalinSlots;

	UINT8 index;
	DeviceStatus retValue = FindOrInsertNeighbor(address, &index);
	if (retValue == DS_Success && ISMAC_VALID(address)){
		Neighbor[index].MACAddress = address;
		Neighbor[index].neighborStatus = status;
		Neighbor[index].LastHeardTime = LastHeardTime;
		if (Neighbor[index].ReceiveLink.AvgRSSI == 0) {
			Neighbor[index].ReceiveLink.AvgRSSI = (UINT8)(rssi);
			Neighbor[index].ReceiveLink.LinkQuality = (UINT8)lqi;
		} else {
			Neighbor[index].ReceiveLink.AvgRSSI =  (UINT8)((float)Neighbor[index].ReceiveLink.AvgRSSI*0.8 + (float)rssi*0.2);
			Neighbor[index].ReceiveLink.LinkQuality =  (UINT8)((float)Neighbor[index].ReceiveLink.LinkQuality*0.8 + (float)lqi*0.2);
		}

		Neighbor[index].nextSeed = nextSeed;
		Neighbor[index].mask = mask;
		Neighbor[index].nextwakeupSlot = nextwakeupSlot;
		Neighbor[index].seedUpdateIntervalinSlots = seedUpdateIntervalinSlots;
	}
	return retValue;
}

DeviceStatus NeighborTable::UpdateNeighbor(const NeighborTableCommonParameters_One_t *neighborTableCommonParameters_One_t){
	UINT16 address = neighborTableCommonParameters_One_t->MACAddress;
	NeighborStatus status = neighborTableCommonParameters_One_t->status;
	UINT64 LastHeardTime = neighborTableCommonParameters_One_t->lastHeardTime;
	UINT8 lqi = neighborTableCommonParameters_One_t->linkQualityMetrics.LinkQuality;
	UINT8 rssi = neighborTableCommonParameters_One_t->linkQualityMetrics.AvgRSSI;
	UINT8 delay = neighborTableCommonParameters_One_t->linkQualityMetrics.AveDelay;

	if(status == NbrStatusError){
		return DS_Fail;
	}
	if(LastHeardTime == 0){
		return DS_Fail;
	}

	UINT8 index;
	DeviceStatus retValue = FindOrInsertNeighbor(address, &index);
	if (retValue == DS_Success && ISMAC_VALID(address)){
		Neighbor[index].ReceiveLink.AvgRSSI =  (UINT8)((float)Neighbor[index].ReceiveLink.AvgRSSI*0.8 + (float)rssi*0.2);
		Neighbor[index].ReceiveLink.LinkQuality =  (UINT8)((float)Neighbor[index].ReceiveLink.LinkQuality*0.8 + (float)lqi*0.2);
		Neighbor[index].CountOfPacketsReceived++;
		Neighbor[index].LastHeardTime = LastHeardTime;
		Neighbor[index].neighborStatus = status;
		/*
		Neighbor[index].dataInterval = dataInterval;
		Neighbor[index].radioStartDelay = radioStartDelay;
		Neighbor[index].counterOffset = counterOffset;
		Neighbor[index].lastSeed = seed;
		 */
	}

	return retValue;
}

DeviceStatus NeighborTable::RecordTimeSyncRequestSent(UINT16 address, UINT64 _LastTimeSyncTime){
	UINT8 index;
	DeviceStatus retValue = FindOrInsertNeighbor(address, &index);

	if ( (retValue==DS_Success) && ISMAC_VALID(address)){
		Neighbor[index].LastTimeSyncRequestTime = _LastTimeSyncTime;
		return DS_Success;
	}
	else {
		return DS_Fail;
	}
}

DeviceStatus NeighborTable::RecordTimeSyncSent(UINT16 address, UINT64 _LastTimeSyncTime){
	UINT8 index;
	DeviceStatus retValue = FindOrInsertNeighbor(address, &index);


	if ( (retValue==DS_Success) && ISMAC_VALID(address)){
		Neighbor[index].LastTimeSyncSendTime = _LastTimeSyncTime;
		return DS_Success;
	}
	else {
		return DS_Fail;
	}
}

/*UINT64 NeighborTable::GetLastTimeSyncRequestTime(UINT16 address){ //TODO BK: We should eventually use a class for Neighbor rather than a struct, this would enable protecting variables.
	 UINT8 index;
	 DeviceStatus retValue = FindIndex(address, &index);

	if ( (retValue==DS_Success) && ISMAC_VALID(address)){
		return(Neighbor[index].LastTimeSyncRequestTime);
	}
	else {
		//Bk: Maybe we should abort or define an uninitialized TimeValue here.
		return 0;
	}
}*/

DeviceStatus NeighborTable::RecordTimeSyncRecv(UINT16 address, UINT64 _LastTimeSyncTime){
	UINT8 index;
	DeviceStatus retValue = FindOrInsertNeighbor(address, &index);


	if ( (retValue==DS_Success) && ISMAC_VALID(address)){
		Neighbor[index].LastTimeSyncRecvTime = _LastTimeSyncTime;
		return DS_Success;
	}
	else {
		return DS_Fail;
	}
}

UINT64 NeighborTable::GetLastTimeSyncRecv(UINT16 address){
	UINT8 index;
	DeviceStatus retValue = FindIndex(address, &index);

	if ( (retValue==DS_Success) && ISMAC_VALID(address)){
		return(Neighbor[index].LastTimeSyncRecvTime);
	}
	else {
		return (0);
	}
}





Neighbor_t* NeighborTable::GetCritalSyncNeighborWOldestSyncPtr(const UINT64& curticks, const UINT64& request_limit, const UINT64& forcererequest_limit, const UINT64& fast_disco_request_interval ){
	Neighbor_t* rn = NULL;
	int tableIndex;
	for (tableIndex=0; tableIndex<MAX_NEIGHBORS; tableIndex++){
		if (Neighbor[tableIndex].neighborStatus != Dead){
			//Get neighbor which has to be sent a timesync packet asap
			if(rn == NULL || Neighbor[tableIndex].LastTimeSyncSendTime < rn->LastTimeSyncSendTime || Neighbor[tableIndex].IsInitializationTimeSamplesNeeded() ){ //Consider this neighbor

				if((curticks - Neighbor[tableIndex].LastTimeSyncSendTime > request_limit || curticks - Neighbor[tableIndex].LastTimeSyncRecvTime > forcererequest_limit || Neighbor[tableIndex].IsInitializationTimeSamplesNeeded() )){

					if(Neighbor[tableIndex].LastTimeSyncRequestTime == 0  || curticks - Neighbor[tableIndex].LastTimeSyncRequestTime  > request_limit || curticks - Neighbor[tableIndex].LastTimeSyncRequestTime  > forcererequest_limit ){
						rn = &Neighbor[tableIndex];
					}
					else if(Neighbor[tableIndex].IsInitializationTimeSamplesNeeded() && curticks - Neighbor[tableIndex].LastTimeSyncRequestTime  > fast_disco_request_interval){
						rn = &Neighbor[tableIndex];
					}
				}
			}
		}
	}

	return rn;
}

void NeighborTable::DegradeLinks(){
	UINT8 i=0;
	for (i=0; i < NumberValidNeighbor; i++){
		//Neighbor[index].Status = status;
		/*if(Neighbor[i].ReceiveLink.Quality >2){
				Neighbor[i].ReceiveLink.Quality--;
		}*/
	}
}


struct PACK MACNeighborInfo	//6bytes
{
	UINT16 MACAddress;
	NeighborStatus neighborStatus;
	bool IsAvailableForUpperLayers;
	UINT8 NumTimeSyncMessagesSent;
	UINT8 NumTimeSyncMessagesRecv;
};

struct PACK MACNeighborLinkInfo	//8bytes
{
	Link_t SendLink;
	Link_t ReceiveLink;
};


struct PACK EntendedMACInfoMsgSummary //3 bytes
{
	UINT8 NumTotalEntries;
	UINT8 NNeigh_AFUL;
	UINT8 NumEntriesInMsg;
};


#endif /* NEIGHBORS_H_ */
