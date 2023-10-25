#include "node.h"

Node::Node()
{
	m_parent = nullptr;
	x = -1;
	y = -1;
	f = -1;
	g = -1;
	h = -1;
}

Node::Node(Node* parent, int posx, int posy)
{
	m_parent = parent;
	x = posx;
	y = posy;
	f = 0;
	g = 0;
	h = 0;
}

bool Node::operator<(const Node& rhs) const noexcept
{
	return (x < rhs.x) && (y < rhs.y);
}

bool Node::operator==(const Node& rhs) const noexcept
{
	return (x == rhs.x) && (y == rhs.y);
}

bool Node::isDestination(int* map, int height)
{
	return (*(map + x * height + y) == 3);
}

bool Node::isBlocked(int* map, int height)
{
	return (*(map + x * height + y) == 1);
}

bool Node::isValid(int* map, int height, int width)
{
	return (x >= 0) && (x < width) && (y >= 0) && (y < height);
}

float Node::calculatef()
{
	this->f = this->g + this->h;
	return this->f;
}

float Node::calculateg()
{
	int a = m_parent->getx() - x;
	int b = m_parent->gety() - y;
	this->g = (float)sqrt(pow(a, 2) + pow(b, 2));
	return this->g;
}

float Node::calculateh(int destX, int destY)
{
	int a = this->x - destX;
	int b = this->y - destY;
	this->h = (float)sqrt(pow(a, 2) + pow(b, 2));
	return this->h;
}

float Node::getf()
{
	return this->f;
}

float Node::getg()
{
	return this->g;
}

float Node::geth()
{
	return this->h;
}

int Node::getx()
{
	return this->x;
}

int Node::gety()
{
	return this->y;
}

Node* Node::getParent()
{
	return m_parent;
}

void Node::setg(float& newg)
{
	this->g = newg;
}

void Node::seth(float& newh)
{
	this->h = newh;
}

void Node::setParent(Node* parent)
{
	m_parent = parent;
}