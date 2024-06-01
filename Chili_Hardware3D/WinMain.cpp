#include "WindowsIncluder.h"
#include "Window.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){

	Window mainWnd(640, 480, "Main Window");

	// Get Messages
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gResult == -1) {
		return -1;
	} else {
		return msg.wParam;
	}

	return 0;
}