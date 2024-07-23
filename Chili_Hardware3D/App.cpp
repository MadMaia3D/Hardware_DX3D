#include "App.h"

App::App()
	:
	wnd("Hardware 3D Window", 640, 480) {
}

int App::Go() {
	int frame = 0;
	while (true) {
		if (const std::optional<int> result = Window::ProcessMessages()) {
			return *result;
		}
		DoFrame();
	}
	return 0;
}

void App::DoFrame() {}
