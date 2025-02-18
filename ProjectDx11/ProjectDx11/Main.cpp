#include <iostream>
#include "Application.hpp"



int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application application;

	application.run();

	return 0;
}
