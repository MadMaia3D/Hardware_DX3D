#include "Mouse.h"

// ******************************** Event Class ********************************
Mouse::Event::Event() : type(Type::Invalid), isLeftPressed(false), isRightPressed(false), x(0), y(0) {}

Mouse::Event::Event(Type type, const Mouse & parent)
	: type(type),
	isLeftPressed(parent.isLeftPressed),
	isRightPressed(parent.isRightPressed),
	x(parent.x),
	y(parent.y) {
}

bool Mouse::Event::IsLeftPressed() const {
	return isLeftPressed;
}

bool Mouse::Event::IsRightPressed() const {
	return isRightPressed;
}

std::pair<int, int> Mouse::Event::GetPos() const {
	return { x, y };
}

int Mouse::Event::GetPosX() const {
	return x;
}

int Mouse::Event::GetPosY() const {
	return y;
}

Mouse::Event::Type Mouse::Event::GetType() const {
	return type;
}

bool Mouse::Event::IsValid() const {
	return type != Type::Invalid;
}
// ******************************** Position ********************************
std::pair<int, int> Mouse::GetPos() const {
	return { x,y };
}

int Mouse::GetPosX() const {
	return x;
}

int Mouse::GetPosY() const {
	return y;
}

// ******************************** Buttons ********************************

bool Mouse::IsLeftPressed() const {
	return isLeftPressed;
}
bool Mouse::IsRightPressed() const {
	return isRightPressed;
}
// ******************************** Event Queue ********************************
Mouse::Event Mouse::Read() {
	if (eventBuffer.empty()) {
		return Event();
	}
	Event e = eventBuffer.front();
	eventBuffer.pop();
	return e;
}

void Mouse::Flush() {
	eventBuffer = std::queue<Event>();
}

bool Mouse::IsEmpty() const {
	return eventBuffer.empty();
}

bool Mouse::IsInWindow() const {
	return false;
}
// ******************************** Private Interface For Friend Window ********************************
void Mouse::OnMouseMove(int x, int y) {
	this->x = x;
	this->y = y;
	eventBuffer.emplace(Event::Type::Move, *this);
	TrimBuffer();
}

void Mouse::OnLeftButtonPress(int x, int y) {
	isLeftPressed = true;
	eventBuffer.emplace(Event::Type::LPress, *this);
	TrimBuffer();
}

void Mouse::OnLeftButtonRelease(int x, int y) {
	isLeftPressed = false;
	eventBuffer.emplace(Event::Type::LRelease, *this);
	TrimBuffer();
}

void Mouse::OnRightButtonPress(int x, int y) {
	isRightPressed = true;
	eventBuffer.emplace(Event::Type::RPress, *this);
	TrimBuffer();
}

void Mouse::OnRightButtonRelease(int x, int y) {
	isRightPressed = false;
	eventBuffer.emplace(Event::Type::RRelease, *this);
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) {
	eventBuffer.emplace(Event::Type::WheelDown, *this);
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) {
	eventBuffer.emplace(Event::Type::WheelUp, *this);
	TrimBuffer();
}

void Mouse::TrimBuffer() {
	while (eventBuffer.size() > maxBufferSize) {
		eventBuffer.pop();
	}
}
