#include "Spitfire.h"

Spitfire::Spitfire(std::string meshdir, std::string texturedir, Vector3 position) 
	: Plane(meshdir, texturedir, position){
	helix   = (MeshManager::getInstance())->get("..\\..\\data\\helice");
	back_pitch   = (MeshManager::getInstance())->get("..\\..\\data\\spitfire_back_pitch");
	back_pitch_m = matrix_;
	back_pitch_m.traslateLocal(0,0.45,-5.7);

	helix_t = (TextureManager::getInstance())->get("..\\..\\data\\helice.tga",true);
	helix_m = matrix_;
	helix_m.traslateLocal(0,0,2.6);
	helix_m.rotateLocal(DEGTORAD(180),Vector3(0,1,0));

	helix_r = 0;
}

void Spitfire::update(double elapsed_time){
	MovingObject::update(elapsed_time);

	helix_r += DEG2RAD; //1 degree
	if(helix_r > ONESPIN)
		helix_r = helix_r - ONESPIN;

	helix_m = matrix_;
	helix_m.traslateLocal(0,0.2,2.2);
	helix_m.rotateLocal(DEGTORAD(180),Vector3(0,1,0));

	helix_m.rotateLocal(helix_r,Vector3(0,0,1));

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

	glPushMatrix();
	//Pintamos la hélice
	glEnable (GL_BLEND);

		helix_t->bind();
	
		helix_m.set();
		helix->render();

		helix_t ->unbind();

	glDisable(GL_BLEND);

	glPopMatrix();
	
}