#include "Ship.h"
#include "Game.h"
#include "Bullet.h"

using namespace sf;
using namespace std;

// Ship class

const float Ship::initialFadeTimer = .5f;

Ship::Ship() {}

Ship::Ship(IntRect ir) : Sprite() {
	_exploded = false;
	_sprite = ir;
	animate = false;

	_fadeTimer = initialFadeTimer;
	setTexture(spritesheet);
	setTextureRect(_sprite);
}

void Ship::update(const float &dt) {
	// if ship is exploded it is gradually faded
	if (_exploded) {
		_fadeTimer -= dt;
		if (_fadeTimer >= 0)
			setColor({ 255, 255, 255, Uint8(_fadeTimer * 255 / initialFadeTimer) });
	}
}

bool Ship::isExploded() const {
	return _exploded;
}

void Ship::explode() {
	_exploded = true;
}

bool Ship::isPlayer() const {
	return false;
}

bool Ship::isInvader() const {
	return false;
}

Ship::~Ship() = default;

// Invader class

bool Invader::direction;
float Invader::speed;
float Invader::bulletMinTimer;

bool Invader::mothershipDirection;
float Invader::mothershipInitialTimer;
float Invader::mothershipSpeed;

Invader::Invader() : Ship() {}

Invader::Invader(IntRect ir, Vector2f pos, const int score) : Invader(false, ir, pos, score) {}

Invader::Invader(bool isMothership, sf::IntRect ir, sf::Vector2f pos, const int score) : Ship(ir) {
	this->isMothership = isMothership;
	setOrigin(spriteWidth * (isMothership ? 1.f : .5f), spriteHeight * .5f);
	setPosition(pos);
	this->score = score;
}


void Invader::update(const float &dt) {
	Ship::update(dt);

	// regular invader behaviour
	if (!isMothership) {
		move(dt * (direction ? 1.f : -1.f) * speed, 0);

		// if invader is exploded it is not updated
		if (!_exploded) {
			// animate the sprite
			if (animationTimer <= 0) {
				animate = !animate;
				if (animate)
					setTextureRect({ _sprite.left + spriteWidth, _sprite.top, _sprite.width, _sprite.height });
				else
					setTextureRect(_sprite);
			}

			// move down and change direction when an invader reaches an edge
			if (direction && getPosition().x > gameWidth - spriteWidth * .5f || !direction && getPosition().x < spriteWidth * .5f) {
				direction = !direction;
				// move all ships downwards except last one (player)
				for (unsigned int i = 0; i < ships.size() - 6; i++)
					ships[i]->move(0, spriteHeight * .75f);
			}

			// game over if an invader reaches the bottom
			if (getPosition().y >= gameHeight - spriteHeight * 1.25f) {
				gameOver(false);
				return;
			}

			// fire
			static float fireCooldown = 0.f;
			fireCooldown -= dt;

			if (fireCooldown <= 0 && rand() % 100 == 0) {
				Bullet::fire(getPosition(), true);
				fireCooldown = bulletMinTimer + rand() % 60;
			}
		}
	}
	// mothership behaviour
	else {
		static float mothershipHelloThereTimer = mothershipInitialTimer;
		static bool mothershipHelloThere = false;

		if (!mothershipHelloThere) {
			mothershipHelloThereTimer -= dt;

			if (mothershipHelloThereTimer <= 0 && rand() % 1000 == 0)
				mothershipHelloThere = true;
		}
		else {
			move(dt * (mothershipDirection ? 1.f : -1.f) * mothershipSpeed, 0);

			// if the mothership moved off screen
			if (!mothershipDirection && getPosition().x <= -spriteWidth || mothershipDirection && getPosition().x >= gameWidth + spriteWidth) {
				// reset the mothership if it was exploded
				if (_exploded) {
					_exploded = false;
					setOrigin(spriteWidth, spriteHeight * .5f);
					setTextureRect(_sprite);
					setColor({ 255, 255, 255, 255 });
				}

				mothershipDirection = !mothershipDirection;
				mothershipHelloThere = false;
				mothershipHelloThereTimer = mothershipInitialTimer;
				_fadeTimer = initialFadeTimer;
			}
		}
	}
}

void Invader::explode() {
	Ship::explode();

	// set exploded texture
	setTextureRect(IntRect(spriteWidth * 4, spriteHeight, spriteWidth, spriteHeight));

	playerScore += score;

	if (!isMothership) {
		// update invader properties
		speed += 2.f;
		bulletMinTimer -= .5f;
		invaderCount--;
	}
	else {
		// the mothership sprite is wider than the explosion sprite so the origin is changed
		setOrigin(spriteWidth * .5f, spriteHeight * .5f);
	}
}

bool Invader::isPlayer() const {
	return false;
}

bool Invader::isInvader() const {
	return true;
}

// Player class

float Player::speed;
int Player::initialLives;

Player::Player() : Ship(IntRect(spriteWidth * 5, spriteHeight, spriteWidth, spriteHeight)) {
	setOrigin(spriteWidth * .5f, spriteHeight * .5f);
	setPosition({ gameWidth * .5f, gameHeight - spriteHeight * .75f });
	lives = initialLives;
}

void Player::update(const float &dt) {
	Ship::update(dt);

	if (!_exploded) {
		float direction = 0.f;
		if (getPosition().x > spriteWidth * .5f && Keyboard::isKeyPressed(playerLeft))
			direction--;
		if (getPosition().x < gameWidth - spriteWidth * .5f && Keyboard::isKeyPressed(playerRight))
			direction++;

		static float fireCooldown = 0.f;
		fireCooldown -= dt;
		if (fireCooldown <= 0 && Keyboard::isKeyPressed(playerShoot)) {
			Bullet::fire(getPosition(), false);
			fireCooldown = .7f;
		}

		move(dt * direction * speed, 0);

		// if no invader remains trigger victory game over
		if (invaderCount <= 0) {
			static float gameOverTimer = 1.f;
			gameOverTimer -= dt;
			if (gameOverTimer <= 0) {
				gameOverTimer = 1.f;
				gameOver(true);
			}
		}
	}
	else {
		// if lives remaining then respawn
		if (lives > 0) {
			static float respawnTimer = 1.f;
			respawnTimer -= dt;
			if (respawnTimer <= 0) {
				_exploded = false;
				_fadeTimer = initialFadeTimer;
				respawnTimer = 1.f;
				setTextureRect(IntRect(spriteWidth * 5, spriteHeight, spriteWidth, spriteHeight));
				setColor({ 255, 255, 255, 255 });
			}
		}
		// if no lives left then game over
		else {
			static float gameOverTimer = 1.f;
			gameOverTimer -= dt;
			if (gameOverTimer <= 0) {
				gameOverTimer = 1.f;
				gameOver(false);
			}
		}
	}
}

void Player::explode() {
	Ship::explode();
	setTextureRect(IntRect(spriteWidth * 0, spriteHeight, spriteWidth, spriteHeight));
	lives--;
	livesText.setString(livesString + to_string(lives));
}

bool Player::isPlayer() const {
	return true;
}

bool Player::isInvader() const {
	return false;
}