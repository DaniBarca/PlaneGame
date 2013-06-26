#include "Spitfire.h"

Spitfire::Spitfire(std::string meshdir, std::string texturedir, Vector3 position) 
	: Plane(meshdir, texturedir, position){
	helix_ = Helix(matrix_);
	pitch_ = Pitch();
}

void Spitfire::update(double elapsed_time){
	MovingObject::update(elapsed_time);

	helix_.update(elapsed_time, matrix_);
	pitch_.update(elapsed_time, matrix_);
}

void Spitfire::Roll(std::string dir, double elapsed_time){
	if(dir.compare("RIGHT") == 0){
		matrix_.rotateLocal(-getRoll()* elapsed_time, Vector3(0,0,1));
		pitch_.D();
	}
	else{
		matrix_.rotateLocal(getRoll()* elapsed_time, Vector3(0,0,1));
		pitch_.A();
	}

}

void Spitfire::hRoll(std::string dir, double elapsed_time){
	if(dir.compare("RIGHT") == 0){
		matrix_.rotateLocal(getHRoll() * elapsed_time, Vector3(0,1,0));
		pitch_.M();
	}
	else{
		matrix_.rotateLocal(-getHRoll()* elapsed_time, Vector3(0,1,0));
		pitch_.N();
	}
}

void Spitfire::vRoll(std::string dir, double elapsed_time){
	if(dir.compare("UP") == 0){
		matrix_.rotateLocal(-getVRoll()* elapsed_time, Vector3(1,0,0));
		pitch_.W();
	}
	else{
		matrix_.rotateLocal(getVRoll() * elapsed_time, Vector3(1,0,0));
		pitch_.S();
	}
}

void Spitfire::render(){
	GameObject::render();
	
	pitch_.render();
	helix_.render();
}