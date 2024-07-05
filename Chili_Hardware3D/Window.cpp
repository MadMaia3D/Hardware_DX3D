#include "Window.h"

Window::WindowClass Window::WindowClass::wndClassSingleton;

Window::WindowClass::WindowClass()
	:
	hInstance(GetModuleHandle(nullptr)) {

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_OWNDC;
	wndClass.lpfnWndProc = HandleMsgSetup;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = GetInstance();
	wndClass.hIcon = nullptr;
	wndClass.hCursor = nullptr;
	wndClass.hbrBackground = nullptr;
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = GetName();
	wndClass.hIconSm = nullptr;

	RegisterClassEx(&wndClass);
}

Window::WindowClass::~WindowClass() {
	UnregisterClass(GetName(), GetInstance());
}

const char* Window::WindowClass::GetName() {
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() {
	return wndClassSingleton.hInstance;
}

Window::Window(const char* title, int clientWidth, int clientHeight)
	:
	width(clientWidth),
	height(clientHeight) {
	DWORD wndStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	RECT sizeRect = { 0 };
	sizeRect.top = 0;
	sizeRect.left = 0;
	sizeRect.bottom = clientHeight;
	sizeRect.right = clientWidth;
	AdjustWindowRect(&sizeRect, wndStyle, FALSE);

	const int finalWidth = sizeRect.right - sizeRect.left;
	const int finalHeight = sizeRect.bottom - sizeRect.top;

	HWND wndHandler = CreateWindowEx(0,
									 WindowClass::GetName(),
									 title,
									 wndStyle,
									 CW_USEDEFAULT, CW_USEDEFAULT, finalWidth, finalHeight,
									 nullptr,
									 nullptr,
									 WindowClass::GetInstance(),
									 this);

	ShowWindow(wndHandler, SW_SHOWNORMAL);
}

Window::~Window() {
	DestroyWindow(hWnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_CREATE) {
		const CREATESTRUCTW *pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
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
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}