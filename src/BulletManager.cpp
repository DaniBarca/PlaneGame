/* Dani Barca - Marc Mateu 2013
 * Codi per a gestionar les bales dels avions
 * Patró singleton
 */

#ifndef BULLET_MANAGER
#define BULLET_MANAGER

#include <assert.h>
#include <string>
#include <map>
#include "bullet.h"
#include "framework.h"

class BulletManager{
private:
	static BulletManager* instance;
public:
	std::vector<Bullet*>* bulletVector;
	BulletManager(){
		assert(instance == NULL);
		instance = this;
		bulletVector = new std::vector<Bullet*>();
	}

	static BulletManager* getInstance(){
		if(instance == NULL){
			instance = new BulletManager();
		}
		return instance;
	}

	void shoot(Matrix44 position, bool thrownByPlayer = false){
		if(!bulletVector->empty()){
			for(unsigned int i = 0; i < bulletVector->size(); ++i){
				if(bulletVector->at(i)->isDead){
					bulletVector->at(i)->relife(position);
					return;
				}
			}
		}
		Bullet* b = new Bullet(position, thrownByPlayer);
		bulletVector->push_back(b);
	}

	void update(const double elapsed_time){
		for(unsigned int i = 0; i < bulletVector->size(); ++i)
			bulletVector->at(i)->update(elapsed_time);
	}

	void render(){
		for(unsigned int i = 0; i < bulletVector->size(); ++i)
			bulletVector->at(i)->render();
	}
};

#endif