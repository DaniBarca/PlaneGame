#ifndef BOMBER_H
#define BOMBER_H

#include "../visualstudio/libs/include/bass24/c/bass.h"
#include "BulletManager.cpp"
#include "plane.h"

class Bomber : public Plane{
	double cadencia_b;
	double lastBombThrown;

	HSAMPLE bomb;
	HCHANNEL bombSampleChannel;

public:
	Bomber(Vector3);
	
	virtual void shoot(bool thrownByPlayer);
	void throwBomb(bool, vector<GameObject*>);
};

#endif