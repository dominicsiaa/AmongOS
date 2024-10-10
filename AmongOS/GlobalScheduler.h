#pragma once

//Im just guessing here
class GlobalScheduler
{
public:
 
    static GlobalScheduler* getInstance();


    void tick();


    GlobalScheduler(const GlobalScheduler&) = delete;
    GlobalScheduler& operator=(const GlobalScheduler&) = delete;

private:
   
    GlobalScheduler() = default;

  
    static GlobalScheduler* instance;
};
