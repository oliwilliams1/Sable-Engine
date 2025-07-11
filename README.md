# Building from source
Install the vulkan sdk based on your platform: https://vulkan.lunarg.com

Ensure you have CMake installed, then clone this repo
## Windows
Ensure you have Visual Studio (MSVC), as this is the build tool these instructions follow.
You may have to restart to ensure CMake can detect the new Vulkan installation
Then you can run the following commands in a new terminal to setup a .sln file
```bash
cd <repo-name>
mkdir build
cd build
cmake ..
```
Open the .sln file in the /build directory, set the engine as startup project, then building should work.

## MacOS
Once you have installed VulkanSDK, you must run ```setup-env.sh```  to setup the vulkan env on macos
You must also have installed python 3.0+ for the install script
```bash
cd <vulkan-sdk-dir>
python3 install_vulkan.py
```
Then you can run these commands to to build the executable
```bash
cd <repo-name>
mkdir build
cd build
cmake ..
make
./Sable-Editor
```
