#ifndef GPARTICLE_H
#define GPARTICLE_H

#include "GEntity.h"
#include <iostream>
#include <ParticleData.h>

// Fast Forward Declaration
class TFParticleSystem;
class ParticleManager;

class GParticle: public GEntity{
	friend class GraphicEngine;

public:
	GParticle(vector3df position);
	~GParticle();

	void SetQuantityPerSecond(int particles);
	void SetTexture(std::string path);
	void SetType(PARTICLE_TYPE type);
	void SetPos(vector3df position);
	void SetPosSpecial(vector3df position);
	void SetRot(vector3df rotation);

	void Update();

	ParticleManager* GetPM();
	
private:
	TFParticleSystem* ps;
	ParticleManager* pm;
	
};

#endif