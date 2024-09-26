#pragma once
#include "typedefRepo.h"

class Process
{
public:
    Process(const String& processName, int totalLines);
    ~Process() = default;

    String getName() const;
    int getCurrentLine() const;
    int getTotalLines() const;
   
private:
    String name;         
    int currentLine;         
    int totalLines;
};