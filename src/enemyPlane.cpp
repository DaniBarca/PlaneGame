/* enemyPlane.cpp
 */

#include "enemyPlane.h"
#define NUMPOINTS 3
#define DISTANCE 100

EnemyPlane::EnemyPlane(std::string meshdir, std::string texturedir) : Plane(meshdir,texturedir,Vector3(rand()%10000 - 5000, rand()%2000 - 800, rand()%10000 - 5000))
{
	Entity* vaux;
	bool done;
	vaux = new Entity(Vector3(rand()%10000 - 5000, rand()%2000 - 800, rand()%10000 - 5000));
	wayPoints.push_back(vaux);

	for(int i = 0; i < NUMPOINTS; ++i){
		done = false;

		while(!done){
			vaux = new Entity(Vector3(rand()%10000 - 5000, rand()%2000 - 800, rand()%10000 - 5000));
			for(unsigned int j = 0; j < wayPoints.size(); ++j){
				if(wayPoints.at(j)->distance(vaux) > DISTANCE)
					done = true;
			}
		}
		wayPoints.push_back(vaux);
	}

	desired_speed = 50;
	friction = 0;
	roll = 0.1;

	nextWaypoint = 0;
}

void EnemyPlane::alert(){
	alertTime = SDL_GetTicks();
	alerted = true;
	desired_speed = 150;
}

void EnemyPlane::update(double elapsed_time){
	matrix_.traslateLocal(0,0,speed*elapsed_time);

	if(alerted && (SDL_GetTicks() - alertTime) * 0.001 > 30){
		alerted = false;
		desired_speed = 50;
	}

	if(isNearerThan(World::getInstance()->mainCharacter, 150)) //Si estem a prop del mainCharacter
		alert();

	if(alerted)							
		pursuit(elapsed_time);
	else
		patrol(elapsed_time);
}

void EnemyPlane::pursuit(double elapsed_time){
	//Hacemos que persiga un poco por detr�s al jugador
	goTo((World::getInstance()->mainCharacter->getMatrix() * Vector3(0,0,1) - Vector3(0,0,10)) - getMatrix()*Vector3(0,0,1), elapsed_time);
}

void EnemyPlane::patrol(double elapsed_time){
	Vector3 nextPoint = wayPoints.at(nextWaypoint)->getMatrix() * Vector3(0,0,1) - getMatrix()*Vector3(0,0,1);

	goTo(nextPoint, elapsed_time);

	if(isNearerThan(wayPoints.at(nextWaypoint), DISTANCE)){ //Si la distancia con el waypoint actual es inferior a DISTANCE
		if(nextWaypoint == NUMPOINTS)							 //Nos vamos al siguiente waypoint (o al primero)
			nextWaypoint = 0;
		else
			nextWaypoint++;
	}
}

void EnemyPlane::goTo(Vector3 nextPoint, double elapsed_time){
	nextPoint.normalize();

	//Calculamos los �ngulos y las direcciones
	float anguloa = 1 - matrix_.frontVector().dot(nextPoint);
	float angulob = 1 - matrix_.frontVector().dot(nextPoint);
	float anguloc = 1 - matrix_.topVector().dot(Vector3(0,1,0));
	float dir     = matrix_.rightVector().dot(nextPoint);
	float dir2    = matrix_.topVector()  .dot(nextPoint);
	float dir3    = matrix_.rightVector().dot(Vector3(0,1,0));

	h_roll = anguloa*2;
	v_roll = angulob*2;
	roll   = anguloc*2;

	double et_i = 1/elapsed_time;

	//Comprobamos si ya estamos casi en l�nea, estos 3 ifs evitan el efecto Parkinson
	if(anguloa < 0.002)
		h_roll = anguloa*et_i;
	if(angulob < 0.002)
		v_roll = angulob*et_i;
	if(anguloc < 0.05)
		roll   = DEGTORAD(anguloc)*et_i;

	//Y giramos:
	//Horizontalmente
	if(dir < 0)
		hRoll("RIGHT",elapsed_time);
	if(dir > 0)
		hRoll("LEFT", elapsed_time);

	//Verticalmente (pitch)
	if(dir2 > 0)
		vRoll("DOWN", elapsed_time);
	if(dir2 < 0)
		vRoll("UP", elapsed_time);

	//Y sobre nosotros, para mantenernos derechos
	if(dir3 < 0)
		Roll("RIGHT", elapsed_time);
	if(dir3 > 0)
		Roll("LEFT", elapsed_time);
}

void EnemyPlane::render(){
	GameObject::render();
	/*Matrix44 aux; aux.setIdentity();
	glColor3f(255,255,255);
	for(int i = 0; i < NUMPOINTS; ++i){
		glPushMatrix();
		wayPoints.at(i)->getMatrix().set();
		glutWireSphere(10,20,20);
		glPopMatrix();
	}

	glBegin(GL_LINE_STRIP);
		glVertex3f ((matrix_*Vector3(0,0,1)).x, (matrix_*Vector3(0,0,1)).y, (matrix_*Vector3(0,0,1)).z);
		Vector3 auxb = wayPoints.at(nextWaypoint)->getMatrix() * Vector3(0,0,1);
		glVertex3f (auxb.x, auxb.y, auxb.z);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glColor3f(255,0,0);
		glVertex3f((matrix_*Vector3(0,0,1)).x, (matrix_*Vector3(0,0,1)).y, (matrix_*Vector3(0,0,1)).z);
		Vector3 front = matrix_.frontVector();
		front.normalize();
		front = (front*10) + matrix_*Vector3(0,0,1);
		glVertex3f(front.x,front.y,front.z);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glColor3f(0,255,0);
		glVertex3f((matrix_*Vector3(0,0,1)).x, (matrix_*Vector3(0,0,1)).y, (matrix_*Vector3(0,0,1)).z);
		Vector3 right = matrix_.rightVector();
		right.normalize();
		right = matrix_*Vector3(0,0,1) + (right * 10);
		glVertex3f(right.x,right.y,right.z);
	glEnd();

	glBegin(GL_LINE_STRIP);
		glColor3f(0,0,255);
		glVertex3f((matrix_*Vector3(0,0,1)).x, (matrix_*Vector3(0,0,1)).y, (matrix_*Vector3(0,0,1)).z);
		Vector3 top = matrix_.topVector();
		top.normalize();
		top = (top*10) + matrix_*Vector3(0,0,1);
		glVertex3f(top.x,top.y,top.z);
	glEnd();*/

}
