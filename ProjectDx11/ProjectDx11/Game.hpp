#pragma once
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Window.hpp"
#include "InputHandler.hpp"

class Game
{
private:
	Scene scene;
	Window& window;
	InputHandler inputHandler;

public:
	Game(Renderer& renderer, Window& window);
	~Game();

	void addGameObject(GameObject* gameObject);
	void update(Renderer& renderer);
	void draw(Renderer& renderer);

	InputHandler& getInputHandler() { return inputHandler; }
};

