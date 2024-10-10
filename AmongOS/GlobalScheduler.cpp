#include "GlobalScheduler.h"
#include <iostream> 



//also guessing here 
GlobalScheduler* GlobalScheduler::instance = nullptr;

GlobalScheduler* GlobalScheduler::getInstance()
{
    if (instance == nullptr)
    {
        instance = new GlobalScheduler();
    }
    return instance;
}

void GlobalScheduler::tick()
{
    std::cout << "GlobalScheduler is ticking..." << std::endl;
}
