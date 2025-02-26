#include "Scene.hpp"

Scene::Scene(Renderer& renderer)
	:m_camera(renderer)
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

	m_camera.update(renderer);
}

void Scene::draw(Renderer& renderer)
{
	for (auto* gameObjects : m_gameObjects)
	{
		gameObjects->draw(renderer);
	}

}
