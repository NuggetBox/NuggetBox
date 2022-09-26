#include "NuggetBox.pch.h"
#include "ColorEditCommand.h"

ColorEditCommand::ColorEditCommand(std::shared_ptr<Vector4f> aEditedColor, const Vector4f& aColorDiff)
{
	myEditedColor = aEditedColor;
	myColorDiff = aColorDiff;
}

void ColorEditCommand::Do()
{
	*myEditedColor += myColorDiff;
}

void ColorEditCommand::Undo()
{
	*myEditedColor -= myColorDiff;
}