#pragma once

#include "Container.hpp"
#include "List.hpp"
#include "Container_Literals.h"

/*
*	@brief Container that is contiguous in memory and has a last in first out approach.
*/
template<class T> class Stack : public Container<T> {
public:
	Stack() = default;

	virtual void PushBack(T a_item) override {
		m_top = m_size++;							// Set top of list to bottom element index

		m_data.PushBack(a_item);					// Push back copy of item to data list
	}

	virtual void PopBack() override { 
		assert(m_size > 0 && "Attempting to pop back on an empty container.");
		// Decrease size of list to essentially 'ignore' last element and update 0-based top of list
		--m_top;
		--m_size;

		m_data.PopBack();
	}

	/**
	*	@brief	Search through container to see if item is in stack.
	*	@param	a_item is the item to search for.
	*/
	bool Contains(T a_item) {
		for (size_t i = 0; i < m_data.GetSize(); ++i) {
			if (m_data[i] == a_item) {
				return true;
			}
		}
		return false;
	}

	/**
	*	@brief	Return alias of element at the top of the container (relative).
	*/
	T&		Top() { return m_data[m_top]; }
protected:
	List<T>		m_data;								/*Dynamic list of data.*/
};