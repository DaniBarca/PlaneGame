/* Dani Barca 2013
 * Codi per a gestionar les textures de diversos mesh en un joc evitant carregarles més d'un cop
 * Patró singleton
 */

#ifndef TEXTURE_MANAGER
#define TEXTURE_MANAGER

#include <assert.h>
#include <string>
#include <map>
#include "texture.h"

class TextureManager{
	static TextureManager* instance;
	std::map<std::string, Texture*>* textureMap;

public:
	TextureManager(){
		assert(instance == NULL);
		instance = this;
		textureMap = new std::map<std::string, Texture*>();
	}

	static TextureManager* getInstance(){
		if(instance == NULL){
			instance = new TextureManager();
		}
		return instance;
	}

	Texture* get(std::string dir, bool mipmapping = true){
		std::map<std::string, Texture*>::iterator it = textureMap->find(dir);

		if(it != textureMap->end()){ //La textura ya existia
			return (it->second);
		}
		else{						 //La texture no existia, la carreguem
			Texture* tx = new Texture();
			if(!tx->load(dir.c_str(), mipmapping)){
				std::cout << "ERROR AL LLEGIR TEXTURA: " << dir.c_str() << std::endl;
				exit(0);
			}
			textureMap->insert(std::pair<std::string,Texture*>(dir,tx));			//Y la desem per tenir-la en el futur
			return tx;
		}
	}
};

#endif