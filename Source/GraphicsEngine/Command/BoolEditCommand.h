#pragma once
#include "Command/Command.h"

class BoolEditCommand : public Command
{
public:
	BoolEditCommand(std::shared_ptr<bool> aEditedBool, bool aNewValue);
	void Do() override;
	void Undo() override;
private:
};

