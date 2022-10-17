#pragma once
#pragma comment(lib, "XInput.lib") 
#include <Windows.h>
#include <Xinput.h>
#include <cstdint>
namespace CommonUtilities 
{
	class Gamepad
	{
	public:
		enum class KeyCode : uint32_t
		{
			Dpad_Up = 0x0001,
			Dpad_Down = 0x0002,
			Dpad_Left = 0x0004,
			Dpad_Right = 0x0008,
			Start = 0x0010,
			Back = 0x0020,
			Left_Thumb_Stick = 0x0040,
			Right_Thumb_Stick = 0x0080,
			Left_Shoulder_Button = 0x0100,
			Right_Shoulder_Button = 0x0200,
			A_Button = 0x1000,
			B_Button = 0x2000,
			X_Button = 0x4000,
			Y_Button = 0x8000
		};

		Gamepad() : deadzoneX(0.085f), deadzoneY(0.085f) {}
		Gamepad(float dzX, float dzY) : deadzoneX(dzX), deadzoneY(dzY) {}

		float leftStickX = 0;
		float leftStickY = 0;
		float previousLeftStickX = 0;
		float previousLeftStickY = 0;
		float rightStickX = 0;
		float rightStickY = 0;
		float leftTrigger = 0;
		float rightTrigger = 0;

		void UnloadGamepad();

		int GetPort();
		XINPUT_GAMEPAD* GetState();
		bool IsConnected(int aIndex);
		bool Update();
		bool Refresh(/*float aDeltaTime*/);
		bool IsPressed(Gamepad::KeyCode aButton);
		bool IsHeld(Gamepad::KeyCode aButton);

		void SetVibrationToggle(bool aState);

		void Vibration(WORD aLeftVibrationAmount, WORD aRightVibrationAmount/*, float aDuration*/);
		WORD GetLeftVibration();
		WORD GetRightVibration();

	private:
		int cId = 0;
		XINPUT_STATE myPreviousState;
		XINPUT_STATE myCurrentState;
		XINPUT_VIBRATION myVibration;

		bool myVibartionToggleState;

		WORD myLeftVibration = 0;
		WORD myRightVibration = 0;

		float deadzoneX;
		float deadzoneY;

		float myCurrentCooldown;
	};
}
