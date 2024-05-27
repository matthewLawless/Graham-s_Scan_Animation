#include <iostream>
#include "SFML/Graphics.hpp"
#include <vector> 
#include <fstream> 
#include <string>
#include <sstream>


bool compareVertices(sf::Vertex p1, sf::Vertex p2) {

	if (p1.position.x > p2.position.x) { return false; } 
	else { return true; }

}

int main(int argc, char* argv[]) {

	int width;
	int height;
	int scale = 10;

	std::cout << argc << std::endl;
	std::cout << argv[1] << std::endl;

	std::vector<sf::Vertex> pointList;

	std::ifstream inputFile(argv[1]);

	std::string line;

	std::stringstream curLine;

	std::string currentToken;

	int yMax = INT_MIN, xMax = INT_MIN;
	int yMin = INT_MAX, xMin = INT_MAX;

	while (getline(inputFile, line, '\n')) {

		curLine << line;
		std::cout << "curline: " << curLine.str() << std::endl;

		while (curLine.peek() != EOF) {

			std::getline(curLine, currentToken, ' ');
			int x, y;
			std::string xString = currentToken.substr(1, currentToken.size() - 2);
			x = stoi(xString);
			std::getline(curLine, currentToken, ' ');
			std::string yString = currentToken.substr(0, currentToken.size() - 1);
			y = stoi(yString);
			std::cout << "(" << x << ", " << y << ")" << std::endl;

			if (x > xMax) {
				xMax = x;
			}

			if (x < xMin) {
				xMin = x;
			}

			if (y > yMax) {
				yMax = y;
			}

			if (y < yMin) {

				yMin = y;

			}

			sf::Vertex point(sf::Vector2f(x * scale, y * scale), sf::Color::Red);
			pointList.push_back(point);

		}

		curLine.clear();
		curLine.str(std::string());

	}


	/*
	For now we will assume we are working with all positive values so our window will just be width = xMax, height = yMax;
	*/

	width = xMax * scale + scale;
	height = yMax * scale + scale;

	sf::RenderWindow window(sf::VideoMode(width, height), "Plane", sf::Style::Fullscreen);
	sf::Event e;

	/*for (sf::Vertex v : pointList) {
	
		window.draw(&v, 1, sf::Points);
		sf::CircleShape c(5);
		c.setFillColor(sf::Color(100, 250, 50));
		c.setPosition(v.position.x, v.position.y);
		window.draw(c);
	
	}*/

	bool algoBegan = false;

	while (window.isOpen()){

		while (window.pollEvent(e)) {

			window.clear();

			if (e.type == sf::Event::Closed) {
			
				window.close();
			
			}

			for (sf::Vertex v : pointList) {

				window.draw(&v, 1, sf::Points);
				sf::CircleShape c(5);
				c.setFillColor(sf::Color(100, 250, 50));
				c.setPosition(v.position.x, v.position.y);
				window.draw(c);

			}

			if (e.type == sf::Event::KeyPressed) {

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				
					window.close();
				
				}
			
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				
					algoBegan = true;
					std::cout << "---Graham's Scan initiated---" << std::endl;



					std::cout << "---Sorting points---" << std::endl;

					std::cout << "Unsorted:" << std::endl;

					for (sf::Vertex v : pointList) {

						std::cout << "(" << v.position.x << ", " << v.position.y << "), ";

					}

					std::cout << std::endl;

					sort(pointList.begin(), pointList.end(), compareVertices);

					std::cout << "Sorted:" << std::endl;

					for (sf::Vertex v : pointList) {

						std::cout << "(" << v.position.x << ", " << v.position.y << "), ";

					}

					std::cout << std::endl;
				
				}
			
			}

			if (algoBegan == true) {
			
				/*std::cout << "---Sorting points---" << std::endl;

				std::cout << "Unsorted:" << std::endl;

				for (sf::Vertex v : pointList) {
				
					std::cout << "(" << v.position.x << ", " << v.position.y << "), ";
				
				}

				std::cout << std::endl;

				sort(pointList.begin(), pointList.end(), compareVertices);

				std::cout << "Sorted:" << std::endl;

				for (sf::Vertex v : pointList) {

					std::cout << "(" << v.position.x << ", " << v.position.y << "), ";

				}

				std::cout << std::endl;*/




			}

			

			window.display();
		}
		
	}


	return 0;


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





