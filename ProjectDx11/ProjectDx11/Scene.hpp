#pragma once
#include <vector>
#include "Renderer.hpp"
#include "GameObject.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "PhongMaterial.hpp"

using namespace std;

class Scene
{
private:
	vector<GameObject*> m_gameObjects;
	vector<Light*> m_lights;
	vector<PhongMaterial*> m_materials;
	Camera m_camera;
	float moveSpeed; // Camera movement speed

	InputHandler* inputHandler = nullptr; // Pointer to InputHandler

public:
	Scene(Renderer& renderer, float cameraMovementSpeed = 0.1f);
	~Scene();

	void addGameObject(GameObject* gameObject);
	void clearGameObjects();
	const vector<GameObject*>& getGameObjects() const;

	void addLight(Light* light);
	void clearLights();

	void addMaterial(PhongMaterial* material);
	void clearMaterials();

	void setInputHandler(InputHandler* handler) { inputHandler = handler; }

	Camera& getCamera();

	void update(Renderer& renderer);
	void draw(Renderer& renderer);
};