#include "plane.h"

Plane::Plane(std::string meshdir, std::string texturedir, Vector3 position)
: MovingObject(meshdir, texturedir, position, false){
	this->speed        = 0;
	this->acceleration = 50;
	this->deceleration = 5;
	this->max_speed	   = 200;
	this->min_speed	   = 50;
	this->std_speed    = 5;
	this->roll         = 3;
	this->v_roll       = 2;
	this->h_roll       = 1;
	this->friction     = 0.01;

	numBullets   = 10000;
	bulletsShoot = 0;

	cadencia = 0.25;

	name_ = "Plane " + id;

	motor = BASS_SampleLoad(false,"..\\..\\data\\sound\\motor.mp3",0,0,3,BASS_SAMPLE_LOOP);
	motorSampleChannel = BASS_SampleGetChannel(motor,false);
	BASS_ChannelPlay(motorSampleChannel,true);
	BASS_ChannelSetAttribute(motorSampleChannel,BASS_ATTRIB_VOL,0.5);
	BASS_ChannelSet3DAttributes(motorSampleChannel,BASS_3DMODE_NORMAL,1,500,360,360,0.1);

	bullet = BASS_SampleLoad(false,"..\\..\\data\\sound\\shot.mp3",0,0,3,0);
	bulletSampleChannel = BASS_SampleGetChannel(bullet,false);
	BASS_ChannelSetAttribute(bulletSampleChannel,BASS_ATTRIB_VOL,0.7);
	BASS_ChannelSet3DAttributes(bulletSampleChannel,BASS_3DMODE_NORMAL,0,500,360,360,0.1);
}

void Plane::shoot(bool thrownByPlayer){
	if(bulletsShoot >= numBullets) return;

	double time = (SDL_GetTicks() - lastBulletThrown) * 0.001;

	Matrix44 A = matrix_; A.traslateLocal(-2,0,0);
	Matrix44 B = A; A.traslateLocal(4, 0,0);
	
	if(time > cadencia){
		(BulletManager::getInstance())->shoot(A,thrownByPlayer);
		(BulletManager::getInstance())->shoot(B,thrownByPlayer);
		bulletsShoot+=2;
		lastBulletThrown = SDL_GetTicks();
		
		BASS_ChannelPlay(bulletSampleChannel,true);
	}
}

void Plane::setSoundPosition(Vector3 p){
	BASS_3DVECTOR pos = BASS_3DVECTOR(p.x,p.y,p.z);
	BASS_ChannelSet3DPosition(motorSampleChannel,&pos,NULL,NULL);
	BASS_ChannelSet3DPosition(bulletSampleChannel,&pos,NULL,NULL);
}

void Plane::setListenerPosition(Vector3 p){
	BASS_3DVECTOR sposb = BASS_3DVECTOR(p.x,p.y,p.z);
	BASS_Set3DPosition(&sposb,NULL,NULL,NULL);
}