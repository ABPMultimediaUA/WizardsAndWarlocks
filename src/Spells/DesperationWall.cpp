#include "DesperationWall.h"
#include "./../Managers/ObjectManager.h"

DesperationWall::DesperationWall(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(costPM, tCast, tCoolDown, SPELL_WALL,"./../assets/textures/HUD/Spells/SPELL_WALL.png", optHP, optMP){
	
}
	
void DesperationWall::Lanzar(Player* p){	// Estaria bien que se pusiera justo en el suelo
	ObjectManager* objectMaster = ObjectManager::GetInstance();
	vector3df pos = p->GetPos();
	vector3df rot = p->GetRot();

	float dist = 1.75f;
	pos.X = pos.X + sin(rot.Y)*dist;
	pos.Z = pos.Z + cos(rot.Y)*dist;

	rot.Z = 0.0f; rot.X = 0.0f;
	rot.Y = rot.Y * 180 / M_PI;

	pos.Y += 0.25;
	objectMaster->AddInvocation(pos, vector3df(1.75,2.0,0.25), rot, INVO_WALL);

	Hechizo::Lanzar(p);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/