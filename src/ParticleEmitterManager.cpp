#ifndef PARTICLE_EMITTER_MANAGER
#define PARTICLE_EMITTER_MANAGER

#include "ParticleEmitter.h"
#include <assert.h>

class ParticleEmitterManager{
	ParticleEmitterManager* instance;
	std::vector<ParticleEmitter*>* particleEmitters;

public:
	ParticleEmitterManager(){
		assert(instance == NULL);

		instance  = this;
		particleEmitters = new std::vector<ParticleEmitter*>();
	}

	ParticleEmitterManager* getInstance(){
		if(instance == NULL)
			instance = new ParticleEmitterManager();
		return instance;
	}

	void update(double elapsed_time){
		for(int i = 0; i < particleEmitters->size(); ++i)
			particleEmitters->at(i)->update(elapsed_time);
	}

	void render(){
		for(int i = 0; i < particleEmitters->size(); ++i)
			particleEmitters->at(i)->render();
	}
};

#endif