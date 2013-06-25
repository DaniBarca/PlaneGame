
#ifndef SPITFIRE_H
#define SPITFIRE_H
#include "plane.h"

class Spitfire : public Plane{
	Mesh*    back_pitch;
	Matrix44 back_pitch_m;
	float    back_pitch_r;
	float    desired_bp_r;

	Mesh*    helix;
	Texture* helix_t;
	Matrix44 helix_m;
	float    helix_r;
public:
	Spitfire(std::string meshdir, std::string texturedir, Vector3 position);
	virtual void update(double);
	virtual void render();

	virtual void Roll(std::string dir, double elapsed_time);

	virtual void hRoll(std::string dir, double elapsed_time);

	virtual void vRoll(std::string dir, double elapsed_time);
};

#endif