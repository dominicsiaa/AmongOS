#pragma once
#include "IThread.h"
#include "CPUCore.h"

class CPUCoreWorker : public IThread
{
public:
    CPUCoreWorker(std::shared_ptr<CPUCore> core);

    void update(bool isRunning);
    void run() override;

private:
    bool isRunning;
    std::shared_ptr<CPUCore> core;
};