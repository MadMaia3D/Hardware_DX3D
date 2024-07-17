#pragma once
#include <exception>
#include <string>

#define GET_FILE_NAME __FILE__
#define GET_LINE_NUMBER __LINE__

class ExtendedException : public std::exception {
public:
	ExtendedException(const char *fileName, size_t lineNumber) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	const std::string GetFileName() const noexcept;
	size_t GetLineNumber() const noexcept;
	std::string GetOriginString() const noexcept;
private:
	std::string fileName;
	size_t lineNumber;
protected:
	mutable std::string whatBuffer;
};