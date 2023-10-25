#pragma once
#include <cmath>

class Node 
{
private:
	Node* m_parent;
	int x, y;
	float f, g, h;
public:
	Node();
	Node(Node* parent, int posx, int posy);
	bool operator<(const Node& rhs) const noexcept;
	bool operator==(const Node& rhs) const noexcept;
	bool isDestination(int* map, int height);
	bool isBlocked(int* map, int height);
	bool isValid(int* map, int height, int width);
	float calculatef();
	float calculateg();
	float calculateh(int destX, int destY);
	float getf();
	float getg();
	float geth();
	int getx();
	int gety();
	Node* getParent();
	void setg(float& newg);
	void seth(float& newh);
	void setParent(Node* parent);
};