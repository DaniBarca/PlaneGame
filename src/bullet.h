/*
 * Classe per les bales
 */

#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"
#include "includes.h"

class Bullet : public Entity{
protected:
	float speed;
	float range;
	float distance_traveled;

	Matrix44 matrix_;
	Vector3 aux,auxb;
	
public:
	bool isDead;
	Bullet(Matrix44 position);

	void update(double elapsed_time);
	void render();
	void kill(){isDead = true;}
	void relife(Matrix44 position);
};

#endif