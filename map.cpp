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
	m_playerStartX = 0;
	m_playerStartY = 0;
	m_destinationX = 0;
	m_destinationY = 0;

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
				*(m_map + i * m_height + j) = 2;
			}
			else if (color == sf::Color::Red)
			{
				m_destinationX = i;
				m_destinationY = j;
				*(m_map + i * m_height + j) = 3;
			}
		}
	}
}

Map::~Map()
{
	delete[] m_map;
}

void Map::aStar(int startX, int startY)
{
	std::set<Node> openList;
	std::set<Node> closedList;
	Node start(nullptr, startX, startY);
	openList.insert(start);

	while (!openList.empty())
	{
		Node current = *openList.begin();
		std::set<Node>::iterator it = openList.begin();
		while (it != openList.end())
		{
			it++;
			Node temp = *it;
			if (temp.getf() <= current.getf())
			{
				if (temp.geth() < current.geth())
					current = temp;
			}
		}

		closedList.insert(current);
		openList.erase(openList.begin());

		if (current.getx() == m_destinationX && current.gety() == m_destinationY)
		{
			break;
		}

		std::vector<Node> neighbors;
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if ((i != 0) && (j != 0))
				{
					Node neighbor(&current, i, j);
					neighbor.calculateg();
					neighbor.calculateh(m_destinationX, m_destinationY);
					neighbor.calculatef();
					neighbors.push_back(neighbor);
				}
			}
		}

		float newG;
		for (size_t i = 0; i < neighbors.size(); i++)
		{
			Node currNeighbor = neighbors[i];
			if ((currNeighbor.isValid(m_map, m_height, m_width) && 
				!currNeighbor.isBlocked(m_map, m_height)) ||
				closedList.find(currNeighbor) != closedList.end())
			{
				continue;
			}
			newG = current.getg() + (float)sqrt(pow(currNeighbor.getx() - current.getx(), 2) + pow(currNeighbor.gety() - current.gety(), 2));
			if (newG < currNeighbor.getg() || openList.find(currNeighbor) == openList.end())
			{
				currNeighbor.setg(newG);
				currNeighbor.seth((float)sqrt(pow(currNeighbor.getx() - m_destinationX, 2) + pow(currNeighbor.gety() - m_destinationY, 2)));
				currNeighbor.calculatef();
				currNeighbor.setParent(&current);
				if (openList.find(currNeighbor) == openList.end())
					openList.insert(currNeighbor);
			}
		}

	}
}

int Map::getHeight()
{
	return m_height;
}

int Map::getStartX()
{
	return m_playerStartX;
}

int Map::getStartY()
{
	return m_playerStartY;
}

int* Map::loadMap()
{
	return m_map;
}

sf::Image Map::getMapImage()
{
	return m_mapImage;
}
