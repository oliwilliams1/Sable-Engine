#include <iostream>
#include "App.h"

int main()
{
	App::Init();
	App::GetInstance().LoadProject("test.sbproj");
	App::GetInstance().Mainloop();
	App::Shutdown();
	return 0;
}
