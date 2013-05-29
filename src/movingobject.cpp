/* Dani Barca 2013
 * Classe que engloba aquells gameobjects que es mouen
 */

#ifndef MOVING_OBJECT
#define MOVING_OBJECT

#include "gameobject.cpp"

class MovingObject : public GameObject{
protected:
	float speed;
	float acceleration;
	float deceleration;

	float max_speed;
	float min_speed;
	float std_speed;
	float   roll;
	float v_roll;
	float h_roll;

	float friction;
public:
	MovingObject(std::string meshdir, std::string texturedir, Vector3 position = Vector3(0,0,0), bool mipmapping = true) 
	: GameObject(meshdir,texturedir,position,mipmapping){
		this->speed        = 0;
		this->acceleration = 1;
		this->deceleration = 1;
		this->max_speed    = 10;
		this->min_speed    = -10;
		this->std_speed    = 0;
		this->roll         = 1;
		this->v_roll       = 1;
		this->h_roll       = 1;
		this->friction     = 0.001;

		name_ = "MovingObject " + id;
	}

	/*Métodes "estàndar" de moviment, pensats per poder modificar-se segons les necessitats*/
	//Alguns objectes poden voler una acceleració/deceleració/rotació etc. dinàmica
	virtual float getAcceleration(){ 
		return acceleration;
	}

	virtual float getDeceleration(){
		return deceleration;
	}

	virtual float getRoll(){
		return roll;
	}

	virtual float getVRoll(){
		return v_roll;
	}

	virtual float getHRoll(){
		return h_roll;
	}

	virtual float getFriction(){
		if(speed < 0) //Vol dir que l'objecte va marxa enrere, la fricció haurà d'ésser oposada
			return -friction;
		return friction;
	}

	virtual void accelerate(double elapsed_time){
		if((speed += getAcceleration() * elapsed_time) > max_speed)
			speed =  max_speed;
	}
	
	virtual void decelerate(double elapsed_time){
		if((speed -= getDeceleration() * elapsed_time) < min_speed){
			speed = min_speed;
		}
	}

	virtual void Roll(std::string dir, double elapsed_time){
		if(dir.compare("RIGHT") == 0)
			matrix_.rotateLocal(-getRoll()* elapsed_time, Vector3(0,0,1));
		else
			matrix_.rotateLocal(getRoll()* elapsed_time, Vector3(0,0,1));
	}

	virtual void hRoll(std::string dir, double elapsed_time){
		if(dir.compare("RIGHT") == 0)
			matrix_.rotateLocal(getHRoll() * elapsed_time, Vector3(0,1,0));
		else
			matrix_.rotateLocal(-getHRoll()* elapsed_time, Vector3(0,1,0));
	}

	virtual void vRoll(std::string dir, double elapsed_time){
		if(dir.compare("UP") == 0)
			matrix_.rotateLocal(-getVRoll()* elapsed_time, Vector3(1,0,0));
		else
			matrix_.rotateLocal(getVRoll() * elapsed_time, Vector3(1,0,0));
	}

	virtual void update(double elapsed_time){
		if((speed -= getFriction()) < min_speed)
			speed = min_speed;

		matrix_.traslateLocal(0,0,speed*elapsed_time);
	}
};

#endif