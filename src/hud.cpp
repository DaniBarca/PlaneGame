
#include "hud.h"

Hud::Hud(){
	characterLife = 100;
	barLength     = (characterLife*0.5)*0.01*WINDOW_WIDTH;
	
	camera2D = new Camera();
	camera2D->setOrthographic(0,WINDOW_WIDTH,0,WINDOW_HEIGHT,0,1);

	cameraWidth = 0.1;
	cameraHeight= 0.1;
}

void Hud::render(std::vector<EnemyPlane*> enemies, Camera* camera3D){
	GLdouble model_view[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, model_view);

	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	camera2D->set();

	glDisable(GL_DEPTH_TEST);

	glLineWidth(2);
	glColor3f(255,255,255);
	glBegin(GL_LINE_LOOP);
		glVertex2f(0.02 * WINDOW_WIDTH,0.02 * WINDOW_HEIGHT);
		glVertex2f(0.5  * WINDOW_WIDTH,0.02 * WINDOW_HEIGHT);
		glVertex2f(0.5  * WINDOW_WIDTH,0.05 * WINDOW_HEIGHT);
		glVertex2f(0.02 * WINDOW_WIDTH,0.05 * WINDOW_HEIGHT);
	glEnd();

	glBegin(GL_QUADS);	
		glColor3f(255,0,0);
		glVertex2f(0.02 * WINDOW_WIDTH , 0.02 * WINDOW_HEIGHT); //Top    left
		glVertex2f(0.02 * WINDOW_WIDTH , 0.05 * WINDOW_HEIGHT);  //Bottom left
		glVertex2f(barLength  ,			 0.05 * WINDOW_HEIGHT);  //Bottom right
		glVertex2f(barLength  ,			 0.02 * WINDOW_HEIGHT); //Top    right
	glEnd();

	Vector3  pos = Vector3();
	Vector3 spos = Vector3();
	GLdouble x,y,z;
	Matrix44 mat = Matrix44();
	glPointSize(8);

	for(unsigned int i = 0; i < enemies.size(); ++i){
		if(enemies[i]->isDead()) continue;

		mat = enemies[i]->getMatrix();
		pos = Vector3(mat.m[12],mat.m[13],mat.m[14]);

		gluProject(pos.x,pos.y,pos.z, model_view,projection,viewport,&x,&y,&z);
		y=WINDOW_HEIGHT-y;

		if(z>1){
			x = -1;
		}

		if(x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT){
			if(x <= 0)
				x = 0.01 * WINDOW_WIDTH;
			if(x >= WINDOW_WIDTH)
				x = 0.99 * WINDOW_WIDTH;
			if(y <= 0)
				y = 0.01 * WINDOW_HEIGHT;
			if(y >= WINDOW_HEIGHT)
				y = 0.99 * WINDOW_HEIGHT;

			glBegin(GL_POINTS);
				glVertex2f(x,y);
			glEnd();
		}
		else{
			if(z<1){
				glBegin(GL_LINE_LOOP);
					glVertex2f(x-5,y-5);
					glVertex2f(x+5,y-5);
					glVertex2f(x+5,y+5);
					glVertex2f(x-5,y+5);
				glEnd();
			}
		}
	}

	glEnable(GL_DEPTH_TEST);
}