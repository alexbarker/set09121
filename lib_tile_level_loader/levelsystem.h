#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include "maths.h"

#define ls LevelSystem

class LevelSystem {
public:
	enum Tile {
		EMPTY,
		START,
		END,
		WALL,
		ENEMY,
		WAYPOINT
	};

	static void loadLevelFile(const std::string &filepath, float tileSize = 100.f);
	static void render(sf::RenderWindow &window);
	static sf::Color getColor(Tile tile);
	static void setColor(Tile tile, sf::Color color);
	static Tile getTile(sf::Vector2ul coordinates);
	static sf::Vector2f getTilePosition(sf::Vector2ul coordinates);
	static Tile getTileAt(sf::Vector2f position);
	static void setOffset(sf::Vector2f offset);
	static sf::Vector2f getStartPosition();
	static size_t getWidth();
	static size_t getHeight();
	static float getTileSize();
	static std::vector<sf::Vector2ul> findTiles(Tile tile);


protected:
	static std::unique_ptr<Tile[]> tiles;
	static size_t width;
	static size_t height;
	static float tileSize;
	static sf::Vector2f offset;
	static std::map<Tile, sf::Color> colors;
	static std::vector<std::unique_ptr<sf::RectangleShape>> sprites;
	static sf::Vector2f startPosition;

	static void buildSprites();

private:
	LevelSystem() = delete;
	~LevelSystem() = delete;
};