/*
 * OMAC_Common_Buffer_I.h
 *
 *  Created on: Mar 23, 2017
 *      Author: Bora
 */

#ifndef MICROFRAMEWORKPK_V4_3_DEVICECODE_INCLUDE_SAMRAKSH_BUFFERS_OMAC_COMMON_BUFFER_I_H_
#define MICROFRAMEWORKPK_V4_3_DEVICECODE_INCLUDE_SAMRAKSH_BUFFERS_OMAC_COMMON_BUFFER_I_H_

#include <Samraksh\Message.h>

class OMAC_Common_Buffer_I {
public:
	virtual ~OMAC_Common_Buffer_I() = 0;
	virtual bool DeletePacket(Message_15_4_t* msg_carrier) = 0;
	virtual bool RemovePacketsWithRetryAttemptsGreaterThan(UINT8 r) = 0;

	virtual bool IsThereATSRPacketWithDest(const UINT16 address) = 0;
	virtual bool IsThereADataPacketWithDest(const UINT16 address) = 0;
	virtual Message_15_4_t* FindTSRPacketForNeighbor(const UINT16 neigh) = 0;
	virtual Message_15_4_t* FindDataPacketForNeighbor(const UINT16 neigh) = 0;

	virtual bool InsertMessage(Message_15_4_t* msg_carrier) = 0;
	virtual UINT16 GetNumberofElements() const = 0;

	friend class OMAC_Common_Buffer_Test;
};


#endif /* MICROFRAMEWORKPK_V4_3_DEVICECODE_INCLUDE_SAMRAKSH_BUFFERS_OMAC_COMMON_BUFFER_I_H_ */
