#include "Entity.h"

#include <algorithm>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "Scene.h"

using namespace Strife;
using namespace std;
using boost::uuids::uuid;

Entity::Components::Components(const Entity* const entity) :
    entity_(entity) {};

Component* const Entity::Components::add(const type_index type) const {
    return entity_->scene->components.add(type, entity_);
};

Component* const Entity::Components::add(const type_index type, const uuid id) const {
    return entity_->scene->components.add(type, id, entity_);
};

void Entity::Components::remove(const type_index type) const {
    entity_->scene->components.remove(type, entity_);
};

Component* const Entity::Components::get(const type_index type) const {
    return entity_->scene->components.get(type, entity_);
};

Entity::Entity(const Entity& entity) :
    Unique(entity),
    scene(entity.scene),
    components(this) {};

Entity::Entity(Scene* const scene) :
    scene(scene),
    components(this) {};
    
Entity::Entity(const boost::uuids::uuid id, Scene* const scene) :
    Unique(id),
    scene(scene),
    components(this) {};