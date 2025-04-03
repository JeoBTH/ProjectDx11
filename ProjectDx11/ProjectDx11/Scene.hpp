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
	float moveSpeed; // Camera movement speed

	InputHandler* inputHandler = nullptr; // Pointer to InputHandler

public:
	Scene(Renderer& renderer, float cameraMovementSpeed = 1);
	~Scene();

	void addGameObject(GameObject* gameObject);
	void clearGameObjects();

	void addLight(Light* light);
	void clearLights();

	void setInputHandler(InputHandler* handler) { inputHandler = handler; } // Setter for input

	Camera& getCamera();

	void update(Renderer& renderer);
	void draw(Renderer& renderer);
};