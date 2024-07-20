#pragma once
#include <utility>
#include <queue>

class Mouse {
	friend class Window;
public:
	// ******************************** Mouse Event ********************************
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
			Invalid
		};
	private:
		Type type;
		bool isLeftPressed;
		bool isRightPressed;
		int x;
		int y;
	public:
		Event();
		Event(Type type, const Mouse& parent);
		bool IsLeftPressed() const;
		bool IsRightPressed() const;
		std::pair<int, int> GetPos() const;
		int GetPosX() const;
		int GetPosY() const;
		Type GetType() const;
		bool IsValid() const;
	};
public:
	// ******************************** Special Members ********************************
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	// ******************************** Position ********************************
	std::pair<int, int> GetPos() const;
	int GetPosX() const;
	int GetPosY() const;
	// ******************************** Buttons ********************************
	bool IsLeftPressed() const;
	bool IsRightPressed() const;
	// ******************************** Event Queue ********************************
	Event Read();
	void Flush();
	bool IsEmpty() const;
	bool IsInWindow() const;
	// ******************************** Private Interface For Friend Window ********************************
private:
	void OnMouseMove(int x, int y);
	void OnLeftButtonPress(int x, int y);
	void OnLeftButtonRelease(int x, int y);
	void OnRightButtonPress(int x, int y);
	void OnRightButtonRelease(int x, int y);
	void OnWheelDown(int x, int y);
	void OnWheelUp(int x, int y);
	void TrimBuffer();
private:
	static constexpr unsigned char maxBufferSize = 16u;
	int x;
	int y;
	bool isLeftPressed = false;
	bool isRightPressed = false;
	std::queue<Event> eventBuffer;
};