#include "pch.h"
#include "Gamepad.h"
#include <math.h>
#include <cmath>
#include <cassert>
using namespace CommonUtilities;

void Gamepad::UnloadGamepad()
{
	ZeroMemory(&myCurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&myVibration, sizeof(XINPUT_VIBRATION));
	XInputSetState(cId, &myVibration);
}

int Gamepad::GetPort()
{
	return cId + 1;
}

XINPUT_GAMEPAD* Gamepad::GetState()
{
	return &myCurrentState.Gamepad;
}

bool Gamepad::IsConnected(int aIndex)
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	if (XInputGetState(aIndex, &state) == ERROR_SUCCESS)
	{
		return true;
	}
	return false;
}

bool Gamepad::Update()
{
	int controllerId = -1;

	for (DWORD i = 0; i < XUSER_MAX_COUNT && controllerId == -1; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		if (XInputGetState(i, &state) == ERROR_SUCCESS)
			controllerId = i;
	}

	cId = controllerId;

	return controllerId != -1;
}

// Returns false if the controller has been disconnected
bool Gamepad::Refresh()
{
	if (cId == -1)
		Update();

		myPreviousState.Gamepad.wButtons = myCurrentState.Gamepad.wButtons;

	if (cId != -1)
	{
		ZeroMemory(&myCurrentState, sizeof(XINPUT_STATE));
		if (XInputGetState(cId, &myCurrentState) != ERROR_SUCCESS)
		{
			cId = -1;
			return false;
		}

		float normLX = fmaxf(-1, (float)myCurrentState.Gamepad.sThumbLX / 32767);
		float normLY = fmaxf(-1, (float)myCurrentState.Gamepad.sThumbLY / 32767);

		previousLeftStickY = leftStickY;

		leftStickX = (abs(normLX) < deadzoneX ? 0 : (abs(normLX) - deadzoneX) * (normLX / abs(normLX)));
		leftStickY = (abs(normLY) < deadzoneY ? 0 : (abs(normLY) - deadzoneY) * (normLY / abs(normLY)));

		if (deadzoneX > 0) leftStickX *= 1 / (1 - deadzoneX);
		if (deadzoneY > 0) leftStickY *= 1 / (1 - deadzoneY);

		float normRX = fmaxf(-1, (float)myCurrentState.Gamepad.sThumbRX / 32767);
		float normRY = fmaxf(-1, (float)myCurrentState.Gamepad.sThumbRY / 32767);

		previousLeftStickX = leftStickX;

		rightStickX = (abs(normRX) < deadzoneX ? 0 : (abs(normRX) - deadzoneX) * (normRX / abs(normRX)));
		rightStickY = (abs(normRY) < deadzoneY ? 0 : (abs(normRY) - deadzoneY) * (normRY / abs(normRY)));

		if (deadzoneX > 0) rightStickX *= 1 / (1 - deadzoneX);
		if (deadzoneY > 0) rightStickY *= 1 / (1 - deadzoneY);

		leftTrigger = (float)myCurrentState.Gamepad.bLeftTrigger / 255;
		rightTrigger = (float)myCurrentState.Gamepad.bRightTrigger / 255;

		/*VibrationUpdate(aDeltaTime);*/

		ZeroMemory(&myVibration, sizeof(XINPUT_VIBRATION));
		myVibration.wLeftMotorSpeed = myLeftVibration; // use any value between 0-65535 here
		myVibration.wRightMotorSpeed = myRightVibration; // use any value between 0-65535 here
		XInputSetState(cId, &myVibration);

		return true;
	}
	return false;
}

bool Gamepad::IsPressed(Gamepad::KeyCode aButton)
{
	DWORD button = static_cast<DWORD>(aButton);

	if ((myCurrentState.Gamepad.wButtons & button) && !(myPreviousState.Gamepad.wButtons & button))
	{
		return (myCurrentState.Gamepad.wButtons & button) != 0;
	}
	else
	{
		return false;
	}
	//return (state.Gamepad.wButtons & button) != 0;
}

bool Gamepad::IsHeld(Gamepad::KeyCode aButton)
{
	DWORD button = static_cast<DWORD>(aButton);

	if ((myCurrentState.Gamepad.wButtons & button) && (myPreviousState.Gamepad.wButtons & button))
	{
		return (myCurrentState.Gamepad.wButtons & button) != 0;
	}
	else
	{
		return false;
	}
}

void Gamepad::SetVibrationToggle(bool aState)
{
	myVibartionToggleState = aState;
}

void Gamepad::Vibration(WORD aLeftVibrationAmount, WORD aRightVibrationAmount/*, float aDuration*/)
{
	if (myVibartionToggleState == false)
	{
		return;
	}

	//myCurrentCooldown = aDuration;
	assert(0.0f <= aLeftVibrationAmount && "Vibration value on left side is negativ. Min value: 0.0f. Max value: 65535.0f");
	assert(65535.0f >= aLeftVibrationAmount && "Vibration value on left side is over max amount. Min value: 0.0f. Max value: 65535.0f.");

	if (0.0f <= aLeftVibrationAmount && aLeftVibrationAmount <= 65535.0f)
	{
		myLeftVibration = aLeftVibrationAmount;
	}

	assert(0.0f <= aRightVibrationAmount && "Vibration value on right side is negativ. Min value: 0.0f. Max value: 65535.0f");
	assert(65535.0f >= aRightVibrationAmount && "Vibration value on right side is over max amount. Min value: 0.0f. Max value: 65535.0f.");

	if (0.0f <= aRightVibrationAmount && aRightVibrationAmount <= 65535.0f)
	{
		myRightVibration = aRightVibrationAmount;
	}
}

WORD Gamepad::GetLeftVibration()
{
	return myLeftVibration;
}

WORD Gamepad::GetRightVibration()
{
	return myRightVibration;
}

//void Gamepad::VibrationUpdate(float aDeltaTime)
//{
//	myCurrentCooldown -= aDeltaTime;
//	if (myCurrentCooldown <= 0)
//	{
//		myLeftVibration = 0;
//		myRightVibration = 0;
//		myCurrentCooldown = 0;
//	}
//	ZeroMemory(&myVibration, sizeof(XINPUT_VIBRATION));
//	myVibration.wLeftMotorSpeed = myLeftVibration; // use any value between 0-65535 here
//	myVibration.wRightMotorSpeed = myRightVibration; // use any value between 0-65535 here
//	XInputSetState(cId, &myVibration);
//}
