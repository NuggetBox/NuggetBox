#include "NuggetBox.pch.h"
#include "FloatEditCommand.h"

FloatEditCommand::FloatEditCommand(std::shared_ptr<float> aEditedFloat, float aDiff)
{
	myEditedFloat = aEditedFloat;
	myDiff = aDiff;
}

void FloatEditCommand::Do()
{
	*myEditedFloat += myDiff;
}

void FloatEditCommand::Undo()
{
	*myEditedFloat -= myDiff;
}