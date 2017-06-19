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

		bool HasLeft() { return m_left != nullptr; }
		bool HasRight() { return m_right != nullptr; }

		PairNode* m_pair = nullptr;
		TreeNode* m_left = nullptr;
		TreeNode* m_right = nullptr;
	};
#pragma endregion
	
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
		if (a_node->HasLeft()) {
			RecurDestroyTree(a_node->m_left);
		}

		// Recursively destroy right branch
		if (a_node->HasRight()) {
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

		// Binary tree is empty, set root node
		if (m_rootNode == nullptr) {
			m_rootNode = new TreeNode(a_pair);
			return;
		}
		
		// Search starts at root of the tree
		TreeNode* currentParent = m_rootNode;
		
		// Find the target parent with null children
		while (currentParent) {
			targetParent = currentParent;

			// There are duplicate keys, insert aborted.
			if (a_pair->m_key == currentParent->GetKey()) {
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
		if (a_pair->m_key > targetParent->GetKey()) {
			targetParent->m_right = new TreeNode(a_pair);
		}
		else {
			targetParent->m_left = new TreeNode(a_pair);
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
	TreeNode* m_rootNode = nullptr;							/*Root parent of the binary tree.*/
};