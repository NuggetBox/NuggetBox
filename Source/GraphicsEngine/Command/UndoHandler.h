#pragma once
#include "Command/Command.h"
#include "Stack.hpp"

class UndoHandler
{
public:
	static void Do(std::shared_ptr<Command> aCommand);
	static void Undo();
	static void Redo();

private:
	static Utility::Stack<std::shared_ptr<Command>> ourUndoStack;
	static Utility::Stack<std::shared_ptr<Command>> ourRedoStack;
};