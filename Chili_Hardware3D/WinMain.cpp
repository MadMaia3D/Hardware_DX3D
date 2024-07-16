#include "WindowsIncluder.h"

LRESULT CALLBACK HardwareProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE:
		PostQuitMessage(42);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	const char *pClassName = "Hardware 3D Class";
	const char *pWindowName = "Hardware 3D Window";

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HardwareProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);

	const DWORD wndStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	HWND hWnd = CreateWindowEx(0, pClassName, pWindowName, wndStyle, 200, 200, 640, 480, nullptr, nullptr, hInstance, nullptr);

	ShowWindow(hWnd, SW_SHOWNORMAL);

	MSG msg;
	BOOL gResult;
	while((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnregisterClass(pClassName, hInstance);

	if (gResult != 0) {
		return gResult;
	} else {
		return (int)msg.wParam;
	}
}