#include <iostream>
#include "App.h"

int main()
{
	App::Init();
	App::GetInstance().Mainloop();
	App::Shutdown();
	return 0;
}
