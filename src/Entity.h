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

	float distance(Entity* b);

protected:
	std::string name_;
	Matrix44 matrix_;
	int id;
};

#endif