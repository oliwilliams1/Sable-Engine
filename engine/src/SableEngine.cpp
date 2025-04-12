#include "SableEngine.h"
#include "DebugLog.h"
#include "ECS.h"

using namespace SB;

static SBEngine* s_Instance = nullptr;

SBEngine::SBEngine()
{
    Console::Init();
	ECS::Init();
}

SBEngine::~SBEngine()
{
    Console::Shutdown();
	ECS::Shutdown();
}

void SBEngine::Init()
{
	s_Instance = new SBEngine();
}

void SBEngine::Shutdown()
{
	delete s_Instance;
	s_Instance = nullptr;
}

SBEngine& SBEngine::GetInstance()
{
    return *s_Instance;
}