#include "ControllerHandler.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>

void ControllerHandler::Initialize()
{
    for (int i = 0; i < XUSER_MAX_COUNT; ++i)
    {
        ourXboxControllers[i] = XboxController();
    }
}

void ControllerHandler::CheckForNewControllers()
{
    DWORD result;

    for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        result = XInputGetState(i, &state);

        if (result == ERROR_SUCCESS)
        {
            // Controller is connected
            ourXboxControllers[i].SetConnection(true);
            ourXboxControllers[i].SetID(i);
        }
        else
        {
            // Controller is not connected
            ourXboxControllers[i].SetConnection(false);
        }
    }
}

void ControllerHandler::UpdateInput(float aDeltaTime)
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
                    controller.UpdateInput(state);
                }

                controller.UpdateVibration(aDeltaTime);
            }
            // Controller is not connected
            else
            {
                controller.SetConnection(false);
            }
        }
    }
}