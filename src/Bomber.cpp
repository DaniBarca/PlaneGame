#include "Bomber.h"

Bomber::Bomber(Vector3 pos) : Plane("..\\..\\data\\bomber", "..\\..\\data\\bomber.tga",pos){
	max_speed = 100;
	cadencia_b = 3;
	name_= "Bomber";

	bomb = BASS_SampleLoad(false,"..\\..\\data\\sound\\bomb_whistle.mp3",0,0,3,BASS_SAMPLE_LOOP);
	bombSampleChannel = BASS_SampleGetChannel(motor,false);
}

void Bomber::shoot(bool thrownByPlayer){
	if(bulletsShoot >= numBullets) return;

	double time = (SDL_GetTicks() - lastBulletThrown) * 0.001;
	
	if(time > cadencia){
		(BulletManager::getInstance())->shoot(matrix_,thrownByPlayer);
		bulletsShoot++;
		lastBulletThrown = SDL_GetTicks();

		BASS_ChannelPlay(bulletSampleChannel,true);
	}
}

void Bomber::throwBomb(bool thrownByPlayer, vector<GameObject*> scene){
	double time = (SDL_GetTicks() - lastBombThrown) * 0.001;

	if(time > cadencia_b){
		(BulletManager::getInstance())->throwBomb(matrix_,scene,thrownByPlayer);
		lastBombThrown = SDL_GetTicks();

		BASS_ChannelPlay(bombSampleChannel,true);
	}
}