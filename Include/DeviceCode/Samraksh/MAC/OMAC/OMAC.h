/*
 * Copyright: The Samraksh Company
 * Author: Mukundan.Sridharan, Nived.Sivadas, Bora Karaoglu
 * Description :  OMAC Implementation, v 2.0
 * Created on: April, 2015
 */


#ifndef OMAC_H_
#define OMAC_H_

//#define TWO_NODES_TX_RX
//#define OMAC_DEBUG_GPIO
//#define OMAC_DEBUG_PRINTF

#define OMAC_DEBUG_PRINTF_EXTENDEDMACINfo 0
#define OMAC_DEBUG_SEND_EXTENDEDMACINfo 1

// BK: VARIOUS DEBUG STatement enabling PIN for debuggin network issues


#define OMAC_DEBUG_PRINTF_SETCONFIG 0
#define OMAC_DEBUG_PRINTF_PACKETREC 0
#define OMAC_DEBUG_PRINTF_ACKREC 0
#define OMAC_DEBUG_DRH_SEND_ACK 0
#define OMAC_DEBUG_PACKET_REJECTION 0
#define OMAC_DEBUG_PRINTF_PACKETDROP_SUCESS 0
#define OMAC_DEBUG_PRINTF_PACKETDROP_MAXTRIES 0
#define OMAC_DEBUG_PRINTF_PACKET_ACK_RX_FAIL 0
#define OMAC_DEBUG_PRINTF_CCA_TXFAIL 0
#define OMAC_DEBUG_PRINTF_TXATTEMPT_SUCCESS 0
#define OMAC_DEBUG_PRINTF_PACKET_ACK_RX_SUCCESS 0
#define OMAC_DEBUG_PRINTF_HIGH_DISCO_MODE 0
#define OMAC_DEBUG_PRINTF_ID 0

#define OMAC_DEBUG_PRINTF_NEIGHCHANGE 0
#define OMAC_DEBUG_PRINTF_DISCO_RX 0
#define OMAC_RECEIVE_DEBUGGING_FOR_MF 0

#define OMAC_DEBUG_PRINTF_TS_RX 0
#define OMAC_DEBUG_PRINTF_TSREQ_TX 0

#define OMAC_DEBUG_PRINTF_FAILSAFE_STOP 0

#define OMAC_DEBUG_PRINTF_RADIOCONTROL_SEND 0
#ifdef OMAC_DEBUG_PRINTF_RADIOCONTROL_SEND
	#define OMAC_DEBUG_PRINTF_RADIOCONTROL_SEND_EXCEPT_DISCO 0
#else
	#define OMAC_DEBUG_PRINTF_RADIOCONTROL_SEND_EXCEPT_DISCO 0
#endif

#if defined(TWO_NODES_TX_RX)

#define def_Neighbor2beFollowed

/* // Nathan's nodes
//#define TXNODEID 18134
//#define RXNODEID 20181
*/
//Bora's Nodes
#define TXNODEID 30906
//#define TXNODEID 4028
#define RXNODEID 32696
// Ananth's Nodes
//#define TXNODEID 3505
//#define RXNODEID 6846
#endif

#include <Samraksh/MAC.h>
#include <Samraksh/Radio_decl.h>
#include <Samraksh/Message.h>
#include <Samraksh/Buffer.h>
#include <Samraksh/Neighbors.h>

#include "OMACConstants.h"
#include "OMACClock.h"
#include "Scheduler.h"
#include "Handlers.h"

#ifdef OMAC_DEBUG_GPIO
#define OMAC_CPU_GPIO_SetPinState(X,Y) (CPU_GPIO_SetPinState(X,Y))
#define OMAC_CPU_GPIO_EnableOutputPin(X,Y) (CPU_GPIO_EnableOutputPin(X,Y))
#endif
#ifndef OMAC_DEBUG_GPIO
#define OMAC_CPU_GPIO_SetPinState(X,Y) (void)0
#define OMAC_CPU_GPIO_EnableOutputPin(X,Y) (void)0
#endif

#ifdef OMAC_DEBUG_PRINTF
#define OMAC_HAL_PRINTF(...) hal_printf(__VA_ARGS__)
#endif
#ifndef OMAC_DEBUG_PRINTF
#define OMAC_HAL_PRINTF(...) (void)0
#endif

//typedef Buffer_15_4<8> Buffer_15_4_t;

//#define TIMESYNC_SENDPIN 0 // 3 // PA3 J11-6
//#define TIMESYNC_RECEIVEPIN 31 // 23 //PB7 J11-10

//extern Buffer_15_4_t g_send_buffer;
extern Buffer_15_4_t g_receive_buffer;
extern NeighborTable g_NeighborTable;

/*
 *
 */
class OMACType: public MAC<Message_15_4_t, MACConfig>{
  private:
	//BK Not Used

	//BK: Non-esentials but used
	softwareACKHeader* swAckHeader;

	//BK: USed but don't know why
	UINT8 CurrentActiveApp;
	UINT16 MyID; 						//Radio MAC ID is stored locally to speed up packet reception instead of going to radio

	//Continuataion Related
	static HAL_CONTINUATION OMAC_callback_continuation;

	static const UINT8 payloadTypeArrayMaxValue = 8;
	static UINT8 payloadTypeArrayIndex;
	UINT8 payloadTypeArray[payloadTypeArrayMaxValue];

	//BK: Used for valid reason

	//Underlying radio variables
	static const UINT8 NumberRadios = 1;










	//Protocol variables
	//-------------------------------
	static const UINT16 GUARDTIME_MICRO_OMAC = GUARDTIME_MICRO;
	static const UINT16 OMAC_TIME_ERROR = 3*MILLISECINMICSEC;

	static const UINT16 DELAY_DUE_TO_CCA_MICRO = CCA_PERIOD_MICRO;
private:
	static const UINT16 RETRY_FUDGE_FACTOR = 0.3*MILLISECINMICSEC;

  private:
	UINT16 RETRY_RANDOM_BACKOFF_DELAY_MICRO;
	UINT16 RETRANS_DELAY_DUE_TO_MISSING_ACK;
	UINT16 DELAY_IN_RECEIVING_ACK;
	UINT16 DELAY_IN_RECEIVING_HW_ACK;
	UINT16 DELAY_IN_RECEIVING_SW_ACK;
	UINT16 RETRANS_DELAY_DUE_TO_MISSING_HW_ACK;
	UINT16 RETRANS_DELAY_DUE_TO_MISSING_SW_ACK;
	UINT16 DELAY_FROM_RADIO_DRIVER_TX_TO_RADIO_DRIVER_RX;
    UINT16 DELAY_FROM_OMAC_TX_TO_RADIO_DRIVER_TX;
    UINT16 CCA_PERIOD_DESIRED;
    UINT16 CCA_PERIOD_ACTUAL;
    UINT16 RADIO_TURN_ON_DELAY_TX;
    UINT16 RADIO_TURN_ON_DELAY_RX;
    UINT16 TIME_RX_TIMESTAMP_OFFSET_MICRO;

    friend class OMACScheduler;
    friend class DiscoveryHandler ;
    friend class DataReceptionHandler ;
    friend class DataTransmissionHandler ;
    friend class OMACTimeSync ;


	//-------------------------------
//	static const UINT8 SlotLength = 8; //slot Length in milliseconds
	Message_15_4_t* PrepareMessageBuffer(UINT16 address, UINT8 dataType, void* msg, int size);

	void SendRXPacketToUpperLayers(Message_15_4_t *msg, UINT8 payloadType);
	void PrintNeighborTable();
  public:

	//Not used
		Message_15_4_t rx_msg;
		// Pointer to the outgoing message
		Message_15_4_t* tx_msg_ptr;
		// Pointer to the incoming message
		Message_15_4_t* rx_msg_ptr;
		volatile UINT16 tx_length;
		volatile UINT16 rx_length;


	//BK: USed but don't know why
	Message_15_4_t tx_msg;
	volatile bool isSendDone;
	MACReceiveFuncPtrType m_rxAckHandler;
	SendAckFuncPtrType m_txAckHandler;

	// Legitimate Public

#if OMAC_DEBUG_PRINTF_NEIGHCHANGE || OMAC_DEBUG_PRINTF_DISCO_RX || OMAC_DEBUG_PRINTF_TS_RX ||OMAC_DEBUG_PRINTF_TSREQ_TX
	bool is_print_neigh_table;
#endif


#ifdef def_Neighbor2beFollowed
	UINT16 Neighbor2beFollowed;
#endif

#ifdef def_Neighbor2beFollowed2
	UINT16 Neighbor2beFollowed1, Neighbor2beFollowed2;
#endif
	//Neighbors
	//NeighborTable m_NeighborTable;

	//Buffer variables
	//Buffer_15_4_t m_send_buffer;
	//Buffer_15_4_t m_receive_buffer;

	//Protocol variables
	//-------------------------------
	static const UINT8 RANDOM_BACKOFF_COUNT_MAX = 4;
	static const UINT8 RANDOM_BACKOFF_COUNT_MIN = 1;
	UINT8 TX_TIME_PER_BIT_IN_MICROSEC;
	UINT16 ACK_RX_MAX_DURATION_MICRO;
	UINT16 ACK_TX_MAX_DURATION_MICRO;
	UINT16 MAX_PACKET_TX_DURATION_MICRO;
	UINT16 MAX_PACKET_RX_DURATION_MICRO;
	UINT16 DISCO_PACKET_TX_TIME_MICRO;
	UINT16 DISCO_SLOT_PERIOD_MICRO;
	UINT16 DISCO_BEACON_TX_MAX_DURATION_MICRO;
	UINT16 HIGH_DISCO_PERIOD_IN_SLOTS;
	int LISTEN_PERIOD_FOR_RECEPTION_HANDLER;
	//-------------------------------




	OMACClock m_Clock;

	RadioControl_t m_omac_RadioControl;
	OMACScheduler m_omac_scheduler;




	/*Already set in MAC class from which OMAC is derived
	UINT16 GetAddress(){return MyID;}
	BOOL SetAddress(UINT16 address){
		MyAddress = address;
		return TRUE;
	}*/

	BOOL SetRadioAddress(UINT16 address){
		BOOL ret = CPU_Radio_SetAddress(this->radioName, address);
		return ret;
	}

	UINT16 GetRadioAddress(){
		UINT16 temp = CPU_Radio_GetAddress(this->radioName);
		return temp;
	}

	BOOL SetRadioName(UINT8 radioName){
		this->radioName = radioName;
		BOOL ret = CPU_Radio_SetRadioName(radioName);
		return ret;
	}

	BOOL SetRadioTxPower(int power){
		BOOL ret = CPU_Radio_ChangeTxPower(this->radioName, power);
		return ret;
	}

	BOOL SetRadioChannel(int channel){
		BOOL ret = CPU_Radio_ChangeChannel(this->radioName, channel);
		return ret;
	}

	UINT16 GetMaxPayload(){
		return MaxPayload;
	}
	void SetMaxPayload(UINT16 payload){
		MaxPayload = payload;
	}

	void SetCurrentActiveApp(UINT8 CurrentActiveApp){
		this->CurrentActiveApp = CurrentActiveApp;
	}

	UINT8 GetCurrentActiveApp(){
		return this->CurrentActiveApp;
	}

	void SetMyID(UINT16 MyID){
		this->MyID = MyID;
#if OMAC_DEBUG_PRINTF_ID
	hal_printf("MyID is %u \r\n", MyID);
#endif
	}

	UINT16 GetMyID(){
		return this->MyID;
	}

	//Override base class methods here, implement them later in cpp file
	DeviceStatus Initialize(MACEventHandler* eventHandler, UINT8 macName, UINT8 routingAppID, UINT8 radioID, MACConfig *config);
	DeviceStatus SetConfig(MACConfig *config);
	DeviceStatus SetOMACParametersBasedOnRadioName(UINT8 radioID);
	BOOL Send(UINT16 dest, UINT8 dataType, void* msg, int size);
	//BOOL SendTimeStamped(RadioAddress_t dest, UINT8 dataType, Message_15_4_t* msg, int Size, UINT32 eventTime);
	BOOL SendTimeStamped(UINT16 dest, UINT8 dataType, void* msg, int Size, UINT32 eventTime);
	PacketID_T EnqueueToSend(UINT16 address, UINT8 dataType, void* msg, int size);
	PacketID_T EnqueueToSendTimeStamped(UINT16 address, UINT8 dataType, void* msg, int size, UINT32 eventTime);

	DeviceStatus GetPacketWithIndex(UINT8 **managedBuffer, UINT8 size, PacketID_T index);
	DeviceStatus GetPacketSizeWithIndex( UINT8* buffersizeptr, PacketID_T index);
	DeviceStatus DeletePacketWithIndexInternal( PacketID_T index);


	Message_15_4_t* ReceiveHandler(Message_15_4_t* msg, int size);


	BOOL RadioInterruptHandler(RadioInterrupt Interrupt, void* Param);
	void SendAckHandler(void* msg, int Size, NetOpStatus status);
	BOOL UnInitialize(void);
	BOOL HandleBroadcastMessage(Message_15_4_t * msg);
	BOOL HandleUnicastMessage(Message_15_4_t * msg);
	Message_15_4_t* FindFirstSyncedNbrMessage();
	Message_15_4_t* FindFirstMessageForNbr(UINT16 nbr);
	UINT8 UpdateNeighborTable();
	UINT8 GetSendBufferSize();
	UINT8 GetReceiveBufferSize();
	UINT16 GetSendPending();
	UINT16 GetReceivePending();

	void PushPacketsToUpperLayers(void* arg);
};


#endif /* OMAC_H_ */
