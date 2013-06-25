

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
public:

	Hud();

	void update();
	void render(std::vector<EnemyPlane*> enemies, Camera* camera3D);

	void setLife(float life){ characterLife = life; barLength     = (characterLife*0.5)*0.01 * WINDOW_WIDTH; }
};

#endif