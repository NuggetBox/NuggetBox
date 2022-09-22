#pragma once

class Command
{
public:
	virtual void Do() = 0;
	virtual void Undo() = 0;
	virtual ~Command();
};