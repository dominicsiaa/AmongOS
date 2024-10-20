#include "ICommand.h"
#include "PrintCommand.h" // Include other command headers as needed

ICommand::ICommand(int pid, CommandType commandType)
    : pid(pid), commandType(commandType) {}

ICommand::CommandType ICommand::getCommandType() const {
    return commandType;
}

// Static factory method implementation
std::shared_ptr<ICommand> ICommand::createCommand(CommandType commandType, int pid) {
    switch (commandType) {
        case PRINT:
            return std::make_shared<PrintCommand>(pid, std::string("Hello world from Process" + std::to_string(pid)));
        default:
            return nullptr;
    }
}
