#pragma once
#include "IThread.h"

class InputWorker : public IThread
{
public:
    InputWorker();

    void update(bool isRunning);
    void run() override;

private:
    bool isRunning;
};