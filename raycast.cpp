#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>

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

	//Set up clock for displaying FPS
	sf::Clock clock;

	//Set up FPS counter and font
	sf::Text text;
	sf::Font font;

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

			/*If ray direction has negative x or y component, the stepX or stepY will be - 1 accordingly
			  If ray direction has positive x or y component, the stepX or stepY will be +1 accordingly
			  If ray direction has 0 x or y component, the stepX or stepY does not matter since it will be unused*/
			if (rayDirX < 0) //Ray x-direction is negative
			{
				stepX = -1;
				sideDistX = (posX - mapX) * deltaDistX;
			}
			else
			{
				stepX = 1;
				sideDistX = (mapX - posX + 1.0f) * deltaDistX;
			}
			if (rayDirY < 0) //Ray y-direction is negative
			{
				stepY = -1;
				sideDistY = (posY - mapY) * deltaDistY;
			}
			else
			{
				stepY = 1;
				sideDistY = (mapY - posY + 1.0f) * deltaDistY;
			}

			//Start the DDA algorithm
			while (!hit)
			{
				if (sideDistX < sideDistY)   //If we hit a horizontal line first
				{
					sideDistX += deltaDistX; //Increment the ray
					mapX += stepX;			 //Increment ray position in the map
					side = 0;				 //x side of square is hit;
				}
				else						 //If we hit vetical line first
				{
					sideDistY += deltaDistY; //Increment the ray
					mapY += stepY;			 //Increment ray position in map
					side = 1;				 //y side of square is hit
				}
				//Check if ray has hit a square that is a wall (not empty)
				if (worldMap[mapX][mapY] > 0)
					hit = true;
			}

			//Calculate distance from where wall was hit to the camera plane
			if (side == 0)
				perpWallDist = (sideDistX - deltaDistX);
			else
				perpWallDist = (sideDistY - deltaDistY);

			//Calculate height of wall to draw to the screen has one veritcal strip
			int lineHeight = (int)(screenHeight / perpWallDist);

			//Calculate the highest pixel to start drawing
			int drawStart = (screenHeight - lineHeight) / 2;
			if (drawStart < 0)
				drawStart = 0;				//Cap it at the top of the screen (0)
			
			//Calculate the lowest pixel to finish drawing
			int drawEnd = (screenHeight + lineHeight) / 2;
			if (drawEnd < 0)
				drawEnd = screenHeight - 1; //Cap it at the bottom of the screen

			//Choose wall color
			sf::Color wallColor = sf::Color{ 0, 0, 255, 255 };
			if (side == 1)
				wallColor = wallColor - sf::Color{0, 0, 0, 128};

			//Create a line in SFML
			sf::Vertex line[2]
			{
				sf::Vertex(sf::Vector2f(x, drawStart), wallColor),
				sf::Vertex(sf::Vector2f(x, drawEnd), wallColor)
			};
			//Draw line to the window in SFML
			window.draw(line, 2, sf::Lines);
		};

		//Calculate the fps
		float fps = 1.0f / ((clock.restart().asMilliseconds()) / 1000.0f);

		//Load a font
		if (!font.loadFromFile("fonts/Ubuntu-Regular.ttf"))
			std::cerr << "Failed to load font" << std::endl;
		else
		{
			//Draw the fps text to the screen
			text.setFont(font);
			text.setString("FPS: " + std::to_string(fps));
			window.draw(text);
		}

		window.display();
	}

	return 0;
}