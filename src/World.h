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
#include "Bomber.h"
#include "Entity.h"
#include "camera.h"
#include "plane.h"
#include "text.h"



class Hud;
#include "hud.h"

typedef struct{
	string meshdir;
	string textdir;
}buildDir;

class World{
	static World* instance;

	vector<EnemyPlane*> enemyPlanes;
	vector<GameObject*> buildings;
	vector<buildDir> buildsDir;

	GameObject* sky;
	float sceneW;

	Hud * hud;

	void searchEnemyCollision();
	void searchBulletCollisions();
	void searchBombCollisions();
	bool collidesWithTerrain(GameObject* g);
	void alertEnemyTerrainCollision();
	int minDist;

	int level;
	int aliveEnemies;
public:
	vector<GameObject*> scene;
	int cam;
	int enemyFollowing;
	Camera* camera;
	Plane*  mainCharacter;

	static World* getInstance();
	World();
	~World();

	void update(double elapsed_time);
	void render();

	bool readTxt(std::string dir);
	void loadLevel(std::string dir);
	void throwBomb();
};

#endif