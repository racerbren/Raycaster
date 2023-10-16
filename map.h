#pragma once
#include <SFML/Graphics.hpp>
#include <string>

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
	void aStar(sf::Image &mapImage, int* m_map, int startX, int startY, int endX, int endY);
	int getHeight();
	int* loadMap();
};