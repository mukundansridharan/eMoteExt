/*
 * CircularBuffer.h
 *
 *  Created on: Mar 23, 2017
 *      Author: Bora
 */

#ifndef MICROFRAMEWORKPK_V4_3_DEVICECODE_INCLUDE_SAMRAKSH_BUFFERS_CIRCULARBUFFER_H_
#define MICROFRAMEWORKPK_V4_3_DEVICECODE_INCLUDE_SAMRAKSH_BUFFERS_CIRCULARBUFFER_H_

//#define _DEBUG_BUFFER_

template< class MessageT, UINT8 BufferSizeT>
class CircularBuffer  {

	MessageT *msgPtr[BufferSizeT];
	MessageT msg[BufferSizeT];
	UINT8 nextCleanBuffer, firstFullBuffer;
	UINT8 numElements;

public:

	UINT8 GetFirstFullBufferPosition(){
		return firstFullBuffer;
	}

	MessageT* PeekElementAtPosition(UINT8 cur_pos){
		//Check if valid element
		return msgPtr[cur_pos];
	}

	BOOL DropElementAtPosition(UINT8 cur_pos){
		//Exchange pointers
		if (cur_pos == firstFullBuffer) {
			GetFirstFullBuffer();
			return true;
		}
		else{
			UINT8 j,i = cur_pos;
			j = i;
			bool rtn_var = false;
			i += 1;
			if(i >= BufferSizeT){
				i = 0;
			}
			if(i == nextCleanBuffer){
				rtn_var = true;
			}
			else{
				while (i != nextCleanBuffer){
					rtn_var = true;
					msgPtr[j] = msgPtr[i];
					j = i;
					i += 1;
					if(i >= BufferSizeT){
						i = 0;
					}
				}
			}

			if(rtn_var){
				--numElements;
				if(nextCleanBuffer == 0 ) nextCleanBuffer = BufferSizeT - 1;
				else --nextCleanBuffer;
			}
			return rtn_var;
		}

	}

	BOOL Initialize(){
		int i = 0;
		for (i = 0; i < BufferSizeT; i++){
			msgPtr[i] = &msg[i];
		}
		numElements = 0;
		nextCleanBuffer = 0;
		firstFullBuffer = 0;
		return TRUE;
	}

	MessageT** GetNextFreeBufferPtr()
	{
		if(this->IsFull()){
			return (Message_15_4_t**)(NULL);
		}

		MessageT** rtn= &msgPtr[nextCleanBuffer];
		nextCleanBuffer += 1;
		if(nextCleanBuffer >= BufferSizeT){
			nextCleanBuffer = 0;
		}
		numElements++;
#ifdef _DEBUG_BUFFER_
		hal_printf("GetNextFreeBufferPtr numElements: %u\n", numElements);
#endif
		return rtn;

	}

	MessageT* GetNextFreeBuffer(){
		if(this->IsFull()){
			return (Message_15_4_t*)(NULL);
		}

		MessageT* rtn = msgPtr[nextCleanBuffer];
		nextCleanBuffer += 1;
		if(nextCleanBuffer >= BufferSizeT){
			nextCleanBuffer = 0;
		}
		//nextCleanBuffer= (nextCleanBuffer++) % 16;
		numElements++;
#ifdef _DEBUG_BUFFER_
		hal_printf("GetNextFreeBuffer numElements: %u\n", numElements);
#endif
		return rtn;
	}

	MessageT* GetFirstFullBuffer(){
		MessageT* rtn = msgPtr[firstFullBuffer];
		firstFullBuffer += 1;
		if(firstFullBuffer >= BufferSizeT){
			firstFullBuffer = 0;
		}
		//firstFullBuffer= (firstFullBuffer++) % BufferSizeT;
		numElements--;
#ifdef _DEBUG_BUFFER_
		hal_printf("GetFirstFullBuffer numElements: %u\n", numElements);
#endif
		return rtn;
	}

	MessageT* PeekFirstFullBuffer(){
		MessageT* rtn = msgPtr[firstFullBuffer];
		//		firstFullBuffer += 1;
		//		if(firstFullBuffer >= BufferSizeT){
		//			firstFullBuffer = 0;
		//		}
		//		//firstFullBuffer= (firstFullBuffer++) % BufferSizeT;
		//		numElements--;
#ifdef _DEBUG_BUFFER_
		hal_printf("GetFirstFullBuffer numElements: %u\n", numElements);
#endif
		return rtn;
	}

	MessageT** GetFirstFullBufferPtr(){
		MessageT** rtn = &msgPtr[firstFullBuffer];
		firstFullBuffer += 1;
		if(firstFullBuffer >= BufferSizeT){
			firstFullBuffer = 0;
		}
		//firstFullBuffer= (firstFullBuffer++) % BufferSizeT;
		numElements--;
#ifdef _DEBUG_BUFFER_
		hal_printf("GetFirstFullBufferPtr numElements: %u\n", numElements);
#endif
		return rtn;
	}

	MessageT* GetLastFullBuffer(){
		nextCleanBuffer -= 1;
		UINT8 lastFullBuffer = nextCleanBuffer % BufferSizeT;
		MessageT* rtn = msgPtr[lastFullBuffer];
		nextCleanBuffer = lastFullBuffer;
		numElements--;
#ifdef _DEBUG_BUFFER_
		hal_printf("GetLastFullBuffer numElements: %u\n", numElements);
#endif
		return rtn;
	}

	void ClearBuffer(){
		while(!IsEmpty()){
			GetFirstFullBuffer();
		}
	}

	BOOL IsFull(){
#ifdef _DEBUG_BUFFER_
		hal_printf("IsFull numElements: %u\n", numElements);
#endif
		if(numElements == BufferSizeT){
			return TRUE;
		}
		else{
			return FALSE;
		}
	}

	BOOL IsEmpty(){
#ifdef _DEBUG_BUFFER_
		hal_printf("IsFull numElements: %u\n", numElements);
#endif
		if(numElements == 0){
			return TRUE;
		}
		else{
			return FALSE;
		}
	}

	UINT8 GetNumberMessagesInBuffer(){
		return numElements;
	}

	UINT8 Size(){
		return BufferSizeT;
	}


};



#endif /* MICROFRAMEWORKPK_V4_3_DEVICECODE_INCLUDE_SAMRAKSH_BUFFERS_CIRCULARBUFFER_H_ */
