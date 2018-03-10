#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "levelsystem.h"
#include "maths.h"

using namespace std;
using namespace sf;

unique_ptr<LevelSystem::Tile[]> LevelSystem::tiles;
size_t LevelSystem::width;
size_t LevelSystem::height;

float LevelSystem::tileSize(100.f);
Vector2f LevelSystem::offset(0.f, 0.f);
Vector2f LevelSystem::startPosition;

vector<unique_ptr<RectangleShape>> LevelSystem::sprites;

map<LevelSystem::Tile, Color> LevelSystem::colors{
	{ WALL, Color::White },
{ END, Color::Red },
{ START, Color::Green }
};

Color LevelSystem::getColor(Tile tile) {
	auto it = colors.find(tile);
	return it == colors.end() ? Color::Transparent : colors[tile];
}

void LevelSystem::setColor(Tile tile, Color color) {
	colors[tile] = color;
}

void LevelSystem::loadLevelFile(const std::string& filepath, float tileSize) {
	LevelSystem::tileSize = tileSize;

	size_t width = 0;
	size_t height = 0;
	string buffer;

	ifstream file(filepath);
	if (file.good()) {
		file.seekg(0, ios::end);
		buffer.resize(file.tellg());
		file.seekg(0);
		file.read(&buffer[0], buffer.size());
		file.close();
	}
	else
		//throw string("Couldn't open level file : ") + filepath;

	vector<Tile> tempTiles;
	for (auto i = 0; i < buffer.size(); i++) {
		const char c = buffer[i];
		switch (c) {
		case 'w':
			tempTiles.push_back(WALL);
			break;
		case 's':
			tempTiles.push_back(START);
			startPosition = getTilePosition({ (height == 0 ? i : (i - height) % width), height });
			break;
		case 'e':
			tempTiles.push_back(END);
			break;
		case ' ':
			tempTiles.push_back(EMPTY);
			break;
		case '+':
			tempTiles.push_back(WAYPOINT);
			break;
		case 'n':
			tempTiles.push_back(ENEMY);
			break;
		case '\n':
			if (width == 0)
				width = i;
			height++;
			break;
		default:
			cout << "Unknown tile type : " << c << endl;
			break;
		}
	}

	if (tempTiles.size() != width * height)
		throw string("An error occured while parsing level file");

	LevelSystem::width = width;
	LevelSystem::height = height;
	tiles = make_unique<Tile[]>(LevelSystem::width * LevelSystem::height);

	copy(tempTiles.begin(), tempTiles.end(), &tiles[0]);

	cout << "Level file " << filepath << " loaded. Level size : " << width << 'x' << height << endl;

	buildSprites();
}

void LevelSystem::buildSprites() {
	sprites.clear();
	for (size_t y = 0; y < height; y++)
		for (size_t x = 0; x < width; x++) {
			auto shape = make_unique<RectangleShape>();
			shape->setSize({ tileSize, tileSize });
			shape->setOrigin(tileSize * .5f, tileSize * .5f);
			shape->setPosition(getTilePosition({ x, y }));
			shape->setFillColor(getColor(getTile({ x, y })));
			sprites.emplace_back(move(shape));
		}
}

Vector2f LevelSystem::getTilePosition(Vector2ul position) {
	return Vector2f(float(position.x), float(position.y)) * tileSize + offset;
}

LevelSystem::Tile LevelSystem::getTile(Vector2ul position) {
	if (position.x > width || position.y > height)
		throw "Tile out of range : " + to_string(position.x) + ", " + to_string(position.y);
	return tiles[position.y * width + position.x];
}

LevelSystem::Tile LevelSystem::getTileAt(Vector2f position) {
	auto a = position - offset;
	if (a.x < 0 || a.y < 0)
		throw "Tile out of range";

	return getTile(Vector2ul(position / tileSize));
}

void LevelSystem::render(RenderWindow& window) {
	for (size_t i = 0; i < width * height; i++) {
		try {
			window.draw(*sprites[i]);
		}
		catch (...) {
			cout << "exception while drawing sprite" << endl;
		}
	}
}

void LevelSystem::setOffset(Vector2f offset) {
	LevelSystem::offset = offset;
}

Vector2f LevelSystem::getStartPosition() {
	return startPosition;
}

size_t LevelSystem::getWidth() {
	return width;
}

size_t LevelSystem::getHeight() {
	return height;
}

float LevelSystem::getTileSize() {
	return tileSize;
}

std::vector<Vector2ul> LevelSystem::findTiles(Tile tile) {
	vector<Vector2ul> positions;
	for (auto i = 0; i < width * height; i++) {
		if (tiles[i] == tile)
			positions.emplace_back(i % width, i / width);
	}

	return positions;
}