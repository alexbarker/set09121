#include "scene_decision.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_basic_movement.h"
#include "../components/cmp_state_machine.h"
#include "../steering_states.h"
#include "../steering_decisions.h"
#include <LevelSystem.h>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace sf;

void DecisionScene::Load()
{
	auto player = makeEntity();
	player->addTag("player");
	player->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, Engine::GetWindow().getSize().y / 2));
	auto s = player->addComponent<ShapeComponent>();
	s->setShape<CircleShape>(10.0f);
	s->getShape().setFillColor(Color::White);
	player->addComponent<BasicMovementComponent>();

	random_device dev;
	default_random_engine engine(dev());
	uniform_real_distribution<float> x_dist(0, Engine::GetWindow().getSize().x);
	uniform_real_distribution<float> y_dist(0, Engine::GetWindow().getSize().y);

	for (size_t n = 0; n < 100; ++n)
	{
		auto enemy = makeEntity();
		enemy->setPosition(Vector2f(x_dist(engine), y_dist(engine)));
		auto s = enemy->addComponent<ShapeComponent>();
		s->setShape<CircleShape>(10.0f);
		s->getShape().setFillColor(Color::Blue);

		auto sm = enemy->addComponent<StateMachineComponent>();
		sm->addState("stationary", make_shared<StationaryState>());
		sm->addState("seek", make_shared<SeekState>(enemy, player));
		sm->addState("flee", make_shared<FleeState>(enemy, player));

		auto decision = make_shared<DistanceDecision>(
			player,
			50.0f,
			make_shared<FleeDecision>(),
			make_shared<DistanceDecision>(
				player,
				100.0f,
				make_shared<RandomDecision>(
					make_shared<SeekDecision>(),
					make_shared<StationaryDecision>()),
				make_shared<SeekDecision>()
				)
			);

		enemy->addComponent<DecisionTreeComponent>(decision);
	}
}

void DecisionScene::UnLoad() { Scene::UnLoad(); }

void DecisionScene::Update(const double& dt) { Scene::Update(dt); }

void DecisionScene::Render() { Scene::Render(); }