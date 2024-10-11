#include "PrintCommand.h"
#include "IThread.h"
#include <fstream>
#include <sstream>
#include <filesystem> 

//#include "MessageBuffer.h"


PrintCommand::PrintCommand(int pid, const String& toPrint) : ICommand(pid, PRINT)
{
	this->toPrint = toPrint;
}


void PrintCommand::execute()
{
    std::string folder = "temp"; // Specify the folder name
    std::ostringstream filename;
    filename << folder << "/pid" << pid << ".txt"; // Construct the filename with folder

    // Ensure the folder exists
    std::filesystem::create_directories(folder);

    std::ofstream outFile(filename.str(), std::ios_base::out | std::ios_base::app); // Open file in append mode, create if it doesn't exist
    if (outFile.is_open()) {
        outFile << "\"" << toPrint << "\"" <<  std::endl;
        outFile.close();
    }
    else {
        std::cerr << "Error: Could not open or create file " << filename.str() << std::endl;
    }
}