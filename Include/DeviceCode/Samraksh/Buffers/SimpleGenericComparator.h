/*
//  * SimpleGenericComparator.h
 *	Implements a simple comparator to be used in various data structures. The comparator, namely SimpleGenericComparator, uses overloaded <  and == operators for decisions.
 *  Created on: Feb 10, 2015
 *      Author: Bora Karaoglu
 *

 */
#ifndef SIMPLEGENERICCOMPARATOR_H_
#define SIMPLEGENERICCOMPARATOR_H_

template<class T>
class SimpleGenericComparator {
public:
	static bool LessThan (const T& A, const T& B) {
		return (A < B);
	}
	static bool EqualTo (const T& A, const T& B) {
		return (A == B);
	}
};


#endif //SIMPLEGENERICCOMPARATOR_H_
