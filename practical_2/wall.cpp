#include "Wall.h"
#include "Game.h"

const int Wall::baseHealth = 3;

Wall::Wall() {}

Wall::Wall(const sf::Vector2f& pos) : Ship(sf::IntRect(spriteWidth * 6, spriteHeight, spriteWidth, spriteHeight)) {
	setOrigin(spriteWidth / 2.f, spriteHeight / 2.f);

	setPosition(pos);

	scale(1.5f, 1.f);

	this->health = baseHealth;
}

void Wall::update(const float& dt) {
	if (health <= baseHealth / 2)
		setTextureRect(sf::IntRect(spriteWidth * 7, spriteHeight, spriteWidth, spriteHeight));
}

void Wall::explode() {
	health--;
	if (health <= 0) {
		Ship::explode();
		setColor(sf::Color::Black);
	}
}

bool Wall::isPlayer() const {
	return false;
}

bool Wall::isInvader() const {
	return false;
}