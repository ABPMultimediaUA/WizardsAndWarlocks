#ifndef ICEAREA_H
#define ICEAREA_H

#include "./DamageArea.h"

class GParticle;

class IceArea: public DamageArea{
public:
	IceArea(float dam, vector3df TPosition, vector3df TScale, vector3df TRotation);
	~IceArea();

	bool Update(float deltaTime) override;

	void Contact(void* punt, EntityEnum tipo) override;
	void Deactivate();

	void SetEmisor(int emi);
	void SetPosition(vector3df TPosition) override;

protected:
	void CreateIceArea(vector3df TPosition, vector3df TScale, vector3df TRotation);

	GParticle* particle;

	int 		emisor;				// Emisor del area de danyo
	bool 		activated;			// Esta activa? Deberia seguir existiendo? Si/No
	
};

#endif