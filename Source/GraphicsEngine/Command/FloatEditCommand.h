#pragma once
#include "Command/Command.h"

class FloatEditCommand : public Command
{
public:
	FloatEditCommand(std::shared_ptr<float> aEditedFloat, float aDiff);
	void Do() override;
	void Undo() override;
private:
	std::shared_ptr<float> myEditedFloat;
	float myDiff;
};