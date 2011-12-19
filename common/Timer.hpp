#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <ctime>

/**
 * Class for measuring time.
 * Start measurement with start(), stop it with stop() and get the elapsed
 * time in milliseconds by calling elapsed().
 */
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
		return 1000.0 * (end - begin) / CLOCKS_PER_SEC;
	}
};

#endif
