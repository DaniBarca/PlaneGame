#include "game.h"
#include "utils.h"
#include "texture.h"
#include <string.h>
#include "World.h"

World* World::instance = NULL;

float angle = 0;

Game::Game()
{
	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl

	keystate = NULL;
	mouse_locked = true;
}

//Here we have already GL working, so we can create meshes and textures
void Game::init(void)
{
	srand(time(NULL));
	std::cout << "initiating game..." << std::endl;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	World::getInstance();

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
	mouse_control = false;
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw out world
	//drawGrid(500); //background grid
	//draw the plane

	glColor3f(1,1,1);

	//landscape->render();
	World::getInstance()->render();

	//plane->render();	//render plane
	//planeb->render();

	//swap between front buffer and back buffer
	SDL_GL_SwapBuffers();
}

void Game::update(double seconds_elapsed)
{
	if(mouse_control){
		if(mouse_delta.x > 0)
			World::getInstance()->mainCharacter->Roll("LEFT", seconds_elapsed);
		if(mouse_delta.x < 0)
			World::getInstance()->mainCharacter->Roll("RIGHT", seconds_elapsed);
		if(mouse_delta.y > 0)
			World::getInstance()->mainCharacter->vRoll("UP", seconds_elapsed);
		if(mouse_delta.y < 0)
			World::getInstance()->mainCharacter->vRoll("DOWN", seconds_elapsed);

		//to navigate with the mouse fixed in the middle
		if (mouse_locked)
		{
			SDL_WarpMouse(WINDOW_WIDTH*0.5, WINDOW_HEIGHT*0.5); //put the mouse back in the middle of the screen
			this->mouse_position.x = WINDOW_WIDTH*0.5;
			this->mouse_position.y = WINDOW_HEIGHT*0.5;
		}
	

		double speed = seconds_elapsed * 50;//seconds_elapsed * 1000; //the speed is defined by the seconds_elapsed so it goes constant

		//mouse input to rotate the cam
		if ((mouse_state & SDL_BUTTON_LEFT)) //is left button pressed?
		{
			World::getInstance()->mainCharacter->shoot(true);
		}
	}

	if (keystate[SDLK_d])    { World::getInstance()->mainCharacter->Roll("RIGHT", seconds_elapsed);		}
	if (keystate[SDLK_a])    { World::getInstance()->mainCharacter->Roll("LEFT" , seconds_elapsed);		}
	if (keystate[SDLK_w])    { World::getInstance()->mainCharacter->vRoll("UP"  , seconds_elapsed);		}
	if (keystate[SDLK_s])    { World::getInstance()->mainCharacter->vRoll("DOWN", seconds_elapsed);		}
	if (keystate[SDLK_q])    { World::getInstance()->mainCharacter->accelerate(seconds_elapsed);	        }
	if (keystate[SDLK_e])    { World::getInstance()->mainCharacter->decelerate(seconds_elapsed);     	}
	if (keystate[SDLK_n])    { World::getInstance()->mainCharacter->hRoll("LEFT", seconds_elapsed);	    }
	if (keystate[SDLK_m])    { World::getInstance()->mainCharacter->hRoll("RIGHT",   seconds_elapsed);	}
	if (keystate[SDLK_SPACE]){ World::getInstance()->mainCharacter->shoot(true); }
	if (keystate[SDLK_b])    { World::getInstance()->throwBomb(); } 
	if (keystate[SDLK_l])    { std::cout << (World::getInstance()->mainCharacter->getMatrix()*Vector3(0,0,1)).y << std::endl;}
	if (keystate[SDLK_v])    { World::getInstance()->cam = (World::getInstance()->cam == 0) ? 1 : 0; }
	if (keystate[SDLK_c])    {
		cout << "controls changed" << endl;
		if(mouse_control){
			mouse_control = false;
			World::getInstance()->mainCharacter->v_roll = 2;
			World::getInstance()->mainCharacter->roll   = 3;
		}
		else{
			mouse_control = true;
			World::getInstance()->mainCharacter->v_roll = 6;
			World::getInstance()->mainCharacter->roll   = 6;
		}
	}

	World::getInstance()->update(seconds_elapsed);
}

//Keyboard event handler (sync input)
void Game::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: exit(0);
	}
}


void Game::onMouseButton( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE)
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}
