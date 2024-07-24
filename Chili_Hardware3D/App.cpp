#include "App.h"

App::App()
	:
	wnd("Main Window", 640, 480) {
}

int App::Go() {
	while (true) {
		if (const std::optional<int> return_code = wnd.ProcessMessages()) {
			return return_code.value();
		}
		DoFrame();
	}
}

void App::DoFrame() {
}
