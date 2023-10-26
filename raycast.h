#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include "map.h"

#define mapWidth 24
#define mapHeight 24
#define screenWidth 1280
#define screenHeight 1020

Map map("demo/maps/maptest.png");
int* worldMap = map.loadMap();

//int worldMap[mapWidth][mapHeight] =
//{
//  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
//  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
//  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
//};

void drawText(sf::RenderWindow* window, std::string text, std::string path)
{
	//Set up text and font
	sf::Text sf_text;
	sf::Font font;

	if (!font.loadFromFile(path))
		std::cerr << "Failed to load font" << std::endl;
	else
	{
		//Draw the fps text to the screen
		sf_text.setFont(font);
		sf_text.setString(text);
		window->draw(sf_text);
	}
}

void handleInput(float& posX, float& posY, float& dirX, float& dirY, float moveSpeed, float rotationSpeed, float& planeX, float& planeY)
{
	//Move the player by adjusting the posX and posY values when W or S is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		if (*(worldMap + int(posX + dirX * moveSpeed) * map.getHeight() + int(posY)) == false)
			posX += dirX * moveSpeed;
		if (*(worldMap + int(posX) * map.getHeight() + int(posY + dirY * moveSpeed)) == false)
			posY += dirY * moveSpeed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		if (*(worldMap + int(posX - dirX * moveSpeed) * map.getHeight() + int(posY)) == false)
			posX -= dirX * moveSpeed;
		if (*(worldMap + int(posX) * map.getHeight() + int(posY - dirY * moveSpeed)) == false)
			posY -= dirY * moveSpeed;
	}

	/*Rotate using A and D keys by rotation the camera plane and
	* the direction by multiplaying the vector by the rotation matrix
	*			[ cos(x)	-sin(x) ]
	*			[ sin(x)	 cos(x) ]
	*/

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		float oldDirX = dirX;
		dirX = dirX * cos(-rotationSpeed) - dirY * sin(-rotationSpeed);
		dirY = oldDirX * sin(-rotationSpeed) + dirY * cos(-rotationSpeed);
		float oldPlaneX = planeX;
		planeX = planeX * cos(-rotationSpeed) - planeY * sin(-rotationSpeed);
		planeY = oldPlaneX * sin(-rotationSpeed) + planeY * cos(-rotationSpeed);
	}
	//Rotate opposite direction
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		float oldDirX = dirX;
		dirX = dirX * cos(rotationSpeed) - dirY * sin(rotationSpeed);
		dirY = oldDirX * sin(rotationSpeed) + dirY * cos(rotationSpeed);
		float oldPlaneX = planeX;
		planeX = planeX * cos(rotationSpeed) - planeY * sin(rotationSpeed);
		planeY = oldPlaneX * sin(rotationSpeed) + planeY * cos(rotationSpeed);
	}
}

void drawWalls(sf::RenderWindow* window, sf::Color color, int& start, int& end, int& side, int& screenVertLine)
{
	//If y side of square then draw color half alpha to mimic "shadows"
	if (side == 1)
		color = color - sf::Color{ 0, 0, 0, 128 };

	//Create a line in SFML
	sf::Vertex line[2]
	{
		sf::Vertex(sf::Vector2f(screenVertLine, start), color),
		sf::Vertex(sf::Vector2f(screenVertLine, end), color)
	};
	//Draw line to the window in SFML
	window->draw(line, 2, sf::Lines);
}

void drawMap(sf::RenderWindow* window, Map &map, int x, int y)
{
	sf::Texture tex;
	sf::Sprite sprite;
	tex.loadFromImage(map.getMapImage());
	sprite.setTexture(tex, true);
	sprite.setScale(10, 10);
	window->draw(sprite);
}
