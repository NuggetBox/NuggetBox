#include "XboxController.h"

#define SHORT_MAX (32767.0f)
#define UCHAR_MAX (255.0f)

XboxController::XboxController()
{
    myState = {};
    myPreviousState = {};
}

void XboxController::Update(const XINPUT_STATE& aState)
{
    myPreviousState = myState;

    myState.Dpad.Up = aState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
    myState.Dpad.Down = aState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
    myState.Dpad.Left = aState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
    myState.Dpad.Right = aState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

    myState.A = aState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
    myState.B = aState.Gamepad.wButtons & XINPUT_GAMEPAD_B;
    myState.X = aState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
    myState.Y = aState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;

    myState.LeftStickX = aState.Gamepad.sThumbLX;
    myState.LeftStickY = aState.Gamepad.sThumbLY;
    myState.RightStickX = aState.Gamepad.sThumbRX;
    myState.RightStickY = aState.Gamepad.sThumbRY;

    myState.LeftTrigger = aState.Gamepad.bLeftTrigger;
    myState.RightTrigger = aState.Gamepad.bRightTrigger;

    myState.LeftThumb = aState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
    myState.RightThumb = aState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;

    myState.LeftBumper = aState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
    myState.RightBumper = aState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;

    myState.Start = aState.Gamepad.wButtons & XINPUT_GAMEPAD_START;
    myState.Back = aState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;

    myState.PreviousPackageNumber = aState.dwPacketNumber;
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

float XboxController::GetLeftStickX() const
{
    return static_cast<float>(myState.LeftStickX) / SHORT_MAX;
}

float XboxController::GetLeftStickXDelta() const
{
    return static_cast<float>(myState.LeftStickX - myPreviousState.LeftStickX) / SHORT_MAX;
}

float XboxController::GetLeftStickY() const
{
    return static_cast<float>(myState.LeftStickY) / SHORT_MAX;
}

float XboxController::GetLeftStickYDelta() const
{
    return static_cast<float>(myState.LeftStickY - myPreviousState.LeftStickY) / SHORT_MAX;
}

float XboxController::GetRightStickX() const
{
    return static_cast<float>(myState.RightStickX) / SHORT_MAX;
}

float XboxController::GetRightStickXDelta() const
{
    return static_cast<float>(myState.RightStickX - myPreviousState.RightStickX) / SHORT_MAX;
}

float XboxController::GetRightStickY() const
{
    return static_cast<float>(myState.RightStickY) / SHORT_MAX;
}

float XboxController::GetRightStickYDelta() const
{
    return static_cast<float>(myState.RightStickY - myPreviousState.RightStickY) / SHORT_MAX;
}

float XboxController::GetLeftTrigger() const
{
    return static_cast<float>(myState.LeftTrigger) / UCHAR_MAX;
}

float XboxController::GetLeftTriggerDelta() const
{
    return static_cast<float>(myState.LeftTrigger - myPreviousState.LeftTrigger) / UCHAR_MAX;
}

float XboxController::GetRightTrigger() const
{
    return static_cast<float>(myState.RightTrigger) / UCHAR_MAX;
}

float XboxController::GetRightTriggerDelta() const
{
    return static_cast<float>(myState.RightTrigger - myPreviousState.RightTrigger) / UCHAR_MAX;
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