#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <bitset>

namespace Utility
{
	struct KeyboardState
	{
		std::bitset<256> virtualKeyPresses;
	};

	struct MouseState
	{
		POINT mousePos;

		bool leftClick;
		bool rightClick;
		bool middleClick;
		short scrollWheelCount;
	};

	class InputHandler
	{
	public:
		InputHandler() = delete;
		~InputHandler() = delete;

		static bool GetKeyDown(const int aKeyCode);
		static bool GetKeyHeld(const int aKeyCode);
		static bool GetKeyUp(const int aKeyCode);

		static bool GetLeftClickDown();
		static bool GetLeftClickHeld();
		static bool GetLeftClickUp();

		static bool GetRightClickDown();
		static bool GetRightClickHeld();
		static bool GetRightClickUp();

		static bool GetMiddleClickDown();
		static bool GetMiddleClickHeld();
		static bool GetMiddleClickUp();

		static POINT GetMousePosition();
		static POINT GetMouseDelta();

		// Positive = Scroll away from user, Negative = Scroll towards user
		static int GetScrollWheelDelta();

		static void SetMousePosition(const HWND& aHandle, const int anX, const int anY);

		//void ConfineMousePosition(const HWND& aHandle);

		static bool UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);
		static void UpdatePreviousState();
		static void ResetAllInput();

	private:
		static bool UpdateKeyboard(UINT message, WPARAM wParam, LPARAM lParam);
		static bool UpdateMouse(UINT message, WPARAM wParam, LPARAM lParam);

		static KeyboardState myKeyboardState;
		static KeyboardState myPreviousKeyboardState;
		static MouseState myMouseState;
		static MouseState myPreviousMouseState;
	};
}