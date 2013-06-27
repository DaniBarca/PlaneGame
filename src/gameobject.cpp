/*
 * Dani Barca 2013
 * Arxiu que engloba tots aquells objectes del joc que tinguin una mesh i una textura
 */

#ifndef GAME_OBJECT
#define GAME_OBJECT

#include "TextureManager.cpp"
#include "MeshManager.cpp"
#include "framework.h"
#include "Entity.h"
#include <string>

class GameObject : public Entity{
protected:
	Mesh*    mesh_;		//Informació dels polígons
	Texture* texture_;	//Informació de les textures

	int life;
public:

	GameObject(std::string meshdir, std::string texturedir, Vector3 position = Vector3(0,0,0),  bool mipmapping = true)
	: Entity(position)    {
		mesh_    = (MeshManager   ::getInstance())->get(meshdir);
		texture_ = (TextureManager::getInstance())->get(texturedir, mipmapping);

		name_ = "GameObject " + id;

		life = 100;
	}

	virtual void render(){
		glPushMatrix();
		matrix_.set();

		texture_->bind();
		mesh_   ->render();
		texture_->unbind();

		glPopMatrix();
	}

	bool processAction(const std::string action, Vector3 v){
		return false;
	}

	void setPos(Vector3 v){
		matrix_.setIdentity();
		matrix_.traslateLocal(v.x,v.y,v.z);
	}

	Mesh getMesh(){ return *mesh_;}

	bool isDead(){ return life <= 0; }

	void hurt(int damage){
		life-=damage;
	}

	int getLife(){return life;}

	virtual void reset(){
		matrix_ = initial_matrix;
		life = 100;
	}
};
#endif