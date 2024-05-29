#pragma once
#include "WindowsIncluder.h"

class Window {
private:
	class WindowClass {
	public:
		static const char* GetName();
		static HINSTANCE GetInstance();
	private:
		WindowClass();
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
	private:
		static constexpr const char* wndClassName = "Hardware 3D Engine Class";
		static WindowClass wndClass;
		HINSTANCE hInstance;
	};
public:
	Window(int width, int height, const char* title);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT uInt, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT uInt, WPARAM wParam, LPARAM lParam);
	LRESULT WINAPI HandleMsg(HWND hWnd, UINT uInt, WPARAM wParam, LPARAM lParam);
private:
	HWND hWnd;
};