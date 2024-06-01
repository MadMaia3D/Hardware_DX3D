#include "Window.h"

Window::WindowClass Window::WindowClass::wcSingleton;

Window::WindowClass::WindowClass()
	:
	hInstance(GetModuleHandle(nullptr))
{
	// Register Window Class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMessageSetup;
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

const char * Window::WindowClass::GetName() {
	return className;
}

HINSTANCE Window::WindowClass::GetInstance() {
	return hInstance;
}

Window::Window(int width, int height, const char * title) {
	// Create Window Instance
	const DWORD wndStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	RECT clientSize = { 0 };
	clientSize.left = 0;
	clientSize.right = clientSize.left + width;
	clientSize.top = 0;
	clientSize.bottom = clientSize.top + height;

	AdjustWindowRect(&clientSize, wndStyle, FALSE);
	const int totalWidth = clientSize.right - clientSize.left;
	const int totalHeight = clientSize.bottom - clientSize.top;

	hWnd = CreateWindowEx(0,
						  Window::WindowClass::GetName(),
						  title,
						  wndStyle,
						  CW_USEDEFAULT, CW_USEDEFAULT, totalWidth, totalHeight,
						  nullptr,
						  nullptr,
						  Window::WindowClass::wcSingleton.GetInstance(),
						  this);

	ShowWindow(hWnd, SW_SHOW);
}

Window::~Window() {
}

LRESULT CALLBACK Window::HandleMessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (uMsg = WM_NCCREATE) {
		// recover window pointer
		const CREATESTRUCT* create = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* wnd = static_cast<Window*>(create->lpCreateParams);
		// store window pointer for later retrieval and change window procedure
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&wnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMessageThunk));

		return wnd->HandleMessage(hWnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMessageThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Window* wnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return wnd->HandleMessage(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
