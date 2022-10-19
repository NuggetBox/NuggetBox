#pragma once
enum class XboxButton
{
	DpadUp = 0x0001,
	DpadDown = 0x0002,
	DpadLeft = 0x0004,
	DpadRight = 0x0008,
	Start = 0x0010,
	Back = 0x0020,
	LeftThumb = 0x0040,
	RightThumb = 0x0080,
	LeftBumper = 0x0100,
	RightBumper = 0x0200,
	A = 0x1000,
	B = 0x2000,
	X = 0x4000,
	Y = 0x8000
};

struct XboxControllerState
{
	struct DPad { bool Up, Down, Left, Right; } Dpad;
	bool Start, Back; //Buttons at the front
	bool LeftThumb, RightThumb; //Thumbsticks pressed
	bool LeftBumper, RightBumper; //The bumper is above the triggers
	bool A, B, X, Y;

	short LeftStickX, LeftStickY, RightStickX, RightStickY; //-32768 - 32767. Negative: Left or down. Positive: Right or Up.
	unsigned char LeftTrigger, RightTrigger; //0-255, The triggers are below the bumpers

	unsigned long PreviousPackageNumber;
	bool Connected;
};