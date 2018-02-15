#pragma once
#include <SFML/Graphics.hpp>

class Bullet : public sf::Sprite {

public:
	static float speed;

	// update all bullets
	static void update(const float &dt);

	// render all bullets
	static void render(sf::RenderWindow &window);

	// reset all bullets
	static void reset();

	// fire a bullet
	static void fire(const sf::Vector2f &pos, const bool mode);

	Bullet(const sf::Vector2f &pos, const bool mode);
	~Bullet() = default;

protected:
	Bullet();
	bool _mode; // false : player bullet , true : invader bullet

				// bullet pool
	static unsigned char bulletPointer;
	static Bullet bullets[256];

	// update individual bullet
	void _update(const float &dt);
};