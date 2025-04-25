#include "Scene.hpp"

Scene::Scene(Renderer& renderer, float cameraMovementSpeed)
	:m_camera(renderer),
    moveSpeed(cameraMovementSpeed)
{
}

Scene::~Scene()
{
}

void Scene::addGameObject(GameObject* gameObject)
{
	m_gameObjects.push_back(gameObject);
}

void Scene::clearGameObjects()
{
	for (auto* GameObject : m_gameObjects)
	{
		delete GameObject;
	}
	m_gameObjects.clear();
}

const vector<GameObject*>& Scene::getGameObjects() const
{
	return m_gameObjects;
}

void Scene::addLight(Light* light)
{
	m_lights.push_back(light);
}

void Scene::clearLights()
{
	for (auto* light : m_lights)
	{
		delete light;
	}
	m_lights.clear();
}

Camera& Scene::getCamera()
{
	return m_camera;
}

void Scene::update(Renderer& renderer)
{
	for (auto* gameObject : m_gameObjects)
	{
		gameObject->update(renderer);
	}

	for (auto* light : m_lights)
	{
		light->update(renderer);
	}


    // Camera WASDEQ movement
    if (inputHandler->IsKeyDown('W')){ m_camera.move(0, 0, -moveSpeed); }
    if (inputHandler->IsKeyDown('S')) { m_camera.move(0, 0, moveSpeed); }
    if (inputHandler->IsKeyDown('A')) { m_camera.move(moveSpeed, 0, 0); }
    if (inputHandler->IsKeyDown('D')) { m_camera.move(-moveSpeed, 0, 0); }
	if (inputHandler->IsKeyDown('E')) { m_camera.move(0, moveSpeed, 0); }
	if (inputHandler->IsKeyDown('Q')) { m_camera.move(0, -moveSpeed, 0); }

	
	// Camera Mouse movement
	if (inputHandler->IsMouseButtonDown(VK_RBUTTON)) // Checks for Right Click
	{
		float deltaX = inputHandler->GetMouseDeltaX();
		float deltaY = inputHandler->GetMouseDeltaY();

		m_camera.processMouseMovement(deltaX, -deltaY);
	}

	inputHandler->update();

    m_camera.update(renderer);
}

void Scene::draw(Renderer& renderer)
{
	// Shadow Pass
	for (auto* light : m_lights) 
	{
		light->renderBeginShadowMap(renderer);

		for (auto* gameObjects : m_gameObjects)
		{
			gameObjects->drawShadows(renderer);
		}

		light->renderEndShadowMap(renderer);
	}

	// Main Pass
	for (auto* gameObjects : m_gameObjects)
	{
		gameObjects->draw(renderer);
	}
}
