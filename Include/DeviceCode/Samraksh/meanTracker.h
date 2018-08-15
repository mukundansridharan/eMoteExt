#ifndef _MEANTRACKER_H_INCLUDED_
#define _MEANTRACKER_H_INCLUDED_

typedef int Item;

typedef struct HeapTrack_t
{
   Item* data;  //circular queue of values
   int*  pos;   //index into `heap` for each value
   int*  heap;  //max/median/min heap holding indexes into `data`.
   int   N;     //allocated size.
   int   idx;   //position in circular queue
   int   minCt; //count of items in min heap
   int   maxCt; //count of items in max heap
} HeapTrack;

HeapTrack* HeapTrackNew(int nItems);
void HeapTrackInsert(HeapTrack* m, Item v);
Item HeapTrackMedian(HeapTrack* m);
void ResetHeapTrack(HeapTrack* m, int nItems);

int heapTest(void);

#endif
