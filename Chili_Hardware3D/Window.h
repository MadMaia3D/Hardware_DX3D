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
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		~WindowClass();
		static constexpr const char* wndClassName = "My Window Class";
		static WindowClass wndClassSingleton;
		HINSTANCE hInstance;
	};
public:
	Window(const char* title, int width, int height);
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	~Window();
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	int width;
	int height;
	HWND hWnd;
};