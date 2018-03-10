// remove console window in release build
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <SFML/Graphics.hpp>
#include <iostream>
#include "main.h"
#include "player.h"
#include "levelsystem.h"

using namespace sf;
using namespace std;

Font font;

Player* player;

float currentTime = 0.f;
float previousTime = 0.f;
float bestTime = 0.f;

Text currentTimeText;
Text previousTimeText;

string currentTimeLabel = "Current time : ";
string previousTimeLabel = "Previous time : ";
string bestTimeLabel = "Best time : ";

bool isGameOver = false;

void load() {
	font.loadFromFile("res/fonts/RobotoMono-Regular.ttf");
	player = new Player();

	currentTimeText.setFont(font);
	currentTimeText.setCharacterSize(24);
	currentTimeText.setFillColor(Color::Black);

	previousTimeText.setFont(font);
	previousTimeText.setCharacterSize(24);
	previousTimeText.setFillColor(Color::Black);
	previousTimeText.setPosition(0.f, currentTimeText.getCharacterSize());
}

void reset(RenderWindow &window) {
	isGameOver = false;

	previousTime = currentTime;
	if (bestTime > currentTime || bestTime == 0)
		bestTime = currentTime;
	currentTime = 0.f;

	previousTimeText.setString(previousTimeLabel + to_string(previousTime) + '\n' + bestTimeLabel + to_string(bestTime));

	ls::setOffset({ tileSize * .5f, tileSize * .5f });
	ls::loadLevelFile("res/maze_2.txt", tileSize);
	window.create(VideoMode(float(ls::getWidth()) * ls::getTileSize(), float(ls::getHeight()) * ls::getTileSize()), "Maze Game", Style::Titlebar | Style::Close);
	cout << "start position : " << ls::getStartPosition();
	player->setPosition(ls::getStartPosition());
}

void update(RenderWindow &window) {
	static Clock clock;
	const float dt = clock.restart().asSeconds();

	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::Closed)
			window.close();
	}

	if (Keyboard::isKeyPressed(Keyboard::Escape))
		window.close();

	if (!isGameOver) {
		currentTime += dt;
		currentTimeText.setString(currentTimeLabel + to_string(currentTime));
		player->update(dt);
	}
	else
		if (Keyboard::isKeyPressed(Keyboard::Return))
			reset(window);
}

void render(RenderWindow &window) {
	try {
		ls::render(window);
		player->render(window);
		window.draw(currentTimeText);
		window.draw(previousTimeText);
	}
	catch (...) {
		cout << "exception while rendering" << endl;
	}
}

int main() {
	RenderWindow window;

	load();
	reset(window);

	while (window.isOpen()) {
		window.clear();
		update(window);
		render(window);
		window.display();
	}

	return 0;
}

void gameOver() {
	isGameOver = true;
}