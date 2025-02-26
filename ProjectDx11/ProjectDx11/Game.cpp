#include "Game.hpp"

Game::Game(Renderer& renderer)
	:scene(renderer)
{

}

Game::~Game()
{

}

void Game::addObject()
{
	scene.addObject();
}

void Game::update(Renderer& renderer)
{
	scene.update(renderer);
}
