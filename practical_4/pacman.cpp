#include "pacman.h"
#include "ecm.h"
#include <SFML/Graphics.hpp>
#include "system_renderer.h"
#include <iostream>
#include "cmp_sprite.h"
#include "cmp_actor_movement.h"
#include "LevelSystem.h"
#include "cmp_pickup.h"
#include <sstream>

using namespace std;
using namespace sf;

Font font;
#define GHOSTS_COUNT 4

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
	text.setCharacterSize(72);
	text.setPosition(65, 265);
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

vector<shared_ptr<Entity>> ghosts;
shared_ptr<Entity> player;
vector<shared_ptr<Entity>> nibbles;
vector<shared_ptr<Entity>> npcs;

std::shared_ptr<Entity> GameScene::makeNibble(sf::Vector2f& nl, sf::Color c, float size, int pts) {
	auto cherry = make_shared<Entity>();
	cherry->setPosition(nl);

	auto s = cherry->addComponent<ShapeComponent>();
	s->setShape<sf::CircleShape>(size);
	s->getShape().setFillColor(c);
	s->getShape().setOrigin(size, size);

	auto ps = cherry->addComponent<PickUpComponent>();
	ps->setEntities(npcs);
	ps->setPoints(pts);

	return cherry;
}

void GameScene::Respawn() {
	for (auto n : nibbles) {
		n->setForDelete();
		n.reset();

	}
	nibbles.clear();
	player->setPoints(0);
	//player->getPoints();

	_ents.list[0]->setPosition(ls::findTiles(ls::START)[0]);

	auto ghost_spawns = ls::findTiles(ls::ENEMY);
	for (int i = 1; i < _ents.list.size(); ++i) {
		_ents.list[i]->setPosition(ghost_spawns[rand() % ghost_spawns.size()]);
	}

	auto nibbleLoc = ls::findTiles(ls::EMPTY);
	for (const auto &nl : nibbleLoc) {
		auto loc = static_cast<sf::Vector2f>(nl);
		auto cherry = makeNibble(loc, Color::Red, 2, 10);
		_ents.list.push_back(cherry);
		nibbles.push_back(cherry);
	}

	nibbleLoc = ls::findTiles(ls::WAYPOINT);
	for (const auto &nl : nibbleLoc) {
		auto loc = static_cast<sf::Vector2f>(nl);
		auto cherry = makeNibble(loc, Color::White, 5, 30);
		_ents.list.push_back(cherry);
		nibbles.push_back(cherry);
	}
}

Text textPoints;
Text textTime;
//sf::Clock scoreClock;
//float pastTime = 0.0f;
//
void GameScene::Update(double dt) {
	//pastTime = scoreClock.getElapsedTime().asSeconds();
	//Time timerText = scoreClock.getElapsedTime();
	//timerText.setString(ss.str().c_str());
	
	//float timer = scoreClock.getElapsedTime().asSeconds();
	textPoints.setString("Points: " + std::to_string(player->getPoints()));
	textTime.setString("Time: ");
	textTime.setCharacterSize(22);
	textTime.setPosition(265, 0);

	
	//timerText.setString(scoreClock.getElapsedTime().asSeconds());
	//textClock.setString(time);
	//textClock.setCharacterSize(22);
	//textClock.setPosition(0, 265);
	//textClock.setFont(font);

	if (Keyboard::isKeyPressed(Keyboard::Tab)) {
		activeScene = menuScene;
	}

	for (auto &g : ghosts) {
		if (length(g->getPosition() - player->getPosition()) < 30.0f) {
			std::cout << "Game over!" << std::endl;
			player->setPoints(0);
			//player->getPoints();
			Respawn();
		}
	}

	_ents.Update(dt);
	Scene::Update(dt);
}

void GameScene::Render() {
	ls::Render(Renderer::getWindow());
	
	Renderer::queue(&textPoints);
	Renderer::queue(&textTime);

	_ents.Render();	
	Scene::Render();
}

void GameScene::Load() {	
	textPoints.setString("Points: ");
	textPoints.setCharacterSize(22);
	textPoints.setFont(font);
	textTime.setString("Time: ");
	textTime.setCharacterSize(22);
	textTime.setFont(font);

	ls::loadLevelFile("res/pacman.txt", 25.0f);
	
	auto pl = make_shared<Entity>();
	auto mp = pl->addComponent<PlayerMovementComponent>();
	mp->setSpeed(100.0f);

	auto s = pl->addComponent<ShapeComponent>();
	s->setShape<sf::CircleShape>(10.0f);
	s->getShape().setFillColor(Color::Yellow);
	s->getShape().setOrigin(10.0f, 10.0f);
	_ents.list.push_back(pl);
	npcs.push_back(pl);
	player = pl;

	const sf::Color ghost_cols[]{
		{208, 62, 25},
		{219, 133, 28},
		{70, 191, 238},
		{234, 130, 229} };

	Vector2f pos = Vector2f{ 300.0f, 300.0f };
	for (int i = 0; i < GHOSTS_COUNT; ++i) {
		auto ghost = make_shared<Entity>();
		ghost->addComponent<EnemyAIComponent>();

		auto s = ghost->addComponent<ShapeComponent>();
		s->setShape<sf::CircleShape>(10.0f);
		s->getShape().setFillColor(ghost_cols[i % 4]);
		s->getShape().setOrigin(Vector2f(10.f, 10.f));

		_ents.list.push_back(ghost);
		ghosts.push_back(ghost);
		npcs.push_back(ghost);
		pos += Vector2f(70.0f, 0);
	}

	Respawn();
}

