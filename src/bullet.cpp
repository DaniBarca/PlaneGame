/*
 * Bullet.cpp
 */

#include "bullet.h"

Bullet::Bullet(Matrix44 position, bool thrownByPlayer, int damage):Entity(Vector3(position.m[12],position.m[13],position.m[14])){
	speed = 900;
	range = 7000;
	distance_traveled = 0;

	matrix_ = position;

	aux = Vector3(0,0,0);
	auxb= Vector3(0,0,0);

	isDead = false;

	name_ = "Bullet " + id;

	this->damage = damage;
	this->thrownByPlayer = thrownByPlayer;
}

void Bullet::update(double elapsed_time){
	if(isDead) return;
	matrix_.traslateLocal(0,0,speed*elapsed_time);            //Movimiento
	distance_traveled += speed*elapsed_time;			      //Distancia recorrida

	if(distance_traveled > range)						      //Si superamos el rango, matamos la bala
		isDead = true;
}

void Bullet::render(){
	if(isDead) return;
	Matrix44 maux = matrix_;
	
	maux.traslateLocal(0,0,20);
	aux = maux*Vector3(0,0,1);

	glLineWidth(1);
	glBegin(GL_LINE_STRIP);
		glColor3f(255,255,255);
		glVertex3f ((matrix_*Vector3(0,0,1)).x, (matrix_*Vector3(0,0,1)).y, (matrix_*Vector3(0,0,1)).z);
		glColor3f(190,190,0);
		glVertex3f (aux.x, aux.y, aux.z);
	glEnd();
}

void Bullet::relife(Matrix44 position, bool thrownByPlayer){
	this->thrownByPlayer = thrownByPlayer;
	distance_traveled = 0;

	aux = Vector3(0,0,0);
	auxb= Vector3(0,0,0);

	matrix_ = position;

	isDead = false;
}