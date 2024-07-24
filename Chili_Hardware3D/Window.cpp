#include "Window.h"
#include "resource.h"

/********************************* Window::WindowClass *********************************/

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
	wndClass.hIcon = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 64, 64, 0));
	wndClass.hCursor = nullptr;
	wndClass.hbrBackground = nullptr;
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = GetName();
	wndClass.hIconSm = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, 0));

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

/********************************* Window *********************************/

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

	if (AdjustWindowRect(&sizeRect, wndStyle, FALSE) == 0) {
		throw Window::Exception(GET_FILE_NAME, GET_LINE_NUMBER, GetLastError());
	}

	const int finalWidth = sizeRect.right - sizeRect.left;
	const int finalHeight = sizeRect.bottom - sizeRect.top;

	hWnd = CreateWindowEx(0,
						  WindowClass::GetName(),
						  title,
						  wndStyle,
						  CW_USEDEFAULT, CW_USEDEFAULT, finalWidth, finalHeight,
						  nullptr,
						  nullptr,
						  WindowClass::GetInstance(),
						  this);

	if (hWnd == nullptr) {
		throw Window::Exception(GET_FILE_NAME, GET_LINE_NUMBER, GetLastError());
	}

	ShowWindow(hWnd, SW_SHOWNORMAL);
}

Window::~Window() {
	DestroyWindow(hWnd);
}

void Window::SetTitle(std::string title) {
	SetWindowText(hWnd, title.c_str());
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
		/**************** On Window Focus Loss ****************/
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
		/**************** keyboard Messages ****************/
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || kbd.IsAutorepeatEnabled()) {
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
	case WM_SYSCHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
		/**************** Mouse Messages ****************/
	case WM_MOUSEMOVE:
	{
		const POINTS pos = MAKEPOINTS(lParam);
		if (pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height) {
			mouse.OnMouseMove(pos.x, pos.y);
			if (!mouse.IsInWindow()) {
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		} else {
			if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
				mouse.OnMouseMove(pos.x, pos.y);
			} else {
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
		mouse.OnLeftPressed();
		break;
	case WM_LBUTTONUP:
		mouse.OnLeftReleased();
		break;
	case WM_RBUTTONDOWN:
		mouse.OnRightPressed();
		break;
	case WM_RBUTTONUP:
		mouse.OnRightReleased();
		break;
	case WM_MOUSEWHEEL:
		mouse.OnWheelDelta(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/********************************* Window::Exception *********************************/

Window::Exception::Exception(const char * fileName, size_t lineNumber, HRESULT errorCode) noexcept
	:
	ExtendedException(fileName, lineNumber),
	errorCode(errorCode) {
}

char const * Window::Exception::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << ": \n" <<
		GetErrorString() << "\n" <<
		"caught at: \n" <<
		GetFormattedFileInfo();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

char const * Window::Exception::GetType() const noexcept { return "Window Exception"; }

std::string Window::Exception::TranslateErrorCode(HRESULT errorCode) const noexcept {
	char* pMsgBuffer;
	DWORD msgLength = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
									nullptr,
									errorCode,
									MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
									reinterpret_cast<LPSTR>(&pMsgBuffer),
									0,
									nullptr);
	if (msgLength == 0) {
		return "Unidentified error code";
	}
	std::string errorString(pMsgBuffer);
	LocalFree(pMsgBuffer);
	return errorString;
}

std::string Window::Exception::GetErrorString() const noexcept {
	return TranslateErrorCode(errorCode);
}

HRESULT Window::Exception::GetErrorCode() const noexcept {
	return errorCode;
}
