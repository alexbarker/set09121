#include "cmp_actor_movement.h"
#include "levelsystem.h"

using namespace sf;

void ActorMovementComponent::Render() {}

void ActorMovementComponent::Update(double dt) {}

ActorMovementComponent::ActorMovementComponent(Entity *p) : _speed(100.0f), Component(p) {}

bool ActorMovementComponent::validMove(const sf::Vector2f &pos) {
	return (LevelSystem::getTileAt(pos) != LevelSystem::WALL);
}

void ActorMovementComponent::move(const Vector2f &p) {
	auto pp = _parent->getPosition() + p;
	if (validMove(pp)) {
		_parent->setPosition(pp);
	}
}

void ActorMovementComponent::move(float x, float y) {
	move(Vector2f(x, y));
}

float ActorMovementComponent::getSpeed() const {
	return _speed;
}

void ActorMovementComponent::setSpeed(float speed) {
	_speed = speed;
}

void ActorMovementComponent::increaseSpeed(float speed) {
	_speed += speed;
}

PlayerMovementComponent::PlayerMovementComponent(Entity *p) : ActorMovementComponent(p) {}

void PlayerMovementComponent::Update(double dt) {
	int xdir = 0, ydir = 0;
	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		move(Vector2f(0, -_speed * dt));
	}
	if (Keyboard::isKeyPressed(Keyboard::Down)) {
		move(Vector2f(0, _speed * dt));
	}
	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		move(Vector2f(-_speed * dt, 0));
	}
	if (Keyboard::isKeyPressed(Keyboard::Right)) {
		move(Vector2f(_speed * dt, 0));
	}

	ActorMovementComponent::Update(dt);
}

void PlayerMovementComponent::Render() {
	ActorMovementComponent::Render();
}

static const Vector2i directions[] = { Vector2i{ 1, 0 }, Vector2i{ 0, 1 }, Vector2i{ 0, -1 }, Vector2i{ -1, 0 } };
EnemyAIComponent::EnemyAIComponent(Entity * p) : ActorMovementComponent(p) {
	_state = ROAMING;
	_speed = 100.0f;
	_direction = Vector2f(directions[rand() % 4]);
}

//ENEMY COMPONENT
void EnemyAIComponent::Update(double dt) {
	//amount to move
	const auto mva = (float)(dt * _speed);
	//current position
	const Vector2f pos = _parent->getPosition();
	//next pos
	const Vector2f newPos = pos + _direction * mva;
	//inverse of current pos
	const Vector2i badDir = -1 * Vector2i(_direction);
	//random new direction
	Vector2i newDir = directions[(rand() % 4)];

	switch (_state)
	{
	case EnemyAIComponent::ROAMING:
		if (ls::getTileAt(newPos) == ls::WALL || ls::getTileAt(pos) == ls::WAYPOINT) { //if wall in front or at waypoint
			_state = ROTATING; //start rotating
		}
		else {
			move(_direction * mva); //keep moving
		}
		break;
	case EnemyAIComponent::ROTATING:
		//don't reverse and don't pick a direction which leads to a wall
		//std::cout << badDir << ";" << _direction << std::endl;
		while (newDir == badDir || ls::getTileAt(pos + Vector2f(newDir) * mva) == ls::WALL) {
			newDir = directions[rand() % 4]; 		//pick a new direction
			//std::cout << "WHILE LOOP: " << _state << newDir << badDir << std::endl;
		}
		_direction = Vector2f(newDir);
		_state = ROTATED;
		break;
	case EnemyAIComponent::ROTATED:
		//has it left the waypoint?
		if (ls::getTileAt(newPos) != ls::WAYPOINT) {
			_state = ROAMING; //yes, roam
		}
		move(_direction * mva); //No
		break;
	default:
		break;
	}

	ActorMovementComponent::Update(dt);
}

void EnemyAIComponent::Render() {
	ActorMovementComponent::Render();
}
