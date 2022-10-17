#include "ControllerHandler.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>

//#define RIGHT_THUMB_DEADZONE_FLOAT (XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / 32767.0f)
//#define LEFT_THUMB_DEADZONE_FLOAT (XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / 32767.0f)
//#define TRIGGER_DEADZONE_FLOAT (XINPUT_GAMEPAD_TRIGGER_THRESHOLD / 255.0f)

//Thumbstick deadzones:
//XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE
//XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
//Lower limit of trigger input to detect input: XINPUT_GAMEPAD_TRIGGER_THRESHOLD 

void ControllerHandler::Initialize()
{
    ZeroMemory(ourXboxControllers, sizeof(XboxControllerState) * MAX_CONTROLLERS);
}

void ControllerHandler::CheckForNewControllers()
{
    DWORD result;

    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        result = XInputGetState(i, &state);

        if (result == ERROR_SUCCESS)
        {
            // Controller is connected
            ourXboxControllers[i].SetConnection(true);
        }
        else
        {
            // Controller is not connected
            ourXboxControllers[i].SetConnection(false);
        }
    }
}

void ControllerHandler::Update()
{
    DWORD result;
    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        XboxController& controller = ourXboxControllers[i];

        if (controller.GetConnection())
        {
            XINPUT_STATE state;
            ZeroMemory(&state, sizeof(XINPUT_STATE));

            result = XInputGetState(i, &state);

            // Controller is connected
            if (result == ERROR_SUCCESS)
            {
                //Input has changed
                if (state.dwPacketNumber != controller.GetPackageNumber())
                {
                    controller.Update(state);
                }
            }
            // Controller is not connected
            else
            {

            }

            //Vibration example
            XINPUT_VIBRATION vibration;
            ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
            vibration.wLeftMotorSpeed = 32000; //Low frequency motor, 0-65535
            vibration.wRightMotorSpeed = 16000; //High frequency motor, 0-65535
            XInputSetState(i, &vibration);
            //
        }
    }
}