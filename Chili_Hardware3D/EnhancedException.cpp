#include "EnhancedException.h"
#include <sstream>

EnhancedException::EnhancedException(const char * file, int line) noexcept
	:
	file(file),
	line(line) {
}

const char * EnhancedException::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char * EnhancedException::GetType() const noexcept {
	return "Enhanced Exception";
}

int EnhancedException::GetLine() const noexcept {
	return line;
}

const std::string EnhancedException::GetFile() const noexcept {
	return file;
}

std::string EnhancedException::GetOriginString() const noexcept {
	std::ostringstream oss;
	oss << "File: " << GetFile() << std::endl
		<< "Line: " << GetLine();
	return oss.str();
}
