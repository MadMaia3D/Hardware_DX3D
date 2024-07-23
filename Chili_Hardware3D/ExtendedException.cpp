#include "ExtendedException.h"

ExtendedException::ExtendedException(const char * fileName, size_t lineNumber)  noexcept
	:
	fileName(fileName),
	lineNumber(lineNumber)
{
}

char const * ExtendedException::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << " caught at:" << "\n\n" <<
		GetFormattedFileInfo() << "\n";
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

char const * ExtendedException::GetType() const noexcept {
	return "Extended Exception";
}

std::string ExtendedException::GetFormattedFileInfo() const noexcept {
	std::ostringstream oss;
	oss << "[File]: " << GetFileName() << "\n"
		<< "[Line]: " << GetLineNumber() << "\n";
	return oss.str();
}

size_t ExtendedException::GetLineNumber() const noexcept {
	return lineNumber;
}

std::string ExtendedException::GetFileName() const noexcept {
	return fileName;
}
