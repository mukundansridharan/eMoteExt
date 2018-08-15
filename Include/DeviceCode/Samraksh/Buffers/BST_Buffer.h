/*
 * BST_Buffer.h
 *
 *  Created on: Mar 23, 2017
 *      Author: Bora
 */

#ifndef MICROFRAMEWORKPK_V4_3_DEVICECODE_INCLUDE_SAMRAKSH_BUFFERS_BST_BUFFER_H_
#define MICROFRAMEWORKPK_V4_3_DEVICECODE_INCLUDE_SAMRAKSH_BUFFERS_BST_BUFFER_H_

#include <Samraksh\Message.h>
#include "FAAVLBinarySearchTreeT.h"

class Message_15_4_t_Searchable : public Message_15_4_t{
public:
	bool SearchWithDest;
	bool SearchWithPayloadType;
	bool SearchWithRetryAttempt;
	bool SearchWithReceiveTimeStamp;

	//	Message_15_4_t_Searchable() : SearchWithDest(true), SearchWithPayloadType(true), SearchWithRetryAttempt(true), SearchWithReceiveTimeStamp(true){
	//		ClearMsgContents((Message_15_4_t*)this);
	//	};

	Message_15_4_t_Searchable(){
		Clear();
	}

	void Clear(){
		SearchWithDest = true;
		SearchWithPayloadType = true;
		SearchWithRetryAttempt = true;
		SearchWithReceiveTimeStamp = true;
		ClearMsgContents((Message_15_4_t*)this);
	}
};


class MeesageReceiveTimeStampComparator {
public:
	static bool LessThan (const Message_15_4_t_Searchable& A, const Message_15_4_t_Searchable& B) {
		if( !(A.SearchWithReceiveTimeStamp && B.SearchWithReceiveTimeStamp) ) return false;
		return(A.GetMetaDataConst()->GetReceiveTimeStamp() < B.GetMetaDataConst()->GetReceiveTimeStamp());
	}
	static bool EqualTo (const Message_15_4_t_Searchable& A, const Message_15_4_t_Searchable& B) {
		if(!(A.SearchWithReceiveTimeStamp && B.SearchWithReceiveTimeStamp)) return true;
		return(A.GetMetaDataConst()->GetReceiveTimeStamp() == B.GetMetaDataConst()->GetReceiveTimeStamp());
	}
};

class MeesageRetryAttemptComparator {
public:
	static bool LessThan (const Message_15_4_t_Searchable& A, const Message_15_4_t_Searchable& B) {
		if(!(A.SearchWithRetryAttempt && B.SearchWithRetryAttempt) ) return false;
		return(A.GetMetaDataConst()->GetRetryAttempts() < B.GetMetaDataConst()->GetRetryAttempts());
	}
	static bool EqualTo (const Message_15_4_t_Searchable& A, const Message_15_4_t_Searchable& B) {
		if(!(A.SearchWithRetryAttempt && B.SearchWithRetryAttempt )) return true;
		return(A.GetMetaDataConst()->GetRetryAttempts() == B.GetMetaDataConst()->GetRetryAttempts());
	}
};

class MeesagePayloadTypeComparator {
public:
	static bool LessThan (const Message_15_4_t_Searchable& A, const Message_15_4_t_Searchable& B) {
		if(!(A.SearchWithPayloadType && B.SearchWithPayloadType )) return false;
		return(A.GetHeaderConst()->payloadType < A.GetHeaderConst()->payloadType);
	}
	static bool EqualTo (const Message_15_4_t_Searchable& A, const Message_15_4_t_Searchable& B) {
		if(!(A.SearchWithPayloadType && B.SearchWithPayloadType )) return true;
		return(A.GetHeaderConst()->payloadType == A.GetHeaderConst()->payloadType);
	}
};

class MeesageDestIDComparator {
public:
	static bool LessThan (const Message_15_4_t_Searchable& A, const Message_15_4_t_Searchable& B) {
		if( !(A.SearchWithDest && B.SearchWithDest) ) return false;
		return(A.GetHeaderConst()->GetDestConst() < A.GetHeaderConst()->GetDestConst());
	}
	static bool EqualTo (const Message_15_4_t_Searchable& A, const Message_15_4_t_Searchable& B) {
		if( !(A.SearchWithDest && B.SearchWithDest) ) return true;
		return(A.GetHeaderConst()->GetDestConst() == A.GetHeaderConst()->GetDestConst());
	}
};

class CustomDestIDPayloadTypeRetryAttemptMessageCreationTimeComparator {
public:
	static bool LessThan (const Message_15_4_t_Searchable& A, const Message_15_4_t_Searchable& B) {
		if(MeesageDestIDComparator::EqualTo(A,B)){
			if(MeesageReceiveTimeStampComparator::EqualTo(A,B)){
				if(MeesagePayloadTypeComparator::EqualTo(A,B)){
					if(MeesageRetryAttemptComparator::EqualTo(A,B)){
						return false;
					}
					else return MeesageReceiveTimeStampComparator::LessThan(A,B);
				}
				else return MeesageRetryAttemptComparator::LessThan(A,B);
			}
			else return MeesagePayloadTypeComparator::LessThan(A,B);
		}
		else return MeesageDestIDComparator::LessThan(A,B);

	}
	static bool EqualTo (const Message_15_4_t_Searchable& A, const Message_15_4_t_Searchable& B) {
		if( MeesageDestIDComparator::EqualTo(A,B)
		&& 	MeesagePayloadTypeComparator::EqualTo(A,B)
		&& 	MeesageRetryAttemptComparator::EqualTo(A,B)
		&& 	MeesageReceiveTimeStampComparator::EqualTo(A,B)
		) return true;
		else return false;
	}
};


void ClearMsgContents(Message_15_4_t_Searchable* msg){
	memset(msg,0,sizeof(Message_15_4_t_Searchable));
}

template <FAAVLBST_Size_t BufferSizeT>
class Buffer_15_4_3 : public FAAVLBST_T<BufferSizeT, Message_15_4_t_Searchable, CustomDestIDPayloadTypeRetryAttemptMessageCreationTimeComparator>, public OMAC_Common_Buffer_I {
private:

	FAAVLBSTElement<Message_15_4_t_Searchable>* FindRightMostElement(const Message_15_4_t_Searchable &_data){
		FAAVLBSTElement<Message_15_4_t_Searchable>* el_ptr = this->Search(_data);
		FAAVLBSTElement<Message_15_4_t_Searchable>* next_el_ptr;
		while(el_ptr){
			next_el_ptr = this->Next(el_ptr);
			if(next_el_ptr && CustomDestIDPayloadTypeRetryAttemptMessageCreationTimeComparator::EqualTo(next_el_ptr->data,_data)){
				el_ptr = next_el_ptr;
			}
			else break;
		}
		return(el_ptr);
	}
	FAAVLBSTElement<Message_15_4_t_Searchable>* FindLeftMostElement(const Message_15_4_t_Searchable &_data){
		FAAVLBSTElement<Message_15_4_t_Searchable>* el_ptr = this->Search(_data);
		FAAVLBSTElement<Message_15_4_t_Searchable>* next_el_ptr;
		while(el_ptr){
			next_el_ptr = this->Previous(el_ptr);
			if(next_el_ptr && CustomDestIDPayloadTypeRetryAttemptMessageCreationTimeComparator::EqualTo(next_el_ptr->data,_data)){
				el_ptr = next_el_ptr;
			}
			else break;
		}
		return(el_ptr);
	}


public:
	void DeletePacket(Message_15_4_t* msg_carrier){
		Message_15_4_t_Searchable temp_msg;
		Message_15_4_t* temp_base_ptr = &temp_msg;
		memcpy(temp_base_ptr,msg_carrier, sizeof(Message_15_4_t));
		this->DeleteElement(this->Search(temp_msg));
		//		DeleteElement(Search(temp_msg));
	}
	bool RemovePacketsWithRetryAttemptsGreaterThan(UINT8 r){
		bool ret = false;
		for(FAAVLBSTElement<Message_15_4_t_Searchable>* it = this->Begin(); it != NULL; it = this->Next(it)){
			if(it->data.GetMetaDataConst()->GetRetryAttempts() > r){
				this->DeleteElement(it);
				if(!ret) ret = true;
			}
		}
		return ret;
	}

	bool IsThereATSRPacketWithDest(const UINT16 address){
		Message_15_4_t_Searchable temp_msg;
		temp_msg.GetHeader()->dest = address;
		temp_msg.GetHeader()->payloadType = MFM_OMAC_TIMESYNCREQ;
		temp_msg.SearchWithRetryAttempt = false;
		temp_msg.SearchWithReceiveTimeStamp = false;
		if(this->Search(temp_msg)){
			return true;
		}
		else
			return false;

	}
	bool IsThereADataPacketWithDest(const UINT16 address){
		Message_15_4_t_Searchable temp_msg;
		temp_msg.GetHeader()->dest = address;
		temp_msg.SearchWithPayloadType = false;
		temp_msg.SearchWithRetryAttempt = false;
		temp_msg.SearchWithReceiveTimeStamp = false;
		FAAVLBSTElement<Message_15_4_t_Searchable>* el_ptr = FindLeftMostElement(temp_msg);
		if(el_ptr && el_ptr->data.GetHeader()->payloadType != MFM_OMAC_TIMESYNCREQ){
			return true;
		}
		else
			return false;
	}
	Message_15_4_t* FindTSRPacketForNeighbor(const UINT16 neigh){
		Message_15_4_t_Searchable temp_msg;
		temp_msg.GetHeader()->dest = neigh;
		temp_msg.GetHeader()->payloadType = MFM_OMAC_TIMESYNCREQ;
		temp_msg.SearchWithRetryAttempt = false;
		temp_msg.SearchWithReceiveTimeStamp = false;
		FAAVLBSTElement<Message_15_4_t_Searchable>* el_ptr = FindRightMostElement(temp_msg);

		if(el_ptr){
			Message_15_4_t* ret_ptr = &(el_ptr->data);
			return ret_ptr;
		}
		else return NULL;
	}
	Message_15_4_t* FindDataPacketForNeighbor(const UINT16 neigh){
		Message_15_4_t_Searchable temp_msg;
		temp_msg.GetHeader()->dest = neigh;
		temp_msg.SearchWithPayloadType = false;
		temp_msg.SearchWithRetryAttempt = false;
		temp_msg.SearchWithReceiveTimeStamp = false;
		FAAVLBSTElement<Message_15_4_t_Searchable>* el_ptr = FindLeftMostElement(temp_msg);

		FAAVLBSTElement<Message_15_4_t_Searchable>* next_el_ptr;
		FAAVLBSTElement<Message_15_4_t_Searchable>* selected_el_ptr = NULL;
		while(el_ptr){
			if(el_ptr->data.GetHeaderConst()->payloadType != MFM_OMAC_TIMESYNCREQ){
				if(selected_el_ptr) {
					if(MeesageReceiveTimeStampComparator::LessThan(selected_el_ptr->data,el_ptr->data)){
						selected_el_ptr = el_ptr;
					}
				}
				else{
					selected_el_ptr = el_ptr;
				}
			}
			next_el_ptr = this->Next(el_ptr);
			if(next_el_ptr && CustomDestIDPayloadTypeRetryAttemptMessageCreationTimeComparator::EqualTo(next_el_ptr->data,temp_msg)){
				el_ptr = next_el_ptr;
			}
			else break;
		}

		if(selected_el_ptr){
			Message_15_4_t* ret_ptr = &(selected_el_ptr->data);
			return ret_ptr;
		}
		else return NULL;
	}
	bool InsertMessage(Message_15_4_t* msg_carrier){
		Message_15_4_t_Searchable temp_msg;
		Message_15_4_t* temp_base_ptr = &temp_msg;
		memcpy(temp_base_ptr,msg_carrier, sizeof(Message_15_4_t));
		return this->Insert(temp_msg);
	}
	UINT16 GetNumberofElements() const{
		return this->Size();
	}
};



#endif /* MICROFRAMEWORKPK_V4_3_DEVICECODE_INCLUDE_SAMRAKSH_BUFFERS_BST_BUFFER_H_ */
