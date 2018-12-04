#include "Scene.h"

#include "Engine.h"

using namespace Strife::Core;
using namespace std;
using boost::uuids::uuid;

const unsigned int Scene::Entities::AddEvent::Priority = Dispatcher::Final;
const unsigned int Scene::Entities::RemoveEvent::Priority = Dispatcher::Final;

const Entity Scene::Entities::add() {
    const Entity entity(*scene_);
	scene_.dispatcher_.emit<AddEvent>(entity);
	return entity;
}

void Scene::Entities::remove(const Entity& entity) {
	scene_.components.remove(entity);
	scene_.dispatcher_.emit<RemoveEvent>(entity);
}

Scene::Components::Components(Scene& scene)
    : scene_(scene) {}
    
Scene::Components::~Components() {
    for (auto [ type, storage ] : components_) {
		delete storage;
	}
}

Component& Scene::Components::add(const type_index type, const Entity& entity) {
	return components_.at(type)->add(entity);
}

Component& Scene::Components::add(const type_index type, const uuid id, const Entity& entity) {
	return components_.at(type)->add(id, entity);
}

void Scene::Components::remove(const type_index type, const Entity& entity) {
	return components_.at(type)->remove(entity);
}

void Scene::Components::remove(const Entity& entity) {
	for (auto [ type, storage ] : components_) {
		storage->remove(entity);
	}
}

Component& Scene::Components::get(const type_index type, const Entity& entity) const {
	return components_.at(type)->get(entity);
}

Scene::Systems::Systems(Scene& scene)
    : scene_(scene) {}
    
Scene::Systems::~Systems() {
	for (auto [ type, system ] : systems_) {
		delete system;
	}
}

Scene::Scene()
    : entities(*this)
    , components(*this)
    , systems(*this) {}