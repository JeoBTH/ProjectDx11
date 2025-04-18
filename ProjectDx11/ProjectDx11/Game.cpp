#include "Game.hpp"

Game::Game(Renderer& renderer, Window& window)
	:scene(renderer),
	window(window),
    inputHandler("Jesper")
{
    window.setInputHandler(&inputHandler); // Pass input handler to window
    scene.setInputHandler(&inputHandler); // Pass input handler to scene

	GameObject* cube = new GameObject(renderer,"Cube.obj", "T_CubeTest_D.png");
	cube->translate(0.0f, 0.5f, 0.0f);
	scene.addGameObject(cube);

	GameObject* landscape = new GameObject(renderer,"Landscape.obj", "T_BTH_D.png");
	scene.addGameObject(landscape);

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
