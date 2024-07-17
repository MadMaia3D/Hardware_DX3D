#include "Window.h"
#include "ExtendedException.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	try {
		Window window("Hardware 3D Window", 640, 480);

		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (gResult != 0) {
			return gResult;
		}
		
		return (int)msg.wParam;

	} catch (const ExtendedException& e) {
		MessageBox(nullptr, e.what(), e.GetType(), MB_ICONWARNING | MB_OK);
	} catch (const std::exception& e) {
		MessageBox(nullptr, e.what(), "Standard Exception", MB_ICONWARNING | MB_OK);
	} catch (...) {
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_ICONWARNING | MB_OK);
	}

	return 1;
}