#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>

using namespace std;
using namespace sf;

void MenuScene::Load() {
  {
    auto txt = makeEntity();
    auto t = txt->addComponent<TextComponent>(
        "Movement demos\nPress 1 for steering\nPress 2 for pathfinding");
  }
  setLoaded(true);
}

void MenuScene::Update(const double& dt) {
  if (sf::Keyboard::isKeyPressed(Keyboard::Num1)) {
	  // New line
    Engine::ChangeScene(&steeringScene);
  }
  else if (sf::Keyboard::isKeyPressed(Keyboard::Num2)) {
	  Engine::ChangeScene(&pathfindingScene);
  }

  Scene::Update(dt);
}
