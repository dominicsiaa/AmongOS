#pragma once

class CPUCoreWorker {
public:
    CPUCoreWorker();

    void update(bool isRunning);
    void run();

private:
    bool isRunning;
};
