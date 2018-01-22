#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

const Keyboard::Key controls[4] = {
	Keyboard::A,   // Player1 UP
	Keyboard::Z,   // Player1 DOWN
	Keyboard::Up,  // Player2 UP
	Keyboard::Down // Player2 DOWN
};

const Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;

CircleShape ball;
RectangleShape paddles[2];

Vector2f ballVelocity;
bool server = false;

Font font;
Text scoreText;
int score = 0;
bool two_player = false;

void reset() {
	// reset paddle position
	paddles[0].setPosition(10 + paddleSize.x / 2, gameHeight / 2);
	paddles[1].setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
	// reset ball position
	ball.setPosition(gameWidth / 2, gameHeight / 2);
	ballVelocity = { server ? 100.0f : -100.0f, 60.0f };
	// Update score text
	scoreText.setString(to_string(score));
	scoreText.setPosition((gameWidth * 0.5f) - (scoreText.getLocalBounds().width * 0.5f), 0);
}

void Load() {
	// Set size and origin of paddles
	for (auto &p : paddles) {
		p.setSize(paddleSize - Vector2f(3, 3));
		p.setOrigin(paddleSize / 2.f);
	}
	// Set size and origin of ball
	ball.setRadius(ballRadius - 3);
	ball.setOrigin(ballRadius / 2, ballRadius / 2);
	// Load font-face from res dir
	font.loadFromFile("RobotoMono-Regular.ttf");
	// Set text element to use font
	scoreText.setFont(font);
	// set the character size to 24 pixels  
	scoreText.setCharacterSize(24);
	reset();
}

void Update(RenderWindow &window) {
	// Reset clock, recalculate deltatime
	static Clock clock;
	float dt = clock.restart().asSeconds();
	// check and consume events
	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::Closed) {
			window.close();
			return;
		}
	}

	// Quit via ESC key
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		window.close();
	}

	// handle paddle movement
	float direction = 0.0f;
	if (Keyboard::isKeyPressed(controls[0])) {
		direction--;
	}
	if (Keyboard::isKeyPressed(controls[1])) {
		direction++;
	}
	paddles[0].move(0, direction * paddleSpeed * dt);

	// start
	if (paddles[0].getPosition().y - paddleSize.y / 2 < 0) {
		paddles[0].move(0, 2);
	}
	else if (paddles[0].getPosition().y + paddleSize.y / 2> gameHeight) {
		paddles[0].move(0, -2);
	}

	direction = 0.0f;
	if (Keyboard::isKeyPressed(controls[2])) {
		two_player = true;
		direction--;
	}
	if (Keyboard::isKeyPressed(controls[3])) {
		two_player = true;
		direction++;
	}
	paddles[1].move(0, direction * paddleSpeed * dt);

	if (!two_player) {
		if (paddles[1].getPosition().y < ball.getPosition().y) {
			direction++;
		}
		else if (paddles[1].getPosition().y > ball.getPosition().y) {
			direction--;
		}
		paddles[1].move(0, direction * paddleSpeed * dt);
		if (paddles[1].getPosition().y - paddleSize.y / 2 < 0) {
			paddles[1].move(0, 2);
		}
		else if (paddles[1].getPosition().y + paddleSize.y / 2 > gameHeight) {
			paddles[1].move(0, -2);
		}
	}
	ball.move(ballVelocity * dt);

	// check ball collision
	const float bx = ball.getPosition().x;
	const float by = ball.getPosition().y;
	if (by > gameHeight) {
		// bottom wall
		ballVelocity.x *= 1.1f;
		ballVelocity.y *= -1.1f;
		ball.move(0, -10);
	}
	else if (by < 0) {
		// top wall
		ballVelocity.x *= 1.1f;
		ballVelocity.y *= -1.1f;
		ball.move(0, 10);
	}
	else if (bx > gameWidth) {
		// right wall
		score++;
		reset();
	}
	else if (bx < 0) {
		// left wall
		score--;
		reset();
	}
	else if (
		// ball is inline or behind paddle
		bx < paddleSize.x + 10 &&
		// AND ball is below top edge of paddle
		by > paddles[0].getPosition().y - (paddleSize.y * 0.5) &&
		// AND ball is above bottom edge of paddle
		by < paddles[0].getPosition().y + (paddleSize.y * 0.5)
		) {
		// bounce off left paddle
		ballVelocity.x *= -1.1f;
		ballVelocity.y *= 1.1f;
		ball.move(10, 0);
	}
	else if (
		// ball is inline or behind paddle
		bx > gameWidth - paddleSize.x - 10 &&
		// AND ball is below top edge of paddle
		by > paddles[1].getPosition().y - (paddleSize.y * 0.5) &&
		// AND ball is above bottom edge of paddle
		by < paddles[1].getPosition().y + (paddleSize.y * 0.5)
		) {
		// bounce off right paddle
		ballVelocity.x *= -1.1f;
		ballVelocity.y *= 1.1f;
		ball.move(-10, 0);
	}
}

void Render(RenderWindow &window) {
	// Draw everything
	window.draw(paddles[0]);
	window.draw(paddles[1]);
	window.draw(ball);
	window.draw(scoreText);
}

int main() {
	RenderWindow window(VideoMode(gameWidth, gameHeight), "PONG");
	Load();

	while (window.isOpen()) {
		window.clear();
		Update(window);
		Render(window);
		window.display();
	}
	return 0;
}