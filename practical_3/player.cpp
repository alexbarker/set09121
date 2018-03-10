#include "player.h"
#include "main.h"
#include "levelsystem.h"

using namespace sf;
using namespace std;

Player::Player() : Entity(make_unique<CircleShape>(playerSize * .5f)), speed(playerSpeed) {
	shape->setFillColor(playerColor);
	shape->setOrigin({ playerSize * .5f, playerSize * .5f });
}

void Player::update(const float dt) {
	Vector2f pos = getPosition();

	if (Keyboard::isKeyPressed(playerUp))
		move({ 0, -speed * dt });
	if (Keyboard::isKeyPressed(playerLeft))
		move({ -speed * dt, 0 });
	if (Keyboard::isKeyPressed(playerDown))
		move({ 0, speed * dt });
	if (Keyboard::isKeyPressed(playerRight))
		move({ speed * dt, 0 });

	if (!validMove(getPosition()))
		setPosition(pos);

	Entity::update(dt);

	if (ls::getTileAt(getPosition()) == ls::END)
		gameOver();
}

void Player::render(RenderWindow& window) const {
	window.draw(*shape);
}

bool Player::validMove(Vector2f pos) {
	return ls::getTileAt(pos) != ls::WALL;
}