/*
 * Entity
 */

#ifndef ENTITY_H
#define ENTITY_H

#include "framework.h"
#include <string>

class Entity{
public:
	static int last_id;
	Entity(Vector3 position = Vector3(0,0,0));
	Matrix44 getMatrix(){return matrix_;}

	bool  Entity::isNearerThan(Entity* ent, int dist);
	float distance(Entity* b);
	virtual void reset();

	std::string name_;

protected:

	Matrix44 matrix_;
	Matrix44 initial_matrix;
	int id;
};

#endif