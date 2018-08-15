/*
 * Buffer.cpp
 *
 *  Created on: Aug 31, 2012
 *      Author: Mukundan Sridharan
 */
#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <tinyhal.h>
#include "Message.h"

#include <Samraksh/Buffers/CircularBuffer.h>
#include <Samraksh/Buffers/ArrayBuffer.h>

//#include <Samraksh/Buffers/BSTBuffer.h>

//#define BUFFERSIZE 10


template <UINT8 BufferSize>
class Buffer_15_4: public  CircularBuffer<Message_15_4_t, BufferSize>{
public:
	BOOL Initialize(){
		return CircularBuffer<Message_15_4_t, BufferSize>::Initialize();
	}

	BOOL CopyPayload(void* payload, UINT8 size){
		Message_15_4_t* dummy;
		if(CircularBuffer<Message_15_4_t, BufferSize>::IsFull()){
			return FALSE;
		}
		if(size > dummy->GetPayloadSize()){
			return FALSE;
		}
		Message_15_4_t* msg = this->GetNextFreeBuffer();
		memset(msg->GetPayload(), 0 , size);
		memcpy(msg->GetPayload(), payload , size);
		return TRUE;
	}

	BOOL Store(void* msg, UINT8 size){
		Message_15_4_t* dummy;
		if(CircularBuffer<Message_15_4_t, BufferSize>::IsFull()){
			return FALSE;
		}
		if(size > dummy->GetMessageSize()){
			return FALSE;
		}
		Message_15_4_t* buffer = this->GetNextFreeBuffer();
		memset(buffer, 0, sizeof(Message_15_4_t));
		memcpy(buffer, msg, sizeof(Message_15_4_t));
		return TRUE;
	}

	Message_15_4_t* GetOldest(){
		if(this->IsEmpty()){
			return (Message_15_4_t *)(NULL);
		}

		return this->GetFirstFullBuffer();
	}

	Message_15_4_t* GetOldestwithoutRemoval(){
		if(this->IsEmpty()){
			return (Message_15_4_t *)(NULL);
		}

		return this->PeekFirstFullBuffer();
	}

	BOOL IsBufferEmpty(){
		return this->IsEmpty();
	}

	BOOL IsBufferFull(){
		return this->IsFull();
	}

	Message_15_4_t** GetOldestPtr(){
		if(this->IsEmpty()){
			return (Message_15_4_t **)(NULL);
		}

		return this->GetFirstFullBufferPtr();
	}

	Message_15_4_t* GetNewest(){
		if(this->IsEmpty()){
			return (Message_15_4_t *)(NULL);
		}

		return this->GetLastFullBuffer();
	}

	void Erase(){
		this->ClearBuffer();
	}

	void DropOldest(UINT8 numMessages){
		for (int i = 0; i < numMessages; i++){
			GetOldest();
		}
	}

	void DropNewest(UINT8 numMessages){
		for (int i = 0; i < numMessages; i++){
			GetNewest();
		}
	}
};





#endif

//#include "Message.h"

/*
template<class MessageT>
struct Buffer : public HAL_DblLinkedNode<Buffer<MessageT> > {
	MessageT* msg;
};

template<class BufferT, class MessageT, UINT8 ListSizeT>
struct BufferList  {
	HAL_DblLinkedList<BufferT> List;
	BufferT buf[ListSizeT];
	MessageT msg[ListSizeT];

	void Initialize(){
		int i=0;
		//List.Insert(NULL,&buf[1],buf);
		buf[0].Initialize();
		buf[0].msg = &msg[0];
		buf[i].SetNext(&buf[1]);

		for (i=1; i<ListSizeT-1; i++){
			buf[i].msg = &msg[i];
			buf[i].Initialize();
			buf[i].SetNext(&buf[i+1]);
			buf[i].SetPrev(&buf[i-1]);
		}
		buf[ListSizeT-1].SetPrev(&buf[ListSizeT-2]);
		buf[ListSizeT-1].msg = &msg[ListSizeT-1];

		List.Initialize();
	}
};

 */
