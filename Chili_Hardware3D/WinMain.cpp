#include "Window.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	Window window("Hardware 3D Window", 640, 480);

	MSG msg;
	BOOL gResult;
	while((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gResult != 0) {
		return gResult;
	} else {
		return (int)msg.wParam;
	}
}