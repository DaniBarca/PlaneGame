/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	The Mesh contains the info about how to render a mesh and also how to parse it from a file.
*/

#ifndef MESH_H
#define MESH_H

#include <vector>
#include "framework.h"
#include "text.h"
#include "coldet/coldet.h"

class Mesh{
public:
	std::vector< Vector3 > vertices; //here we store the vertices
	std::vector< Vector3 > normals;	 //here we store the normals
	std::vector< Vector2 > uvs;	 //here we store the texture coordinates

	std::vector< Vector3 > bounds;
	float boundsRadius;

	int primitive; //used to tell which primitive to use when rendering (quad, points, lines)
	bool init;

	Vector3 bound_center, bound_halfsize;

	Mesh();
	void clear();
	void render();
	void renderdebug();
	void createPlane(float size);
	void getBounds();
	void renderBounds();

	bool meshdefitxer(char *ase, char *bin); //Si el binari del ase no existeix, crea el binari
	bool loadASE(char *dir); //Carrega ase a partir d'un binari
};

#endif