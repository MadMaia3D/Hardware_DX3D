#pragma once
#include <exception>
#include <string>

#define GET_FILE_NAME __FILE__
#define GET_LINE_NUMBER __LINE__

class EnhancedException : public std::exception {
public:
	EnhancedException(const char* file, int line) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string GetFile() const noexcept;
	std::string GetOriginString() const noexcept;
private:
	int line;
	std::string file;
protected:
	mutable std::string whatBuffer;
};