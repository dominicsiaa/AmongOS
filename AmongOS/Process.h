#pragma once
#include "typedefRepo.h"

#include <sstream>

class Process
{
public:
    Process(const String& processName, int totalLines, std::stringstream timeStamp);
    ~Process() = default;

    String getName() const;
    int getCurrentLine() const;
    int getTotalLines() const;
    const std::stringstream& getTimeStamp() const;
   
private:
    String name;         
    int currentLine;         
    int totalLines;
    std::stringstream timeStamp;
};