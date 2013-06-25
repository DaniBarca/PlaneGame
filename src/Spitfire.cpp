#include "Spitfire.h"

Spitfire::Spitfire(std::string meshdir, std::string texturedir, Vector3 position) 
	: Plane(meshdir, texturedir, position){
	back_pitch   = (MeshManager::getInstance())->get("..\\..\\data\\spitfire_back_pitch");
	back_pitch_m = matrix_;
	back_pitch_m.traslateLocal(0,0.45,-5.7);

	helix_ = Helix(matrix_);
}

void Spitfire::update(double elapsed_time){
	MovingObject::update(elapsed_time);

	helix_.update(elapsed_time, matrix_);

	back_pitch_m = matrix_;
	back_pitch_m.traslateLocal(0,0.45,-5.7);
}

void Spitfire::Roll(std::string dir, double elapsed_time){
	if(dir.compare("RIGHT") == 0)
		matrix_.rotateLocal(-getRoll()* elapsed_time, Vector3(0,0,1));
	else
		matrix_.rotateLocal(getRoll()* elapsed_time, Vector3(0,0,1));
}

void Spitfire::hRoll(std::string dir, double elapsed_time){
	if(dir.compare("RIGHT") == 0)
		matrix_.rotateLocal(getHRoll() * elapsed_time, Vector3(0,1,0));
	else
		matrix_.rotateLocal(-getHRoll()* elapsed_time, Vector3(0,1,0));
}

void Spitfire::vRoll(std::string dir, double elapsed_time){
	if(dir.compare("UP") == 0)
		matrix_.rotateLocal(-getVRoll()* elapsed_time, Vector3(1,0,0));
	else
		matrix_.rotateLocal(getVRoll() * elapsed_time, Vector3(1,0,0));
}

void Spitfire::render(){
	GameObject::render();
	
	glPushMatrix();

	back_pitch_m.set();

	texture_->bind();
	//Pintamos el back pitch
	back_pitch->render();

	texture_->unbind();

	glPopMatrix();

	helix_.render();
}