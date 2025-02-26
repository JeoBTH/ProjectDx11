#pragma once
#include "Renderer.hpp"
#include "GameObject.hpp"

class Scene
{
private:

	GameObject gameObject;

public:
	Scene(Renderer& renderer);
	~Scene();

	void addObject();
	void update(Renderer& renderer);

};