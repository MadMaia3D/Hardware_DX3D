#pragma once
#include "WindowsIncluder.h"

class Window {
private:
	class WindowClass {
	public:
		static const char* GetName();
		HINSTANCE GetInstance();
	private:
		WindowClass();
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
	private:
		static constexpr const char* className = "Hardware3D_Window_Class";
		HINSTANCE hInstance;
	public:
		static WindowClass wcSingleton;
	};
public:
	Window(int width, int height, const char* title);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMessageThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND hWnd;
};