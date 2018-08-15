/*
 * Copyright The Samraksh Company
 *
 * Author: Mukundan.Sridharan, Nived.Sivadas
 *
 * Description :  Radio HAL APIs, v 1.0
 *
 *
*/

#ifndef _RADIO_DECL_H_
#define _RADIO_DECL_H_

#include <tinyhal.h>
#include "Hal_util.h"

//Common RadioAddress type for all types of radios and MACs
typedef UINT16 RadioAddress_t;
#define RADIO_BROADCAST_ADDRESS 65535


/////////////////////////////////////Enum Definitions /////////////////////////////////////////////////////

// Represents a generic group of interrupts supported by any radio. There is a good chance some of these are not relevant to a radio,
// its is upto to the radio layer to determine this and inform the user appropriately

enum RadioInterrupt
{
	PreambleDetect =1,
	StartFrameDetect=(1 << 1),
	StartOfTransmission=(1 << 2),
	EndOfTransmission=(1 << 3),
	StartOfReception=(1 << 4),
	EndOfReception=(1 << 5),
	CRCError=(1 << 6),
	EncryptionError=(1 << 7),
	DecryptionError=(1 << 8),
	InitializationError=(1 << 9),
	SynchronizationError=(1 << 10),
};

// Defines the type of the radio that is being configured
enum RadioType{
	IEEE802_15_4,
	IEEE802_11,
	IEEE802_15_1,
	ByteRadio,
	SoftwareDefined,
};

enum RadioName
{
	RF231RADIO,
	RF231RADIOLR,
	SI4468_SPI2,
};

enum RadioAckType
{
	HARDWARE_ACK,
	SOFTWARE_ACK,
	NO_ACK
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////Typedef Definitions //////////////////////////////////////////////////

typedef UINT8 ErrorType;

// Typedef defining the signature of the receive function

typedef  void* (*ReceiveFuncPtrType) (void *msg, UINT16 Size);

// Typedef defining the signature of the send function
typedef void (*SendAckFuncPtrType) (void* msg, UINT16 Size, NetOpStatus status, UINT8 radioAckStatus);

// Typedef defining the signature of the RadioInterruptFuncPtr function
typedef BOOL (*RadioInterruptFuncPtrType) (RadioInterrupt Interrupt, void *param);



////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////// Class Definitions //////////////////////////////////////////////////////
struct RadioConfig{
	UINT8 TxPower;
	UINT8 Sensitivity;
    UINT8 Channel;
    UINT8 TimeStampOffset;
};



typedef  class RadioEventHandler{

public:
	UINT32 RadioInterruptMask;
	ReceiveFuncPtrType ReceiveHandler;
	SendAckFuncPtrType SendAckHandler;
	RadioInterruptFuncPtrType RadioInterruptHandler;

	/*RadioEventHandler(ReceiveFuncPtrType recieve_handler, SendAckFuncPtrType send_ack_handler,RadioInterruptFuncPtrType  radio_interrupt_handler, UINT32 radio_interrupt_mask )
	{
		this->RecieveHandler = recieve_handler;
		this->SendAckHandler = send_ack_handler;
		this->RadioInterruptHandler = radio_interrupt_handler;
		this->RadioInterruptMask = radio_interrupt_mask;
	}*/

	void SetReceiveHandler(ReceiveFuncPtrType receive_handler)
	{
		this->ReceiveHandler = receive_handler;
	}

	void SetSendAckHandler(SendAckFuncPtrType send_ack_handler)
	{
		this->SendAckHandler = send_ack_handler;
	}

	void SetRadioInterruptHandler(RadioInterruptFuncPtrType radio_interrupt_handler)
	{
		this->RadioInterruptHandler = radio_interrupt_handler;
	}

	ReceiveFuncPtrType GetReceiveHandler()
	{
		return this->ReceiveHandler;
	}

	SendAckFuncPtrType GetSendAckHandler()
	{
		return this->SendAckHandler;
	}

	RadioInterruptFuncPtrType GetRadioInterruptHandler()
	{
		return this->RadioInterruptHandler;
	}

}RadioEventHandler_t;

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//extern "C"
//{
//Radio HAL declarations


// Called by MAC layers responsible for registering of eventhandlers
DeviceStatus CPU_Radio_Initialize(RadioEventHandler* eventHandlers, UINT8 radioIDs, UINT8 numberRadios , UINT8 mac_id); //Initializes Return the ID of the Radio layer that was initialized
BOOL CPU_Radio_UnInitialize(UINT8 radioIDs);
UINT8 CPU_Radio_GetRadioIDs(UINT8* radioIDs);
void* CPU_Radio_Preload(UINT8 radioID,void * msg, UINT16 size);
void* CPU_Radio_SendRetry(UINT8 radioID);
void* CPU_Radio_SendStrobe(UINT8 radioID, UINT16 size);
void* CPU_Radio_Send(UINT8 radioID,void * msg, UINT16 size);
void* CPU_Radio_Send_TimeStamped(UINT8 radioID,void * msg, UINT16 size, UINT32 eventTime);
NetOpStatus CPU_Radio_PreloadMessage(UINT8* msg, UINT16 size);
DeviceStatus CPU_Radio_Send_Strobe(UINT8 radioID);	//Send preloaded message
DeviceStatus CPU_Radio_ClearChannelAssesment (UINT8 radioID);
DeviceStatus CPU_Radio_ClearChannelAssesment(UINT8 radioID, UINT32 numberMicroSecond);
//DeviceStatus CPU_Radio_EnableCSMA(UINT8 radioID);
//DeviceStatus CPU_Radio_DisableCSMA(UINT8 radioID);
DeviceStatus CPU_Radio_TurnOnRx(UINT8 radioID);
DeviceStatus CPU_Radio_TurnOffRx(UINT8 radioID);
DeviceStatus CPU_Radio_TurnOnPLL(UINT8 radioID);
DeviceStatus CPU_Radio_Sleep(UINT8 radioID, UINT8 level);
BOOL CPU_Radio_SetTimeStamp(UINT8 radioID);
BOOL CPU_Radio_SetTimeStamp2(UINT8 radioID, UINT64 timeStamp);
UINT32 CPU_Radio_GetSNR(UINT8 radioID);
UINT32 CPU_Radio_GetRSSI(UINT8 radioID);
UINT32 CPU_Radio_SetDefaultRxState(UINT8 state); // default state is sleep, so CSMA needs to call this to keep RX always on

//Aggregate apis
BOOL CPU_Radio_SwitchType(UINT8 radioID);
//Radio* CPU_Radio_GetRadio(UINT8 radioID);
BOOL CPU_Radio_Intialize(UINT8  radioID);
BOOL CPU_Radio_UnInitialize(UINT8 radioID);
UINT8 CPU_Radio_NumberRadiosSupported();

BOOL CPU_Radio_Reset(UINT8 radioID);
UINT16 CPU_Radio_GetAddress(UINT8 radioID);
BOOL CPU_Radio_SetAddress(UINT8 radioID, UINT16 address);

RadioType CPU_Radio_GetRadioType();
DeviceStatus CPU_Radio_SetRadioType(RadioType radioType);

INT8 CPU_Radio_GetRadioAckType();
INT8 CPU_Radio_GetRadioName();
DeviceStatus CPU_Radio_SetRadioName(INT8 radioName);

DeviceStatus CPU_Radio_ChangeTxPower(UINT8 radioID, int power);

DeviceStatus CPU_Radio_ChangeChannel(UINT8 radioID, int channel);

UINT32 CPU_Radio_GetTxPower(UINT8 radioID);

UINT32 CPU_Radio_GetChannel(UINT8 radioID);

//}


#endif //_RADIO_DECL_H_
