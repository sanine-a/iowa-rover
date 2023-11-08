#pragma once

// generic linked-list node
template <typename T>
class LLNode {
	public:
	LLNode() : next(nullptr), prev(nullptr) {};
	LLNode(T value) : value(value), next(nullptr), prev(nullptr) {};

	// add a new value to the end of the list
	void append(T value) {
		if (next == nullptr) {
			LLNode *node = new LLNode(value);
			node->prev = this;
			next = node;
		} else {
			next->append(value);
		}
	}
	
	// remove the current node from the list
	// if provided, `left` stores the node's ancestor and `right` stores
	// its descendant (which will be linked together after erasing)
	void erase(LLNode **left=nullptr, LLNode **right=nullptr) {
		if (prev != nullptr) {
			prev->next = next;
		}
		if (next != nullptr) {
			next->prev = prev;
		}

		// provide data about where the hole was closed
		if (left  != nullptr) { *left  = prev; }
		if (right != nullptr) { *right = prev; }

		// possibly evil, be careful
		delete this;
	}

	void erase_next() {
		if (next != nullptr) { next->erase(); }
	}

	// getters
	LLNode * get_next() { return next; }
	LLNode * get_prev() { return prev; }
	T& get_value() { return value; }

	protected:
	T value;
	LLNode *next;
	LLNode *prev;
};
