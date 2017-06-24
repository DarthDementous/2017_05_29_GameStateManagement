#pragma once

#include "Container.hpp"
#include "LinkedList.hpp"
#include "Container_Literals.h"

/*
*	@brief Container that is contiguous in memory and has a first in first out approach.
*/
template<class T> class Queue : public Container<T> {
public:
	Queue() {
		m_top = 0;
	}

	virtual void PushBack(T a_item) override {
		++m_size;

		m_data.PushBack(a_item);					// Push back item to end of the queue
	}

	virtual void PopBack() override {
		assert(m_size > 0 && "Attempting to pop front on an empty container.");
		m_size--;									// Decrease size

		m_data.PopFront();							// Remove item at the front of the queue
	}

	/**
	*	@brief	Return alias of element at the top of the container (relative).
	*/
	T&		Top() { return (*m_data.begin()); }
protected:
	LinkedList<T>		m_data;						/*Linked list of data, no need to shift memory on every list modification.*/
};