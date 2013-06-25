#ifndef WORLD_H
#define WORLD_H

#include "TextureManager.cpp"
#include "BulletManager.cpp"
#include "MeshManager.cpp"
#include "gameobject.cpp"
#include "enemyPlane.h"
#include "framework.h"
#include "includes.h"
#include "Spitfire.h"
#include "Entity.h"
#include "camera.h"
#include "plane.h"
#include "text.h"

class Hud;
#include "hud.h"

class World{
	static World* instance;

	vector<EnemyPlane*> enemyPlanes;

	GameObject* sky;
	vector<GameObject*> scene;
	float sceneW, sceneH;

	Hud * hud;

	void searchEnemyCollision();
	void searchBulletCollisions();
	bool collidesWithTerrain(GameObject* g);
	int minDist;
public:
	Camera* camera;
	Plane*  mainCharacter;

	static World* getInstance();
	World();
	~World();

	void update(double elapsed_time);
	void render();

	bool readTxt(std::string dir);
};

#endif