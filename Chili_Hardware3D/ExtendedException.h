#pragma once
#include <exception>
#include <string>
#include <sstream>

#define GET_FILE_NAME __FILE__
#define GET_LINE_NUMBER __LINE__

class ExtendedException : public std::exception {
public:
	ExtendedException(const char *fileName, size_t lineNumber) noexcept;
	virtual char const* what() const noexcept override;
	virtual char const* GetType() const noexcept;
	std::string GetFormattedFileInfo() const noexcept;
	size_t GetLineNumber() const noexcept;
	std::string GetFileName() const noexcept;
private:
	size_t lineNumber;
	std::string fileName;
protected:
	mutable std::string whatBuffer;
};