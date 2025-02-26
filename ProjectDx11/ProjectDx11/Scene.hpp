#pragma once
#include "Renderer.hpp"
#include "GameObject.hpp"
#include "Light.hpp"
#include "Camera.hpp"

class Scene
{
private:

	GameObject gameObject;
	Light light;
	Camera camera;

public:
	Scene(Renderer& renderer);
	~Scene();

	void addObject();
	void update(Renderer& renderer);
	void draw(Renderer& renderer);
};