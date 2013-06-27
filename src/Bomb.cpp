#include "Bomb.h"

Bomb::Bomb(Matrix44 position, bool thrownByPlayer,vector<GameObject*> scene, int damage) : Bullet(position, thrownByPlayer, damage){

	speed = 50;

	texture_ = (TextureManager::getInstance())->get("..\\..\\data\\bomb.tga");
	mesh_    = (MeshManager::getInstance())->get("..\\..\\data\\bomb");

	//Averiguamos en qué punto caerá la bomba
	Vector3 aux = Vector3(0,-1,0);
	Vector3 pos = matrix_.getPos();
	bool found = false;

	matrix_ = Matrix44();
	matrix_.m[12] = position.m[12];
	matrix_.m[13] = position.m[13];
	matrix_.m[14] = position.m[14];

	for(unsigned int i = 0; i < scene.size(); ++i){
		scene[i]->getMesh().collisionModel->setTransform(scene[i]->getMatrix().m);
		if(scene[i]->getMesh().collisionModel->rayCollision(pos.v,(pos + aux).v,true)){
			found = true;
			scene[i]->getMesh().collisionModel->getCollisionPoint(CollisionPoint.v,true);
			cout << CollisionPoint.x << " - " << CollisionPoint.y << " - " << CollisionPoint.z << endl;
			break;
		}
	}

	if(!found) //Significa que la bomba nunca llegará a colisionar con el suelo
		isDead = true;
}

void Bomb::update(double elapsed_time){
	if(isDead) return;
	matrix_.traslateLocal(0,-speed*elapsed_time,0);                //Movimiento
	distance_traveled += speed*elapsed_time;			      //Distancia recorrida

	if(distance_traveled > range)						      //Si superamos el rango, matamos la bala
		isDead = true;
}

void Bomb::render(){
	if(isDead)return;
	glColor3f(255,255,255);
	glPushMatrix();
		matrix_.set();
		texture_->bind();
		mesh_   ->render();
		texture_->unbind();

	glPopMatrix();
}

void Bomb::relife(Matrix44 position, bool thrownByPlayer, vector<GameObject*> scene){
	Bullet::relife(position,thrownByPlayer);
	
	Vector3 aux = Vector3(0,-1,0);
	Vector3 pos = matrix_.getPos();
	bool found = false;

	for(unsigned int i = 0; i < scene.size(); ++i){
		if(scene[i]->getMesh().collisionModel->rayCollision(pos.v,(pos + aux).v,true)){
			found = true;
			scene[i]->getMesh().collisionModel->getCollisionPoint(CollisionPoint.v,true);
			break;
		}
	}

	if(!found) //Significa que la bomba nunca llegará a colisionar con el suelo
		isDead = true;
}