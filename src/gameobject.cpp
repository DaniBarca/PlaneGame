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

	int priority;
public:

	GameObject(std::string meshdir, std::string texturedir, Vector3 position = Vector3(0,0,0),  bool mipmapping = true, bool scale = false)
	: Entity(position)    {
		mesh_    = (MeshManager   ::getInstance())->get(meshdir);
		texture_ = (TextureManager::getInstance())->get(texturedir, mipmapping);

		priority = 1;

		name_ = "GameObject " + id;

		if(scale){
			matrix_.m[0] =  2;
			matrix_.m[5] =  1;
			matrix_.m[10]=  2;
		}
	}

	virtual void render(){
		glPushMatrix();	
		matrix_.set();


		//mesh_->renderBounds();

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

	int getPriority() const	{ return priority;}

	Mesh getMesh(){ return *mesh_;}
};
#endif