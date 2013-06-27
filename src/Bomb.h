#ifndef BOMB_H
#define BOMB_H

#include "gameobject.cpp"
#include "TextureManager.cpp"
#include "MeshManager.cpp"
#include "bullet.h"

class Bomb : public Bullet{
	Texture* texture_;
	Mesh*    mesh_;

	int speed_b;

public:
	Vector3  CollisionPoint;
	Bomb(Matrix44 position,bool thrownByPlayer,vector<GameObject*> scene, int damage = 200);
	void relife(Matrix44, bool, vector<GameObject*>);

	void update(double);
	void render();
};

#endif