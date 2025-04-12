#pragma once
#include <string>

class SBEngine
{
public:
    SBEngine(const SBEngine&) = delete;
    SBEngine& operator=(const SBEngine&) = delete;

    static void Init();
    static void Shutdown();

    static SBEngine& GetInstance();

private:
    SBEngine();
    ~SBEngine();
};