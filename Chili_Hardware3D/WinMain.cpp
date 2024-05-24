#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
	const char* const pClassName = "hw3d";

	// Register Window Class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = DefWindowProc;
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

	// Create Window Instance
	const char* const wndName = "3D Wanna Be";
	const DWORD wndStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	HWND hWnd = CreateWindowEx(0, pClassName, wndName, wndStyle, 300, 250, 640, 480, nullptr, nullptr, hInstance, nullptr);	

	// Show The Window
	ShowWindow(hWnd, SW_SHOW);

	while (true);
	return 0;
}