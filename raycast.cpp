#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#define mapWidth 24
#define mapHeight 24
#define screenWidth 640
#define screenHeight 480

int worldMap[mapWidth][mapHeight] =
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int main()
{
	//Player position vector
	float posX = 22.0f, posY = 12.0f;

	//Player direction vector
	float dirX = -1.0f, dirY = 0.0f;

	//Camera plane vector (must be perpendicular to direction vector)
	float planeX = 0.0f, planeY = 0.66f;

	//Time of current frame and time of old frame to display fps counter
	float time = 0.0f, oldTime = 0.0f;

	//Create a window in SFML and limit framerate to 60FPS
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Raycaster");
	window.setFramerateLimit(60);

	bool run = true;
	while (run)
	{
		//Check if window was closed
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				run = false;
			}
		}
		//Clear old window before displaying new stuff
		window.clear();

		//Render stuff here
		for (int x = 0; x < screenWidth; x++)
		{
			//Calculate the direction of the ray based off the camera x value
			float cameraX = 2 * x / (float)screenWidth - 1;
			float rayDirX = dirX + planeX * cameraX;
			float rayDirY = dirY + planeY * cameraX;

			//Calculate what box of the map the ray is in starting at the player's position
			int mapX = (int)posX;
			int mapY = (int)posY;

			//Calculate the length of the ray from start to the next square side on x or y axis
			float sideDistX;
			float sideDistY;

			//Calculate the distance from one x or y side to the next x or y side respectively
			//This equations are created by using pythagorean theorem deltaDistX^2 = 1^2 + (rayDirY/rayDirX)^2
			float deltaDistX = std::abs(1 / rayDirX); //Simplified from deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
			float deltaDistY = std::abs(1 / rayDirY); //Simplified from deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

			//
			float perpWallDist;

			//The direction of the "step" the ray takes (always +/- 1)
			int stepX;
			int stepY;

			bool hit = false; //Determine if the ray has hit a "wall"
			int side; //0 if x side of square was hit, 1 if y side of square was hit
		};

		window.display();
	}

	return 0;
}