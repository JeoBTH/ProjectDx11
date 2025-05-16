#include "Game.hpp"

Game::Game(Renderer& renderer, Window& window)
	:scene(renderer),
	window(window),
    inputHandler()
{
    window.setInputHandler(&inputHandler); // Pass input handler to window
    scene.setInputHandler(&inputHandler); // Pass input handler to scene
}

Game::~Game()
{
	scene.clearGameObjects();
	scene.clearLights();
	scene.clearMaterials();
}

void Game::update(Renderer& renderer)
{
	scene.update(renderer);
	
}

void Game::draw(Renderer& renderer)
{
	scene.draw(renderer);
}
