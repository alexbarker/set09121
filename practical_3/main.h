#pragma once
#include <SFML/Graphics.hpp>
#include "player.h"

// window settings

constexpr int gameWidth = 800;
constexpr int gameHeight = 600;

// map settings
constexpr float tileSize = 100.f;

// player settings

constexpr float playerSpeed = 200.f;
constexpr float playerSize = 48.f;
const sf::Color playerColor = sf::Color::Magenta;

// controls settings

const sf::Keyboard::Key playerUp = sf::Keyboard::Z;
const sf::Keyboard::Key playerLeft = sf::Keyboard::Q;
const sf::Keyboard::Key playerDown = sf::Keyboard::S;
const sf::Keyboard::Key playerRight = sf::Keyboard::D;

// global functions
void gameOver();