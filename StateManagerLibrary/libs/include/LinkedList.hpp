#pragma once

#include "List.hpp"

/**
*	@brief Container that is non-contiguous in memory.
*/
template<class T> class LinkedList : public Container<T> {
public:
	LinkedList() = default;

	LinkedList(LinkedList& a_other) {
		Copy(a_other);
	}

	/**
	*	@brief Delete all dynamically allocated LinkNodes
	*/
	~LinkedList() {
		Erase(begin(), end());
	}

	LinkedList& operator=(LinkedList& a_other) {
		Copy(a_other);
	}

	/**
	*	@brief Deep copy all variables and nodes in the LinkedList (necessary for pointers)
	*	NOTE: Gets run INSTEAD of default constructor if its called on declaration of new container.
	*	@param a_other is the LinkedList to copy data from.
	*/
	void Copy(LinkedList& a_other) {
		// Free up memory in previous LinkedList if it hasn't been initialized
		if (m_firstNode) {
			Erase(begin(), end());
		}

		// Create new linked list from the values in the old one
		for (auto val : a_other) {
			PushBack(val);
		}
	}

	virtual void PushBack(T a_item) override {
		++m_size;

		// Dynamically allocate memory for the node (only once or else memory leaks)
		LinkNode* tmp = new LinkNode(a_item);

		// Initialize first and last nodes with new node in no longer empty list
		if (m_firstNode == nullptr) {
			m_firstNode = tmp;
			m_lastNode	= tmp;		
		}
		// If LinkedList is not empty, point new LinkNode at previously last element and position it in the list with pointers
		else {
			m_lastNode->m_next	= tmp;
			tmp->m_prev			= m_lastNode;
			m_lastNode			= tmp;
		}
	}

	void PushFront(T a_item) {
		++m_size;

		LinkNode* tmp = new LinkNode(a_item);

		// Initialize first and last nodes with new node in no longer empty list
		if (m_firstNode == nullptr) {
			m_firstNode = tmp;
			m_lastNode	= tmp;
		}
		// If LinkedList is not empty, point new LinkNode at previously last element and position it in the list with pointers
		else {
			m_firstNode->m_prev = tmp;
			tmp->m_next = m_firstNode;
			m_firstNode = tmp;
		}
	}

	virtual void PopBack() override {
		assert(m_firstNode != nullptr && "Attempting to pop back on empty linked list.");

		// Erase end node
		Erase(LinkIterator(m_lastNode));
	}

	void PopFront() {
		assert(m_firstNode != nullptr && "Attempting to pop front on empty linked list.");

		// Erase front node
		Erase(LinkIterator(m_firstNode));
	}

#pragma region LinkNode
	/**
	*	@brief Entity that holds a value and pointers to the previous and next element in the linked list.
	*/
	struct LinkNode {
		LinkNode()	 = default;
		LinkNode(T a_val) : m_value(a_val) {}
		~LinkNode()	 = default;

		T			m_value;						// Current copy in possession
		LinkNode*	m_next	 = nullptr;				// Next element in linked list
		LinkNode*	m_prev   = nullptr;				// Previous element in linked list
		
		T& GetValue() { return m_value; }
	};
#pragma endregion

#pragma region Iteration functionality
	/**
	*	@brief Entity that holds a pointer to a link node and can be used in iteration structures.
	*/
	struct LinkIterator {
		LinkIterator(LinkNode* a_link) : m_link(a_link) {}
		LinkNode*	m_link;						/*LinkNode we're pointing at*/

		LinkIterator& operator--() {			/*PREFIX: Move to previous link node value*/
			m_link = m_link->prev;
			return *this;
		}

		LinkIterator operator--(int) {			/*POSTFIX: Move to the previous link node value after leaving the expression*/
			LinkNode result(*this);
			--(*this);
			return result;
		}

		LinkIterator& operator++() {			/*PREFIX: Move to next link node value*/
			m_link = m_link->m_next;
			return *this;
		}

		LinkIterator operator++(int) {			/*POSTFIX: Move to the next link node value after leaving the expression*/
			LinkNode result(*this);
			++(*this);
			return result;
		}

		///Check if the link node is the sentinel node by checking if its pointing to nullptr
		bool operator==(LinkIterator a_iter) {
			return (m_link == a_iter.GetLink());
		}

		bool operator!=(LinkIterator a_iter) {
			return (m_link != a_iter.GetLink());
		}

		LinkNode* GetLink() { return m_link; }

		void EraseNode() {
			delete m_link;
			m_link = nullptr;
		}

		T& operator*() {						/*De-reference and return aliased value in LinkNode*/
			return m_link->GetValue();
		}
	};

#pragma endregion

	/**
	*	@brief Delete dynamically allocated memory held by iterator
	*	@param a_iter is the iterator position to delete at
	*/
	LinkIterator Erase(LinkIterator a_iter) {
		assert(a_iter.GetLink() != nullptr && "Attempting to erase a null iterator.");
		
		// Decrease size
		--m_size;

		// Save next position
		LinkIterator nextPos = a_iter;
		++nextPos;

		// Only sever ties if there are any
		LinkNode* nextNode = a_iter.GetLink()->m_next;
		LinkNode* prevNode = a_iter.GetLink()->m_prev;

		if (nextNode) {
			nextNode->m_prev = prevNode;
		} else {
			// We are erasing the last node.
			m_lastNode = prevNode;			
		}
		if (prevNode) {
			prevNode->m_next = nextNode;
		}
		else {
			// We are erasing the first node.
			m_firstNode = nextNode;			
		}

		// Clear dynamically allocated memory
		a_iter.EraseNode();

		return nextPos;
	}

	/**
	*	@brief Delete dynamically allocated memory held by iterator in a range
	*	@param a_first is the minimum range of removal
	*	@param a_last is the maximum range of removal
	*	@returns iterator position after deleted elements in linked list
	*/
	LinkIterator Erase(LinkIterator a_first, LinkIterator a_last) {
		auto currentIter = a_first;
		while (currentIter != a_last) {
			currentIter = Erase(currentIter);
		}

		return currentIter;
	}

#pragma region Ranged-for functionality
	LinkIterator begin() {
		return LinkIterator(m_firstNode);
	}

	LinkIterator end() {						/*Point after the linked list ends (sentinel iterator)*/
		return LinkIterator(nullptr);
	}
#pragma endregion

protected:
	// Keep track of beginning and end of linked list
	LinkNode* m_firstNode	= nullptr;
	LinkNode* m_lastNode	= nullptr;
};
