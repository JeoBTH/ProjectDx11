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


    // Camera movement with WASD
    if (inputHandler->IsKeyDown('W')){ m_camera.move(0, 0, moveSpeed); }
    if (inputHandler->IsKeyDown('S')) { m_camera.move(0, 0, -moveSpeed); }
    if (inputHandler->IsKeyDown('A')) { m_camera.move(-moveSpeed, 0, 0); }
    if (inputHandler->IsKeyDown('D')) { m_camera.move(moveSpeed, 0, 0); }

	// Camera mouse movement
	inputHandler->updateMouseDelta();
	if (inputHandler->IsMouseButtonDown(VK_RBUTTON))
	{
		float deltaX = inputHandler->GetMouseDeltaX();
		float deltaY = inputHandler->GetMouseDeltaY();
		if (deltaX > 0.00f)
		{
			int a = 2;
		}

		m_camera.processMouseMovement(deltaX, -deltaY);
	}

	inputHandler->update();

    m_camera.update(renderer);

}

void Scene::draw(Renderer& renderer)
{
	for (auto* gameObjects : m_gameObjects)
	{
		gameObjects->draw(renderer);
	}

}
