#pragma once
#include <iostream>
#include <Windows.h>
#include "Window.hpp"
#include "Renderer.hpp"
#include "Game.hpp"


class Application
{
private:
	Window window;
	Renderer renderer;
	Game game;

	void AttachConsoleWindow();

public:
	Application();
	~Application();

	void run();

};

