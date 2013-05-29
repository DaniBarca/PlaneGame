/* Dani Barca 2013
 * Codi per a gestionar les meshes de diversos objectes en un joc evitant carregarles més d'un cop
 * Patró singleton
 */

#ifndef MESH_MANAGER
#define MESH_MANAGER

#include <assert.h>
#include <string>
#include <map>
#include "mesh.h"

class MeshManager{
	static MeshManager* instance;
	std::map<std::string, Mesh*>* meshMap;

public:
	MeshManager(){
		assert(instance == NULL);
		instance = this;
		meshMap  = new std::map<std::string,Mesh*>();
	}

	static MeshManager* getInstance(){
		if(instance == NULL){
			instance = new MeshManager();
		}
		return instance;
	}

	Mesh* get(std::string dir){
		std::map<std::string, Mesh*>::iterator it = meshMap->find(dir);

		if(it != meshMap->end()){    //La mesh ja existia
			return (it->second);
		}
		else{						//La mesh no existia, la carreguem
			Mesh* msh = new Mesh();
			
			msh->loadASE((char*)dir.c_str());

			meshMap->insert(std::pair<std::string,Mesh*>(dir,msh));			//Y la desem per tenir-la en el futur
			return msh;
		}
	}
};

#endif