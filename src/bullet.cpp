/*
 * Bullet.cpp
 */

#include "bullet.h"

Bullet::Bullet(Matrix44 position){
	speed = 600;
	range = 2000;
	distance_traveled = 0;

	aux = Vector3(0,0,0);
	auxb= Vector3(0,0,0);

	matrix_ = position;

	float angulo = 1 - matrix_.topVector().dot(Vector3(0,1,0));
	std::cout << angulo << std::endl;
	matrix_.rotate(0, Vector3(1,0,0));

	isDead = false;

	name_ = "Bullet " + id;
}

void Bullet::update(double elapsed_time){
	if(isDead) return;
	matrix_.rotateLocal(-elapsed_time * 0.1, Vector3(1,0,0)); //Gravedad

	matrix_.traslateLocal(0,0,speed*elapsed_time);         //Movimiento
	distance_traveled += speed*elapsed_time;			   //Distancia recorrida

	if(distance_traveled > range)						   //Si superamos el rango, matamos la bala
		isDead = true;
}

void Bullet::render(){
	if(isDead) return;
	Matrix44 maux = matrix_;
	
	maux.traslateLocal(0,0,20);
	aux = maux*Vector3(0,0,1);
	glBegin(GL_LINE_STRIP);
		glColor3f(255,255,255);
		glVertex3f ((matrix_*Vector3(0,0,1)).x, (matrix_*Vector3(0,0,1)).y, (matrix_*Vector3(0,0,1)).z);
		glColor3f(190,190,0);
		glVertex3f (aux.x, aux.y, aux.z);
	glEnd();
}