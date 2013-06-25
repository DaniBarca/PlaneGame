/*
 * Class enemyPlane
 */
#ifndef ENEMY_PLANE
#define ENEMY_PLANE

#include "plane.h"
#include <string>
#include "framework.h"
#include <time.h>
#include <random>
#include "World.h"
#include "movingobject.cpp"

class EnemyPlane : public Plane{
	std::vector<Entity*> wayPoints;
	int nextWaypoint;
	bool alerted;
	bool inPanic;
	double alertTime;
	double panicTime;
	double lastEvasion;
	int    panicCount;
	int    evasionDir;

public:
	void patrol(double elapsed_time);
	void goTo(Vector3 nextPoint, double elapsed_time);
	void pursuit(double elapsed_time);
	void evade(double elapsed_time);
	void alert();
	void panic();

	EnemyPlane(std::string meshdir, std::string texturedir);

	virtual void update(double elapsed_time);
	virtual void render();
};

#endif