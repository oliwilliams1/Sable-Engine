#include "SableEngine.h"
#include "AssetManager.h"

using namespace SableEngine;

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