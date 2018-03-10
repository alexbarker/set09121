#pragma once
#include "entity.h"

class Player : public Entity {
private:
	float speed;

	bool validMove(sf::Vector2f pos);

public:
	Player();

	void update(float dt) override;
	void render(sf::RenderWindow& window) const override;
};