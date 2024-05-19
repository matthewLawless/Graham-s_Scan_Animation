#include <iostream>
#include "SFML/Graphics.hpp"
#include <vector>  

int main() {

	sf::RenderWindow window(sf::VideoMode(400, 400), "Test");
	sf::Event e;

	sf::Vertex point(sf::Vector2f(400, 400), sf::Color::Blue);
	window.draw(&point, 5, sf::Points);
	sf::CircleShape shape(50);
	shape.setFillColor(sf::Color(100, 250, 50));
	shape.setPosition(100, 100);
	window.draw(shape);

	window.display();

	int x = 100;
	int y = 100;

	bool l = false, r = false,  u = false, d = false;


	while (window.isOpen()) {

	
		while (window.pollEvent(e)) {

			if (e.type == sf::Event::Closed) {
			
				window.close();

			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				// left key is pressed: move our character
				x--;
				shape.setPosition(x, y);
				window.draw(shape);
				window.display();

			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				// left key is pressed: move our character
				x++;
				shape.setPosition(x, y);
				window.draw(shape);
				window.display();
			}



		}
	
	}
	return 0;
		
}



