#include "World.h"

TextureManager* TextureManager::instance = NULL;
BulletManager * BulletManager ::instance = NULL;
MeshManager   * MeshManager   ::instance = NULL;
int             Entity        ::last_id  = NULL;

World::World(){
	bool success = readTxt("..\\..\\data\\levels\\1.txt");
	assert(success);

	for(unsigned int i = 0; i < enemyPlanes.size(); ++i)
		enemyPlanes.at(i) = new EnemyPlane("..\\..\\data\\spitfire", "..\\..\\data\\spitfire_color_spec.tga");

	camera = new Camera();
	camera->lookAt(Vector3(0,250,250),Vector3(0,0,0), Vector3(0,1,0));
	camera->setPerspective(70,WINDOW_WIDTH/(float)WINDOW_HEIGHT,0.1,10000);

	camera->center = mainCharacter->getMatrix() * Vector3(1,1,1);
	camera->up     = mainCharacter->getMatrix().topVector();
	camera->eye    = mainCharacter->getMatrix() * Vector3(0,3,-10);

	hud = new Hud();
	cam = 0;
	enemyFollowing = 0;
}

World* World::getInstance(){
	if(instance == NULL)
		instance = new World();
	return instance;
}

bool World::collidesWithTerrain(GameObject* g){
	if((g->getMatrix() * Vector3(0,0,1)).y > minDist) return false;

	for(unsigned int i = 0; i < scene.size(); ++i){
		if(scene[i]->getMesh().collisionModel->collision(g->getMesh().collisionModel,-1,0,g->getMatrix().m))
			return true;
	}
	return false;
}

void World::searchEnemyCollision(){
	bool mainTrSet = false;
	for(unsigned int i = 0; i < enemyPlanes.size(); ++i){
		if(enemyPlanes[i]->isDead())continue;
		if(mainCharacter->isNearerThan(enemyPlanes[i],20)){
			if(!mainTrSet){
				mainCharacter->getMesh().collisionModel->setTransform(mainCharacter->getMatrix().m);
				mainTrSet=true;
			}
			if(mainCharacter->getMesh().collisionModel->collision(enemyPlanes[i]->getMesh().collisionModel,-1,0,enemyPlanes[i]->getMatrix().m)){
				mainCharacter->reset();
			}
		}
	}
}

void World::searchBulletCollisions(){
	vector<Bullet*>* bulletVector = (BulletManager::getInstance())->bulletVector;
	Bullet* b;

	for(unsigned int i = 0; i < bulletVector->size(); ++i){
		b = bulletVector->at(i);
		if(bulletVector->at(i)->isDead) continue;

		for(unsigned int j = 0; j < enemyPlanes.size(); ++j){
			if(enemyPlanes[j]->isDead()) continue;
			if(bulletVector->at(i)->isNearerThan(enemyPlanes[j],20)){
				bulletVector->at(i)->kill();
				enemyPlanes[j]->hurt(bulletVector->at(i)->damage);
				enemyPlanes[j]->alert();
				enemyPlanes[j]->panic();
				break;
			}
		}

		if(bulletVector->at(i)->thrownByPlayer) continue;
		if(bulletVector->at(i)->isNearerThan(mainCharacter,20)){
				bulletVector->at(i)->kill();
				mainCharacter->hurt(bulletVector->at(i)->damage);
				break;
		}
	}
}

void World::update(double elapsed_time){
	mainCharacter->update(elapsed_time);

	//Set camera behind the plane
	if(cam == 0){
		camera->center  = mainCharacter->getMatrix() * Vector3(0,5,1);
		camera->up      = mainCharacter->getMatrix().topVector();
		camera->eye     = (camera->eye - camera->center).normalize()*20 + camera->center; // mainCharacter->getMatrix() * Vector3(0,3,-10);
	}
	if(cam == 1){
		while(enemyPlanes[enemyFollowing]->isDead()){
			enemyFollowing++;
			if(enemyFollowing >= enemyPlanes.size())
				enemyFollowing = 0;
		}
		camera->center  = enemyPlanes[enemyFollowing]->getMatrix() * Vector3(0,5,1);
		camera->up      = enemyPlanes[enemyFollowing]->getMatrix().topVector();
		camera->eye     = (camera->eye - camera->center).normalize()*20 + camera->center; // mainCharacter->getMatrix() * Vector3(0,3,-10);
	}

	//Set sky over the camera
	sky->setPos(Vector3(camera->center.x, camera->center.y-500, camera->center.z));

	for(unsigned int i = 0; i < enemyPlanes.size(); ++i){
		if(enemyPlanes[i]->isDead()) continue;
		enemyPlanes[i]->update(elapsed_time);
	}

	(BulletManager::getInstance())->update(elapsed_time);

	if(collidesWithTerrain(mainCharacter))
		mainCharacter->reset();

	searchBulletCollisions();
	searchEnemyCollision();
}

void World::render(){
	camera->set();

	glDisable(GL_DEPTH_TEST);
		sky->render();
	glEnable(GL_DEPTH_TEST);
	
	for(unsigned int i = 0; i < scene.size(); ++i)
		scene[i]->render();

	for(unsigned int i = 0; i < enemyPlanes.size(); ++i){
		if(enemyPlanes[i]->isDead()) continue;
		enemyPlanes[i]->render();
	}

	mainCharacter->render();
	(BulletManager::getInstance())->render();

	hud->render(enemyPlanes,camera);
}

bool World::readTxt(std::string dir){
	text myParser;
	if ( !myParser.create( dir.c_str() ) )
		return false;

	char* w = myParser.getword();

	if(strcmp(w,"ISLAND") == 0){
		scene.resize(1);
		scene[0] = new GameObject("..\\..\\data\\island","..\\..\\data\\island.tga",Vector3(myParser.getint(),myParser.getint(),myParser.getint()));
		scene[0]->getMesh().collisionModel->setTransform(scene[0]->getMatrix().m);
	}
	else if(strcmp(w,"TERRAIN_AIRPORT") == 0){
		GameObject* g;
		myParser.getint();
		int y = myParser.getint();
		int cont = 0;
		scene.resize(9);
		for(int i = -5000; i <= 5000; i += 5000){
			for(int j = -5000; j <= 5000; j += 5000){
				g = new GameObject("..\\..\\data\\terrain_airport","..\\..\\data\\terrain_airport.tga",Vector3(i,y,j));
				g->getMesh().collisionModel->setTransform(g->getMatrix().m);
				scene[cont] = g;
				cont++;
			}
		}
		myParser.getint();
	}
	else{
		cout << "ERROR reading level: " << dir << endl;
		return false;
	}

	minDist = myParser.getint();
	w = myParser.getword();

	//Edit the following code to add new skys
	if(strcmp(w,"CIELO") == 0){
		sky = new GameObject("..\\..\\data\\cielo","..\\..\\data\\cielo.tga",Vector3(0,0,0));
	}
	else{
		cout << "ERROR reading level: " << dir << endl;
		return false;
	}
	
	enemyPlanes.resize(myParser.getint());
	myParser.getint();						//Building num

	w = myParser.getword(); //main Character plane type

	if(strcmp(w,"SPITFIRE") == 0){
		mainCharacter = new Spitfire("..\\..\\data\\spitfire_base", "..\\..\\data\\spitfire_color_spec.tga", Vector3(myParser.getint(),myParser.getint(),myParser.getint()));
	}
	else{
		cout << "ERROR reading level: " << dir << endl;
		return false;
	}

	return true;
}