
#ifndef SPITFIRE_H
#define SPITFIRE_H
#include "plane.h"
#include "helix.cpp"
#include "pitch.cpp"

class Spitfire : public Plane{
	Mesh*    back_pitch;
	Matrix44 back_pitch_m;

	Helix helix_;
	Pitch pitch_;

public:
	Spitfire(std::string meshdir, std::string texturedir, Vector3 position);
	virtual void update(double);
	virtual void render();

	virtual void Roll(std::string dir, double elapsed_time);

	virtual void hRoll(std::string dir, double elapsed_time);

	virtual void vRoll(std::string dir, double elapsed_time);
};

#endif