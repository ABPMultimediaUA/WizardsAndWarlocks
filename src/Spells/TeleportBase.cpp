#include "TeleportBase.h"

#include "./../Managers/ObjectManager.h"
#include "./../Invocations/BaseT.h"
#include "./../Players/Player.h"
#include <SoundEngine/SoundSystem.h>
#include <cmath>

TeleportBase::TeleportBase(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(costPM, tCast, tCoolDown, SPELL_TELEPORTBASE,"./../assets/textures/HUD/Spells/SPELL_TELEPORTBASE.png", optHP, optMP){
	createSoundEvent();
	base = nullptr;
	justPlaced = false;
	
}

TeleportBase::~TeleportBase(){
	if(base!=nullptr){
		base->Deactivate();
		base = nullptr;
	}
	if (spawnEvent != nullptr) {
		if (spawnEvent->isPlaying()) spawnEvent->stop();
		spawnEvent->release();
		delete spawnEvent;
	}
}

void TeleportBase::DieReset(){
	if(base!=nullptr){
		base->Deactivate();
		base = nullptr;
		Hechizo::DieReset();
	}
}

void TeleportBase::WasteMana(Player* p, float deltaTime){
	if(base == nullptr){
		p->ChangeMP(costePM);
	}
}

bool TeleportBase::CheckMP(float MP){
	if(base!=nullptr){
		return true;
	}
	return -costePM<=MP;
}

void TeleportBase::Lanzar(Player* p){	// Estaria bien que se pusiera justo en el suelo
	if(base == nullptr){
		vector3df rot = p->GetRot();
		rot.X = 0; rot.Y = 0;
		vector3df pos = p->GetPos();

		playSoundEvent(spawnEvent, pos);	
		playSoundEvent(voiceEvent, pos); //Play the voice event

		base = (BaseT*)ObjectManager::GetInstance()->AddInvocation(pos, vector3df(1,1,1), rot, INVO_TELEPORT);
		justPlaced = true;
	}
	else{
		vector3df pos = base->GetPosition();

		playSoundEvent(shotEvent, pos);		//Play the teleport sound

		p->SetPosition(pos);

		base->Deactivate();
		base = nullptr;
		justPlaced = true;
	}
	Hechizo::Lanzar(p);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void TeleportBase::createSoundEvent() {
	shotEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/Teleport");
	voiceEvent = SoundSystem::getInstance()->createEvent("event:/Character/Spells/Teleport Base");
	spawnEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/Teleport Base");
}

