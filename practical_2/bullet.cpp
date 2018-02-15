#include "Bullet.h"
#include "Game.h"

unsigned char Bullet::bulletPointer;
Bullet Bullet::bullets[256];

float Bullet::speed;

Bullet::Bullet() {}

Bullet::Bullet(const sf::Vector2f& pos, const bool mode) {
	setOrigin(3.5f, 6.f);
	setPosition(pos);

	_mode = mode;

	setTexture(spritesheet);

	if (_mode)
		setTextureRect(sf::IntRect(spriteWidth * 1 + 12, spriteHeight + 10, 7, 12));
	else
		setTextureRect(sf::IntRect(spriteWidth * 3 + 12, spriteHeight + 10, 7, 12));
}

void Bullet::update(const float &dt) {
	for (auto &bullet : bullets) {
		bullet._update(dt);
	}
}

void Bullet::render(sf::RenderWindow &window) {
	for (const auto &bullet : bullets) {
		window.draw(bullet);
	}
}

void Bullet::reset() {
	for (int i = 0; i < 256; i++)
		bullets[i].setPosition({ -100, -100 });
}

void Bullet::fire(const sf::Vector2f& pos, const bool mode) {
	bullets[bulletPointer++] = Bullet(pos, mode);
}

void Bullet::_update(const float &dt) {
	// bullet is off-screen
	if (getPosition().y <= -32 || getPosition().y >= gameHeight + 32)
		return;

	move(0, dt * speed * (_mode ? 1.f : -1.f));
	const sf::FloatRect bulletBounds = getGlobalBounds();

	for (auto ship : ships) {
		// player invincibility (cheat code)
		if (invicible && ship->isPlayer())
			continue;
		// player bullet on player collision
		if (!_mode && ship->isPlayer())
			continue;
		// invader bullet on invader collision
		if (_mode && ship->isInvader())
			continue;
		// all other collisions
		if (!ship->isExploded() && ship->getGlobalBounds().intersects(bulletBounds)) {
			ship->explode();
			setPosition(-100, -100);
			return;
		}
	}
}

