/* 
 * Classe Entity
 */

#include "Entity.h"
#include "includes.h"

Entity::Entity(Vector3 position){
	matrix_ = Matrix44();
	matrix_.setIdentity();
	matrix_.traslate(position.x,position.y,position.z);

	matrix_.set();
	initial_matrix = matrix_;

	id = last_id;
	last_id++;

	name_ = "Entity " + id;
}

float Entity::distance(Entity* b){
	return (matrix_*Vector3(0,0,1)).distance(b->getMatrix() * Vector3(0,0,1));
}

//Efficiently find if the distance is small
bool Entity::isNearerThan(Entity* ent, int dist){
	Matrix44 mat = ent->getMatrix();
	Vector3 b = Vector3(mat.m[12],mat.m[13],mat.m[14]);
	Vector3 pos = Vector3(matrix_.m[12],matrix_.m[13],matrix_.m[14]);

	return (pos.x-b.x)*(pos.x-b.x)+(pos.y-b.y)*(pos.y-b.y)+(pos.z-b.z)*(pos.z-b.z) < (dist*dist);
}

void Entity::reset(){matrix_ = initial_matrix; }