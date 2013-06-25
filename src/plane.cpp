#include "plane.h"

Plane::Plane(std::string meshdir, std::string texturedir, Vector3 position)
: MovingObject(meshdir, texturedir, position, false){
	this->speed        = 0;
	this->acceleration = 50;
	this->deceleration = 5;
	this->max_speed	   = 200;
	this->min_speed	   = 50;
	this->std_speed    = 5;
	this->roll         = 3;
	this->v_roll       = 2;
	this->h_roll       = 0.5;
	this->friction     = 0.01;

	numBullets   = 10000;
	bulletsShoot = 0;

	cadencia = 0.25;

	name_ = "Plane " + id;
}

void Plane::shoot(bool thrownByPlayer){
	if(bulletsShoot >= numBullets) return;

	double time = (SDL_GetTicks() - lastBulletThrown) * 0.001;

	Matrix44 A = matrix_; A.traslateLocal(-2,0,0);
	Matrix44 B = A; A.traslateLocal(4, 0,0);
	
	if(time > cadencia){
		(BulletManager::getInstance())->shoot(A,thrownByPlayer);
		(BulletManager::getInstance())->shoot(B,thrownByPlayer);
		bulletsShoot+=2;
		lastBulletThrown = SDL_GetTicks();
	}
}
