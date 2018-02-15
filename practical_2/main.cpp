#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include "Game.h" 
#include "Ship.h"
#include "Bullet.h"
#include "Wall.h"

using namespace sf;
using namespace std;

Texture spritesheet;

vector<Ship*> ships;
Ship* player;

bool isPaused = false;
bool isOver = false;

Font font;
Text mainText;
Text startText;
Text startText1;

Text scoreText;
String scoreString = "Score : ";
Text livesText;
String livesString = "Lives : ";
Text yourScore;
String yourScoreString = "High Score : ";

RectangleShape bottomLimit;

Sprite pauseSprite;

uint16_t invaderCount;

bool invicible = false;

int playerScore = 0;
int highScore = 0;
int go = 0;

const int invaderPoints[] = { 500, 200, 100, 50, 25, 10 };

constexpr float initialAnimationTimer = 1.f;
float animationTimer = initialAnimationTimer;

// default text hinting the help button
void defaultText() {
	mainText.setCharacterSize(20);
	mainText.setString("Press P to pause.");
}

// pause text
void pauseText1() {
	mainText.setCharacterSize(64);
	mainText.setString("Pause");
}
void pauseText2() {
	mainText.setCharacterSize(22);
	mainText.setPosition(300, 280);
	mainText.setString("Press H for Help.");
}

// help text displaying controls and goal
void helpText() {
	mainText.setCharacterSize(32);
	mainText.setString(
		"Controls :\n\tLeft Arrow : Move left.\n\tRight Arrow : Move right.\n\tUp Arrow : Fire bullet.\n\tP : Pause game.\n\tH : Display help.\n\tEsc : quit game.\n");
}

void gameOverText(bool victory) {
	mainText.setCharacterSize(64);
	if (victory)
		mainText.setString("You Win");
	else
		mainText.setString("Game Over");
	//mainText.setPosition(8, 8);
	//mainText.setString("Your Score: " , playerScore);
	//highScore = playerScore;
	//playerScore = 0;
}

void startScreen(RenderWindow &window) {

	startText.setFont(font);
	startText.setOutlineColor(Color::Green);
	startText.setOutlineThickness(3.f);
	startText.setCharacterSize(48);
	startText.setPosition(200, 128);
	startText.setString("SPACE INVADERS");
	window.draw(startText);

	startText1.setFont(font);
	startText1.setOutlineColor(Color::Red);
	startText1.setOutlineThickness(1.f);
	startText1.setCharacterSize(20);
	startText1.setPosition(220, 260);
	startText1.setString("Press the Spacebar to start.");
	window.draw(startText1);

	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		go = 1;
		return;
	}

}

// load/set static stuff
void load() {
	
	// load spritesheet
	if (!spritesheet.loadFromFile("res/img/invaders_sheet.png"))
		cerr << "Could not load spritesheet :(" << endl;

	// load font
	if (!font.loadFromFile("res/fonts/RobotoMono-Regular.ttf"))
		cerr << "Could not load font :(" << endl;

	

	// set player, bullets and invader properties
	Player::speed = 140.f;
	Player::initialLives = 3;

	Bullet::speed = 300.f;

	Invader::mothershipSpeed = 100.f;
	Invader::mothershipInitialTimer = 10.f;

	// set base text properties
	mainText.setFont(font);
	mainText.setOutlineColor(Color::Black);
	mainText.setOutlineThickness(3.f);

	scoreText.setFont(font);
	scoreText.setOutlineColor(Color::Black);
	scoreText.setOutlineThickness(3.f);
	scoreText.setCharacterSize(18);
	scoreText.setString(scoreString + "0");

	yourScore.setFont(font);
	yourScore.setOutlineColor(Color::Black);
	yourScore.setOutlineThickness(3.f);
	yourScore.setCharacterSize(32);
	yourScore.setString(yourScoreString);

	livesText.setFont(font);
	livesText.setOutlineColor(Color::Black);
	livesText.setOutlineThickness(3.f);
	livesText.setCharacterSize(18);
	livesText.setString(livesString + to_string(Player::initialLives));

	// set grey bar properties
	bottomLimit.setFillColor({ 204, 204, 204 });
	bottomLimit.setSize({ float(gameWidth), 4.f });
	bottomLimit.setPosition({ 0, gameHeight - spriteHeight * 1.25f });

	// set pause sprite properties
	pauseSprite.setTexture(spritesheet);
	pauseSprite.setOrigin(spriteWidth / 2.f, spriteHeight / 2.f);
}

// reset stuff
void reset(RenderWindow &window) {
	playerScore = 0;

	// reset rand seed
	srand(time(nullptr));

	// reset game state
	isPaused = false;
	isOver = false;

	// reset invader count
	invaderCount = invaderRows * invaderCols;

	// reset invader properties
	Invader::direction = false;
	Invader::speed = 0.9f;
	Invader::bulletMinTimer = 50.f;
	Invader::mothershipDirection = false;

	// reset all bullets
	Bullet::reset();

	// reset text
	defaultText();

	// empty ship array
	vector<Ship*> empty;
	ships.swap(empty);

	// spawn invaders
	for (int row = 0; row <= invaderRows; row++) {
		auto rect = IntRect((row % 2 == 0 ? row : row - 1) * spriteWidth, 0, spriteWidth, spriteHeight);
		for (int col = 0; col < invaderCols; col++) {
			constexpr float halfSpriteWidth = spriteWidth * .5f;
			constexpr float halfSpriteHeight = spriteHeight * .5f;
			Vector2f position = {
				float(halfSpriteWidth + spriteWidth * col + halfSpriteWidth * col),
				float(halfSpriteHeight + spriteHeight * row + halfSpriteHeight * row)
			};
			auto invader = new Invader(rect, position, invaderPoints[row]);
			ships.push_back(invader);
		}
	}

	// spawn mothership
	auto mothership = new Invader(true, IntRect(spriteWidth * 6, 0, spriteWidth * 2, spriteHeight), Vector2f(gameWidth + spriteWidth, spriteHeight), 1000);
	ships.push_back(mothership);

	// spawn player
	player = new Player();
	ships.push_back(player);

	// spawn walls
	for (auto i = 0; i < 4; i++) {
		const auto wallSlotWidth = gameWidth / 4.f;
		auto wall = new Wall(Vector2f(wallSlotWidth * i + wallSlotWidth / 2.f, gameHeight - spriteHeight * 1.75f));
		ships.push_back(wall);
	}
}

// update stuff
void update(RenderWindow& window) {
	static Clock clock;
	float dt = clock.restart().asSeconds();

	// update animation timer
	animationTimer -= dt;

	// check and consume events
	Event event;
	static bool focus = true;
	while (window.pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			window.close();
			break;
			// auto pause when window loses focus
		case Event::LostFocus:
			focus = false;
			pause(true);
			break;
		case Event::GainedFocus:
			focus = true;
			break;
		default:
			break;
		}
	}

	// key interactions only when window is focused
	if (focus) {

		// quit via ESC key
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();

		// pause via P key
		static float pauseCooldown = 0.f;
		pauseCooldown -= dt;
		if (Keyboard::isKeyPressed(Keyboard::P)) {
			if (pauseCooldown <= 0) {
				pauseCooldown = 0.2f;
				if (!isOver)
					pause(!isPaused);
				// pressing the pause key on the game over screen resets the game
				else
					reset(window);
			}
		}

		// help via H key
		static float helpCooldown = 0.f;
		helpCooldown -= dt;
		if (!isOver && Keyboard::isKeyPressed(Keyboard::H))
			if (helpCooldown <= 0) {
				helpCooldown = 0.2f;
				pause(!isPaused);
				if (isPaused)
					helpText();
			}
	}

	// only update ships and bullets if game is not paused
	if (!isPaused) {
		for (auto& s : ships)
			s->update(dt);

		Bullet::update(dt);

		scoreText.setString(scoreString + to_string(playerScore));
		mainText.setPosition(8, gameHeight - 20);
		scoreText.setPosition(8, 8);
		livesText.setPosition(gameWidth - livesText.getLocalBounds().width - 8, 8);
	}
	// else update pause screen
	else {
		//yourScore.setCharacterSize(32);
		yourScore.setPosition(28, 28);
		yourScore.setString(yourScoreString + to_string(playerScore));

		mainText.setPosition(gameWidth / 2.f - mainText.getLocalBounds().width / 2.f,
			gameHeight / 2.f - mainText.getLocalBounds().height / 2.f);

		// move the pause sprite around
		static auto direction = rand() % 4;
		static const auto speed = 100.f;

		// change direction
		if (rand() % 4000 == 0)
			direction = rand() % 4;

		// move left
		if (direction == 0) {
			pauseSprite.move(speed * dt * -1, 0);
			if (pauseSprite.getPosition().x <= spriteWidth / 2.f)
				direction = 1;
		}
		// move right
		if (direction == 1) {
			pauseSprite.move(speed* dt * 1, 0);
			if (pauseSprite.getPosition().x >= gameWidth - spriteWidth / 2.f)
				direction = 0;

		}
		// move up
		if (direction == 2) {
			pauseSprite.move(0, speed * dt * -1);
			if (pauseSprite.getPosition().y <= spriteHeight / 2.f)
				direction = 3;
		}
		// move down
		if (direction == 3) {
			pauseSprite.move(0, speed * dt * 1);
			if (pauseSprite.getPosition().y >= gameHeight - spriteHeight / 2.f)
				direction = 2;
		}
	}

	// reset animation timer if necessary
	if (animationTimer <= 0)
		animationTimer = initialAnimationTimer;
}

// render stuff
void render(RenderWindow& window) {
	// render help text behind everything when not paused
	if (!isPaused) {
		// render the grey bar
		window.draw(bottomLimit);

		window.draw(mainText);
		window.draw(scoreText);
		window.draw(livesText);

		// render ships
		for (const auto s : ships)
			window.draw(*s);

		// render bullets
		Bullet::render(window);
	}
	// render pause text in front of everything when paused
	else {
		window.draw(pauseSprite);
		window.draw(mainText);
		window.draw(yourScore);
	}
}

// main game loop
int main() {
	// Create a new un-resizable window
	RenderWindow window(VideoMode(gameWidth, gameHeight), "Space Invaders", Style::Titlebar | Style::Close);
	
	

	load();
	
	reset(window);
	
	while (window.isOpen() && go == 0) {
		window.clear();
		startScreen(window);
		window.display();

	}

	while (window.isOpen() && go == 1) {
		window.clear();
		update(window);
		render(window);
		window.display();
		
	}
	return 0;
}

// triggers the game over
void gameOver(bool victory) {
	pause(true);
	isOver = true;
	gameOverText(victory);
}

// sets the pause state of the game based on the given bool
void pause(bool pause) {
	isPaused = pause;
	if (isPaused) {
		pauseText1();
		pauseText2();
		// randomise pause sprite
		pauseSprite.setTextureRect({ spriteWidth * (rand() % 6), 0, spriteWidth, spriteHeight });
		pauseSprite.setPosition(rand() % gameWidth, rand() % gameHeight);
	}
	else
		defaultText();
}