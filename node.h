#pragma once
#include <cmath>

class Node 
{
private:
	int parentX, parentY, x, y, f, g, h;
public:
	Node();
	Node(int px, int py, int posx, int posy);
	bool isDestination(int* map, int height);
	bool isBlocked(int* map, int height);
	bool isValid(int* map, int height, int width);
	int calculatef();
	int calculateg();
	int calculateh(int destX, int destY);
};