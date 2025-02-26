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

	void addObject();
	void update(Renderer& renderer);

};

