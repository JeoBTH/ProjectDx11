#pragma once
#include <vector>
#include "Renderer.hpp"
#include "GameObject.hpp"
#include "Light.hpp"
#include "Camera.hpp"

using namespace std;

class Scene
{
private:

	vector<GameObject*> m_gameObjects;
	vector<Light*> m_lights;
	Camera m_camera;

public:
	Scene(Renderer& renderer);
	~Scene();

	void addGameObject(GameObject* gameObject);
	void clearGameObjects();

	void addLight(Light* light);
	void clearLights();

	Camera& getCamera();

	void update(Renderer& renderer);
	void draw(Renderer& renderer);
};