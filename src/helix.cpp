#ifndef HELIX_H
#define HELIX_H

#include "gameobject.cpp"

class Helix : public GameObject{
	float    helix_r;
public:
	Helix():GameObject("..\\..\\data\\helice","..\\..\\data\\helice.tga"){}
	Helix(Matrix44 m):GameObject("..\\..\\data\\helice","..\\..\\data\\helice.tga"){
		matrix_ = m;
		matrix_.traslateLocal(0,0,2.6);
		matrix_.rotateLocal(DEGTORAD(180),Vector3(0,1,0));

		helix_r = 0;
	}

	virtual void update(double elapsed_time, Matrix44 m){
		helix_r += DEG2RAD; //1 degree
		if(helix_r > ONESPIN)
			helix_r = helix_r - ONESPIN;

		matrix_ = m;
		matrix_.traslateLocal(0,0.2,2.2);
		matrix_.rotateLocal(DEGTORAD(180),Vector3(0,1,0));

		matrix_.rotateLocal(helix_r,Vector3(0,0,1));
	}

	virtual void render(){

		glPushMatrix();
		//Pintamos la hélice
		glEnable (GL_BLEND);

		texture_->bind();
	
		matrix_.set();
		mesh_->render();

		texture_->unbind();

		glDisable(GL_BLEND);

		glPopMatrix();
	}
};

#endif