#error "THIS IS NOT THE HEAP YOU ARE LOOKING FOR..."

/*
*	Name	:	heap.h
*   Author  :   nived.sivadas
*   Description : This data structure is primarily designed for the virtual timer interface
*/

#ifndef _SAMRAKSH_HEAP_H_
#define _SAMRAKSH_HEAP_H_



#if defined(WIN_DEBUG_ENABLE)
#include <iostream>
#include "MFUtil.h"

using namespace std;
#else

#include <tinyhal.h>

#endif
// Macros defined in priority queue of known size

// Parent and child pointers are satisfied by assuming constant positions in the array with respect to each other in a heap
#define PARENT(x) ((x == 0) ? -1 : (x-1)/2)
#define LEFTCHILD(x) (2 * x + 1)
#define RIGHTCHILD(x) (2 * x + 2)


template<typename T>
struct myless
{
public:
	BOOL operator()(T item1, T item2)
	{
		return (item1 > item2);
	}
};

// The class is defaulted to a min heap, change to a max heap by changing functor
template<typename T, size_t size, typename Compare = myless<T> >
class Hal_Heap_KnownSize
{
	T data[size] = {0};					/* body of the queue */

	UINT8 n = 0;		/* number of queue elements */

	Compare comp;

	void swap(int p1, int p2)
	{
		T temp = data[p1];
		data[p1] = data[p2];
		data[p2] = temp;
	}

	void bubble_up(int pos)
	{

		if(PARENT(pos) == -1) return; /* at root of heap, there are no parents */

		if(comp(data[PARENT(pos)], data[pos]))
		{
			swap(PARENT(pos), pos);
			bubble_up(PARENT(pos));
		}

	}


	void bubble_down(int p)
	{
		int child =  0;
		int minIndex = p;

		child = LEFTCHILD(p);

		// Compare with both the children and replace with the
		for(int i = 0; i <= 1; i++)
		{
			if(child + i <= n)
			{
				if(comp(data[minIndex], data[child + i]))
				{
					minIndex = child + i;
				}
			}
		}

		if(minIndex != p)
		{
			swap(p,minIndex);
			bubble_down(minIndex);
		}

	}

public:

	void heapify(int p)
	{
		//Don't heapify if there is only one element in the heap
		if(n <= 1)
			return;

		bubble_down(p);
	}

	BOOL Insert(T item)
	{
		if(n >= size)
		{
			return FALSE;
		}
		else
		{

			data[n] = item;
			bubble_up(n);
			n++;
		}

		return TRUE;

	}

	T ExtractTop()
	{
		T min = (T) 0;

		if(n <= 0)
		{
			return (T) 0;  /* Attempting to extract from an empty queue */
		}
		else
		{
			min = data[0];

			data[0] = data[n - 1];
			--n;
			bubble_down(0);

		}

		return min;
	}

	T inline PeekTop()
	{
		return data[0];
		/*T min = (T) 0;

		if(n <= 0)
		{
			return (T) 0;
		}
		else
		{
			bubble_down(0);
		}

		return data[0];*/
	}



#if defined(WIN_DEBUG_ENABLE)
	void PrintHeap()
	{
		for(int i = 0; i < size; i++)
		{
			T val = ExtractTop();
			std::cout << *val << endl;
		}
	}

#endif

};

#endif
