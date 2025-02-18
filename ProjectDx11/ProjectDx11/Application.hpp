#pragma once
#include <iostream>
#include <Windows.h>
#include "Window.hpp"
#include "Renderer.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Light.hpp"


class Application
{
private:
	Window window;
	Renderer renderer;
	Mesh mesh;
	Camera camera;
	Light light;

	void AttachConsoleWindow();

public:
	Application();
	~Application();

	void run();

};

