#pragma once
#include <SFML/Graphics.hpp>
#include "Ship.h"

constexpr uint16_t gameWidth = 800;
constexpr uint16_t gameHeight = 600;
constexpr uint16_t invaderRows = 4;
constexpr uint16_t invaderCols = 10;
extern uint16_t invaderCount;

extern sf::Texture spritesheet;
constexpr uint16_t spriteWidth = 32;
constexpr uint16_t spriteHeight = 32;

extern std::vector<Ship*> ships;

const sf::Keyboard::Key playerLeft = sf::Keyboard::Left;
const sf::Keyboard::Key playerRight = sf::Keyboard::Right;
const sf::Keyboard::Key playerShoot = sf::Keyboard::Up;

extern Ship* player;

extern bool invicible;

extern int playerScore;
extern sf::Text livesText;
extern sf::String livesString;

extern float animationTimer;

void gameOver(bool);
void pause(bool);