#pragma once
#include "Renderer.hpp"
#include "Scene.hpp"

class Game
{
private:
	Scene scene;

public:
	Game(Renderer& renderer);
	~Game();

	void addGameObject(GameObject* gameObject);
	void update(Renderer& renderer);
	void draw(Renderer& renderer);

};

