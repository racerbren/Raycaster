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
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			if (*(m_map + i * m_height + j) == 4)
			{
				*(m_map + i * m_height + j) = 0;
				m_mapImage.setPixel(i, j, sf::Color::White);
			}
		}
	}

	std::set<Node*> openList;
	std::set<Node*> closedList;
	std::vector<Node*> path;
	std::vector<Node*> neighbors;
	std::set<Node*>::iterator it;

	Node start(nullptr, startX, startY);

	openList.insert(&start);
	bool found = false;

	while (!found)
	{
		it = openList.begin();
		Node* current = *it;
		it++;
		while(it != openList.end())
		{
			Node* temp = *it;
			if (temp->getf() <= current->getf())
			{
				if (temp->geth() < current->geth())
					current = temp;
			}
			it++;
		}

		closedList.insert(current);
		openList.erase(openList.begin());

		if (((int)current->getx() == m_destinationX) && ((int)current->gety() == m_destinationY))
		{
			std::cout << "found" << std::endl;
			Node* temp = current;
			while (current->getParent() != nullptr)
			{
				path.push_back(current);
				current = current->getParent();
			}
			found = true;
			break;
		}

		neighbors.clear();
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if ((i == 0) && (j == 0))
					continue;
				int x = current->getx() + i;
				int y = current->gety() + j;
				Node* neighbor = new Node(current, x, y);
				neighbor->calculateg();
				neighbor->calculateh(m_destinationX, m_destinationY);
				neighbor->calculatef();
				neighbors.push_back(neighbor);
			}
		}

		for (size_t i = 0; i < neighbors.size(); i++)
		{
			Node* currNeighbor = neighbors[i];
			if ((!currNeighbor->isValid(m_map, m_height, m_width) ||
				currNeighbor->isBlocked(m_map, m_height)) ||
				closedList.find(currNeighbor) != closedList.end())
			{
				continue;
			}
			else if (openList.find(currNeighbor) == openList.end())
				openList.insert(currNeighbor);
			else if (openList.find(currNeighbor) != openList.end())
			{
				float newCostToCurrNeighbor = (float)sqrt(pow(currNeighbor->getx() - current->getx(), 2) + pow(currNeighbor->gety() - current->gety(), 2));
				if (newCostToCurrNeighbor < currNeighbor->getg())
				{
					currNeighbor->setg(newCostToCurrNeighbor);
					currNeighbor->calculatef();
					currNeighbor->setParent(current);
				}
			}
		}
	}
	for (size_t i = 0; i < path.size(); i++)
	{
		Node* current = path[i];
		int x = current->getx();
		int y = current->gety();
		*(m_map + x * m_height + y) = 4;
		m_mapImage.setPixel(x, y, sf::Color::Magenta);
	}

	for (int k = 0; k < neighbors.size(); k++)
	{
		delete neighbors[k];
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
