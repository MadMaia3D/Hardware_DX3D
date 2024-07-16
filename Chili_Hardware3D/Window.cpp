#include "Window.h"

Window::WindowClass Window::WindowClass::singleton;

Window::WindowClass::WindowClass()
:
	hInstance(GetModuleHandle(NULL))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetHINSTANCE();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() {
	UnregisterClass(GetName(), GetHINSTANCE());
}

const char* Window::WindowClass::GetName() {
	return className;
}
HINSTANCE Window::WindowClass::GetHINSTANCE() {
	return WindowClass::singleton.hInstance;
}

Window::Window(const char *title, int width, int height)
	:
	title(title),
	width(width),
	height(height)
{
	const DWORD wndStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	RECT windowRect = { 0 };
	windowRect.left = 100;
	windowRect.top = 100;
	windowRect.right = width + windowRect.left;
	windowRect.bottom = height + windowRect.top;
	AdjustWindowRect(&windowRect, wndStyle, FALSE);

	const int windowFinalWidth = windowRect.right - windowRect.left;
	const int windowFinalHeight = windowRect.bottom - windowRect.top;

	hWnd = CreateWindowEx(0,
						  WindowClass::GetName(),
						  title,
						  wndStyle,
						  CW_USEDEFAULT, CW_USEDEFAULT, windowFinalWidth, windowFinalHeight,
						  nullptr,
						  nullptr,
						  WindowClass::GetHINSTANCE(),
						  this);

	ShowWindow(hWnd, SW_SHOWNORMAL);
}

Window::~Window() {
	DestroyWindow(hWnd);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_CREATE) {
		const CREATESTRUCT *const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window *const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		return pWnd->HandleMsg(hWnd, uMsg, wParam, lParam);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Window *pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}