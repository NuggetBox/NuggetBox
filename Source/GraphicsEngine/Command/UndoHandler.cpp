#include "NuggetBox.pch.h"
#include "UndoHandler.h"

void UndoHandler::Do(Command* aCommand)
{
	aCommand->Do();
	ourRedoStack.Clear();
	ourUndoStack.Push(aCommand);
}

void UndoHandler::Undo()
{
	Command* latestDo = ourUndoStack.GetTop();
	latestDo->Undo();
	ourRedoStack.Push(latestDo);
	ourUndoStack.Pop();
}

void UndoHandler::Redo()
{
	Command* latestUndo = ourRedoStack.GetTop();
	latestUndo->Do();
	ourUndoStack.Push(latestUndo);
	ourRedoStack.Pop();
}