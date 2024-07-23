#include "Keyboard.h"

/********************************* Keyboard::Event *********************************/
Keyboard::Event::Event() :
	type(Type::Invalid),
	code(0)
{}

Keyboard::Event::Event(Type event, unsigned char code)
	:
	type(event),
	code(code)
{}

bool Keyboard::Event::IsPress() const {
	return type == Type::Pressed;
}

bool Keyboard::Event::IsRelease() const {
	return type == Type::Released;
}

bool Keyboard::Event::IsValid() const {
	return type != Type::Invalid;
}

unsigned char Keyboard::Event::GetCode() const {
	return code;
}

/********************************* Keyboard *********************************/

bool Keyboard::IsKeyPressed(unsigned char code) const {
	return keyStates.test(code);
}

Keyboard::Event Keyboard::ReadKey() {
	if (IsKeyEmpty()) {
		return {};
	}
	Event e = keyBuffer.front();
	keyBuffer.pop();
	return e;
}

bool Keyboard::IsKeyEmpty() const {
	return keyBuffer.empty();
}

unsigned char Keyboard::ReadChar() {
	if (IsCharEmpty()) {
		return 0;
	}
	char c = charBuffer.front();
	charBuffer.pop();
	return c;
}

bool Keyboard::IsCharEmpty() const {
	return charBuffer.empty();
}

void Keyboard::FlushKey() {
	keyBuffer = std::queue<Event>();
}

void Keyboard::FlushChar() {
	charBuffer = std::queue<unsigned char>();
}

void Keyboard::Flush() {
	FlushKey();
	FlushChar();
}

bool Keyboard::IsAutorepeatEnabled() const {
	return autorepeatEnabled;
}

void Keyboard::EnableAutorepeat() {
	autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() {
	autorepeatEnabled = false;
}

void Keyboard::OnKeyPressed(unsigned char code) {
	keyBuffer.emplace(Keyboard::Event::Type::Pressed, code);
	keyStates.set(code, true);
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char code) {
	keyBuffer.emplace(Keyboard::Event::Type::Released, code);
	keyStates.set(code, false);
	TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(unsigned char code) {
	charBuffer.emplace(code);
	TrimBuffer(charBuffer);
}

void Keyboard::ClearState() {
	keyStates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(T & buffer) {
	while (buffer.size() > maxBufferSize) {
		buffer.pop();
	}
}