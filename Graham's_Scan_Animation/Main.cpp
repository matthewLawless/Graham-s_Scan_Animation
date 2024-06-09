#include <iostream>
#include "SFML/Graphics.hpp"
#include <vector> 
#include <fstream> 
#include <string>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <windows.h>



bool compareVertices(sf::Vertex p1, sf::Vertex p2) {

	/*if (p1.position.x > p2.position.x) { return false; } 
	else { return true; }*/
	return (p1.position.x < p2.position.x);

}

bool isMouseHovering(sf::Vertex v, sf::CircleShape c, sf::Vector2i mousePosition) {

	double radius = c.getRadius();
	//Find distance between center points (v) and the current mouse position
	double distance = sqrt(pow((v.position.x - mousePosition.x), 2) + pow((v.position.y - mousePosition.y), 2));
	//Check if distance is less than radius
	if (distance < radius) {
	
		return true;
	
	}
	return false;

};

void drawPointsToWindow(std::vector<sf::Vertex> pointList, sf::RenderWindow *window) {

	for (sf::Vertex v : pointList) {


		//Draw each point and a circle around it

		window->draw(&v, 1, sf::Points);
		sf::CircleShape c(5);
		c.setFillColor(sf::Color(100, 250, 50));
		c.setPosition(v.position.x, v.position.y);
		c.setOrigin(c.getRadius(), c.getRadius());
		window->draw(c);

		//check to see if mouse is hovering overany point(s)
		if (isMouseHovering(v, c, sf::Mouse::getPosition(*window))) {

			//std::cout << "Hovering" << std::endl;
			///*sf::Text text;
			//text.setFont(font);
			//std::string s = "Point: ("
			//text.setString(s);*/
			std::cout << "(" << v.position.x << ", " << v.position.y << ")" << std::endl;


		}

	}


}

void drawUpperHullToWindow(std::vector<sf::Vertex> Lupper, sf::RenderWindow* window) {

	for (int i = 0; i < Lupper.size() - 1; i++) {

		sf::Vertex vList2[2] =
		{
			sf::Vertex(Lupper[i]),
			sf::Vertex(Lupper[i + 1])

		};

		vList2[0].color = sf::Color::Blue;
		vList2[1].color = sf::Color::Blue;

		window->draw(vList2, 2, sf::Lines);

	}
	

}

void populateFileWithRandomPointset(std::string filename, int numberOfPoints) {

	std::ofstream file(filename);

	srand((unsigned)time(NULL));
	for (int i = 0; i < numberOfPoints; i++) {
	
		int x = 1 + (rand() % 100);
		int y = 1 + (rand() % 100);
		file << "(" << x << ", " << y << ")\n";

	
	}

}

/*
Computes the cross product of the vectors:
v1->v2 and v2->v3
*/
double computeCrossProduct(sf::Vertex p1, sf::Vertex p2, sf::Vertex p3) {

	sf::Vector2<float> v1(p2.position.x - p1.position.x, p2.position.y - p1.position.y);
	sf::Vector2<float> v2(p3.position.x - p2.position.x, p3.position.y - p2.position.y);

	return (v1.x * v2.y) - (v1.y * v2.x);

}

//bool drawLinesBetweenPoints(std::vector<sf::Vertex> pointList, sf::RenderWindow *window) {
//
//	sf::Vertex vList2[pointList.size()] =
//	{
//		sf::Vertex(p2),
//		sf::Vertex(p3)
//
//	};
//
//	window.draw(vList2, 2, sf::Lines);
//
//}

int main(int argc, char* argv[]) {

	//Take care of font stuff
	sf::Font font;
	if (!font.loadFromFile("Arial.ttf")) {

		std::cout << "Failed to load font" << std::endl;
		return 0;

	}

	int width;
	int height;
	double scale = 7.5;

	std::cout << argc << std::endl;
	std::cout << argv[1] << std::endl;

	std::vector<sf::Vertex> pointList;
	
	//populateFileWithRandomPointset(argv[1], 20);

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
	bool LupperIsReadyToDraw = false;
	bool LlowerIsReadyToDraw = false;

	std::vector<sf::Vertex>  Lupper;
	std::vector<sf::Vertex> Llower;
	

	while (window.isOpen()) {

		

		while (window.pollEvent(e)) {

			window.clear();

			/*sf::RectangleShape r(sf::Vector2f(150, 5));
			r.rotate(45);
			r.setFillColor(sf::Color(25, 41, 88));
			window.draw(r);*/

			if (e.type == sf::Event::Closed) {

				window.close();

			}

			drawPointsToWindow(pointList, &window);

			if (LupperIsReadyToDraw) {
			
				drawUpperHullToWindow(Lupper, &window);
			
			}

			if (LlowerIsReadyToDraw) {
			
				drawUpperHullToWindow(Llower, &window);
			
			}
			

			//for (sf::Vertex v : pointList) {


			//	//Draw each point and a circle around it

			//	window.draw(&v, 1, sf::Points);
			//	sf::CircleShape c(5);
			//	c.setFillColor(sf::Color(100, 250, 50));
			//	c.setPosition(v.position.x, v.position.y);
			//	c.setOrigin(c.getRadius(), c.getRadius());
			//	window.draw(c);

			//	//check to see if mouse is hovering overany point(s)
			//	if (isMouseHovering(v, c, sf::Mouse::getPosition(window))) {
			//	
			//		//std::cout << "Hovering" << std::endl;
			//		///*sf::Text text;
			//		//text.setFont(font);
			//		//std::string s = "Point: ("
			//		//text.setString(s);*/
			//		std::cout << "(" << v.position.x << ", " << v.position.y << ")" << std::endl;

			//	
			//	}

			//}

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

					/*std::vector<sf::Vertex>  Lupper;*/
					Lupper.push_back(pointList[0]);
					Lupper.push_back(pointList[1]);

					std::cout << "L_upper:" << std::endl;

					for (sf::Vertex v : Lupper) {

						std::cout << "(" << v.position.x << ", " << v.position.y << "), ";

					}

					std::cout << std::endl;

					std::cout << "---Computing upper hull---" << std::endl;


					for (int i = 2; i <= pointList.size(); i++) {

						window.clear();

						drawPointsToWindow(pointList, &window);
						if (i != pointList.size()) {
							Lupper.push_back(pointList[i]);
						}
						//check to see if the last three points make a right turn
						sf::Vertex p1 = Lupper[Lupper.size() - 3];
						sf::Vertex p2 = Lupper[Lupper.size() - 2];
						sf::Vertex p3 = Lupper[Lupper.size() - 1];

						sf::Vector2<float> v1(p2.position.x - p1.position.x, p2.position.y - p1.position.y);
						sf::Vector2<float> v2(p3.position.x - p2.position.x, p3.position.y - p2.position.y);

						//sf::RectangleShape line1(v1);
						//line1.setPosition(p1.position.x, p1.position.y);
						//line1.setFillColor(sf::Color(25, 41, 88));
						////line1.setScale(5, 5);
						//sf::RectangleShape line2(v2);
						//line2.setPosition(p2.position.x, p2.position.y);
						//line2.setFillColor(sf::Color(150, 41, 10));
						////line2.setScale(5, 5);

						//window.draw(line1);
						//window.draw(line2);


						/*sf::Vertex vList1[2] =
						{
							sf::Vertex(p1),
							sf::Vertex(p2)
						};

						window.draw(vList1, 2, sf::Lines);


						sf::Vertex vList2[2] =
						{
							sf::Vertex(p2),
							sf::Vertex(p3)

						};

						window.draw(vList2, 2, sf::Lines);*/

						LupperIsReadyToDraw = true;

						drawUpperHullToWindow(Lupper, &window);
						std::cout << "Upper hull:" << std::endl;
						for (sf::Vertex v : Lupper) {

							std::cout << "(" << v.position.x << ", " << v.position.y << ")" << std::endl;

						}

						window.display();

						//compute the cross product of these two
						/*
						- Left turn --> cross product > 0
						- Right turn --> cross product < 0
						- Point Collinear --> cross product == 0
						*/

						//int crossProd = (v1.x * v2.y) - (v1.y * v2.x);

						double crossProd = computeCrossProduct(p1, p2, p3);

						while (Lupper.size() > 2 && crossProd < 0) {

							Lupper.erase(Lupper.end() - 2);

							//Have to reupdate crossProd
							if (Lupper.size() > 2) {
								sf::Vertex p1 = Lupper[Lupper.size() - 3];
								sf::Vertex p2 = Lupper[Lupper.size() - 2];
								sf::Vertex p3 = Lupper[Lupper.size() - 1];

								crossProd = computeCrossProduct(p1, p2, p3);
							}
						}

						char c;
						/*while (true) {						
							std::cout << "---Press 'c' to continue---" << std::endl;
							std::cin >> c;
							if (c == 'c') {
								break;
							}
							
						}*/
						Sleep(250);
					}

					std::cout << "Upper hull:" << std::endl;


					for (sf::Vertex v : Lupper) {

						std::cout << "(" << v.position.x << ", " << v.position.y << "), ";
						
					}

					LupperIsReadyToDraw = true;

					window.clear();
					drawPointsToWindow(pointList, &window);

					drawUpperHullToWindow(Lupper, &window);

					window.display();

					std::cout << std::endl;

					std::cout << "---Computing lower hull---" << std::endl;

					Llower.push_back(pointList[pointList.size() - 1]);
					Llower.push_back(pointList[pointList.size() - 2]);

					for (int i = pointList.size() - 3; i >= 0; i--) {

						window.clear();


						drawPointsToWindow(pointList, &window);
						drawUpperHullToWindow(Llower, &window);
						drawUpperHullToWindow(Lupper, &window);


						window.display();
						


						Llower.push_back(pointList[i]);
						sf::Vertex p1 = Llower[Llower.size() - 1];
						sf::Vertex p2 = Llower[Llower.size() - 2];
						sf::Vertex p3 = Llower[Llower.size() - 3];
						double crossProd = computeCrossProduct(p1, p2, p3);

						while (Llower.size() > 2 && crossProd > 0) {
							Llower.erase(Llower.end() - 2);

							p1 = Llower[Llower.size() - 1];
							p2 = Llower[Llower.size() - 2];
							p3 = Llower[Llower.size() - 3];

							crossProd = computeCrossProduct(p1, p2, p3);	
						}
						

						char c;
						while (true) {
							std::cout << "---Press 'c' to continue---" << std::endl;
							std::cin >> c;
							if (c == 'c') {
								break;
							}
						}

						std::cout << "Lower hull:" << std::endl;

						for (sf::Vertex v : Llower) {

							std::cout << "(" << v.position.x << ", " << v.position.y << ")" << std::endl;

						}

						LlowerIsReadyToDraw = true;

						

						

						std::cout << std::endl;
					
					}

					std::cout << "---Graham's Scan Complete---" << std::endl;

				}

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

	bool l = false, r = false, u = false, d = false;


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

}





