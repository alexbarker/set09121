#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

const int gameWidth = 800;
const int gameHeight = 600;

//Player *player;

void Load() {

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

	//player->Update(dt);
}

void Render(RenderWindow &window) {
	//ls::Render(window);
	//player->Render(window);
}

int main() {
	RenderWindow window(VideoMode(800, 600), "Pacman");

	Load();

	while (window.isOpen()) {
		window.clear();
		Update(window);
		Render(window);
		window.display();
	}

	return 0;
}