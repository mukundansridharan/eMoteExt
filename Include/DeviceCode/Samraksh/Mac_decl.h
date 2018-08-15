#ifndef _MAC_DECL_H_
#define _MAC_DECL_H_

#include "Hal_util.h"
#include "Radio_decl.h"
#include "Neighbors.h"
#include "Buffer.h"

//typedef UINT16 MacAddress_t;

enum MACNames
{
	NONE = 0,
	CSMAMAC = 1,
	OMAC = 2,
};



#define MAC_BROADCAST_ADDRESS RADIO_BROADCAST_ADDRESS


//Should be consistent with that in "MACConfiguration" in IMAC.cs (Samraksh_eMote_Net)
struct MACConfig {
	//UINT16 FCF;
	//UINT16 DestPAN;
	//UINT8 Network;
	BOOL CCA;
    UINT8 NumberOfRetries;
    UINT8 CCASenseTime;
    UINT8 BufferSize;
    //UINT8 RadioType;
    UINT32 NeighborLivenessDelay;
};


// Typedef defining the signature of the receive function
//typedef  void (*MacReceiveFuncPtrType) (void *msg, UINT16 Size, UINT16 Src, BOOL Unicast, UINT8 RSSI, UINT8 LinkQuality);
typedef void (*MACReceiveFuncPtrType) (void *msg, UINT16 arg1);

typedef void (*NeighborChangeFuncPtrType) (INT16 arg2);

// Typedef defining the signature of the send function
typedef void (*SendAckFuncPtrType) (void* msg, UINT16 Size, NetOpStatus status, UINT8 radioAckStatus);

typedef  class MACEventHandler{

public:
	MACReceiveFuncPtrType ReceiveHandler;
	SendAckFuncPtrType SendAckHandler;
	NeighborChangeFuncPtrType neighborHandler;

	MACEventHandler(){
		ReceiveHandler = NULL;
		SendAckHandler = NULL;
		neighborHandler = NULL;
	}

	void SetReceiveHandler(MACReceiveFuncPtrType receive_handler)
	{
		this->ReceiveHandler = receive_handler;
	}

	void SetSendAckHandler(SendAckFuncPtrType send_ack_handler)
	{
		this->SendAckHandler = send_ack_handler;
	}

	void SetNeighborChangeHandler(NeighborChangeFuncPtrType neighbor_change_handler)
	{
		this->neighborHandler = neighbor_change_handler;
	}

	MACReceiveFuncPtrType GetReceiveHandler()
	{
		return this->ReceiveHandler;
	}

	SendAckFuncPtrType GetSendAckHandler()
	{
		return this->SendAckHandler;
	}

	NeighborChangeFuncPtrType GetNeighborChangeHandler()
	{
		return this->neighborHandler;
	}

}MACEventHandler_t;


//Basic functions
DeviceStatus MAC_Initialize(MACEventHandler* eventHandler, UINT8 macName, UINT8 routingAppID, UINT8 radioName, void* macConfig);
DeviceStatus MAC_Reconfigure(void* config);
UINT16 MAC_GetRadioAddress();
BOOL MAC_SetRadioAddress(UINT16 address);
BOOL MAC_SetRadioType(RadioType radioType);
BOOL MAC_SetRadioName(INT8 radioName);
BOOL MAC_SetRadioTxPower(int power);
BOOL MAC_SetRadioChannel(int channel);
DeviceStatus MAC_UnInitialize();
UINT8 MAC_GetID();
DeviceStatus MAC_Send(UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size); //msg is just the payload,
DeviceStatus MAC_SendTimeStamped(UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size, UINT32 timeStamp); //msg is just the payload,
DeviceStatus MAC_Config(void *macConfig);
PacketID_T MAC_EnqueueToSend(UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size);
PacketID_T MAC_EnqueueToSendTimeStamped(UINT16 destAddress, UINT8 dataType, void * msg, UINT16 size, UINT32 timeStamp);

//Neighbor functions
//NeighborTable* MAC_GetNeighborTable(UINT8 macID);
//Neighbor_t* MAC_GetNeighbor(UINT8 macID, UINT16 macAddress);

// Get the neighbor list
DeviceStatus MAC_GetNeighborList(UINT16 *buffer);
DeviceStatus MAC_GetMACNeighborList(UINT16 *buffer);
DeviceStatus MAC_GetNeighborStatus(UINT16 macAddress, UINT8 *buffer);

//Channel/freq functions
DeviceStatus MAC_GetNextPacket(UINT8 **managedBuffer);
//Buffer functions
UINT8 MAC_GetSendBufferSize();
UINT8 MAC_GetReceiveBufferSize();
UINT8 MAC_GetPendingPacketsCount_Send();
UINT8 MAC_GetPendingPacketsCount_Receive();
DeviceStatus MAC_RemovePacket(UINT8* msg);
DeviceStatus MAC_GetPacketWithIndex(UINT8 **managedBuffer, UINT8 buffersize, PacketID_T index);
DeviceStatus MAC_GetPacketSizeWithIndex( UINT8* buffersizeptr, PacketID_T index);
DeviceStatus MAC_DeletePacketWithIndexInternal( PacketID_T index);
UINT16 MAC_GetMsgIdWithPtr(Message_15_4_t* msg_carrier);

bool MAC_ChangeOwnerShipOfElementwIndex(PacketID_T index,  BufferOwner n_buf_ow);


//MAC Aggregate APIs
BOOL MACLayer_Initialize();
BOOL MACLayer_UnInitialize();
UINT8 MACLayer_NumberMacsSupported();


#endif
