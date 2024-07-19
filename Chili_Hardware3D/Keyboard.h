#pragma once
#include <queue>
#include <bitset>

class Keyboard {
	friend class Window;
public:
	class Event {
	public:
		enum class Type {
			Press,
			Release,
			Invalid
		};
	private:
		Type type = Type::Invalid;
		unsigned char keycode = 0;
	public:
		Event() = default;
		Event(Type type, unsigned char keycode);
		bool IsPress() const;
		bool IsRelease() const;
		bool IsValid() const;
		unsigned char GetCode() const;
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	// **************** Keyboard Events ********************************
	bool IsKeyPressed(unsigned char keycode) const;
	Event ReadKey();
	bool IsKeyEmpty() const;
	// ******************************** Char Events ********************************
	char ReadChar();
	bool IsCharEmpty() const;
	// ******************************** Buffer Flushers ********************************
	void FlushKeyEvents();
	void FlushCharEvents();
	void FlushAll();
	// ******************************** Auto Repeat ********************************
	void EnableAutoRepeat();
	void DisableAutoRepeat();
	bool IsAutoRepeatEnabled() const;
private:
	// ******************************** Private Interface for friend Window ********************************
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);
	void OnChar(char keycode);
	void ClearState();
	template <typename T>
	static void TrimBuffer(std::queue<T>& buffer);
	// ******************************** Data ********************************
private:
	bool autoRepeatEnabled = false;
	static constexpr unsigned short nKeys = 256u;
	static constexpr unsigned char maxBufferSize = 16u;
	std::bitset<nKeys> keyStates;	// stores currently pressed keys
	std::queue<Event> keyEventBuffer;
	std::queue<unsigned char> charEventBuffer;
};
