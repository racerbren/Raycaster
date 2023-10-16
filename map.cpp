#include "map.h"

Map::Map()
{
	m_width = 0;
	m_height = 0;
	m_map = new int[m_width * m_height];
	m_playerStartX = 0;
	m_playerStartY = 0;
	m_destinationX = 0;
	m_destinationY = 0;
	m_mapImage.create(m_width, m_height);
}

Map::Map(const std::string path)
{
	m_mapImage.loadFromFile(path);
	m_width = m_mapImage.getSize().x;
	m_height = m_mapImage.getSize().y;

	//Initialize map array by dynamically allocating memory for a 2D array
	m_map = new int[m_width * m_height];  

	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			//Get integer value of color for deciding where the walls go, player start, and destination for pathfinding
			sf::Color color = m_mapImage.getPixel(i, j);

			if (color == sf::Color::Black)
			{
				//Increment in the allocated block of memory to match the pixel of the map image
				*(m_map + i * m_height + j) = 1;
			}
			else if(color == sf::Color::White)
			{
				*(m_map + i * m_height + j) = 0;
			}
			else if (color == sf::Color::Blue)
			{
				m_playerStartX = i;
				m_playerStartY = j;
			}
			else if (color == sf::Color::Red)
			{
				m_destinationX = i;
				m_destinationY = j;
			}
		}
	}
}

int Map::getHeight()
{
	return m_height;
}

int* Map::loadMap()
{
	return m_map;
}

Map::~Map()
{
	delete[] m_map;
}