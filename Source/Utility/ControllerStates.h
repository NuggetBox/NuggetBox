#pragma once
enum class XboxButton
{
	DpadUp, DpadDown, DpadLeft, DpadRight,
	A, B, X, Y,
	LeftThumb, RightThumb,
	LeftBumper, RightBumper,
	Start, Back
};

struct XboxControllerState
{
	struct DPad { bool Up, Down, Left, Right; } Dpad;
	bool A, B, X, Y;
	short LeftStickX, LeftStickY, RightStickX, RightStickY; //-32768 - 32767. Negative: Left or down. Positive: Right or Up.
	unsigned char LeftTrigger, RightTrigger; //0-255, The triggers are below the bumpers
	bool LeftThumb, RightThumb; //Thumbsticks pressed
	bool LeftBumper, RightBumper; //The bumper is above the triggers
	bool Start, Back; //Buttons at the front

	unsigned long PreviousPackageNumber;
	bool Connected;
};