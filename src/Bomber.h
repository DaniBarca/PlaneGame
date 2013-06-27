#ifndef BOMBER_H
#define BOMBER_H

#include "BulletManager.cpp"
#include "plane.h"

class Bomber : public Plane{
	double cadencia_b;
	double lastBombThrown;
public:
	Bomber(Vector3);
	
	virtual void shoot(bool thrownByPlayer);
	void Bomber::throwBomb(bool, vector<GameObject*>);
};

#endif