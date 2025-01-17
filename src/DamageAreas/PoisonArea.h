#ifndef POISONAREA_H
#define POISONAREA_H

#include <Alliance.h>
#include "./DamageArea.h"

class GParticle;

class PoisonArea: public DamageArea{
public:
	PoisonArea(float dam, vector3df TPosition, vector3df TScale, vector3df TRotation);
	~PoisonArea();

	bool Update(float deltaTime) override;
	void Contact(void* punt, EntityEnum tipo) override;

	void SetAlliance(Alliance emi);

protected:
	void CreatePoisonArea(vector3df TPosition, vector3df TScale, vector3df TRotation);

	float 		ghostScale;		// Escala del area de danyo
	Alliance 	emisor;			// Alianza emisora del area

	GParticle* particle;
	
};

#endif