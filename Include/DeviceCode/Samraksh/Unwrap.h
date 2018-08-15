#ifndef _UNWRAP_H_INCLUDED_
#define _UNWRAP_H_INCLUDED_

#define MAX_IQ_REJECTION 4000 

INT16 findMedian(UINT16* buffer, INT32 length);
int calculatePhase(UINT16* bufferI, UINT16* bufferQ, UINT16* bufferUnwrap, INT32 length, INT16 medianI, INT16 medianQ, INT32 noiseRejection, UINT16 debugVal, UINT16 IDNumber, UINT16 versionNumber, UINT16 classifierTarget);
int unwrapPhase(INT16 valueI, INT16 valueQ, INT16* arcTan, INT32 noiseRejection);
int unwrapCrossProduct(INT16 valueI, INT16 valueQ, INT32 noiseRejection);
int getUnwrapMax();
int getUnwrapZero();
int getDisplacement(INT32 portion);
int getAbsoluteDisplacement(INT32 portion);
int getRange(INT32 portion);
INT16 getAbsOffsetQ();
INT16 getAbsOffsetI();
INT16 getCountOverTarget();

#endif
