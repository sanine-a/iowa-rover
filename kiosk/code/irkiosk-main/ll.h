#pragma once

template <typename T>
class LLNode {
	public:
	LLNode() : next(nullptr), prev(nullptr) {};
	LLNode(T value) : value(value), next(nullptr), prev(nullptr) {};

	void append(T value) {
		if (next == nullptr) {
			LLNode *node = new LLNode(value);
			node->prev = this;
			next = node;
		} else {
			next->append(value);
		}
	}
	
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

	LLNode * get_next() { return next; }
	LLNode * get_prev() { return prev; }
	T& get_value() { return value; }

	protected:
	T value;
	LLNode *next;
	LLNode *prev;
};
