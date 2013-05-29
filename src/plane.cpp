#include "plane.h"
#include "gameobject.cpp"

Plane::Plane(std::string meshdir, std::string texturedir, Vector3 position)
: MovingObject(meshdir, texturedir, position, false){
	this->speed        = 0;
	this->acceleration = 50;
	this->deceleration = 5;
	this->max_speed	   = 200;
	this->min_speed	   = 0;
	this->std_speed    = 5;
	this->roll         = 3;
	this->v_roll       = 2;
	this->h_roll       = 0.5;
	this->friction     = 0.001;

	numBullets   = 10000;
	bulletsShoot = 0;

	cadencia = 0.2;

	name_ = "Plane " + id;
}

void Plane::shoot(){
	if(bulletsShoot >= numBullets) return;

	double time = (SDL_GetTicks() - lastBulletThrown) * 0.001;
	
	if(time > cadencia){
		(BulletManager::getInstance())->shoot(matrix_);
		bulletsShoot++;
		lastBulletThrown = SDL_GetTicks();
	}
}