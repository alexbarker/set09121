#include "player.h"

using namespace sf;
using namespace std;

void Player::Update(double dt) {
	//move in 4 directions
	int xdir = 0, ydir = 0;
	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		move(Vector2f(0, -_speed * dt));
	}
	if (Keyboard::isKeyPressed(Keyboard::Down)) {
		move(Vector2f(0, _speed * dt));
	}
	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		move(Vector2f(-_speed * dt, 0));
	}
	if (Keyboard::isKeyPressed(Keyboard::Right)) {
		move(Vector2f(_speed * dt, 0));
	}

	Entity::Update(dt);
}

Player::Player() : _speed(200.0), Entity(make_unique<CircleShape>(15.0)) {
	_shape->setFillColor(Color::Yellow);
	_shape->setOrigin(Vector2f(25.0, 25.0));
	_position = Vector2f{ 50.0, 50.0 };
}

void Player::Render(RenderWindow &window) const {
	window.draw(*_shape);
}