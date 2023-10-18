#pragma once

#include "lambda.h"
#include "ll.h"

struct Task {
	Lambda<void(void)> lambda;
	unsigned long start;
	unsigned int id;
	bool ready() { return millis() >= start; }
};
struct TimeoutTask : public Task {
};
struct IntervalTask : public Task {
	unsigned long interval;
	bool next() {
		start += interval;
	}
};


class Scheduler {
	public:
	Scheduler() : interval_id(0), timeout_id(0), intervals(), timeouts() {}

	unsigned int setTimeout(Lambda<void(void)> lambda, unsigned long time_to_start) {
		TimeoutTask t;
		t.lambda = lambda;
		t.id = timeout_id;
		timeout_id++;
		t.start = millis() + time_to_start;
		timeouts.append(t);
		return t.id;
	}
	void clearTimeout(int id) {
		for (auto node = timeouts.get_next(); node != nullptr; node = node->get_next()) {
			if (node->get_value().id == id) {
				node->erase();
				return;
			}
		}
	}


	unsigned int setInterval(Lambda<void(void)> lambda, unsigned long period) {
		IntervalTask i;
		i.lambda = lambda;
		i.id = interval_id;
		interval_id += 1;
		i.start = millis() + period;
		i.interval = period;
		intervals.append(i);
		return i.id;
	}
	void clearInterval(int id) {
		for (auto node = intervals.get_next(); node != nullptr; node = node->get_next()) {
			if (node->get_value().id == id) {
				node->erase();
				return;
			}
		}
	}

	void update() {
		for (auto node = intervals.get_next(); node != nullptr; node = node->get_next()) {
			IntervalTask& interval = node->get_value();
			if (interval.ready()) {
				interval.lambda();
				interval.next();
			}
		}

		for (auto node = timeouts.get_next(); node != nullptr; node = node->get_next()) {
			TimeoutTask& timeout = node->get_value();
			if (timeout.ready()) {
				timeout.lambda();
				node->erase(&node);
			}
		}
	}

	protected:
	LLNode<IntervalTask> intervals;
	size_t interval_id;
	LLNode<TimeoutTask> timeouts;
	size_t timeout_id;
};
