#include "Application.hpp"
#include "Window.hpp"

Application::Application()
	: window(800, 600),
	renderer(window),
	game(renderer, window)
{
	AttachConsoleWindow();
}

Application::~Application()
{

}

void Application::AttachConsoleWindow()
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

void Application::run()
{

	MSG msg = {};
	while (window.processMessages())  // Keep processing messages
	{
		renderer.beginFrame();
		game.update(renderer);
		game.draw(renderer);
		renderer.endFrame();
	}
}

