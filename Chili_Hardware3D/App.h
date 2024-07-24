#pragma once
#include "Window.h"
#include "Timer.h"
#include <sstream>
#include <iomanip>

class App {
public:
	App();
	int Go();
private:
	void DoFrame();
private:
	Window wnd;
};