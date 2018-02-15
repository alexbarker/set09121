#pragma once
#include <SFML/Graphics.hpp>

class Ship : public sf::Sprite {

protected:
	sf::IntRect _sprite;
	bool animate;
	Ship();
	bool _exploded;
	float _fadeTimer;

public:
	static const float initialFadeTimer;
	explicit Ship(sf::IntRect ir);
	virtual ~Ship() = 0;
	virtual void update(const float &dt);
	bool isExploded() const;
	virtual bool isPlayer() const;
	virtual bool isInvader() const;
	virtual void explode();
};

class Invader : public Ship {
protected:
	int score;
	Invader();

public:
	static bool direction;
	static float speed;
	static float bulletMinTimer;

	static bool mothershipDirection;
	static float mothershipInitialTimer;
	static float mothershipSpeed;
	bool isMothership;

	Invader(sf::IntRect ir, sf::Vector2f pos, const int score);
	Invader(bool isMothership, sf::IntRect ir, sf::Vector2f pos, const int score);
	void update(const float &dt) override;
	void explode() override;
	bool isPlayer() const override;
	bool isInvader() const override;
};

class Player : public Ship {
protected:
	int lives;

public:
	static float speed;
	static int initialLives;

	Player();
	void update(const float &dt) override;
	void explode() override;
	bool isPlayer() const override;
	bool isInvader() const override;
};