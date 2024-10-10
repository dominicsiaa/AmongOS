#pragma once
#include "IThread.h"

class ICommand
{

public:
	enum CommandType
	{
		IO,
		PRINT
	};

	ICommand(int pid, CommandType commandType);
    virtual ~ICommand() = default;
	static std::shared_ptr<ICommand> createCommand(CommandType commandType, int pid);
    CommandType getCommandType() const;

    virtual void execute() = 0;

protected:
	int pid;
	CommandType commandType;

};