#include "Game.hpp"

Game::Game(Renderer& renderer, Window& window)
	:scene(renderer),
	window(window),
    inputHandler()
{
    window.setInputHandler(&inputHandler); // Pass input handler to window
    scene.setInputHandler(&inputHandler); // Pass input handler to scene

	GameObject* cube = new GameObject(renderer,"Cube.obj", "T_CubeTest_D.png");
	cube->translate(0.0f, 0.5f, 0.0f);
	scene.addGameObject(cube);

	GameObject* cube2 = new GameObject(renderer, "Cube.obj", "T_CubeTest_D.png");
	cube2->translate(5.0f, 0.5f, 0.0f);
	cube2->setRotateAlongY(0.1f);
	scene.addGameObject(cube2);

	GameObject* landscape = new GameObject(renderer,"Landscape.obj", "T_BTH_D.png");
	scene.addGameObject(landscape);

	Light* ambientLight = new AmbientLight(renderer);
	scene.addLight(ambientLight);

	Light* pointLight = new PointLight(renderer);
	scene.addLight(pointLight);

	Light* directionalLight = new DirectionalLight(renderer);
	scene.addLight(directionalLight);

	PhongMaterial* phongMaterial = new PhongMaterial(renderer);
	scene.addMaterial(phongMaterial);

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
