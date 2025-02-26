#include "Scene.hpp"

Scene::Scene(Renderer& renderer)
	:	gameObject(renderer),
	light(renderer),
	camera(renderer)
{

}

Scene::~Scene()
{

}

void Scene::addObject()
{

}

void Scene::update(Renderer& renderer)
{
	gameObject.update(renderer);
	light.update(renderer);
	camera.update(renderer);
}

void Scene::draw(Renderer& renderer)
{
	gameObject.draw(renderer);
}
