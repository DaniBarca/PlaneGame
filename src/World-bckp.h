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
class Hud;
#include "hud.h"
#include "Spitfire.h"

class World{
	static World* instance;

	GameObject* scenario;
	GameObject* sky;

	Hud* hud;

	std::vector<EnemyPlane*> enemies;
	int num_enemies;

	std::vector<GameObject*> allObjects;
public:
	Camera* camera;
	Spitfire*      mainCharacter;
	World();

	static World* getInstance();

	std::vector<EnemyPlane*> getEnemies(){ return enemies; }
	
	void update(double elapsed_time);

	void render();

	bool strictAreColliding(GameObject* a, GameObject* b);
	bool areColliding(GameObject* a, GameObject* b);
	bool collides(Bullet* b, GameObject* g);
	void searchBulletCollisions(std::vector<EnemyPlane*> enemies, GameObject* mainCharacter, vector<Bullet*>* bulletVector);
};

#endif