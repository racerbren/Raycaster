#include "map.h"
#include "raycast.h"

int main()
{
	//Player position vector
	float posX = map.getStartX(), posY = map.getStartY();

	//Player direction vector
	float dirX = -1.0f, dirY = 0.0f;

	//Camera plane vector (must be perpendicular to direction vector)
	float planeX = 0.0f, planeY = 0.66f;

	//Set up clock for displaying FPS
	sf::Clock clock;

	//Create a window in SFML and limit framerate to 60FPS. Also prevent the user from resizing the window
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Raycaster", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);

	bool run = true;
	while (run)
	{
		map.aStar((int)posX, (int)posY);
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
			//CameraX represents the x-coord on the camera plane that corresponds to the current vertical stripe of the screen
			float cameraX = (2 * (x / (float)screenWidth)) - 1; //Multiply by 2 so that the range is from -1 to 1 instead of -1 to 0
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
				if (*(worldMap + mapX * map.getHeight()+ mapY) == 1)
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
			sf::Color wallColor = sf::Color{ 255, 255, 255, 200 };

			//Draw the walls
			drawWalls(&window, wallColor, drawStart, drawEnd, side, x);
			
		};

		//Calculate the fps
		float frameTime = (clock.restart().asMilliseconds()) / 1000.0f;
		float fps = 1.0f / frameTime;

		//Modify speed for movement and rotation
		float moveSpeed = 5.0f * frameTime;
		float rotationSpeed = 3.0f * frameTime;

		//Draw FPS - Uncomment to unable this
		//drawText(&window, "FPS: " + std::to_string(fps), "demo/fonts/Ubuntu-Regular.ttf");

		//Draw minimap in top window
		drawMap(&window, map, posX, posY);

		//Listen for input
		handleInput(posX, posY, dirX, dirY, moveSpeed, rotationSpeed, planeX, planeY);

		window.display();
	}

	return 0;
}