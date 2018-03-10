#include "entity.h"

using namespace sf;
using namespace std;

Vector2f Entity::getPosition() const {
	return position;
}

void Entity::setPosition(const Vector2f &position) {
	this->position = position;
}

void Entity::move(const Vector2f &position) {
	this->position += position;
}

void Entity::update(const float dt) {
	shape->setPosition(position);
}

Entity::Entity(unique_ptr<Shape> shape) : shape(std::move(shape)) {}