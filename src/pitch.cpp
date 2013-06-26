#ifndef PITCH
#define PITCH

#include "TextureManager.cpp"
#include "MeshManager.cpp"
#include "framework.h"
#include "includes.h"
#include "texture.h"
#include "mesh.h"

#define MAXANGLE DEGTORAD(30)

class Pitch{
	Texture* texture_;
	Texture* btext;
	Mesh   * b_; //Back
	Mesh   * v_; //Vertical
	Mesh   * fl_; //Front left
	Mesh   * fr_; //Front right

	Matrix44 b_m;
	Matrix44 v_m;
	Matrix44 fl_m;
	Matrix44 fr_m;

	float b_r  , db_r;
	float v_r  , dv_r;
	float fl_r , dfl_r;
	float fr_r , dfr_r;

public:
	Pitch(){
		texture_ = (TextureManager::getInstance())->get("..\\..\\data\\spitfire_color_spec.tga");
		btext = (TextureManager::getInstance())->get("..\\..\\data\\spitfire_axis_color_spec.tga");
		b_       = (MeshManager::getInstance())->get("..\\..\\data\\spitfire_back_pitch");
		v_       = (MeshManager::getInstance())->get("..\\..\\data\\spitfire_back_yaw");
		fl_      = (MeshManager::getInstance())->get("..\\..\\data\\spitfire_wing_aileron_left");
		fr_		 = (MeshManager::getInstance())->get("..\\..\\data\\spitfire_wing_aileron_right");

		b_r = 0; db_r = 0;
		v_r = 0; dv_r = 0;
		fl_r= 0; dfl_r= 0;
		fr_r= 0; dfr_r= 0;
	}

	void update(double elapsed_time, Matrix44 m){
		b_r += (db_r - b_r) *0.5;
		v_r += (dv_r - v_r) *0.5;
		fl_r+= (dfl_r- fl_r)*0.5;
		fr_r+= (dfr_r- fr_r)*0.5;

		db_r += (0 - db_r) * 0.01;
		dv_r += (0 - dv_r) * 0.01;
		dfl_r+= (0 - dfl_r)* 0.01;
		dfr_r+= (0 - dfr_r)* 0.01;

		b_m = m;
		v_m = m;
		fl_m= m;
		fr_m= m;

		fl_m.traslateLocal(3.76,-0.24,-0.67);
		fr_m.traslateLocal(-3.76,-0.24,-0.67);
		b_m.traslateLocal(0,0.42,-5.66);
		v_m.traslateLocal(0,0,-5.81);

		fl_m.rotateLocal(fl_r,Vector3(1,0,0));
		fr_m.rotateLocal(fr_r,Vector3(1,0,0));
		b_m. rotateLocal(b_r, Vector3(1,0,0));
		v_m. rotateLocal(v_r, Vector3(0,1,0));
		
	}

	void W(){
		if(db_r < MAXANGLE)
			db_r += DEG2RAD;
		if(dfl_r < MAXANGLE)
			dfl_r += DEG2RAD;
		if(dfr_r < MAXANGLE)
			dfr_r += DEG2RAD;
	}

	void S(){
		if(db_r > -MAXANGLE)
			db_r -= DEG2RAD;
		if(dfl_r > -MAXANGLE)
			dfl_r -= DEG2RAD;
		if(dfr_r > -MAXANGLE)
			dfr_r -= DEG2RAD;
	}

	void A(){
		if(dfl_r > -MAXANGLE)
			dfl_r -= DEG2RAD;
		if(dfr_r < MAXANGLE)
			dfr_r += DEG2RAD;
	}
	
	void D(){
		if(dfl_r < MAXANGLE)
			dfl_r += DEG2RAD;
		if(dfr_r > -MAXANGLE)
			dfr_r -= DEG2RAD;
	}

	void N(){
		if(dv_r < MAXANGLE)
			dv_r += DEG2RAD;
	}

	void M(){
		if(dv_r > -MAXANGLE)
			dv_r -= DEG2RAD;
	}

	void render(){
		texture_->bind();
		glPushMatrix();
		b_m.set();
		b_->render();
		glPopMatrix();

		glPushMatrix();
		v_m.set();
		v_->render();
		glPopMatrix();

		glPushMatrix();
		fl_m.set();
		fl_->render();
		glPopMatrix();

		glPushMatrix();
		fr_m.set();
		fr_->render();
		glPopMatrix();
		texture_->unbind();
	}
};

#endif