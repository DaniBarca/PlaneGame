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

bool World::collides(Bullet* b, GameObject* g){
		if(g->getMatrix().getPos().distance(b->getMatrix().getPos()) > g->getMesh().boundsRadius)
			return false;

		Vector3 point = Vector3();
		g->getMesh().collisionModel->setTransform(g->getMatrix().m);

		//Si no xoca, passa al següent avió
		if(g->getMesh().collisionModel->rayCollision(b->getMatrix().getPos().v, b->getMatrix().frontVector().v, true) == false)
			return false;
				
		return true;
}

void World::searchBulletCollisions(std::vector<EnemyPlane*> enemies, GameObject* mainCharacter, vector<Bullet*>* bulletVector){
	for(unsigned int i = 0; i < bulletVector->size(); ++i){
		if(bulletVector->at(i)->isDead) continue;
		for(unsigned int j = 0; j < enemies.size(); ++j){
			if(collides(bulletVector->at(i), enemies.at(j)))
				enemies.at(j)->hurt(bulletVector->at(i)->damage);
		}
		if(collides(bulletVector->at(i), mainCharacter))
				mainCharacter->hurt(bulletVector->at(i)->damage);
	}
}

World::World(){
	assert(instance == NULL);
	instance = this;
	num_enemies = 5;

	EnemyPlane* enemy;
	mainCharacter = new Spitfire("..\\..\\data\\spitfire_base"    , "..\\..\\data\\spitfire_color_spec.tga", Vector3(0,0,0));
	scenario      = new GameObject("..\\..\\data\\island" , "..\\..\\data\\island_color_luz.tga", Vector3(0,-1000,0),true);

	camera = new Camera();
	camera->lookAt(Vector3(0,250,250),Vector3(0,0,0), Vector3(0,1,0));
	camera->setPerspective(70,WINDOW_WIDTH/(float)WINDOW_HEIGHT,0.1,10000); //set the projection, we want to be perspective
	
	
	camera->center = mainCharacter->getMatrix() * Vector3(1,1,1);
	camera->up     = mainCharacter->getMatrix().topVector();
	camera->eye    = mainCharacter->getMatrix() * Vector3(0,3,-10);

	sky = new GameObject("..\\..\\data\\cielo", "..\\..\\data\\cielo.tga", Vector3(camera->center.x, camera->center.y-500, camera->center.z));

	for(int i = 0; i < num_enemies; ++i){
		enemy = new EnemyPlane("..\\..\\data\\spitfire","..\\..\\data\\spitfire_color_spec.tga", Vector3(i*100, i*100, i*100));
		enemies.push_back(enemy);
	}

	hud = new Hud();
}

World* World::getInstance(){
	if(instance == NULL)
		instance = new World();
	return instance;
}

void World::update(double elapsed_time){
	mainCharacter->update(elapsed_time);
	
	camera->center  = mainCharacter->getMatrix() * Vector3(0,5,1);
	camera->up      = mainCharacter->getMatrix().topVector();
	camera->eye     = (camera->eye - camera->center).normalize()*20 + camera->center; // mainCharacter->getMatrix() * Vector3(0,3,-10);
	camera->updateProjectionMatrix();
	camera->updateViewMatrix();

	//camera->center = enemies.at(0)->getMatrix() * Vector3(0,0,1);
	//camera->up     = enemies.at(0)->getMatrix().topVector();
	//camera->eye    = (camera->eye - camera->center).normalize()*20 + camera->center; // mainCharacter->getMatrix() * Vector3(0,3,-10);

	sky->setPos(Vector3(camera->center.x, camera->center.y-500, camera->center.z));

	if((mainCharacter->getMatrix() * Vector3(0,0,1)).y < -400){
		if(strictAreColliding(mainCharacter, scenario)){
			std::cout << "Collision with ground" << std::endl;
			mainCharacter->reset();
		}
	}

	for(int i = 0; i < num_enemies; ++i){
		enemies[i]->update(elapsed_time);

		if(areColliding(enemies[i], mainCharacter)){
			std::cout << "Collision enemy-mainCharacter" << std::endl;
			enemies[i]->reset();
			mainCharacter->reset();
		}
	}

	(BulletManager::getInstance())->update(elapsed_time);
	searchBulletCollisions(enemies,mainCharacter,(BulletManager::getInstance()->bulletVector));
}

bool World::strictAreColliding(GameObject* a, GameObject* b){
	a->getMesh().collisionModel->setTransform(a->getMatrix().m);

	if(a->getMesh().collisionModel->collision(b->getMesh().collisionModel, -1, 0, b->getMatrix().m))
		return true;
	return false;
}

bool World::areColliding(GameObject* a, GameObject* b){
	//Comprobem si les esferes se solapen
	return (a->distance(b) < (a->getMesh().boundsRadius + b->getMesh().boundsRadius));
}

void World::render(){
	camera->set();

	glDisable(GL_DEPTH_TEST); //Desactivem z-buffer perque no desapareguin objectes rere el cel
	sky->render();
	glEnable(GL_DEPTH_TEST);  //I el reactivem

	scenario->render();

	for(unsigned int i = 0; i < enemies.size(); ++i){
		enemies.at(i)->render();
	}

	
	glBegin(GL_LINE_STRIP);
		glVertex3f((mainCharacter->getMatrix()*Vector3(0,0,1)).x, (mainCharacter->getMatrix()*Vector3(0,0,1)).y, (mainCharacter->getMatrix()*Vector3(0,0,1)).z);
		glVertex3f((enemies.at(0)->getMatrix()*Vector3(0,0,1)).x, (enemies.at(0)->getMatrix()*Vector3(0,0,1)).y, (enemies.at(0)->getMatrix()*Vector3(0,0,1)).z);
	glEnd();

	mainCharacter->render();
	
	(BulletManager::getInstance())->render();
	hud->render(enemies, camera);
}
