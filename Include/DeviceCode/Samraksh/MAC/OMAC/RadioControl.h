/*
 * RadioControl.h
 *
 *  Created on: Sep 10, 2012
 *      Author: Mukundan
 */

#ifndef RADIOCONTROL_H_
#define RADIOCONTROL_H_

#include <Samraksh/Message.h>

/*
 * Radio Wrapper Class
 */
class RadioControl_t{
private:
	bool PiggybackTimeSyncMessage(Message_15_4_t* msg, UINT16 &size);
	bool PiggybackDiscoMessage(Message_15_4_t* msg, UINT16 &size);
	bool PiggybackEntendedMACInfoMsg(Message_15_4_t* msg, UINT16 &size);
	UINT8 next_piggybacked_extendedneighborinfo_index;
public:
#if OMAC_DEBUG_PRINTF_RADIOCONTROL_SEND
	UINT16 packet_seq_num;
#endif
	DeviceStatus Initialize();
	DeviceStatus Uninitialize();
	DeviceStatus StartPLL();
	DeviceStatus StartRx();
	DeviceStatus Preload(RadioAddress_t address, Message_15_4_t * msg, UINT16 size);
	DeviceStatus Send(RadioAddress_t address, Message_15_4_t * msg, UINT16 size);
	DeviceStatus Receive(Message_15_4_t * msg, UINT16 &size);
	DeviceStatus Stop();

	bool PiggybackMessages(Message_15_4_t* msg, UINT16 &size);
	bool stayOn;
	RadioControl_t() : next_piggybacked_extendedneighborinfo_index(0) ,stayOn(false) {};

};


#endif /* RADIOCONTROL_H_ */
