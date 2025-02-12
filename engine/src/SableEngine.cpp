#include "SableEngine.h"
#include "AssetManager.h"

using namespace SableEngine;

static SBEngine* s_Instance = nullptr;

SBEngine::SBEngine()
{
    AssetManager::Init();
}

SBEngine::~SBEngine()
{
    AssetManager::Shutdown();
}

bool SBEngine::LoadTextureFromFile(const std::string& filename, GLuint& id)
{
    SB_TEXTURE tempTexture;

    bool res = AssetManager::LoadTextureFromFile(filename, tempTexture);

    id = tempTexture.id;
    return res;
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