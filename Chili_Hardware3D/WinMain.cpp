#include "App.h"
#include "ExtendedException.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	try {
		return App{}.Go();
	} catch (const ExtendedException& e) {
		MessageBox(nullptr, e.what(), e.GetType(), MB_ICONWARNING | MB_OK);
	} catch (const std::exception& e) {
		MessageBox(nullptr, e.what(), "Standard Exception", MB_ICONWARNING | MB_OK);
	} catch (...) {
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_ICONWARNING | MB_OK);
	}

	return -1;
}