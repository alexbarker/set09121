#include <SFML/Graphics.hpp>


int main(){
	sf::RenderWindow window(sf::VideoMode(200,200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	
	while (window.isOpen()){
		sf::Event event;
		while (window.pollEvent(event)){
		if (event.type == sf::Event::Closed){
			window.close();
			}
			}
			window.clear();
			window.draw(shape);
			window.display();
			}
			return 0;
			}


using namespace sf;
using namespace std;

const Keyboard::Key controls[4] = {
		Keyboard::A,		// Player1 UP
		Keyboard::Z,		// Player1 Down
		Keyboard::Up,		// Player2 UP
		Keyboard::Down		// Player2 Down
};
const Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;

CircleShape ball;
RectangleShape paddles[2];

void Load(){
	//Set size and origin of paddles
	for(auto &p : paddles) {
		p.setSize(paddleSize - Vector2f(3, 3));
		p.setOrigin(paddleSize / 2.f);
	}
	// Set size and origin of ball
	ball.setRadius(ballRadius - 3);
	ball.setOrigin(ballRadius / 2, ballRadius / 2);
	//reset paddle position
	paddles[0].setPosition(10 + paddleSize.x / 2, gameHeight / 2);
	paddles[1].setPosition(100 + paddleSize.x / 2, gameHeight / 2);
	// reset Ball Position
	ball.setPosition(ballRadius / 2, ballRadius / 2);
}

