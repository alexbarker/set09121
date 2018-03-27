#include "scene_pathfinding.h"

#include <LevelSystem.h>
#include "../components/cmp_sprite.h"
#include "../astar.h"
#include "../components/cmp_path_follow.h"

using namespace std;
using namespace sf;

shared_ptr<Entity> character;
shared_ptr<PathfinidngComponent> ai;

void PathfindingScene::Load()
{
	ls::loadLevelFile("res/pacman.txt", 20.0f);

	character = makeEntity();
	Vector2f startPos(ls::getOffset().x + 1 * ls::getTileSize(), ls::getOffset().y + 1 * ls::getTileSize());
	character->setPosition(startPos);
	auto s = character->addComponent<ShapeComponent>();
	s->setShape<CircleShape>(10.0f);
	s->getShape().setFillColor(Color::Red);

	auto path = pathFind(Vector2i(1, 1), Vector2i(ls::getWidth() - 2, ls::getHeight() - 2));
	ai = character->addComponent<PathfinidngComponent>();
	ai->setPath(path);
}

void PathfindingScene::UnLoad() { Scene::UnLoad(); }

void PathfindingScene::Update(const double& dt)
{
	// Click for new destination
	static bool mouse_down = false;
	if (Mouse::isButtonPressed(Mouse::Left) && !mouse_down)
	{
		auto mouse_pos = Mouse::getPosition(Engine::GetWindow());
		mouse_down = true;
		if (ls::isOnGrid(Vector2f(mouse_pos)))
		{
			auto relative_pos = mouse_pos - Vector2i(ls::getOffset());
			auto tile_coord = relative_pos / (int)ls::getTileSize();
			if (ls::getTile(Vector2ul(tile_coord)) != ls::WALL)
			{
				auto char_relative = character->getPosition() - ls::getOffset();
				auto char_tile = Vector2i(char_relative / ls::getTileSize());
				auto path = pathFind(char_tile, tile_coord);
				ai->setPath(path);
			}
		}
	}
	if (mouse_down && !Mouse::isButtonPressed(Mouse::Left))
	{
		mouse_down = false;
	}

	Scene::Update(dt);
}

void PathfindingScene::Render()
{
	ls::render(Engine::GetWindow());
	Scene::Render();
}