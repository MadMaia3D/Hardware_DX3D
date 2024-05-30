#include "Window.h"

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass()
	:
	hInst(GetModuleHandle(nullptr))
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
	UnregisterClass(GetName(), GetInstance());
}

const char* Window::WindowClass::GetName() {
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() {
	return wndClass.hInst;
}

Window::Window(int width, int height, const char* title) {

	DWORD wndStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	RECT windowRegion = { 0 };
	windowRegion.left = 0;
	windowRegion.right = windowRegion.left + width;
	windowRegion.top = 0;
	windowRegion.bottom = windowRegion.top + height;

	AdjustWindowRect(&windowRegion, wndStyle, FALSE);

	const int calculatedWidth = windowRegion.right - windowRegion.left;
	const int calculatedHeight = windowRegion.bottom - windowRegion.top;

	hWnd = CreateWindow(WindowClass::GetName(),
						title,
						wndStyle,
						CW_USEDEFAULT, CW_USEDEFAULT, calculatedWidth, calculatedHeight,
						nullptr,
						nullptr,
						Window::WindowClass::GetInstance(),
						this);

	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window() {
	DestroyWindow(hWnd);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_NCCREATE) {
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		return pWnd->HandleMsg(hWnd, uMsg, wParam, lParam);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd -> HandleMsg(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {
	case WM_CLOSE:
		PostQuitMessage(42);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
