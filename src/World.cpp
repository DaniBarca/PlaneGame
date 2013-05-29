/* Dani Barca 2013
 * Classe Singleton per gestionar tot el món del joc
 */

#include "World.h"

TextureManager* TextureManager::instance = NULL;
MeshManager   * MeshManager   ::instance = NULL;
BulletManager * BulletManager ::instance = NULL;
int				Entity        ::last_id  = NULL;

bool compare(const GameObject* A, const GameObject* B){
	return (A->getPriority() > B->getPriority());
}

World::World(){
	assert(instance == NULL);
	instance = this;
	num_enemies = 6;

	EnemyPlane* enemy;
	mainCharacter = new Plane("..\\..\\data\\spitfire"    , "..\\..\\data\\spitfire_color_spec.tga", Vector3(0,0,0));
	scenario      = new GameObject("..\\..\\data\\island" , "..\\..\\data\\island_color_luz.tga", Vector3(0,-1000,0),true,false);

	camera = new Camera();
	camera->lookAt(Vector3(0,250,250),Vector3(0,0,0), Vector3(0,1,0));
	camera->setPerspective(70,WINDOW_WIDTH/(float)WINDOW_HEIGHT,0.1,10000); //set the projection, we want to be perspective
	
	camera->center = mainCharacter->getMatrix() * Vector3(1,1,1);
	camera->up     = mainCharacter->getMatrix().topVector();
	camera->eye    = mainCharacter->getMatrix() * Vector3(0,3,-10);

	sky = new GameObject("..\\..\\data\\cielo", "..\\..\\data\\cielo.tga", Vector3(camera->center.x, camera->center.y-500, camera->center.z));

	EntityPair aux;
	aux.a = mainCharacter;
	for(int i = 0; i < num_enemies; ++i){
		enemy = new EnemyPlane("..\\..\\data\\spitfire","..\\..\\data\\spitfire_color_spec.tga", Vector3(i*10, i*10, i*10));
		enemies.push_back(enemy);
		aux.b = enemy;
		potentialCollisions.push_back(aux);
	}

	for(int i = 0; i < num_enemies; ++i){
		for(int j = 0; j < num_enemies; ++j){
			if(i == j) continue;
			aux.a = enemies.at(i);
			aux.b = enemies.at(j);
			potentialCollisions.push_back(aux);
		}
	}
}

World* World::getInstance(){
	if(instance == NULL)
		instance = new World();
	return instance;
}

void World::update(double elapsed_time){
	mainCharacter->update(elapsed_time);
	
	camera->center = mainCharacter->getMatrix() * Vector3(0,5,1);
	camera->up      = mainCharacter->getMatrix().topVector();
	camera->eye     = (camera->eye - camera->center).normalize()*20 + camera->center; // mainCharacter->getMatrix() * Vector3(0,3,-10);

	//camera->center = enemies.at(0)->getMatrix() * Vector3(0,0,1);
	//camera->up     = enemies.at(0)->getMatrix().topVector();
	//camera->eye    = (camera->eye - camera->center).normalize()*20 + camera->center; // mainCharacter->getMatrix() * Vector3(0,3,-10);

	sky->setPos(Vector3(camera->center.x, camera->center.y-500, camera->center.z));

	if((mainCharacter->getMatrix() * Vector3(0,0,1)).y < -400)
		if(areColliding(mainCharacter, scenario))
			std::cout << "Collision with ground" << std::endl;

	for(int i = 0; i < num_enemies; ++i){
		enemies[i]->update(elapsed_time);

		if(areColliding(enemies[i], mainCharacter))
			std::cout << "Collision enemy-mainCharacter" << std::endl;

		for(int j = i+1; j < num_enemies; ++j){
			if(areColliding(enemies[i], enemies[j]))
				std::cout << "Collision between enemies" << std::endl;
		}
	}

	(BulletManager::getInstance())->update(elapsed_time);
}

bool World::areColliding(GameObject* a, GameObject* b){
	//Comprobem si les esferes se solapen
	if(a->distance(b) > (a->getMesh().boundsRadius + b->getMesh().boundsRadius)) return false;
	
	//Se solapen, per tant mirem si les meshes colisionen
	a->collisionModel->setTransform(a->getMatrix().m);
	b->collisionModel->setTransform(b->getMatrix().m);

	if(a->collisionModel->collision(b->collisionModel))
		return true;
	return false;
}

void World::render(){
	camera->set();

	glDisable(GL_DEPTH_TEST); //Desactivem z-buffer perque no desapareguin objectes rere el cel
	sky->render();
	glEnable(GL_DEPTH_TEST);  //I el reactivem

	mainCharacter->render();
	scenario->render();
	
	for(unsigned int i = 0; i < enemies.size(); ++i){
		enemies.at(i)->render();
	}

	(BulletManager::getInstance())->render();
}
