#pragma once
#include "WindowsIncluder.h"
#include <unordered_map>

class WindowsMessageMap {
public:
	WindowsMessageMap();
	std::string operator()(UINT uMsg, WPARAM wParam, LPARAM lParam) const;
	std::unordered_map<UINT, std::string> map;
};