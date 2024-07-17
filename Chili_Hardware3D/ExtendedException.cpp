#include "ExtendedException.h"
#include <sstream>

ExtendedException::ExtendedException(const char *fileName, size_t lineNumber) noexcept
	:
	fileName(fileName),
	lineNumber(lineNumber) {
}

const char* ExtendedException::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << " throw at:" << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
};

const char* ExtendedException::GetType() const noexcept {
	return "Extended Exception";
}

const std::string ExtendedException::GetFileName() const noexcept {
	return fileName;
}

size_t ExtendedException::GetLineNumber() const noexcept {
	return lineNumber;
}

std::string ExtendedException::GetOriginString() const noexcept {
	std::ostringstream oss;
	oss << "[File] " << GetFileName() << std::endl
		<< "[Line] " << GetLineNumber();
	return oss.str();
}