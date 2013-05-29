#include "mesh.h"
#include <cassert>
#include "includes.h"
#include <iostream>


typedef struct {
		unsigned int size_vertices;
		unsigned int size_uvs;
		unsigned int size_normals;
		unsigned int size_bounds;
} DataMesh;

Mesh::Mesh()
{
	primitive = GL_TRIANGLES;
	init = false;
}

void Mesh::clear()
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	bounds.clear();
}

void Mesh::getBounds(){
	if(init) {

	Vector3 max = vertices.at(0);
	Vector3 min = vertices.at(0);

	for(int i = 0; i < vertices.size(); ++i){
		if(vertices.at(i).x > max.x) max.x = vertices.at(i).x;
		if(vertices.at(i).y > max.y) max.y = vertices.at(i).y;
		if(vertices.at(i).z > max.z) max.z = vertices.at(i).z;

		if(vertices.at(i).x < min.x) min.x = vertices.at(i).x;
		if(vertices.at(i).y < min.y) min.y = vertices.at(i).y;
		if(vertices.at(i).z < min.z) min.z = vertices.at(i).z;
	}

	bound_halfsize = (min.length() > max.length()) ? min : max;

	bound_center.x = (min.x + max.x)*0.5;
	bound_center.y = (min.y + max.y)*0.5;
	bound_center.z = (min.z + max.z)*0.5;
	std::cout << "BOUNDS: Center x" << bound_center.x << " y " << bound_center.y << " z " << bound_center.z << std::endl;
	}
	init = false;
}

bool Mesh::meshdefitxer(char *ase, char *bin) 
{
	text my_parser;
	if ( !my_parser.create( ase ) )
		return 0;
	
	//Max i min per a la bound
	Vector3 max;
	Vector3 min;
	
	//Carrega Vertex+Cares
	my_parser.seek("*MESH_NUMVERTEX");
	int num_vertex = my_parser.getint(); 

	my_parser.seek("*MESH_NUMFACES"); 	
	int num_faces = my_parser.getint();

	std::vector< Vector3 > vertex_index; //Vector amb l'index de vertex utilitzats
	
	int aux[3];

	for (int i=0; i<num_vertex; i++ ) 
		{
			my_parser.seek("*MESH_VERTEX");
			my_parser.getint();
			double x = my_parser.getfloat();
			double z = -my_parser.getfloat();
			double y =  my_parser.getfloat();
			vertex_index.push_back( Vector3( x, y ,z  ) );

			//Components de vertex maxims i minims per a la bound!!!
			if (i == 0) {
				max.x = x;
				max.y = y;
				max.z = z;
				
				min.x = x;
				min.y = y;
				min.z = z;
			}
			else {
			if( x > max.x ) max.x = x;
		    if( y > max.y ) max.y = y;
		    if( z > max.z ) max.z = z;

		    if( x < min.x ) min.x = x;
		    if( y < min.y ) min.y = y;
		    if( z < min.z ) min.z = z;
			}
		}

	for (int i=0; i<num_faces; i++ ) //Vector final amb tots els vertex de 3
		{
			my_parser.seek("*MESH_FACE");
			
			my_parser.seek("A:");
			vertices.push_back( vertex_index[ my_parser.getint() ] );
			
			my_parser.seek("B:");
			vertices.push_back( vertex_index[ my_parser.getint() ] );
			
			my_parser.seek("C:");
			vertices.push_back( vertex_index[ my_parser.getint() ] );
	}

	//Carrega Textures+Cares
	my_parser.seek("*MESH_NUMTVERTEX");
	int num_tvertex = my_parser.getint();

	std::vector< Vector2 > vertex_tindex; //Vector amb l'index de vertex utilitzats

	for (int i=0; i<num_tvertex; i++ ) 
		{
			my_parser.seek("*MESH_TVERT");
			my_parser.getint();
			double x = my_parser.getfloat();
			double y = my_parser.getfloat();

			vertex_tindex.push_back( Vector2(x,y) );
		}
	my_parser.seek("*MESH_NUMTVFACES");
	int num_tvfaces = my_parser.getint();

	for (int i=0; i<num_tvfaces; i++ ) //Vector final amb tots els vertex de 3 en tres
		{
			my_parser.seek("*MESH_TFACE");
			my_parser.getint();
			uvs.push_back( vertex_tindex[ my_parser.getint() ] );
			uvs.push_back( vertex_tindex[ my_parser.getint() ] );
			uvs.push_back( vertex_tindex[ my_parser.getint() ] );
		}

	//Carrega Normals per cares a pel
	my_parser.seek("*MESH_NORMALS");

	for (int i=0; i<num_faces; i++ ) //Vector final amb la normal de cara i la dels 3 vertex
		{
			for (int j=0; j<3; j++) 
				{
				my_parser.seek("*MESH_VERTEXNORMAL");
				my_parser.getint();
				float x = my_parser.getfloat();
				float y = my_parser.getfloat();
				float z = my_parser.getfloat();
				normals.push_back( Vector3( x, y, z) );
			}
		}

	//Bounds (seria convenient separar-ho
	bounds.resize(2);
	bounds[0] = (min.length() > max.length()) ? min : max;


	bounds[1].x = (min.x + max.x)*0.5;
	bounds[1].y = (min.y + max.y)*0.5;
	bounds[1].z = (min.z + max.z)*0.5;

	//Crear el binari 
	DataMesh data;
	
	FILE *f1 = fopen( bin, "wb" );

	data.size_vertices = vertices.size();
	data.size_uvs = uvs.size();
	data.size_normals = normals.size();
	data.size_bounds = bounds.size();
	
	fwrite( &data, sizeof(DataMesh), 1, f1);

	fwrite( &vertices[0], sizeof(Vector3), vertices.size(), f1);

	fwrite( &uvs[0], sizeof(Vector2), uvs.size(), f1);
	
	fwrite( &normals[0], sizeof(Vector3), normals.size(), f1);
	
	fwrite( &bounds[0], sizeof(Vector3), bounds.size(), f1);
	
	fclose(f1);

	boundsRadius = bounds[0].distance(bounds[1]);
	return 1;
}

bool Mesh::loadASE(char *dir) 
{
	//Neteja
	vertices.clear();
	normals.clear();
	uvs.clear();
	bounds.clear();

	char bin[80];
	strcpy_s(bin,dir);

	strcat_s(bin, ".bin");

	//Mira si existeix fitxer
	FILE *f1 = fopen( bin, "rb" );

	if ( f1 != NULL) {
		//Carrega fitxer
		DataMesh data;
		fread(&data, sizeof(DataMesh), 1, f1);

		//carrega del fitxer bin vertices, uvs, normals
		vertices.resize(data.size_vertices);
		fread(&vertices[0], sizeof(Vector3), data.size_vertices, f1);

		uvs.resize(data.size_uvs);
		fread(&uvs[0], sizeof(Vector2), data.size_uvs, f1);

		normals.resize(data.size_normals);
		fread(&normals[0], sizeof(Vector3), data.size_normals, f1);

		bounds.resize(data.size_bounds);
		fread(&bounds[0], sizeof(Vector3), data.size_bounds, f1);

		fclose(f1);

		boundsRadius = bounds[0].distance(bounds[1]);
		return 1;
	}

	char ase[80];
	strcpy_s(ase,dir);

	strcat_s(ase, ".ase");

	if ( meshdefitxer( ase, bin ) != 0)
		return 1;
	return 0;

}

void Mesh::render()
{
	assert(vertices.size() && "No vertices in this mesh");

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0] );

	if (normals.size())
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, &normals[0] );
	}

	if (uvs.size())
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2,GL_FLOAT, 0, &uvs[0] );
	}

	glDrawArrays(primitive, 0, vertices.size() );
	glDisableClientState(GL_VERTEX_ARRAY);

	if (normals.size())
		glDisableClientState(GL_NORMAL_ARRAY);
	if (uvs.size())
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}

void Mesh::renderdebug() {
	if (normals.size() == vertices.size() ) 
		glColor3d(0,0,1);
	else glColor3d(1,1,1);

	glPointSize(3);
	glBegin (GL_LINE_STRIP);

	for (int i=0; i<vertices.size(); i++)
	{
		glVertex3f( vertices[i].x, vertices[i].y, vertices[i].z );
		glColor3f(normals[i].x, normals[i].y, normals[i].z);
		
	}
	glEnd();

}

void Mesh::createPlane(float size)
{
	vertices.clear();
	normals.clear();
	uvs.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)

	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(size,0,-size) );
	vertices.push_back( Vector3(-size,0,-size) );
	vertices.push_back( Vector3(-size,0,size) );
	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(-size,0,-size) );

	//all of them have the same normal
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );

	//texture coordinates
	uvs.push_back( Vector2(1,1) );
	uvs.push_back( Vector2(1,0) );
	uvs.push_back( Vector2(0,0) );
	uvs.push_back( Vector2(0,1) );
	uvs.push_back( Vector2(1,1) );
	uvs.push_back( Vector2(0,0) );
}

void Mesh::renderBounds(){
	glutWireSphere(boundsRadius, 20, 10);
}