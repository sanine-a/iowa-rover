#pragma once

#include "ll.h"

// implement the observer pattern for c++
//
// each publisher can publish a specific type. To publish multiple
// channels, multiple types are needed.
//
// each subscriber can receive a specific type from a specific publisher
// implementation. it implements a `void on(T msg)` function that will
// be called whenever the publisher it subscribes to publishes a value.

template<typename T>
class Subscriber;

// Publisher (aka "subject") abstract base class
template<typename T>
class Publisher {
	public:
	// add a new subscriber to the publisher
	void attach(Subscriber<T> *subscriber) {
		subscribers.append(subscriber);
	}
	// remove a subscriber from the publisher
	// this function does nothing if the subscriber is not
	// actually subscribed, so it is safe to call multiple times
	void detach(Subscriber<T> *subscriber) {
		for (auto node = subscribers.get_next(); node != nullptr; node = node->get_next()) {
			if (node->get_value() == subscriber) {
				node->erase();
				return;
			}
		}
	}
	// publish a new value
	// all attached subscribers will call their on() methods with the
	// new value
	void publish(T msg) {
		for (auto node = subscribers.get_next(); node != nullptr; node = node->get_next()) {
			node->get_value()->on(msg);
		}
	}
	
	private:
	// linked list to store subscribers
	LLNode<Subscriber<T>*> subscribers;
};


// Subscriber (aka observer) abstract base class
template<typename T>
class Subscriber {
	public:
	// the constructor subscribes to a publisher
	Subscriber(Publisher<T> *publisher) : publisher(publisher) {
		publisher->attach(this);
	}
	// the destructor removes the subscriber from the publisher
	// (if the subscriber was already removed via some other means,
	// this is still fine because Publisher::detach is idempotent).
	~Subscriber() {
		publisher->detach(this);
	}
	// pure virtual function to react to published values
	virtual void on(T msg) = 0;

	private:
	// pointer to the publisher we subscribe to
	Publisher<T> *publisher;
};
