#pragma once
#include "Command/Command.h"
#include "Math/Vector4.hpp"

class ColorEditCommand : public Command
{
public:
	ColorEditCommand(std::shared_ptr<Vector4f> aEditedColor, const Vector4f& aColorDiff);
	void Do() override;
	void Undo() override;

private:
	std::shared_ptr<Vector4f> myEditedColor;
	Vector4f myColorDiff;
};