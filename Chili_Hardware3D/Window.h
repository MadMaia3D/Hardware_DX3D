#pragma once
#include "WindowsIncluder.h"
#include "ExtendedException.h"
#include "Keyboard.h"
#include "Mouse.h"

class Window {
public:
	class Exception : public ExtendedException {
	public:
		Exception(const char *fileName, size_t lineNumber, HRESULT errorCode) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		std::string TranslateErrorCode(HRESULT errorCode) const noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT errorCode;
	};
private:
	class WindowClass {
	public:
		static const char* GetName();
		static HINSTANCE GetHINSTANCE();
	private:
		WindowClass();
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char *className = "Hardware 3D Class";
		static WindowClass singleton;
		HINSTANCE hInstance;
	};
public:
	Window(const char *title, int width, int height);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	const char *title;
	int width;
	int height;
	HWND hWnd;
public:
	Keyboard kbd;
	Mouse mouse;
};