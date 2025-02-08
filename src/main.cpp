#include <stdio.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <imgui.h>
#include <ImGuiFileDialog.h>

int main(char argc, char* argv[])
{
    glm::vec3 x = glm::vec3(1.0f);
    x.x += 1.0f;

    GLuint someValue;
	someValue = 1;
    someValue += 1;


    printf("Hello World! \t %f \t %i \n", x.x, someValue);
    return 0;
}