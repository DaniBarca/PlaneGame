#ifndef Particle
#define Partice

#include "TextureManager.cpp"
#include "framework.h"
#include "texture.h"
#include "includes.h"

class Particle{
	float speed;
	float size;
	double lifeTime;
	float alpha;
	Vector3 Color;

	Vector3 position;

	Texture* texture;
	bool isDead;

public:
	Particle(string text, Vector3 position, double lifeTime, float speed, float size, float alpha, Vector3 Color){
		texture = (TextureManager::getInstance())->get(text);
		this->lifeTime = lifeTime;
		this->speed    = speed;
		this->size     = size;
		this->alpha    = alpha;
		this->Color	   = Color;
		this->position = position;
	}

	void render(){
		float hsize = size*0.5;
		glColor3f(255,255,255);
		texture->bind();
		glBegin(GL_QUADS);
			
		glEnd();
		texture->unbind();
	}

	void update(double elapsed_time){}
};

#endif