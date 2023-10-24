#include "node.h"

Node::Node()
{
	parentX = -1;
	parentY = -1;
	x = -1;
	y = -1;
	f = -1;
	g = -1;
	h = -1;
}

Node::Node(int px, int py, int posx, int posy)
{
	parentX = px;
	parentY = py;
	x = posx;
	y = posy;
	f = 0;
	g = 0;
	h = 0;
}

bool Node::isDestination(int* map, int height)
{
	if (*(map + x * height + y) == 3)
		return true;
	else
		return false;
}

bool Node::isBlocked(int* map, int height)
{
	if (*(map + x * height + y) == 1)
		return true;
	else
		return false;
}

bool Node::isValid(int* map, int height, int width)
{
	return (x >= 0) && (x < width) && (y >= 0) && (y < height);
}

int Node::calculatef()
{
	f = g + h;
	return f;
}

int Node::calculateg()
{
	int a = parentX - x;
	int b = parentY - y;
	g = sqrt(pow(a, 2) + pow(b, 2));
	return g;
}

int Node::calculateh(int destX, int destY)
{
	int a = x - destX;
	int b = y - destY;
	h = sqrt(pow(a, 2) + pow(b, 2));
	return h;
}