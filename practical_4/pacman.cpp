//#include "player.h"
//#include "ghost.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "system_renderer.h"
#include "pacman.h"
#include "ecm.h"
#include "cmp_sprite.h"

using namespace std;
using namespace sf;

#define GHOSTS_COUNT 4
Font font;

MenuScene::MenuScene() {
}

void MenuScene::Load() {
	if (!font.loadFromFile("res/fonts/RobotoMono-Regular.ttf")) {
		cout << "Cannot load font!" << endl;
	}

	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		activeScene = gameScene;
	}
	text.setString("Almost Pacman");
	text.setFont(font);
	text.setCharacterSize(82);
	text.setPosition(90, 220);
	text.setFillColor(Color::Yellow);
	text.setStyle(Text::Bold);
	text.setOutlineThickness(2.0f);
	text.setOutlineColor(Color::Yellow);
}

void MenuScene::Update(double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		activeScene = gameScene;
	}

	Scene::Update(dt);
	_ents.Update(dt);
}

void MenuScene::Render() {
	Scene::Render();
	_ents.Render();
	Renderer::queue(&text);
}

void GameScene::Update(double dt) {
	if (Keyboard::isKeyPressed(Keyboard::Tab)) {
		activeScene = menuScene;
	}
	Scene::Update(dt);
	_ents.Update(dt);
}

void GameScene::Render() {
	Scene::Render();
	_ents.Render();
}

void GameScene::Load() {
	auto pl = make_shared<Entity>();
	auto s = pl->addComponent<ShapeComponent>();
	s->setShape<sf::CircleShape>(12.f);
	s->getShape().setFillColor(Color::Yellow);
	s->getShape().setOrigin(Vector2f(12.f, 12.f));
	_ents.list.push_back(pl);

	const sf::Color ghost_cols[]{{ 208, 62, 25 },		// red Blinky
								 { 219, 133, 28 },		// orange Clyde
								 { 70, 191, 238 },		// cyan Inky
								 { 234, 130, 229 } };	// pink Pinky

	Vector2f pos = Vector2f{ 300.0f, 300.0f };
	for (int i = 0; i < GHOSTS_COUNT; ++i) {
		auto ghost = make_shared<Entity>();

		auto s = ghost->addComponent<ShapeComponent>();
		s->setShape<sf::CircleShape>(12.f);
		s->getShape().setFillColor(ghost_cols[i % 4]);
		s->getShape().setOrigin(Vector2f(12.f, 12.f));

		_ents.list.push_back(ghost);
		pos += Vector2f(70.0f, 0);
	}
}