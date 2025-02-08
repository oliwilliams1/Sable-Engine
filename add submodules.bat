@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

:ask_glm
set /p user_input="Do you want to add GLM submodule? (y/n): "
if /i "!user_input!"=="y" (
    git submodule add https://github.com/g-truc/glm.git vendor/glm
    echo GLM submodule added.
) else (
    echo Skipping GLM submodule.
)

:ask_glfw
set /p user_input="Do you want to add GLFW submodule? (y/n): "
if /i "!user_input!"=="y" (
    git submodule add https://github.com/glfw/glfw.git vendor/glfw
    echo GLFW submodule added.
) else (
    echo Skipping GLFW submodule.
)

:ask_glew
set /p user_input="Do you want to add GLEW submodule? (y/n): "
if /i "!user_input!"=="y" (
    git submodule add https://github.com/Perlmint/glew-cmake.git vendor/glew
    echo GLEW submodule added.
) else (
    echo Skipping GLEW submodule.
)

:ask_imgui
set /p user_input="Do you want to add Dear ImGui submodule? (y/n): "
if /i "!user_input!"=="y" (
    git submodule add -b docking https://github.com/ocornut/imgui.git vendor/imgui
    echo Dear ImGui submodule added.
) else (
    echo Skipping Dear ImGui submodule.
)

:ask_stb
set /p user_input="Do you want to add stb submodule? (y/n): "
if /i "!user_input!"=="y" (
    git submodule add https://github.com/nothings/stb.git vendor/stb
    echo stb submodule added.
) else (
    echo Skipping stb submodule.
)

:ask_imgui_file_dialog
set /p user_input="Do you want to add ImGuiFileDialog submodule? (y/n): "
if /i "!user_input!"=="y" (
    git submodule add https://github.com/aiekick/ImGuiFileDialog.git vendor/imgui-file-dialog
    echo ImGuiFileDialog submodule added.
) else (
    echo Skipping ImGuiFileDialog submodule.
)

:ask_assimp
set /p user_input="Do you want to add Assimp submodule? (y/n): "
if /i "!user_input!"=="y" (
    git submodule add https://github.com/assimp/assimp.git vendor/assimp
    echo Assimp submodule added.
) else (
    echo Skipping Assimp submodule.
)

echo Initializing and updating submodules...
git submodule update --init --recursive

ENDLOCAL