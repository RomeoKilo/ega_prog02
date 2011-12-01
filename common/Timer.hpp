#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <ctime>

class Timer {
private:
	clock_t begin;
	clock_t end;
public:
	void start() {
		begin = clock();
	}
	void stop() {
		end = clock();
	}
	double elapsed() {
		return 1000 * (end - begin) / CLOCKS_PER_SEC;
	}
};

#endif
