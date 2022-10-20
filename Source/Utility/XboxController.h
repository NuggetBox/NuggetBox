#pragma once
#include "ControllerStructs.h"

struct _XINPUT_STATE;

class XboxController
{
public:
	bool GetButtonDown(XboxButton aButton) const;
	bool GetButtonHeld(XboxButton aButton) const;
	bool GetButtonUp(XboxButton aButton) const;

	// Direction vector 0-1.0f depending on how far stick is dragged
	Utility::Vector2f GetLeftStickDir() const;
	Utility::Vector2f GetRightStickDir() const;

	// Direction vector 0-1.0f depending on how far stick is dragged from last frame to this frame
	Utility::Vector2f GetLeftStickDelta() const;
	Utility::Vector2f GetRightStickDelta() const;

	// 0.0f - 1.0f, Released - Pressed
	float GetLeftTrigger() const;
	float GetRightTrigger() const;

	// 0.0f - 1.0f, How much has the press changed since last frame
	float GetLeftTriggerDelta() const;
	float GetRightTriggerDelta() const;

	//0.0f - 1.0f
	void SetVibration(float aLowFrequencyMotorSpeed, float aHighFrequencyMotorSpeed, float aVibrationTime);
	void SetLowFrequencyVibration(float aLowFrequencyMotorSpeed, float aVibrationTime);
	void SetHighFrequencyVibration(float aHighFrequencyMotorSpeed, float aVibrationTime);
	void ResetVibration();

	bool GetConnection() const;

	int GetID() const;

private:
	friend class ControllerHandler;

	XboxController();

	void UpdateInput(const _XINPUT_STATE& aState);
	void UpdateVibration(float aDeltaTime);

	void CalculateThumbSticks(const _XINPUT_STATE& aState);
	void CalculateTriggers(const _XINPUT_STATE& aState);

	void ApplyVibration();

	void SetConnection(bool aConnected = true);
	void SetPackageNumber(unsigned long aPackageNumber);
	unsigned long GetPackageNumber() const;
	void SetID(int aID);

	XboxControllerState myState;
	XboxControllerState myPreviousState;

	int myID;

	float myLowFrequencyVibrationTimer;
	float myHighFrequencyVibrationTimer;
};