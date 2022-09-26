#pragma once
#include "Command/Command.h"

class BoolEditCommand : public Command
{
public:
	BoolEditCommand(std::shared_ptr<bool> aEditedBool);
	void Do() override;
	void Undo() override;

private:
	std::shared_ptr<bool> myEditedBool;
};