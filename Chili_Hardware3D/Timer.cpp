#include "Timer.h"

using namespace std::chrono;

Timer::Timer() {
	last = steady_clock::now();
}

float Timer::Mark() {
	const steady_clock::time_point old = last;
	last = steady_clock::now();
	const duration<float> elapsed = last - old;
	return elapsed.count();
}

float Timer::Peek() const {
	return duration<float>(steady_clock::now() - last).count();
}
