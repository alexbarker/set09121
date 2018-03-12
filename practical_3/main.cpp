#include <SFML/Graphics.hpp>
#include <iostream>
#include "player.h"
#include "levelsystem.h"

using namespace sf;
using namespace std;

Player *player;

void Load() {
	player = new Player();

	ls::loadLevelFile("res/maze_2.txt");
	for (size_t y = 0; y < ls::getHeight(); ++y) {
		for (size_t x = 0; x < ls::getWidth(); ++x) {
			cout << ls::getTile({ x, y });
		}
		cout << endl;
	}
}

void Update(RenderWindow &window) {
	// recalculate delta time
	static Clock clock;
	float dt = clock.restart().asSeconds();
	//check and consume events
	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::Closed) {
			window.close();
			return;
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		window.close();
	}

	player->Update(dt);	
}

void Render(RenderWindow &window) {
	ls::Render(window);
	player->Render(window);
}

  int main() {
	RenderWindow window(VideoMode(800, 600), "Tile Engine");
	
	Load();

	while (window.isOpen()) {
		window.clear();
		Update(window);
		Render(window);
		window.display();
	}

    return 0;
  }