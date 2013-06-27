#include "World.h"

TextureManager* TextureManager::instance = NULL;
BulletManager * BulletManager ::instance = NULL;
MeshManager   * MeshManager   ::instance = NULL;
int             Entity        ::last_id  = NULL;

World::World(){
	//Cargamos las direcciones de los edificios en un vector esto hará que después podamos cargar edificios iterando el vector
	buildsDir.resize(5);
	buildDir bd;
	bd.meshdir = "..\\..\\data\\house1";
	bd.textdir = "..\\..\\data\\houses_and_windows.tga";
	buildsDir[0] = (bd);
	bd.meshdir = "..\\..\\data\\house2";
	buildsDir[1] = (bd);

	bd.meshdir = "..\\..\\data\\bunker";
	bd.textdir = "..\\..\\data\\bunkers_and_forts.tga";
	buildsDir[2] = (bd);
	bd.meshdir = "..\\..\\data\\bunker2";
	buildsDir[3] = (bd);
	bd.meshdir = "..\\..\\data\\superbunker";
	buildsDir[4] = (bd);

	level = 1;
	loadLevel("..\\..\\data\\levels\\1.txt");
}

void World::loadLevel(std::string level){
	enemyPlanes.clear();
	buildings.clear();
	scene.clear();
	(BulletManager::getInstance())->bombVector->clear();
	(BulletManager::getInstance())->bulletVector->clear();

	bool success = readTxt(level);
	assert(success);

	if(this->level == 1)
		aliveEnemies = enemyPlanes.size();
	else if(this->level == 2)
		aliveEnemies = buildings.size();

	for(unsigned int i = 0; i < enemyPlanes.size(); ++i){
		cout << i << endl;
		enemyPlanes.at(i) = new EnemyPlane("..\\..\\data\\spitfire", "..\\..\\data\\spitfire_axis_color_spec.tga");
	}

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
	Vector3 pos = g->getMatrix() * Vector3(0,0,1);
	Vector3 posb;
	if(pos.y > minDist) return false;		//Si la altura del avión es > a "minDist" descartamos que pueda haber colisión

	for(unsigned int i = 0; i < scene.size(); ++i){
		posb = scene[i]->getMatrix() * Vector3(0,0,1);
		if(abs(pos.x - posb.x) > sceneW || abs(pos.z - posb.z) > sceneW) continue;
		
		scene[i]->getMesh().collisionModel->setTransform(scene[i]->getMatrix().m);
		if(scene[i]->getMesh().collisionModel->collision(g->getMesh().collisionModel,-1,0,g->getMatrix().m))
			return true;
	}
	return false;
}

void World::searchBombCollisions(){
	//En este caso no necesitamos calcular una colisión porque la bomba ya sabe en qué punto colisionará
	//así que solo hace falta saber si ha llegado a ese punto
	//El daño del edificio se calcula según la distancia, siendo daño = (100-distancia)*2

	vector<Bomb*>* b = (BulletManager::getInstance())->bombVector;
	float dist;

	for(unsigned int i = 0; i < b->size(); ++i){
		if(b->at(i)->isDead) continue;
		if(b->at(i)->getMatrix().m[13] <= b->at(i)->CollisionPoint.y){ //Si ha llegado al punto de colisión
			b->at(i)->kill();
			for(unsigned int j = 0; j < buildings.size(); ++j){
				if(!b->at(i)->isNearerThan(buildings[j],100)) continue;
				dist = b->at(i)->distance(buildings[j]);
				buildings[j]->hurt((100-dist)*2);
			}
		}
	}
}

void World::searchEnemyCollision(){
	bool mainTrSet = false;
	//No compruebo colisiones entre 2 enemigos porque, además de improbable,
	//sería costoso e inapreciable en la mayoría de casos
	for(unsigned int i = 0; i < enemyPlanes.size(); ++i){
		if(enemyPlanes[i]->isDead())continue;
		if(mainCharacter->isNearerThan(enemyPlanes[i],20)){	//Si está cerca del personaje principal
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

void World::alertEnemyTerrainCollision(){
	Vector3 aux;
	Matrix44 maux;
	for(unsigned int i = 0; i < enemyPlanes.size(); ++i){
		if(enemyPlanes[i]->isDead()) continue;			//Si el avión no está activo lo descartamos
		maux = enemyPlanes[i]->getMatrix();
		if((maux * Vector3(0,0,1)).y > minDist) continue;//Si el avión no vuela bajo lo descartamos
		aux = maux.frontVector();

		for(unsigned int i = 0; i < scene.size(); ++i){
			if(scene[i]->getMesh().collisionModel->rayCollision(maux.getPos().v,(maux.getPos() + aux).v,false))
				enemyPlanes[i]->collisionAlert();
		}
	}
}

void World::searchBulletCollisions(){
	vector<Bullet*>* bulletVector = (BulletManager::getInstance())->bulletVector;
	Bullet* b;

	for(unsigned int i = 0; i < bulletVector->size(); ++i){
		b = bulletVector->at(i);
		if(bulletVector->at(i)->isDead) continue;

		if(bulletVector->at(i)->thrownByPlayer){
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
			continue;
		}

		if(bulletVector->at(i)->isNearerThan(mainCharacter,20)){
				bulletVector->at(i)->kill();
				mainCharacter->hurt(bulletVector->at(i)->damage);
				hud->setLife(mainCharacter->getLife());
				if(mainCharacter->getLife() <= 0){
					mainCharacter->reset();
					hud->setLife(100);
				}
				break;
		}
	}
}

void World::throwBomb(){
	if(mainCharacter->name_.compare("Bomber") == 0)
		((Bomber*)mainCharacter)->throwBomb(true,scene);
}

void World::update(double elapsed_time){
	if(this->level == 1)
		aliveEnemies = enemyPlanes.size();
	else if(this->level == 2)
		aliveEnemies = buildings.size();

	mainCharacter->update(elapsed_time);

	//Set camera behind the plane
	if(cam == 0){
		camera->center  = mainCharacter->getMatrix() * Vector3(0,5,1);
		camera->up      = mainCharacter->getMatrix().topVector();
		camera->eye     = (camera->eye - camera->center).normalize()*20 + camera->center;
	}
	if(cam == 1){
		while(enemyPlanes[enemyFollowing]->isDead()){
			enemyFollowing++;
			if(enemyFollowing >= enemyPlanes.size())
				enemyFollowing = 0;
		}
		camera->center  = enemyPlanes[enemyFollowing]->getMatrix() * Vector3(0,5,1);
		camera->up      = enemyPlanes[enemyFollowing]->getMatrix().topVector();
		camera->eye     = (camera->eye - camera->center).normalize()*20 + camera->center;
	}

	//Set sky over the camera
	sky->setPos(Vector3(camera->center.x, camera->center.y-500, camera->center.z));

	
	for(unsigned int i = 0; i < enemyPlanes.size(); ++i){
		if(enemyPlanes[i]->isDead()){
			if(level == 1) aliveEnemies--;
			continue;
		}
		enemyPlanes[i]->update(elapsed_time);
	}

	if(level==2){
	for(unsigned int i = 0; i < buildings.size(); ++i)
		if(buildings[i]->isDead())
			aliveEnemies--;
	}

	(BulletManager::getInstance())->update(elapsed_time);

	if(collidesWithTerrain(mainCharacter))
		mainCharacter->reset();

	searchBulletCollisions();
	searchEnemyCollision();
	alertEnemyTerrainCollision();
	searchBombCollisions();

	if(level == 1){
		if(aliveEnemies <= 0){
			level++;
			loadLevel("..\\..\\data\\levels\\2.txt");
		}
	}
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

	for(unsigned int i = 0; i < buildings.size(); ++i){
		if(buildings[i]->isDead()) continue;
		buildings[i]->render();
	}

	mainCharacter->render();
	(BulletManager::getInstance())->render();

	hud->render(enemyPlanes,buildings,camera);
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
				cout << i << " .. " <<  j << endl;
				g = new GameObject("..\\..\\data\\terrain_airport","..\\..\\data\\terrain_airport.tga",Vector3(i,y,j));
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
	sceneW  = myParser.getint() * 0.5;
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
	buildings.resize(myParser.getint());						//Building num

	w = myParser.getword(); //main Character plane type

	if(strcmp(w,"SPITFIRE") == 0){
		mainCharacter = new Spitfire("..\\..\\data\\spitfire_base", "..\\..\\data\\spitfire_color_spec.tga", Vector3(myParser.getint(),myParser.getint(),myParser.getint()));
	}
	else if(strcmp(w,"BOMBER") == 0){
		mainCharacter = new Bomber(Vector3(myParser.getint(),myParser.getint(),myParser.getint()));
	}
	else{
		cout << "ERROR reading level: " << dir << endl;
		return false;
	}

	int num;
	for(unsigned int i = 0; i < buildings.size(); ++i){
		num = rand() % 5;
		buildings[i] = new GameObject(buildsDir[num].meshdir, buildsDir[num].textdir,Vector3(myParser.getint(),myParser.getint(),myParser.getint()),false);
	}

	return true;
}