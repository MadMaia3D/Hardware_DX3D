#include "Window.h"
#include <sstream>

// ******************************** Window::WindowClass Definitions ********************************
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

// ******************************** Window Definitions ********************************

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

	if (AdjustWindowRect(&windowRect, wndStyle, FALSE) == 0) {
		throw Window::Exception(GET_FILE_NAME, GET_LINE_NUMBER, GetLastError());
	}

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

	if (hWnd == nullptr) {
		throw Window::Exception(GET_FILE_NAME, GET_LINE_NUMBER, GetLastError());
	}

	ShowWindow(hWnd, SW_SHOWNORMAL);
}

Window::~Window() {
	DestroyWindow(hWnd);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_NCCREATE) {
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

// ******************************** Window::Exception Definitions ********************************

Window::Exception::Exception(const char *fileName, size_t lineNumber, HRESULT errorCode) noexcept
	:
	ExtendedException(fileName, lineNumber),
	errorCode(errorCode) {
}

const char* Window::Exception::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << ":" << "\n"
		<< "[Error Code]: " << GetErrorCode() << "\n"
		<< "[Description]:" << GetErrorString() << "\n"
		<< "throw at:" << "\n" << GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
};

const char* Window::Exception::GetType() const noexcept {
	return "Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT errorCode) const noexcept {
	char* pMsgBuffer;
	DWORD msgLength = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuffer),
		0,
		nullptr
	);
	if (msgLength == 0) {
		return "Unidentified error code";
	}
	std::string errorString = pMsgBuffer;
	LocalFree(pMsgBuffer);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept {
	return errorCode;
}

std::string Window::Exception::GetErrorString() const noexcept {
	return TranslateErrorCode(errorCode);
}