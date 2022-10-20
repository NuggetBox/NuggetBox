#include "XboxController.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>

#include "UtilityFunctions.hpp"
#include "Math/Vector2.hpp"

#define SHORT_MAX (32767.0f)
#define USHORT_MAX (65535.0f)
#define UCHAR_MAX (255.0f)

XboxController::XboxController()
{
    ZeroMemory(&myState, sizeof(XboxControllerState));
    ZeroMemory(&myPreviousState, sizeof(XboxControllerState));
    myID = -1;
    myLowFrequencyVibrationTimer = 0.0f;
    myHighFrequencyVibrationTimer = 0.0f;
}

void XboxController::UpdateInput(const _XINPUT_STATE& aState)
{
    myPreviousState = myState;

    myState.Dpad.Up = aState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
    myState.Dpad.Down = aState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
    myState.Dpad.Left = aState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
    myState.Dpad.Right = aState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

    myState.Start = aState.Gamepad.wButtons & XINPUT_GAMEPAD_START;
    myState.Back = aState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;

    myState.LeftThumb = aState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
    myState.RightThumb = aState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

    myState.LeftBumper = aState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
    myState.RightBumper = aState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

    myState.A = aState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
    myState.B = aState.Gamepad.wButtons & XINPUT_GAMEPAD_B;
    myState.X = aState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
    myState.Y = aState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;

    CalculateThumbSticks(aState);

    CalculateTriggers(aState);

    myState.PreviousPackageNumber = aState.dwPacketNumber;
}

void XboxController::UpdateVibration(float aDeltaTime)
{
    myLowFrequencyVibrationTimer -= aDeltaTime;
    if (myLowFrequencyVibrationTimer <= 0.0f)
    {
        myLowFrequencyVibrationTimer = 0.0f;
        myState.LowFrequencyMotorSpeed = 0;
    }

    myHighFrequencyVibrationTimer -= aDeltaTime;
    if (myHighFrequencyVibrationTimer <= 0.0f)
    {
        myHighFrequencyVibrationTimer = 0.0f;
        myState.HighFrequencyMotorSpeed = 0;
    }

    ApplyVibration();
}

void XboxController::CalculateThumbSticks(const _XINPUT_STATE& aState)
{
    //Left stick
    {
        const float leftX = aState.Gamepad.sThumbLX;
        const float leftY = aState.Gamepad.sThumbLY;

        Utility::Vector2f leftDirection(leftX, leftY);
        float leftMagnitude = leftDirection.Length();
        leftDirection.Normalize();

        myState.LeftStickDir = leftDirection;

        if (leftMagnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
        {
            //Clamp to allowed values
            leftMagnitude = Utility::Clamp(leftMagnitude, 0.0f, SHORT_MAX);

            //Adjust so that magnitude starts at 0 just after deadzone
            leftMagnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

            //normalize magnitude, 0.0f - 1.0f from deadzone to end
            leftMagnitude /= (SHORT_MAX - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
        }
        //stick is inside deadzone
        else
        {
            leftMagnitude = 0.0f;
        }

        myState.LeftStickMagnitude = leftMagnitude;
    }
    //

    //Right Stick
    {
        const float rightX = aState.Gamepad.sThumbRX;
        const float rightY = aState.Gamepad.sThumbRY;

        Utility::Vector2f rightDirection(rightX, rightY);
        float rightMagnitude = rightDirection.Length();
        rightDirection.Normalize();

        if (rightMagnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
        {
            //Clamp to allowed values
            rightMagnitude = Utility::Clamp(rightMagnitude, 0.0f, SHORT_MAX);

            //Adjust so that magnitude starts at 0 just after deadzone
            rightMagnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

            //normalize magnitude, 0.0f - 1.0f from deadzone to end
            rightMagnitude /= (SHORT_MAX - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
        }
        //stick is inside deadzone
        else
        {
            rightMagnitude = 0.0f;
        }

        myState.RightStickMagnitude = rightMagnitude;
    }
    //
}

void XboxController::CalculateTriggers(const _XINPUT_STATE& aState)
{
    //Left Trigger
    {
        float leftTrigger = aState.Gamepad.bLeftTrigger;

        if (leftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
        {
            //Clamp to allowed values
            leftTrigger = Utility::Clamp(leftTrigger, 0.0f, UCHAR_MAX);

            //Adjust so that magnitude starts at 0 just after deadzone
            leftTrigger -= XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

            //normalize trigger, 0.0f - 1.0f from deadzone to end
            leftTrigger /= (UCHAR_MAX - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
        }
        else
        {
            leftTrigger = 0.0f;
        }

        myState.LeftTrigger = leftTrigger;
    }
    //

    //Right trigger
    {
        float rightTrigger = aState.Gamepad.bRightTrigger;

        if (rightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
        {
            //Clamp to allowed values
            rightTrigger = Utility::Clamp(rightTrigger, 0.0f, UCHAR_MAX);

            //Adjust so that magnitude starts at 0 just after deadzone
            rightTrigger -= XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

            //normalize trigger, 0.0f - 1.0f from deadzone to end
            rightTrigger /= (UCHAR_MAX - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
        }
        else
        {
            rightTrigger = 0.0f;
        }

        myState.RightTrigger = rightTrigger;
    }
    //
}

void XboxController::ApplyVibration()
{
    if (myState.Connected)
    {
        XINPUT_VIBRATION vibration;
        ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
        vibration.wLeftMotorSpeed = myState.LowFrequencyMotorSpeed;
        vibration.wRightMotorSpeed = myState.HighFrequencyMotorSpeed;
        XInputSetState(myID, &vibration);
    }
}

void XboxController::SetID(int aID)
{
    myID = aID;
}

bool XboxController::GetButtonDown(XboxButton aButton) const
{
    switch (aButton)
    {
        case XboxButton::DpadUp:
            return myState.Dpad.Up && !myPreviousState.Dpad.Up;
        case XboxButton::DpadDown:
            return myState.Dpad.Down && !myPreviousState.Dpad.Down;
        case XboxButton::DpadLeft:
            return myState.Dpad.Left && !myPreviousState.Dpad.Left;
        case XboxButton::DpadRight:
            return myState.Dpad.Right && !myPreviousState.Dpad.Right;
        case XboxButton::A:
            return myState.A && !myPreviousState.A;
        case XboxButton::B:
            return myState.B && !myPreviousState.B;
        case XboxButton::X:
            return myState.X && !myPreviousState.X;
        case XboxButton::Y:
            return myState.Y && !myPreviousState.Y;
        case XboxButton::LeftThumb:
            return myState.LeftThumb && !myPreviousState.LeftThumb;
        case XboxButton::RightThumb:
            return myState.RightThumb && !myPreviousState.RightThumb;
        case XboxButton::LeftBumper:
            return myState.LeftBumper && !myPreviousState.LeftBumper;
        case XboxButton::RightBumper:
            return myState.RightBumper && !myPreviousState.RightBumper;
        case XboxButton::Start:
            return myState.Start && !myPreviousState.Start;
        case XboxButton::Back:
            return myState.Back && !myPreviousState.Back;
    }
}

bool XboxController::GetButtonHeld(XboxButton aButton) const
{
    switch (aButton)
    {
    case XboxButton::DpadUp:
        return myState.Dpad.Up && myPreviousState.Dpad.Up;
    case XboxButton::DpadDown:
        return myState.Dpad.Down && myPreviousState.Dpad.Down;
    case XboxButton::DpadLeft:
        return myState.Dpad.Left && myPreviousState.Dpad.Left;
    case XboxButton::DpadRight:
        return myState.Dpad.Right && myPreviousState.Dpad.Right;
    case XboxButton::A:
        return myState.A && myPreviousState.A;
    case XboxButton::B:
        return myState.B && myPreviousState.B;
    case XboxButton::X:
        return myState.X && myPreviousState.X;
    case XboxButton::Y:
        return myState.Y && myPreviousState.Y;
    case XboxButton::LeftThumb:
        return myState.LeftThumb && myPreviousState.LeftThumb;
    case XboxButton::RightThumb:
        return myState.RightThumb && myPreviousState.RightThumb;
    case XboxButton::LeftBumper:
        return myState.LeftBumper && myPreviousState.LeftBumper;
    case XboxButton::RightBumper:
        return myState.RightBumper && myPreviousState.RightBumper;
    case XboxButton::Start:
        return myState.Start && myPreviousState.Start;
    case XboxButton::Back:
        return myState.Back && myPreviousState.Back;
    }
}

bool XboxController::GetButtonUp(XboxButton aButton) const
{
    switch (aButton)
    {
    case XboxButton::DpadUp:
        return !myState.Dpad.Up && myPreviousState.Dpad.Up;
    case XboxButton::DpadDown:
        return !myState.Dpad.Down && myPreviousState.Dpad.Down;
    case XboxButton::DpadLeft:
        return !myState.Dpad.Left && myPreviousState.Dpad.Left;
    case XboxButton::DpadRight:
        return !myState.Dpad.Right && myPreviousState.Dpad.Right;
    case XboxButton::A:
        return !myState.A && myPreviousState.A;
    case XboxButton::B:
        return !myState.B && myPreviousState.B;
    case XboxButton::X:
        return !myState.X && myPreviousState.X;
    case XboxButton::Y:
        return !myState.Y && myPreviousState.Y;
    case XboxButton::LeftThumb:
        return !myState.LeftThumb && myPreviousState.LeftThumb;
    case XboxButton::RightThumb:
        return !myState.RightThumb && myPreviousState.RightThumb;
    case XboxButton::LeftBumper:
        return !myState.LeftBumper && myPreviousState.LeftBumper;
    case XboxButton::RightBumper:
        return !myState.RightBumper && myPreviousState.RightBumper;
    case XboxButton::Start:
        return !myState.Start && myPreviousState.Start;
    case XboxButton::Back:
        return !myState.Back && myPreviousState.Back;
    }
}

Utility::Vector2f XboxController::GetLeftStickDir() const
{
    return myState.LeftStickDir * myState.LeftStickMagnitude;
}

Utility::Vector2f XboxController::GetLeftStickDelta() const
{
    return GetLeftStickDir() - myPreviousState.LeftStickDir * myPreviousState.LeftStickMagnitude;
}

Utility::Vector2f XboxController::GetRightStickDelta() const
{
    return GetRightStickDir() - myPreviousState.RightStickDir * myPreviousState.RightStickMagnitude;
}

Utility::Vector2f XboxController::GetRightStickDir() const
{
    return myState.RightStickDir * myState.RightStickMagnitude;
}

float XboxController::GetLeftTrigger() const
{
    return myState.LeftTrigger;
}

float XboxController::GetRightTrigger() const
{
    return myState.RightTrigger;
}

float XboxController::GetLeftTriggerDelta() const
{
    return myState.LeftTrigger - myPreviousState.LeftTrigger;
}

float XboxController::GetRightTriggerDelta() const
{
    return myState.RightTrigger - myPreviousState.RightTrigger;
}

void XboxController::SetVibration(float aLowFrequencyMotorSpeed, float aHighFrequencyMotorSpeed, float aVibrationTime)
{
    SetLowFrequencyVibration(aLowFrequencyMotorSpeed, aVibrationTime);
    SetHighFrequencyVibration(aHighFrequencyMotorSpeed, aVibrationTime);
}

void XboxController::SetLowFrequencyVibration(float aLowFrequencyMotorSpeed, float aVibrationTime)
{
    if (myState.Connected)
    {
        myLowFrequencyVibrationTimer = aVibrationTime;
        myState.LowFrequencyMotorSpeed = static_cast<WORD>(USHORT_MAX * aLowFrequencyMotorSpeed);
    }
}

void XboxController::SetHighFrequencyVibration(float aHighFrequencyMotorSpeed, float aVibrationTime)
{
    if (myState.Connected)
    {
        myHighFrequencyVibrationTimer = aVibrationTime;
        myState.HighFrequencyMotorSpeed = static_cast<WORD>(USHORT_MAX * aHighFrequencyMotorSpeed);
    }
}

void XboxController::ResetVibration()
{
    myLowFrequencyVibrationTimer = 0.0f;
    myHighFrequencyVibrationTimer = 0.0f;
    myState.LowFrequencyMotorSpeed = 0;
    myState.HighFrequencyMotorSpeed = 0;

    ApplyVibration();
}

void XboxController::SetConnection(bool aConnected)
{
    myState.Connected = aConnected;
}

bool XboxController::GetConnection() const
{
    return myState.Connected;
}

void XboxController::SetPackageNumber(unsigned long aPackageNumber)
{
    myState.PreviousPackageNumber = aPackageNumber;
}

unsigned long XboxController::GetPackageNumber() const
{
    return myState.PreviousPackageNumber;
}

int XboxController::GetID() const
{
    return myID;
}