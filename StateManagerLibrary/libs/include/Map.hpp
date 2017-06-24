#pragma once

#include <assert.h>

/*
CONSTRUCTION:
std::map<textureID, std::shared_ptr<aie::Texture>>  m_textureList;   //We want to assign the same texture to multiple things, therefore shared pointer
std::map<fontID, std::shared_ptr<aie::Font>>		m_fontList;
std::map<audioID, std::shared_ptr<aie::Audio>>      m_audioList;

SETTING VALUES TO KEYS:
m_textureList[TITLE] = ResourceManager::loadResource<aie::Texture>("./Asteroid Fall/Art/title.png");
m_textureList[BTN] = ResourceManager::loadResource<aie::Texture>("./Asteroid Fall/Art/button_base.png");
m_textureList[M_BG] = ResourceManager::loadResource<aie::Texture>("./Asteroid Fall/Art/menu.png");

GETTING VALUES FROM KEYS:
m_Player = EntityMaker::createPlayer(m_textureList[PLAYER_SPRITE].get());

RULES:
- Must sort its values by key in numerical/alphabetical order (smallest to biggest by default)
- Must hold non-contiguous PairNodes
- No two values in a map can have the same key
*/

/**
*	@brief Container of paired key-values stored in a binary tree.
*/
template<typename K, typename V> class Map {
public:
	Map() = default;
	~Map() {
		// If not empty, destroy all nodes
		if (m_rootNode) {
			RecurDestroyTree(m_rootNode);
		}
	}

#pragma region Pair Node
	struct PairNode {
		PairNode(K a_key, V a_val = V()) : m_key(a_key), m_val(a_val) {}

		K m_key;
		V m_val;
	};
#pragma endregion

#pragma region Tree Node
	struct TreeNode {
		TreeNode(PairNode* a_pair) : m_pair(a_pair) {}

		K GetKey() { return m_pair->m_key; }
		V GetValue() { return m_pair->m_value; }

		bool HasValidLeft() { 
			if (m_left) {
				return !m_left->visited;
			}
			return false;
		}

		bool HasValidRight() {
			if (m_right) {
				return !m_right->visited;
			}
			return false;
		}

		// See if there's any unvisited children at any of the visited parents
		TreeNode* GetFamilyParent() { 
			TreeNode* currentParent = m_parent;

			while (currentParent) {
				// A parent with unvisited children still exists, not at the end.
				if (currentParent->HasValidLeft() || currentParent->HasValidRight()) {
					return currentParent;
				}
				currentParent = currentParent->m_parent;
			}

			return nullptr;
		}		

		PairNode* m_pair	= nullptr;
		TreeNode* m_parent	= nullptr;
		TreeNode* m_left	= nullptr;
		TreeNode* m_right	= nullptr;

		// Traversal variables
		bool visited = false;										/*Track if node is visited to avoid returning duplicate pairs*/
	};
#pragma endregion

#pragma region Tree Iterator and Ranged-for Functionality
	struct TreeIterator {
		// I am Map's friend, I have access to its protected and private variables
		friend class Map<K, V>;	

		TreeIterator(TreeNode* a_ptr) : m_ptr(a_ptr) {}

		TreeNode* m_ptr = nullptr;

		K GetKey() { return m_ptr->m_pair->m_key; }

		/**
		*	@brief Find root from current node and return it
		*	@return Node at the root of the tree.
		*/
		TreeNode* GetRoot() {
			TreeNode* rootNode = this->m_ptr;

			while (rootNode->m_parent) {
				rootNode = rootNode->m_parent;
			}

			return rootNode;
		}

		/**
		*	@brief Reset visited status of all tree nodes so that tree can be traversed again.
		*/
		void RecurResetTraversal(TreeNode* a_node) {
			// Still nodes to cover
			if (a_node) {
				RecurResetTraversal(a_node->m_left);						// Recursively destroy left sub-tree

																			// Set node to unvisited
				a_node->visited = false;

				RecurResetTraversal(a_node->m_right);						// Recursively destroy right sub-tree
			}
		}

		PairNode& operator*() {
			return *m_ptr->m_pair;
		}

		TreeIterator& operator++() {									/*PREFIX: Increment in order to next tree node value*/
			/// 1. Node has an unvisited left child, move to it.
			if (m_ptr->HasValidLeft()) {
				m_ptr->m_left->visited = true;

				m_ptr = m_ptr->m_left;
				return *this;
			}

			/// 2. Node has an unvisited right child, move to it.
			if (m_ptr->HasValidRight()) {
				m_ptr->m_right->visited = true;

				m_ptr = m_ptr->m_right;
				return *this;
			}

			/// 3. Node has no valid children, move to unvisited parent or next valid child.
			TreeNode* currentParent = m_ptr->m_parent;

			// Find first unvisited parent
			while (currentParent) {
				if (!currentParent->visited) {
					currentParent->visited = true;

					m_ptr = currentParent;
					return *this;
				}
				currentParent = currentParent->m_parent;
			}

			// Definitely no unvisited parents but there could be unvisited children.
			TreeNode* familyParent = m_ptr->GetFamilyParent();				// Find parent with unvisited children
			if (familyParent) {
				*this = ++TreeIterator(familyParent);						// Use recursion to set iterator to valid child
				return *this;
			}

			/// 4. Node is at the end, it has no more unvisited parents AND no more unvisited children.
			// Reset visitation status of all nodes so that tree can be traversed again
			RecurResetTraversal(GetRoot());
			m_ptr = nullptr;

			return *this;
		}

		TreeIterator operator++(int) {							/*POSTFIX: Move to the next tree node value after leaving the expression*/
			TreeIterator result(*this);
			++(*this);
			return result;
		}

		// Check if the tree node is the sentinel node by checking if its holding nullptr
		bool operator!=(TreeIterator a_iter) {
			return m_ptr != a_iter.m_ptr;
		}
	};
#pragma endregion

	TreeIterator begin() { return TreeIterator(GetFirstNode()); }
	TreeIterator end() { return TreeIterator(nullptr); }

	/**
	*	@brief Get the currently most left-most element in the binary tree.
	*	@return	The first element smallest-biggest in the binary tree.
	*/
	TreeNode* GetFirstNode() {
		TreeNode* currentNode = m_rootNode;

		while (currentNode->m_left) {
			currentNode = currentNode->m_left;
		}

		// First node always counts as visited
		currentNode->visited = true;

		return currentNode;
	}

	/**
	*	@brief Use recursion to destroy entire tree and clean up allocated memory.
	*	@param a_node is the root node the deletion spreads out from.
	*/
	void RecurDestroyTree(TreeNode *a_node) {
		// Nothing left in branch
		if (a_node == nullptr) {
			return;
		}

		// Recursively destroy left branch
		if (a_node->m_left) {
			RecurDestroyTree(a_node->m_left);
		}

		// Recursively destroy right branch
		if (a_node->m_right) {
			RecurDestroyTree(a_node->m_right);
		}

		// Clean up current node
		delete a_node->m_pair;
		a_node->m_pair = nullptr;
		delete a_node;
		a_node = nullptr;
	}

	/**
	*	@brief Add pair to the map and sort it into the binary tree.
	*	@param a_pair is the PairNode to be added.
	*	NOTE: If key is already in map, pair will be ignored.
	*/
	void Insert(PairNode* a_pair) {
		TreeNode* targetParent = nullptr;
		TreeNode* newNode	   = new TreeNode(a_pair);

		// Binary tree is empty, set root node
		if (m_rootNode == nullptr) {
			// Parent is root
			newNode->m_parent = m_rootNode;

			m_rootNode		  = newNode;
			return;
		}
		
		// Search starts at root of the tree
		TreeNode* currentParent = m_rootNode;
		
		// Find the target parent with null children
		while (currentParent) {
			targetParent = currentParent;

			// There are duplicate keys, insert aborted.
			if (a_pair->m_key == currentParent->GetKey()) {
				// Take responsibility of dynamically allocated memory and free up memory for unused nodes
				delete newNode;
				delete a_pair;

				return;
			}

			// Move to right child if key is greater/left if key is smaller
			if (a_pair->m_key > currentParent->GetKey()) {
				currentParent = currentParent->m_right;
			}
			else {
				currentParent = currentParent->m_left;
			}
		}

		// Set new node relative to found parent
		newNode->m_parent = targetParent;

		if (a_pair->m_key > targetParent->GetKey()) {
			targetParent->m_right	= newNode;
		}
		else {
			targetParent->m_left	= newNode;
		}
	}

	/**
	*	@brief Find and return value associated with given key.
	*	@param a_key is the key associated to the value.
	*	@return found node or nullptr if node not found.
	*/
	PairNode* findNode(K a_key) {
		// Search starts at root of the tree
		TreeNode* currentParent = m_rootNode;

		// Find the corresponding node
		while (currentParent) {
			// Found the right node.
			if (a_key == currentParent->GetKey()) {
				return currentParent->m_pair;
			}

			// Move to right child if key is greater/left if key is smaller
			if (a_key > currentParent->GetKey()) {
				currentParent = currentParent->m_right;
			}
			else {
				currentParent = currentParent->m_left;
			}
		}

		// Did not find node.
		return nullptr;
	}

	V& operator[](K a_index) {
		PairNode* foundNode = findNode(a_index);

		if (foundNode) {
			return foundNode->m_val;
		}
		// Create and add new pair, and return empty value
		else {
			PairNode* newNode = new PairNode(a_index);
			Insert(newNode);
			return newNode->m_val;
		}
	}
private:
	TreeNode* m_rootNode = nullptr;										/*Root parent of the binary tree.*/
};