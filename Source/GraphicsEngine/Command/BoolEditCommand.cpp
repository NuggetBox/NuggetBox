#include "NuggetBox.pch.h"
#include "BoolEditCommand.h"

BoolEditCommand::BoolEditCommand(std::shared_ptr<bool> aEditedBool)
{
	myEditedBool = aEditedBool;
}

void BoolEditCommand::Do()
{
	*myEditedBool = !*myEditedBool;
}

void BoolEditCommand::Undo()
{
	*myEditedBool = !*myEditedBool;
}