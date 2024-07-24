#include "Mouse.h"
#include "WindowsIncluder.h"

/********************************* Mouse::Event *********************************/

Mouse::Event::Event()
	:
	xPos(0),
	yPos(0),
	type(Type::Invalid)
{}

Mouse::Event::Event(Type type, const Mouse& parent)
	:
	type(type),
	isLeftPressed(parent.isLeftPressed),
	isRightPressed(parent.isRightPressed),
	xPos(parent.xPos),
	yPos(parent.yPos)
{}

Mouse::Event::Type Mouse::Event::GetType() const {
	return type;
}

bool Mouse::Event::IsValid() const {
	return type != Type::Invalid;
}

bool Mouse::Event::LeftIsPressed() const {
	return isLeftPressed;
}

bool Mouse::Event::RightIsPressed() const {
	return isRightPressed;
}

int Mouse::Event::GetPosX() const {
	return xPos;
}

int Mouse::Event::GetPosY() const {
	return yPos;
}

std::pair<int, int> Mouse::Event::GetPos() const {
	return { xPos, yPos };
}

/********************************* Mouse *********************************/
int Mouse::GetPosX() const {
	return xPos;
}

int Mouse::GetPosY() const {
	return yPos;
}

std::pair<int, int> Mouse::GetPos() const {
	return { xPos, yPos };
}

bool Mouse::IsInWindow() const {
	return isInWindow;
}

bool Mouse::IsLeftPressed() const {
	return isLeftPressed;
}

bool Mouse::IsRightPressed() const {
	return isRightPressed;
}

Mouse::Event Mouse::Read() {
	if (eventsBuffer.size() > 0u) {
		Event e = eventsBuffer.front();
		eventsBuffer.pop();
		return e;
	}
	return Event();
}

bool Mouse::IsEmpty() const {
	return eventsBuffer.empty();
}

void Mouse::FlushBuffer() {
	eventsBuffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int x, int y) {
	xPos = x;
	yPos = y;
	eventsBuffer.emplace(Event::Type::Move, *this);
	TrimBuffer();
}

void Mouse::OnMouseEnter() {
	isInWindow = true;
	eventsBuffer.emplace(Event::Type::Enter, *this);
	TrimBuffer();
}

void Mouse::OnMouseLeave() {
	isInWindow = false;
	eventsBuffer.emplace(Event::Type::Leave, *this);
	TrimBuffer();
}

void Mouse::OnLeftPressed() {
	isLeftPressed = true;
	eventsBuffer.emplace(Event::Type::LPress, *this);
	TrimBuffer();
}

void Mouse::OnLeftReleased() {
	isLeftPressed = false;
	eventsBuffer.emplace(Event::Type::LRelease, *this);
	TrimBuffer();
}

void Mouse::OnRightPressed() {
	isRightPressed = true;
	eventsBuffer.emplace(Event::Type::RPress, *this);
	TrimBuffer();
}

void Mouse::OnRightReleased() {
	isRightPressed = false;
	eventsBuffer.emplace(Event::Type::RRelease, *this);
	TrimBuffer();
}

void Mouse::OnWheelDelta(int delta) {
	accumulatedWheelDelta += delta;

	while (accumulatedWheelDelta >= WHEEL_DELTA) {
		accumulatedWheelDelta -= WHEEL_DELTA;
		OnWheelUp();
	}
	while (accumulatedWheelDelta <= -WHEEL_DELTA) {
		accumulatedWheelDelta += WHEEL_DELTA;
		OnWheelDown();
	}
}

void Mouse::OnWheelUp() {
	eventsBuffer.emplace(Event::Type::WheelUp, *this);
	TrimBuffer();
}

void Mouse::OnWheelDown() {
	eventsBuffer.emplace(Event::Type::WheelDown, *this);
	TrimBuffer();
}

void Mouse::TrimBuffer() {
	while (eventsBuffer.size() > maxBufferSize) {
		eventsBuffer.pop();
	}
}
