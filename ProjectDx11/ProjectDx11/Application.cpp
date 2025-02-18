#include <Windows.h>
#include "Window.hpp"
#include "Renderer.hpp"

#include <iostream>

void AttachConsoleWindow();

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	AttachConsoleWindow();

	Window window(800, 600);
	Renderer renderer(window);

	MSG msg = { 0 };

	while (true)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				break;
			}
		}

		renderer.beginFrame();
		renderer.endFrame();
	}

	return 0;
}


void AttachConsoleWindow()
{
	AllocConsole(); // Allocate a new console
	FILE* newStdout;
	freopen_s(&newStdout, "CONOUT$", "w", stdout); // Redirect stdout to the console
	freopen_s(&newStdout, "CONOUT$", "w", stderr); // Redirect stderr to the console
	freopen_s(&newStdout, "CONIN$", "r", stdin);   // Redirect stdin to the console
	std::cout.clear();  // Clear the state of standard streams
	std::cin.clear();
	std::cerr.clear();
}