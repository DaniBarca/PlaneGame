

#ifndef HUD_H
#define HUD_H

#include "camera.h"
#include "includes.h"

class EnemyPlane;
#include "enemyPlane.h"

class Hud{
	Camera* camera2D;
	float cameraWidth, cameraHeight;

	float characterLife;
	float barLength;

	GLdouble model_view[16];
	GLdouble projection[16];
	GLint viewport[4];

	void localize(GameObject* go, Vector3 color);
public:

	Hud();

	void update();
	void render(std::vector<EnemyPlane*> planes, std::vector<GameObject*> buildings, Camera* camera3D);

	void setLife(float life){ characterLife = life; barLength     = (characterLife*0.5)*0.01 * WINDOW_WIDTH; }
};

#endif