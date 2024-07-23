#pragma once
#include <queue>
#include <bitset>

class Keyboard {
	friend class Window;
public:
	class Event {
	public:
		enum class Type {
			Pressed,
			Released,
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event();
		Event(Type event, unsigned char code);
		bool IsPress() const;
		bool IsRelease() const;
		bool IsValid() const;
		unsigned char GetCode() const;
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
public:
	/**************** key buffer ****************/
	bool IsKeyPressed(unsigned char code) const;
	Event ReadKey();
	bool IsKeyEmpty() const;
	/**************** char buffer ****************/
	unsigned char ReadChar();
	bool IsCharEmpty() const;
	/**************** flush operations ****************/
	void FlushKey();
	void FlushChar();
	void Flush();
	/**************** autorepeat ****************/
	bool IsAutorepeatEnabled() const;
	void EnableAutorepeat();
	void DisableAutorepeat();
	/**************** interface for friend Window class ****************/
private:
	void OnKeyPressed(unsigned char code);
	void OnKeyReleased(unsigned char code);
	void OnChar(unsigned char code);
	void ClearState();
	template<typename T>
	void TrimBuffer(T & buffer);
private:
	static constexpr unsigned char maxBufferSize = 16u;
	static constexpr unsigned char nKeysBits = 255u;
	std::bitset<nKeysBits> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<unsigned char> charBuffer;
	bool autorepeatEnabled = false;
};
