#pragma once

#include "ll.h"

template<typename T>
class Subscriber;

template<typename T>
class Publisher {
	public:
	void attach(Subscriber<T> *subscriber) {
		subscribers.append(subscriber);
	}
	void detach(Subscriber<T> *subscriber) {
		for (auto node = subscribers.get_next(); node != nullptr; node = node->get_next()) {
			if (node->get_value() == subscriber) {
				node->erase();
				return;
			}
		}
	}
	void publish(T msg) {
		for (auto node = subscribers.get_next(); node != nullptr; node = node->get_next()) {
			node->get_value()->on(msg);
		}
	}
	
	private:
	LLNode<Subscriber<T>*> subscribers;
};


template<typename T>
class Subscriber {
	public:
	Subscriber(Publisher<T> *publisher) : publisher(publisher) {
		publisher->attach(this);
	}
	~Subscriber() {
		publisher->detach(this);
	}
	virtual void on(T msg) = 0;

	private:
	Publisher<T> *publisher;
};
