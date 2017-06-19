#pragma once
#include "Container.hpp"
#include "Container_Literals.h"


template<class T> class List : public Container<T> {
public:
	/**
	*	@brief A dynamic flexible array that can have its size increased easily through shifting blocks of memory.
	*	@param a_capacity is the starting amount of data you want the list to hold. (LIST_CAP by default).
	*	@param a_val gives the option of assigning one value to all elements in range of a_capacity. (default constructor value by default).
	*/
	List(size_t a_capacity = LIST_CAP, T a_val = T()) : m_capacity(a_capacity) {	
		m_data = new T[m_capacity];							// Assign m_data a pointer to a new block of memory
	};

	/**
	*	@brief Delete memory in list.
	*/
	~List() { delete[] m_data; }				

	virtual void PushBack(T a_item) override {
		// If adding to list goes over capacity, copy over old data to new one
		m_top = m_size++;
		if (m_size > m_capacity) {
			T* newMem = new T[m_size];						// Reserve space for combined data
			memcpy(newMem, m_data, sizeof(T) * m_size);		// Copy over current data to new space in memory

			// Clear previous data and save pointer to new memory
			delete[] m_data;
			m_data = newMem;

			++m_capacity;
		}

		// Add item to end of list
		m_data[m_top] = a_item;
	}

	virtual void PopBack() override {
		assert(m_size > 0 && "Attempting to pop back on an empty container.");

		m_size--;											// Decrease size to essentially 'ignore' last element
	}

	T& operator[](size_t a_index) {
		assert(a_index < m_size && "Attempting to access list with out-of-bounds index.");

		return m_data[a_index];
	}

	/**
	*	@brief	Return alias of element at the top of the container (relative).
	*/
	T&		Top() { return m_data[m_top]; }
protected:
	T*			m_data;										/* Array of data in container*/
	size_t		m_capacity;									/* How much data the list is intended to hold.*/
};