#ifndef PLANE_H
#define PLANE_H

#include "camera.h"
#include "movingobject.cpp"
#include "includes.h"
#include "BulletManager.cpp"
#include <vector>
#include "gameobject.cpp"
#include "mesh.h"
#include "../visualstudio/libs/include/bass24/c/bass.h"

class Plane : public MovingObject{
protected:
	int numBullets;
	int bulletsShoot;
	double cadencia;

	double lastBulletThrown;

	HSAMPLE motor;
	HCHANNEL motorSampleChannel;

	HSAMPLE bullet;
	HCHANNEL bulletSampleChannel;

public:
	Plane(std::string meshdir, std::string texturedir, Vector3 position = Vector3(0,0,0));

	virtual void shoot(bool thrownByPlayer = false);
	void setSoundPosition(Vector3 p);
	void setListenerPosition(Vector3 p);
};

#endif