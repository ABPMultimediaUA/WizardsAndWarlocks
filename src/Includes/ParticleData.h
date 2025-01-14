#ifndef PARTICLEDATA_H
#define PARTICLEDATA_H

#include <GraphicEngine/Particles/TrapParticle.h>
#include <GraphicEngine/Particles/InvisibleParticle.h>
#include <GraphicEngine/Particles/EffectParticle.h>
#include <GraphicEngine/Particles/ProjectileParticle.h>
#include <GraphicEngine/Particles/PoisonParticle.h>
#include <GraphicEngine/Particles/BloodParticle.h>
#include <GraphicEngine/Particles/WindParticle.h>
#include <GraphicEngine/Particles/GrailParticle.h>
#include <GraphicEngine/Particles/FountainParticle.h>

enum PARTICLE_TYPE{
	ERR_PARTICLE		= -1,
    TRAP_PARTICLE		=  1,
	INVISIBLE_PARTICLE	=  2,
	PROJECTILE_PARTICLE	=  3,
	POISON_PARTICLE		=  4,
	EFFECT_PARTICLE		=  5,
	WIND_PARTICLE		=  6,
	BLOOD_PARTICLE		=  7,
	GRAIL_PARTICLE		=  8,
	FOUNTAIN_PARTICLE	=  9
};

#endif