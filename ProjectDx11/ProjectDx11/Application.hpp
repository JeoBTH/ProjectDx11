#pragma once
#include <iostream>
#include <Windows.h>
#include "Window.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "GameObject.hpp"


class Application
{
private:
	Window window;
	Renderer renderer;
	Camera camera;
	Light light;
	GameObject gameObject;

	void AttachConsoleWindow();

public:
	Application();
	~Application();

	void run();

};

