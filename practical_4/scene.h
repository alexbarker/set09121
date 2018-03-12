#pragma once

#include "entity.h"

class Scene {
public:
	Scene() = default;

	virtual ~Scene() = default;
	virtual void Update(double dt);
	virtual void Render(sf::RenderWindow &window);
	virtual void Load() = 0;
	std::vector<std::shared_ptr<Entity>> &getEnts();

protected:
	EntityManager _ents;
};