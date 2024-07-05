#include <Windows.h>

LRESULT MyWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(42);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	const char* className = "My Window Class";
	const char* windowTitle = "My Title";

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = MyWindowProcedure;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = nullptr;
	wndClass.hCursor = nullptr;
	wndClass.hbrBackground = nullptr;
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = className;
	wndClass.hIconSm = nullptr;

	RegisterClassEx(&wndClass);

	DWORD wndStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZE;
	HWND wndHandler = CreateWindowEx(0, className, windowTitle, wndStyle, 100, 100, 320, 240, nullptr, nullptr, hInstance, nullptr);

	ShowWindow(wndHandler, SW_SHOWNORMAL);

	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	};
	
	UnregisterClass(className, hInstance);

	if (gResult == -1) {
		return gResult;
	} else {
		return msg.wParam;
	}
}