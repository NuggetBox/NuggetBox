#include "NuggetBox.pch.h"
#include "UndoHandler.h"

void UndoHandler::Do(std::shared_ptr<Command> aCommand)
{
	aCommand->Do();
	ourRedoStack.Clear();
	ourUndoStack.Push(aCommand);
}

void UndoHandler::Undo()
{
	std::shared_ptr<Command> latestDo = ourUndoStack.GetTop();
	latestDo->Undo();
	ourRedoStack.Push(latestDo);
	ourUndoStack.Pop();
}

void UndoHandler::Redo()
{
	std::shared_ptr<Command> latestUndo = ourRedoStack.GetTop();
	latestUndo->Do();
	ourUndoStack.Push(latestUndo);
	ourRedoStack.Pop();
}