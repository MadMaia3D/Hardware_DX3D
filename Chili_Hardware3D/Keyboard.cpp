#include "Keyboard.h"

Keyboard::Event::Event(Type type, unsigned char keycode)
	:
	type(type),
	keycode(keycode) {
}
bool Keyboard::Event::IsPress() const {
	return type == Type::Press;
}
bool Keyboard::Event::IsRelease() const {
	return type == Type::Release;
}
bool Keyboard::Event::IsValid() const {
	return type != Type::Invalid;
}
unsigned char Keyboard::Event::GetCode() const {
	return keycode;
}

// ******************************** Keyboard Events ********************************
bool Keyboard::IsKeyPressed(unsigned char keycode) const {
	return keyStates.test(keycode);
}
Keyboard::Event Keyboard::ReadKey() {
	if (!keyEventBuffer.empty()) {
		Event e = keyEventBuffer.front();
		keyEventBuffer.pop();
		return e;
	} else {
		return Event();
	}
}
bool Keyboard::IsKeyEmpty() const {
	return keyEventBuffer.empty();
}
// ******************************** Char Events ********************************
char Keyboard::ReadChar() {
	if (!charEventBuffer.empty()) {
		unsigned char charcode = charEventBuffer.front();
		charEventBuffer.pop();
		return charcode;
	} else {
		return 0;
	}
}
bool Keyboard::IsCharEmpty() const {
	return charEventBuffer.empty();
}
// ******************************** Buffer Flushing ********************************
void Keyboard::FlushKeyEvents() {
	while (!keyEventBuffer.empty()) {
		keyEventBuffer.pop();
	}
}
void Keyboard::FlushCharEvents() {
	while (!charEventBuffer.empty()) {
		charEventBuffer.pop();
	}
}
void Keyboard::FlushAll() {
	FlushKeyEvents();
	FlushCharEvents();
}
// ******************************** Auto Repeat ********************************
void Keyboard::EnableAutoRepeat() {
	autoRepeatEnabled = true;
}
void Keyboard::DisableAutoRepeat() {
	autoRepeatEnabled = false;
}
bool Keyboard::IsAutoRepeatEnabled() const {
	return autoRepeatEnabled;
}

// ******************************** Interface for friend Window ********************************
void Keyboard::OnKeyPressed(unsigned char keycode) {
	keyStates.set(keycode, true);
	keyEventBuffer.push({ Event::Type::Press, keycode });
	TrimBuffer(keyEventBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) {
	keyStates.set(keycode, false);
	keyEventBuffer.push({ Event::Type::Release, keycode });
	TrimBuffer(keyEventBuffer);
}

void Keyboard::OnChar(char character) {
	charEventBuffer.push(character);
	TrimBuffer(charEventBuffer);
}

void Keyboard::ClearState() {
	keyStates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) {
	while (buffer.size() > maxBufferSize) {
		buffer.pop();
	}
}
