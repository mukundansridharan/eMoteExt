/*
 * ArrayBuffer.h
 *
 *  Created on: Mar 23, 2017
 *      Author: Bora
 */

#ifndef MICROFRAMEWORKPK_V4_3_DEVICECODE_INCLUDE_SAMRAKSH_BUFFERS_ARRAYBUFFER_H_
#define MICROFRAMEWORKPK_V4_3_DEVICECODE_INCLUDE_SAMRAKSH_BUFFERS_ARRAYBUFFER_H_

#include <Samraksh\Message.h>
#include "OMAC_Common_Buffer_I.h"


enum BufferOwner{
	BO_UNUSED,
	BO_CSMA,
	BO_OMAC,
	BO_MFUser,

};

typedef UINT16 BufferSize_T;
typedef BufferSize_T PacketID_T;

#define INVALID_PACKET_ID 255
#define ISPACKET_ID_VALID(x) ((x == INVALID_PACKET_ID) ? false : true)



template <BufferSize_T BUFFERSIZE>
class ArrayBuffer {
protected:
	bool RemovePacketsWithRetryAttemptsGreaterThan(UINT8 r){
		for(BufferSize_T i = 0; i < BUFFERSIZE; ++i){
			if( send_packet_buffer[i].GetMetaDataConst()->GetRetryAttempts() > r){
				DeleteElementwIndex(i);
				return true;
			}
		}
		return false;
	}
public:
	BufferSize_T last_used_index;
	BufferSize_T num_elements;

	Message_15_4_t send_packet_buffer[BUFFERSIZE];
	BufferOwner buffer_owner[BUFFERSIZE];
public:
	void DeleteElementwIndex(PacketID_T index){
		if(ISPACKET_ID_VALID(index) && buffer_owner[index] != BO_UNUSED) {
			ClearMsgContents(&(send_packet_buffer[index]));
			buffer_owner[index] = BO_UNUSED;
			--num_elements;
		}
	}
	bool ChangeOwnerShipOfElementwIndex(PacketID_T index, BufferOwner n_buf_ow){
		if(ISPACKET_ID_VALID(index) && buffer_owner[index] != BO_UNUSED) {
			if(n_buf_ow == BO_UNUSED){
				DeleteElementwIndex(index);
				return true;
			}
			else{
				buffer_owner[index] = n_buf_ow;
				return true;
			}
		}
		return false;
	}

	PacketID_T GetIndex(Message_15_4_t & temp_msg){
		//		for(BufferSize_T i = 0; i < BUFFERSIZE; ++i){
		//			if( &(send_packet_buffer[i]) == &temp_msg){
		//				return i;
		//			}
		//		}

		Message_15_4_t* rm = NULL;
		BufferSize_T i = 0;
		for(BufferSize_T j = 0; j < BUFFERSIZE; ++j){
			if(j > last_used_index){
				i = (BUFFERSIZE - j) + last_used_index;
			}
			else{
				i = last_used_index - j;
			}

			if( &(send_packet_buffer[i]) == &temp_msg){
				return i;
			}
		}

		return INVALID_PACKET_ID;
	}


	ArrayBuffer(){
		Initialize();
	}
	void Initialize(){
		last_used_index = 0;
		num_elements = 0;
		for(BufferSize_T i = 0; i < BUFFERSIZE; ++i){
			buffer_owner[i] = BO_UNUSED;
		}
	}
	bool DeletePacket(Message_15_4_t* msg_carrier){
		BufferSize_T i = GetIndex(*msg_carrier);
		if( i < BUFFERSIZE) {
			DeleteElementwIndex(i);
			return true;
		}
		else return false;
	}

	Message_15_4_t* FindStalePacketWithRetryAttemptsGreaterThan(UINT8 r, BufferOwner n_buf_ow){
		for(BufferSize_T i = 0; i < BUFFERSIZE; ++i){
			if( buffer_owner[i] == n_buf_ow && send_packet_buffer[i].GetMetaDataConst()->GetRetryAttempts() > r){
				return &(send_packet_buffer[i]);
			}
		}
		return NULL;
	}


	bool IsThereATSRPacketWithDest(const BufferSize_T address){
		BufferSize_T index = 0;
		BufferSize_T numiter = 0;
		for(BufferSize_T j = 0; j < BUFFERSIZE && numiter < num_elements; ++j){
			index = last_used_index - j;
			if(j > last_used_index){
				index = (BUFFERSIZE - j) + last_used_index;
			}
			else{
				index = last_used_index - j;
			}
			if(buffer_owner[index] != BO_UNUSED) ++numiter;
			if(buffer_owner[index] == BO_OMAC){
				if( send_packet_buffer[index].GetHeaderConst()->payloadType == MFM_OMAC_TIMESYNCREQ && send_packet_buffer[index].GetHeaderConst()->dest == address){
					return true;
				}
			}
		}

		//		for(BufferSize_T index = 0; index < BUFFERSIZE; ++index){
		//			if( send_packet_buffer[index].GetHeaderConst()->payloadType == MFM_OMAC_TIMESYNCREQ && send_packet_buffer[index].GetHeaderConst()->dest == address){
		//				return true;
		//			}
		//		}
		return false;
	}
	bool IsThereADataPacketWithDest(const BufferSize_T address){

		BufferSize_T index = 0;
		BufferSize_T numiter = 0;
		for(BufferSize_T j = 0; j < BUFFERSIZE && numiter < num_elements; ++j){
			index = last_used_index - j;
			if(j > last_used_index){
				index = (BUFFERSIZE - j) + last_used_index;
			}
			else{
				index = last_used_index - j;
			}
			if(buffer_owner[index] != BO_UNUSED) ++numiter;
			if(buffer_owner[index] == BO_OMAC){
				if( send_packet_buffer[index].GetHeaderConst()->payloadType != MFM_OMAC_TIMESYNCREQ && send_packet_buffer[index].GetHeaderConst()->dest == address){
					return true;
				}
			}
		}

		//		for(BufferSize_T index = 0; index < BUFFERSIZE; ++index){
		//			if( send_packet_buffer[index].GetHeaderConst()->payloadType != MFM_OMAC_TIMESYNCREQ && send_packet_buffer[index].GetHeaderConst()->dest == address){
		//				return true;
		//			}
		//		}

		return false;
	}
	Message_15_4_t* FindTSRPacketForNeighbor(const BufferSize_T neigh){
		Message_15_4_t* rm = NULL;
		BufferSize_T index = 0;
		BufferSize_T numiter = 0;
		for(BufferSize_T j = 0; j < BUFFERSIZE && numiter < num_elements; ++j){
			index = last_used_index - j;
			if(j > last_used_index){
				index = (BUFFERSIZE - j) + last_used_index;
			}
			else{
				index = last_used_index - j;
			}
			if(buffer_owner[index] != BO_UNUSED) ++numiter;
			if(buffer_owner[index] == BO_OMAC){
				if( send_packet_buffer[index].GetHeaderConst()->payloadType == MFM_OMAC_TIMESYNCREQ && send_packet_buffer[index].GetHeaderConst()->dest == neigh){
					if(rm == NULL){
						rm = &(send_packet_buffer[index]);
					}
					else if(rm -> GetMetaDataConst()->GetReceiveTimeStamp() > send_packet_buffer[index].GetMetaDataConst()->GetReceiveTimeStamp()){
						rm = &(send_packet_buffer[index]);
					}
				}
			}
		}
		//		for(BufferSize_T index = 0; index < BUFFERSIZE; ++index){
		//			if( send_packet_buffer[index].GetHeaderConst()->payloadType == MFM_OMAC_TIMESYNCREQ && send_packet_buffer[index].GetHeaderConst()->dest == neigh){
		//				if(rm == NULL){
		//					rm = &(send_packet_buffer[index]);
		//				}
		//				else if(rm -> GetMetaDataConst()->GetReceiveTimeStamp() > send_packet_buffer[index].GetMetaDataConst()->GetReceiveTimeStamp()){
		//					rm = &(send_packet_buffer[index]);
		//				}
		//			}
		//		}
		return rm;
	}
	Message_15_4_t* FindDataPacketForNeighbor(const BufferSize_T neigh){
		Message_15_4_t* rm = NULL;
		BufferSize_T index = 0;
		BufferSize_T numiter = 0;
		for(BufferSize_T j = 0; j < BUFFERSIZE && numiter < num_elements; ++j){
			index = last_used_index - j;
			if(j > last_used_index){
				index = (BUFFERSIZE - j) + last_used_index;
			}
			else{
				index = last_used_index - j;
			}
			if(buffer_owner[index] != BO_UNUSED) ++numiter;
			if(buffer_owner[index] == BO_OMAC){
				if( send_packet_buffer[index].GetHeaderConst()->payloadType != MFM_OMAC_TIMESYNCREQ && send_packet_buffer[index].GetHeaderConst()->dest == neigh){
					if(rm == NULL){
						rm = &(send_packet_buffer[index]);
					}
					else if(rm -> GetMetaDataConst()->GetReceiveTimeStamp() > send_packet_buffer[index].GetMetaDataConst()->GetReceiveTimeStamp()){
						rm = &(send_packet_buffer[index]);
					}
				}
			}
		}
		//		for(BufferSize_T index = 0; index < BUFFERSIZE; ++index){
		//			if( send_packet_buffer[index].GetHeaderConst()->payloadType != MFM_OMAC_TIMESYNCREQ && send_packet_buffer[index].GetHeaderConst()->dest == neigh){
		//				if(rm == NULL){
		//					rm = &(send_packet_buffer[index]);
		//				}
		//				else if(rm -> GetMetaDataConst()->GetReceiveTimeStamp() > send_packet_buffer[index].GetMetaDataConst()->GetReceiveTimeStamp()){
		//					rm = &(send_packet_buffer[index]);
		//				}
		//			}
		//		}
		return rm;
	}
	bool InsertMessage(Message_15_4_t* msg_carrier,  BufferOwner bufow = BO_OMAC){
		if(num_elements < BUFFERSIZE){

			for(BufferSize_T i = 0; i < BUFFERSIZE; ++i){
				BufferSize_T index = (i+last_used_index+1)%BUFFERSIZE;
				if( buffer_owner[index] == BO_UNUSED){
					memcpy(&(send_packet_buffer[index]), msg_carrier, sizeof(Message_15_4_t));
					buffer_owner[index] = bufow;
					last_used_index = index;
					++num_elements;
					return true;
				}
			}
		}
		return false;
	}
	PacketID_T InsertMessageGetIndex(Message_15_4_t* msg_carrier, BufferOwner bufow = BO_OMAC){
		if(num_elements < BUFFERSIZE){

			for(BufferSize_T i = 0; i < BUFFERSIZE; ++i){
				BufferSize_T index = (i+last_used_index+1)%BUFFERSIZE;
				if( buffer_owner[index] == BO_UNUSED ){
					memcpy(&(send_packet_buffer[index]), msg_carrier, sizeof(Message_15_4_t));
					buffer_owner[index] = bufow;
					last_used_index = index;
					++num_elements;
					return index;
				}
			}
		}
		return INVALID_PACKET_ID;
	}

	Message_15_4_t* GetPacketPtrWithIndex(PacketID_T index){
		if(ISPACKET_ID_VALID(index) && index < BUFFERSIZE){
			return (&(send_packet_buffer[index]));
		}
		else return NULL;
	}

	BufferSize_T GetNumberofElements() const {
		return num_elements;
	}

	void SetNumberofElements(BufferSize_T x) {
		num_elements = x;
	}

};




#endif /* MICROFRAMEWORKPK_V4_3_DEVICECODE_INCLUDE_SAMRAKSH_BUFFERS_ARRAYBUFFER_H_ */
