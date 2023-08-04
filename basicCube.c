#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#define size 0.5

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

void render_circle()
{
	SDL_Rect center, point;

	center.x = 320;
	center.y = 320;
	center.w = 1;
	center.h = 1;

	point.w = 1;
	point.h = 1;

	double radius = 141;
	double theta = 0;
	
	while (theta < 6.3)	
	{
		point.x = 320 + radius*cos(theta);
		point.y = 320 + radius*sin(theta);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &point);
		
		theta += 0.00314;	
	}
	SDL_RenderDrawRect(renderer, &center);
}

void render_cube(double points[8][3])
{
	double c = 320;
	double r = 200;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (int j = 0; j < 4; j++)
	{

		SDL_RenderDrawLine(renderer, c + r*points[j][0],
									 c + r*points[j][1],
									 c + r*points[(j+1)%4][0],
									 c + r*points[(j+1)%4][1]);
		
		SDL_RenderDrawLine(renderer, c + r*points[j + 4][0],
									 c + r*points[j + 4][1],
									 c + r*points[4 + ((j+1) % 4)][0],
									 c + r*points[4 + ((j+1) % 4)][1]);
		
		SDL_RenderDrawLine(renderer, c + r*points[j][0],
									 c + r*points[j][1],
									 c + r*points[4 + (j % 4)][0],
									 c + r*points[4 + (j % 4)][1]);
	
	}
}

void rotateZ(double points[8][3], double theta)
{
	double a, b, c;
	double m[3][3] = { {cos(theta), -sin(theta), 0},
					   {sin(theta), cos(theta), 0 },
					   {0		  , 0		  , 1}
					 };

	for	(int i = 0; i < 8; i++)
	{
		a = points[i][0]*m[0][0] + points[i][1]*m[0][1] + points[i][2]*m[0][2];
		b = points[i][0]*m[1][0] + points[i][1]*m[1][1] + points[i][2]*m[1][2];
		c = points[i][0]*m[2][0] + points[i][1]*m[2][1] + points[i][2]*m[2][2];
		points[i][0] = a;
		points[i][1] = b;
		points[i][2] = c;
	}
}

void rotateY(double points[8][3], double theta)
{
	double a, b, c;
	double m[3][3] = { {cos(theta), 0, -sin(theta)},
					   {0		  , 1,           0},
					   {sin(theta), 0,  cos(theta)}
					 };

	for	(int i = 0; i < 8; i++)
	{
		a = points[i][0]*m[0][0] + points[i][1]*m[0][1] + points[i][2]*m[0][2];
		b = points[i][0]*m[1][0] + points[i][1]*m[1][1] + points[i][2]*m[1][2];
		c = points[i][0]*m[2][0] + points[i][1]*m[2][1] + points[i][2]*m[2][2];
		points[i][0] = a;
		points[i][1] = b;
		points[i][2] = c;
	}
}

void rotateX(double points[8][3], double theta)
{
	double a, b, c;
	double m[3][3] = { {1		  , 0		  , 0},
					   {0, cos(theta), -sin(theta)},
					   {0, sin(theta), cos(theta)}
					 };

	for	(int i = 0; i < 8; i++)
	{
		a = points[i][0]*m[0][0] + points[i][1]*m[0][1] + points[i][2]*m[0][2];
		b = points[i][0]*m[1][0] + points[i][1]*m[1][1] + points[i][2]*m[1][2];
		c = points[i][0]*m[2][0] + points[i][1]*m[2][1] + points[i][2]*m[2][2];
		points[i][0] = a;
		points[i][1] = b;
		points[i][2] = c;
	}
}

void project(double points[8][3])
{
	double a, b, c;

	for	(int i = 0; i < 8; i++)
	{
		double distants = 2;
		double z = 1 / (distants - points[i][2]);
		double m[3][3] = { {1, 0, 0},
						   {0, 1, 0}
						 };

		a = points[i][0]*m[0][0] + points[i][1]*m[0][1] + points[i][2]*m[0][2];
		b = points[i][0]*m[1][0] + points[i][1]*m[1][1] + points[i][2]*m[1][2];
		//c = points[i][0]*m[2][0] + points[i][1]*m[2][1] + points[i][2]*m[2][2];
		points[i][0] = a;
		points[i][1] = b;
		//points[i][2] = c;
	}
}

int main()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Video Initialization Error: ", SDL_GetError());
    }
    else{ printf("System is Ready to Go!\n"); }

    window = SDL_CreateWindow("Cube",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	double points[8][3] = { {size, -size, size},
							{-size, -size, size},
							{-size, size, size},
							{size, size, size},
							{size, -size, -size},
							{-size, -size, -size},
							{-size, size, -size},
							{size, size, -size}
					      };
	
	double p[2] = { 141, 141 };

	double thetaX = 0;
	double thetaY = 0;
	double thetaZ = 0;
	
    int running = 1;
    while(running == 1)
    {
        SDL_Event event;
        
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                running = 0;
            }
			if (event.type == SDL_KEYDOWN)
			{
				if(event.key.keysym.sym == SDLK_z)
				{
					thetaZ += 0.1;
				}
				if(event.key.keysym.sym == SDLK_y)
				{
					thetaY += 0.1;
				}
				if(event.key.keysym.sym == SDLK_x)
				{
					thetaX += 0.1;
				}
			}
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
		
		//render_circle();
		
		render_cube(points);
		rotateZ(points, thetaZ);
		rotateY(points, thetaY);
		rotateX(points, thetaX);
		project(points);
		
		thetaX = 0.03;			
		thetaY = 0.01;			
		thetaZ = 0.05;			

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}









