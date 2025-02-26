#include "Game.hpp"

Game::Game(Renderer& renderer)
	:scene(renderer)
{
	GameObject* rectangle = new GameObject(renderer);
	scene.addGameObject(rectangle);

	Light* light = new Light(renderer);
	scene.addLight(light);

}

Game::~Game()
{
	scene.clearGameObjects();
	scene.clearLights();
}

void Game::addGameObject(GameObject* gameObject)
{
	scene.addGameObject(gameObject);
}

void Game::update(Renderer& renderer)
{
	scene.update(renderer);
}

void Game::draw(Renderer& renderer)
{
	scene.draw(renderer);
}
