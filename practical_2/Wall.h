#pragma once
#pragma once
#include "Ship.h"

class Wall : public Ship {

public:
	static const int baseHealth;
	Wall(const sf::Vector2f &pos);
	~Wall() = default;

	void update(const float& dt) override;
	void explode() override;
	bool isPlayer() const override;
	bool isInvader() const override;

protected:
	int health;
	Wall();
};