#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"

using namespace std;

MenuScene menu;
StateScene stateScene;
DecisionScene decisionScene;

int main() {
	Engine::Start(1280, 720, "Decision", &menu);
}