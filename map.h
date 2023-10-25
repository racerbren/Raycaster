#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <vector>
#include <set>
#include <iostream>
#include "node.h"

class Map {
private:
	int m_width;
	int m_height;
	int* m_map;
	int m_playerStartX;
	int m_playerStartY;
	int m_destinationX;
	int m_destinationY;
	sf::Image m_mapImage;
public:
	Map();
	Map(const std::string path);
	~Map();
	void aStar(int startX, int startY);
	int getHeight();
	int getStartX();
	int getStartY();
	int* loadMap();
	sf::Image getMapImage();
};