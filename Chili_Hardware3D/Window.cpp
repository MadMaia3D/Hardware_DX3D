#include "Window.h"

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass()
	:
	hInstance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() {
	UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() {
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() {
	return wndClass.hInstance;
}

Window::Window(int width, int height, const char* pWndTitle) {
	DWORD dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	RECT clientRect = { 0 };
	clientRect.left = 100;
	clientRect.right = clientRect.left + width;
	clientRect.top = 100;
	clientRect.bottom = clientRect.top + height;

	AdjustWindowRect(&clientRect, dwStyle, false);

	const int wX = clientRect.left;
	const int wY = clientRect.top;
	const int wWidth = clientRect.right - clientRect.left;
	const int wHeight = clientRect.bottom - clientRect.top;

	hWnd = CreateWindowEx(0, "Hardware 3D Engine Class", pWndTitle, dwStyle,
						  CW_USEDEFAULT, CW_USEDEFAULT, wWidth, wHeight,
						  nullptr, nullptr, WindowClass::GetInstance(), this);

	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window() {
	DestroyWindow(hWnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_NCCREATE) {
		const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}