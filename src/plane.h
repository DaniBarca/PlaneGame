#ifndef PLANE_H
#define PLANE_H


#include "movingobject.cpp"
#include "includes.h"
#include "BulletManager.cpp"
#include <vector>
#include "gameobject.cpp"
#include "mesh.h"

class Plane : public MovingObject{
protected:
	int numBullets;
	int bulletsShoot;
	double cadencia;

	double lastBulletThrown;
public:
	Plane(std::string meshdir, std::string texturedir, Vector3 position = Vector3(0,0,0));

	void shoot(bool thrownByPlayer = false);
};

#endif