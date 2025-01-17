#ifndef PLAYERPARTS_H
#define PLAYERPARTS_H

#include <vector>
#include <vector3d.h>
#include <Alliance.h>
#include "./../Entidad.h"

class GBody;
class BT_Body;
class GParticle;

class PlayerParts : public Entidad{
public:
	PlayerParts(Alliance alliance, vector3df position, vector3df scale, vector3df rotation);
	~PlayerParts();
	bool Update(float deltaTime);

protected:
	float m_force;
	float m_angularForce;
	float m_ragdollTime;
	vector3df m_rotation;
	std::vector<BT_Body*> m_physicParts;
	std::vector<GBody*> m_graphicParts;
	std::vector<GParticle*> m_particles;

	void GenerateWarlockBodyParts(vector3df bodyPos);
	void GenerateWizardBodyParts(vector3df bodyPos);
	void GenerateBodyPart(vector3df bodyPartPosition, vector3df phyisicalScale, std::string objPath);
	void GenerateForce(BT_Body* bodyPart);
	void SyncParts();


};

#endif