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

	id = last_id;
	last_id++;

	name_ = "Entity " + id;
}

float Entity::distance(Entity* b){
	return (matrix_*Vector3(0,0,1)).distance(b->getMatrix() * Vector3(0,0,1));
}