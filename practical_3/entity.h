#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Entity {
protected:
	std::unique_ptr<sf::Shape> shape;
	sf::Vector2f position;
	Entity(std::unique_ptr<sf::Shape> shape);

public:
	Entity() = delete;
	virtual ~Entity() = default;

	virtual void update(const float dt);
	virtual void render(sf::RenderWindow &window) const = 0;

	sf::Vector2f getPosition() const;
	void setPosition(const sf::Vector2f &position);
	void move(const sf::Vector2f &position);
};