#pragma once
#include "Command/Command.h"
#include "Stack.hpp"

class UndoHandler
{
public:
	static void Do(Command* aCommand);
	static void Undo();
	static void Redo();

private:
	static Utility::Stack<Command*> ourUndoStack;
	static Utility::Stack<Command*> ourRedoStack;
};