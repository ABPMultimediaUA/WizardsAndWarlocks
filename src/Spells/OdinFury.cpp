#include "OdinFury.h"

#include "./../Managers/BulletManager.h"
#include "./../Players/Player.h"
#include <SoundEngine/SoundSystem.h>
#include <cmath>

OdinFury::OdinFury(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(
	costPM, 
	tCast, 
	tCoolDown, 
	SPELL_THUNDER, 
	"./../assets/textures/HUD/Spells/SPELL_THUNDER.png",
	optHP, 
	optMP)
{
	createSoundEvent();
}

void OdinFury::Lanzar(Player* p){
	vector3df rot = p->GetRot();

	rot.X = -rot.X;

	vector3df pos = p->GetHeadPos();
	float max = 0;
	pos.X = pos.X + sin(rot.Y)*cos(rot.X)*max;
	pos.Y = pos.Y + sin(rot.X)*max;
	pos.Z = pos.Z + cos(rot.Y)*cos(rot.X)*max;

	BulletManager* bullman = BulletManager::GetInstance();
	vector3df direction = vector3df( sin(rot.Y)*cos(rot.X), sin(rot.X), cos(rot.Y)*cos(rot.X));
	bullman->AddProyectil(pos, rot, direction, p->GetId(), p->GetDamageM(), BULLET_THUNDER);
	
	playSoundEvent(voiceEvent, pos); //Play voice event
	playSoundEvent(shotEvent, pos); //Play shot event
	
	Hechizo::Lanzar(p);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void OdinFury::createSoundEvent() {
	shotEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/Furore Othinus");
	voiceEvent = SoundSystem::getInstance()->createEvent("event:/Character/Spells/Furore Othinus");
}