#ifndef Particle
#define Partice

#include "framework.h"
#include "texture.h"

class Particle{
	float speed;
	float size;
	double lifeTime;
	float alpha;
	Vector3 Color;

	Vector3 position;
	Vector3 rotation;

	Texture* texture;
	bool isDead;

public:
	Particle(Texture* text, Vector3 position, double lifeTime, float speed, float size, float alpha, Vector3 Color, Vector3 direction, Vector3 rotation){
		texture = text;
		this->lifeTime = lifeTime;
		this->speed    = speed;
		this->size     = size;
		this->alpha    = alpha;
		this->Color	   = Color;
		this->rotation = rotation;
	}

	void render(){

	}

	void update(double elapsed_time){}
};

#endif