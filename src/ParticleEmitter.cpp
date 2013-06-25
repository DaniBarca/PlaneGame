#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(std::string textureDir, Vector3 position, double ratio, double lifeTime, float speed, float size, float alpha, Vector3 Color) : Entity(position){
	texture = (TextureManager::getInstance())->get(textureDir, false);

	this->ratio = ratio;
	pLifetime = lifeTime;
	pSpeed    = speed;
	pSize     = size;
	pAlpha    = alpha;
	pColor    = Color;
}

void ParticleEmitter::render(){
}