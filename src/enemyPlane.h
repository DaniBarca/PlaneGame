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

class EnemyPlane : public Plane{
public:
	std::vector<Entity*> wayPoints;
	int nextWaypoint;

	void patrol(double elapsed_time);
	void goTo(Vector3 nextPoint, double elapsed_time);
	void pursuit(double elapsed_time);
public:
	EnemyPlane(std::string meshdir, std::string texturedir, Vector3 position = Vector3(0,0,0));

	virtual void update(double elapsed_time);
	virtual void render();
};

#endif