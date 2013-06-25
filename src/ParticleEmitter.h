#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "framework.h"
#include "TextureManager.cpp"
#include "texture.h"
#include "Entity.h"
#include <string.h>

class ParticleEmitter : public Entity{
	double ratio;
	
	double  pLifetime;
	float   pSpeed;
	float   pSize;
	float   pAlpha;
	Vector3 pColor;

	Texture* texture;

public:
	ParticleEmitter(std::string textureDir, Vector3 position, double ratio, double lifeTime, float speed, float size, float alpha, Vector3 Color);

	void update(double elapsed_time);
	void render();
};

#endif