#pragma once
#include <queue>
#include <utility>

class Mouse {
	friend class Window;
public:
	class Event {
	public:
		enum class Type {
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};
	private:
		Type type;
		bool isLeftPressed;
		bool isRightPressed;
		int xPos;
		int yPos;
	public:
		Event();
		Event(Type type, const Mouse& parent);
		Type GetType() const;
		bool IsValid() const;
		bool LeftIsPressed() const;
		bool RightIsPressed() const;
		int GetPosX() const;
		int GetPosY() const;
		std::pair<int, int> GetPos() const;
	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	/**************** position ****************/
	int GetPosX() const;
	int GetPosY() const;
	std::pair<int, int> GetPos() const;
	bool IsInWindow() const;
	/**************** buttons press ****************/
	bool IsLeftPressed() const;
	bool IsRightPressed() const;
	/**************** buffer ****************/
	Event Read();
	bool IsEmpty() const;
	void FlushBuffer();
	/**************** interface for friend Window class ****************/
private:
	void OnMouseMove(int x, int y);
	void OnMouseEnter();
	void OnMouseLeave();
	void OnLeftPressed();
	void OnLeftReleased();
	void OnRightPressed();
	void OnRightReleased();
	void OnWheelDelta(int delta);
	void OnWheelUp();
	void OnWheelDown();
	void TrimBuffer();
private:
	static constexpr unsigned char maxBufferSize = 16u;
	int xPos{};
	int yPos{};
	bool isLeftPressed = false;
	bool isRightPressed = false;
	bool isInWindow = false;
	int accumulatedWheelDelta = 0;
	std::queue<Event> eventsBuffer;
};