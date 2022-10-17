#pragma once
#include "XboxController.h"

#define MAX_CONTROLLERS (4)

class ControllerHandler
{
public:
	static void Initialize();

	//Should only check for controllers every few seconds for performance, according to Windows
	static void CheckForNewControllers();

	static void Update();

private:
	static inline XboxController ourXboxControllers[MAX_CONTROLLERS];
};