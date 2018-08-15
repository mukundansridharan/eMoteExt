/*
 * csmaMAC.h
 *
 *  Created on: Sep 27, 2012
 *      Author: Nived
 */

#ifndef CSMAMAC_H_
#define CSMAMAC_H_

/*********************************  Included header files **************************************/

#include <tinyhal.h>
#include <Samraksh\Message.h>
#include <Samraksh\Neighbors.h>
#include <Samraksh\MAC.h>
#include <Samraksh\Buffer.h>
#include <Samraksh\Radio_decl.h>

//typedef  struct Buffer<Message_15_4_t> Buffer_15_4_t;

#define LEVEL_0_RECOVER (1 << 0)
#define LEVEL_1_RECOVER (1 << 1)
#define LEVEL_2_RECOVER (1 << 2)

#define DISCOVERY_FREQUENCY 200

//FCF table:
//15 14 13  12  11  10  9  8  7  6  5  4  3  2  1  0 (bits)
//1   0  0   0   1   0  0  0  0  1  1  0  0  0  0  1 (values)
//Frame type (bits 0,1,2)  			- Data
//Security enabled (bit 3) 			- No
//Frame pending (bit 4)				- No
//Ack request (bit 5)				- Yes
//Intra-pan (bit 6)					- Yes, source PAN-ID is omitted
//Bits 7,8,9						- Reserved
//Dest addressing mode (bits 10,11)	- Address field contains a 16-bit short address
//Frame version (bits 12,13)		- Frames are compatible with IEEE 802.15.4 2003
//Src addressing mode (bits 14,15)	- Address field contains a 16-bit short address
#define FCF_WORD_VALUE 0x8861 //34913
#define SRC_PAN_ID	0xAAAA
#define DEST_PAN_ID	0x5555

#define TIMESTAMP_SIZE 4

extern Buffer_15_4_t g_send_buffer;
extern Buffer_15_4_t g_receive_buffer;
extern NeighborTable g_NeighborTable;

//#define DEBUG_CSMAMAC 1

#if defined(DEBUG_CSMAMAC)
#define ENABLE_PIN_CSMA(x,y) CPU_GPIO_EnableOutputPin(x,y)
#define SET_PIN_CSMA(x,y) CPU_GPIO_SetPinState(x,y)
#define DEBUG_PRINTF_CSMA(x, ...) hal_printf("[CSMA]" x, ##__VA_ARGS__)
#else
#define ENABLE_PIN_CSMA(x,y)
#define SET_PIN_CSMA(x,y)
#define DEBUG_PRINTF_CSMA(x, ...)
#endif

extern volatile UINT32 csmaSendToRadioFailCount;

/*******************************************************************************************************/

// core mac class definitions extends the MAC class and implements its virtual functions
// MAC does not have a template because of function pointer issues as they were primarily for the return type of handlers

class csmaMAC: public MAC<Message_15_4_t, MACConfig>
{
	//Buffer variable
	UINT8 CurrentActiveApp;
	BOOL RadioAckPending;
	UINT8 m_recovery;


public:
	BOOL flushTimerRunning;
	UINT16 GetRadioAddress();
	UINT16 GetMaxPayload(){return MaxPayload;}
	BOOL SetRadioAddress(UINT16 address);
	void SetMaxPayload(UINT16 payload){MaxPayload = payload;}
	BOOL SetRadioName(UINT8 radioName);
	BOOL SetRadioTxPower(int power);
	BOOL SetRadioChannel(int channel);


	BOOL Resend(void* msg, int Size);
	DeviceStatus Initialize(MACEventHandler* eventHandler, UINT8 macName, UINT8 routingAppID,UINT8 radioName, MACConfig *config);
	DeviceStatus SetConfig(MACConfig *config);
	BOOL Send(UINT16 dest, UINT8 dataType, void* msg, int Size);
	BOOL SendTimeStamped(UINT16 dest, UINT8 dataType, void* msg, int Size, UINT32 eventTime);
	Message_15_4_t* ReceiveHandler(Message_15_4_t* msg, int Size);
	BOOL RadioInterruptHandler(RadioInterrupt Interrupt, void* Param);
	void SendAckHandler(void* msg, int Size, NetOpStatus status, UINT8 radioAckStatus);
	BOOL UnInitialize(void);
	BOOL HandleBroadcastMessage(Message_15_4_t * msg);
	BOOL HandleUnicastMessage(Message_15_4_t * msg);
	BOOL HandlePromiscousMessage(Message_15_4_t * msg);
	void SendToRadio();
	void UpdateNeighborTable();
	UINT8 GetSendBufferSize();
	UINT8 GetReceiveBufferSize();
	UINT16 GetSendPending(){ return g_send_buffer.GetNumberMessagesInBuffer();}
	UINT16 GetReceivePending(){return g_receive_buffer.GetNumberMessagesInBuffer();}

	DeviceStatus SendHello();

	NeighborTable* GetNeighborTable(){return &g_NeighborTable;}
	Neighbor_t* GetNeighbor(UINT16 macAddress){return g_NeighborTable.GetNeighborPtr(macAddress); }

};




#endif /* CSMAMAC_H_ */
