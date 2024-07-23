#include "Window.h"
#include "ExtendedException.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	try {
		Window my_window("My Window", 640, 480);

		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		};

		if (gResult == -1) {
			return gResult;
		}
		return (int)msg.wParam;
	}
	catch (const ExtendedException& e) {
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	} catch (const std::exception& e) {
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	} catch (...) {
		MessageBox(nullptr, "Caught unknown error\nNo info available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}