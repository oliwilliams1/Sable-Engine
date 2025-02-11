#pragma once
#include <GL/glew.h>
#include <string>

class SBEngine
{
public:
    SBEngine();
    ~SBEngine();

    bool LoadTextureFromFile(const std::string& filename, GLuint& id);

private:

};