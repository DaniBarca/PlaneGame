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

class EnemyPlane : public Plane{
	double alertTime;
	std::vector<Entity*> wayPoints;
	int nextWaypoint;
	bool alerted;

public:
	void patrol(double elapsed_time);
	void goTo(Vector3 nextPoint, double elapsed_time);
	void pursuit(double elapsed_time);
	void alert();

	EnemyPlane(std::string meshdir, std::string texturedir);

	virtual void update(double elapsed_time);
	virtual void render();
};

#endif