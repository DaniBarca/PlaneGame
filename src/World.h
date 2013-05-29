/* Dani Barca 2013
 * Classe Singleton per gestionar tot el món del joc
 */

#ifndef WORLD_H
#define WORLD_H

#include "includes.h"
#include <assert.h>
#include "gameobject.cpp"
#include "Entity.h"
#include "plane.h"
#include <vector>
#include "camera.h"
#include <algorithm>
#include "TextureManager.cpp"
#include "MeshManager.cpp"
#include "BulletManager.cpp"
#include "enemyPlane.h"
#include "coldet/coldet.h"

typedef struct{
	GameObject* a;
	GameObject* b;
}EntityPair;

class World{
	static World* instance;

	GameObject* scenario;
	GameObject* sky;

	std::vector<EnemyPlane*> enemies;
	int num_enemies;

	std::vector<GameObject*> allObjects;

	std::vector<EntityPair> potentialCollisions;

public:
	Camera* camera;
	Plane*      mainCharacter;
	World();

	static World* getInstance();
	
	void update(double elapsed_time);

	void render();

	bool areColliding(GameObject* a, GameObject* b);
};

#endif