#pragma once
#include "ControllerStates.h"

struct _XINPUT_STATE;

class XboxController
{
public:
	XboxController();

	void Update(const _XINPUT_STATE& aState);

	bool GetButtonDown(XboxButton aButton) const;
	bool GetButtonHeld(XboxButton aButton) const;
	bool GetButtonUp(XboxButton aButton) const;

	// -1.0f - 1.0f, Left - Right, Down - Up
	float GetLeftStickX() const;
	float GetLeftStickXDelta() const;
	float GetLeftStickY() const;
	float GetLeftStickYDelta() const;
	float GetRightStickX() const;
	float GetRightStickXDelta() const;
	float GetRightStickY() const;
	float GetRightStickYDelta() const;

	// 0.0f - 1.0f, Released - Pressed
	float GetLeftTrigger() const;
	float GetLeftTriggerDelta() const;
	float GetRightTrigger() const;
	float GetRightTriggerDelta() const;

	void SetConnection(bool aConnected = true);
	bool GetConnection() const;

	void SetPackageNumber(unsigned long aPackageNumber);
	unsigned long GetPackageNumber() const;

private:
	XboxControllerState myState;
	XboxControllerState myPreviousState;
};