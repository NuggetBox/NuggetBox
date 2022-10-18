#include "InputHandler.h"
#include <windowsx.h>

namespace Utility
{
	KeyboardState InputHandler::myKeyboardState;
	KeyboardState InputHandler::myPreviousKeyboardState;
	MouseState InputHandler::myMouseState;
	MouseState InputHandler::myPreviousMouseState;

	//InputHandler::InputHandler()
	//{
	//	GetCursorPos(&myMouseState.mousePos);
	//}

	bool InputHandler::GetKeyDown(const int aKeyCode)
	{
		return myKeyboardState.virtualKeyPresses.test(aKeyCode) && !myPreviousKeyboardState.virtualKeyPresses.test(aKeyCode);
	}
	bool InputHandler::GetKeyHeld(const int aKeyCode)
	{
		return myKeyboardState.virtualKeyPresses.test(aKeyCode) && myPreviousKeyboardState.virtualKeyPresses.test(aKeyCode);
	}
	bool InputHandler::GetKeyUp(const int aKeyCode)
	{
		return !myKeyboardState.virtualKeyPresses.test(aKeyCode) && myPreviousKeyboardState.virtualKeyPresses.test(aKeyCode);
	}

	bool InputHandler::GetLeftClickDown()
	{
		return myMouseState.leftClick && !myPreviousMouseState.leftClick;
	}
	bool InputHandler::GetLeftClickHeld()
	{
		return myMouseState.leftClick && myPreviousMouseState.leftClick;
	}
	bool InputHandler::GetLeftClickUp()
	{
		return !myMouseState.leftClick && myPreviousMouseState.leftClick;
	}

	bool InputHandler::GetRightClickDown()
	{
		return myMouseState.rightClick && !myPreviousMouseState.rightClick;
	}
	bool InputHandler::GetRightClickHeld()
	{
		return myMouseState.rightClick && myPreviousMouseState.rightClick;
	}
	bool InputHandler::GetRightClickUp()
	{
		return !myMouseState.rightClick && myPreviousMouseState.rightClick;
	}

	bool InputHandler::GetMiddleClickDown()
	{
		return myMouseState.middleClick && !myPreviousMouseState.middleClick;
	}
	bool InputHandler::GetMiddleClickHeld()
	{
		return myMouseState.middleClick && myPreviousMouseState.middleClick;
	}
	bool InputHandler::GetMiddleClickUp()
	{
		return !myMouseState.middleClick && myPreviousMouseState.middleClick;
	}

	POINT InputHandler::GetMousePosition()
	{
		return myMouseState.mousePos;
	}

	POINT InputHandler::GetMouseDelta()
	{
		POINT delta = myMouseState.mousePos;
		delta.x -= myPreviousMouseState.mousePos.x;
		delta.y -= myPreviousMouseState.mousePos.y;
		return delta;
	}

	int InputHandler::GetScrollWheelDelta()
	{
		return myMouseState.scrollWheelCount;
	}

	void InputHandler::SetMousePosition(const HWND& aHandle, const int anX, const int anY)
	{
		POINT point = { anX, anY };
		ClientToScreen(aHandle, &point);
		SetCursorPos(point.x, point.y);
	}

	//TODO: Fix confine mouse position function
	//void InputHandler::ConfineMousePosition(const HWND& aHandle)
	//{
	//	RECT windowRect;
	//	GetWindowRect(aHandle, &windowRect);
	//	ClipCursor(&windowRect);
	//}

	bool InputHandler::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
	{
		bool handled = false;

		// Alt-tab -> reset input
		if (message == WM_KILLFOCUS)
		{
			ResetAllInput();
			handled = true;
		}

		if (UpdateKeyboard(message, wParam, lParam))
		{
			handled = true;
		}

		if (UpdateMouse(message, wParam, lParam))
		{
			handled = true;
		}

		return handled;
	}

	bool InputHandler::UpdateKeyboard(UINT message, WPARAM wParam, LPARAM lParam)
	{
		bool handled = false;
		lParam;
		wParam;
		message;

		switch (message)
		{
			case WM_KEYDOWN:
			{
				// If use WM_CHAR message for text input, skip A-Z, 0-9 here
				//myPreviousKeyboardState.virtualKeyPresses.set(wParam, myKeyboardState.virtualKeyPresses.test(wParam));
				//myPreviousKeyboardState.virtualKeyPresses.set(wParam, GetPreviousKeyState(lParam));
				myKeyboardState.virtualKeyPresses.set(wParam);
				handled = true;
				break;
			}
			case WM_KEYUP:
			{
				//myPreviousKeyboardState.virtualKeyPresses.set(wParam, myKeyboardState.virtualKeyPresses.test(wParam));
				//myPreviousKeyboardState.virtualKeyPresses.set(wParam, GetPreviousKeyState(lParam));
				myKeyboardState.virtualKeyPresses.reset(wParam);
				handled = true;
				break;
			}
			default:
			{
				handled = false;
			}
		}

		return handled;
	}

	bool InputHandler::UpdateMouse(UINT message, WPARAM wParam, LPARAM lParam)
	{
		bool handled = true;

		switch (message)
		{
			case WM_LBUTTONDOWN:
			{
				myMouseState.leftClick = true;
				break;
			}
			case WM_LBUTTONUP:
			{
				myMouseState.leftClick = false;
				break;
			}
			case WM_RBUTTONDOWN:
			{
				myMouseState.rightClick = true;
				break;
			}
			case WM_RBUTTONUP:
			{
				myMouseState.rightClick = false;
				break;
			}
			case WM_MBUTTONDOWN:
			{
				myMouseState.middleClick = true;
				break;
			}
			case WM_MBUTTONUP:
			{
				myMouseState.middleClick = false;
				break;
			}
			case WM_MOUSEMOVE:
			{
				myMouseState.mousePos.x = GET_X_LPARAM(lParam);
				myMouseState.mousePos.y = GET_Y_LPARAM(lParam);
				break;
			}
			case WM_MOUSEWHEEL:
			{
				// Scroll bug should be fixed, it wouldn't update until another message was sent before
				myMouseState.scrollWheelCount = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
				break;
			}
			default:
			{
				handled = false;
			}
		}

		return handled;
	}

	void InputHandler::UpdatePreviousState()
	{
		myPreviousKeyboardState = myKeyboardState;
		myPreviousMouseState = myMouseState;

		myMouseState.scrollWheelCount = 0;
	}

	void InputHandler::ResetAllInput()
	{
		ZeroMemory(&myKeyboardState, sizeof(KeyboardState));
		ZeroMemory(&myPreviousKeyboardState, sizeof(KeyboardState));
		ZeroMemory(&myMouseState, sizeof(MouseState));
		ZeroMemory(&myPreviousMouseState, sizeof(MouseState));
	}
}