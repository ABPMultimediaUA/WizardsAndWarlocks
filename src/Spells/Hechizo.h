#ifndef HECHIZO_H
#define HECHIZO_H

#include <iostream>
#include <Alliance.h>
#include <SpellCodes.h>

class GRect;
class Player;
class GSprite;
class SoundEvent;

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;

class Hechizo{
public:
	Hechizo(float costPM, float tCast, float tCoolDown, SPELLCODE code, std::string HUDMiniature, float optHP, float optMP);
	virtual ~Hechizo();

	void EmpezarCast();
	void ResetCooldown();
	virtual void WasteMana(Player* p, float deltaTime);
	virtual void ResetSpell();
	virtual void DieReset();
	bool ComprobarCast(float deltaTime);
	virtual void Lanzar(Player* p);
	float GetCurrentCooldown();
	void DecCooldown(float time);
	virtual bool CheckMP(float MP);

	// Utility
	float GetUtility(Player* p);
	float CalculateUtility(float value, float base);

	// Getters
	float	GetMP();
	SPELLCODE GetType();
	SoundEvent* getShotEvent();
	SoundEvent* getVoiceEvent();

	// Draw
	void DrawHUD(float initX, float initY, float size, float outline, bool current, bool disabled = false, int alliance = ALLIANCE_WARLOCK);
	void EraseHUD();

	std::string GetHUDTexturePath();
	float GetTimeCasting();
	float GetTotalCooldown();
	float GetTotalCasting();
	float GetHUDHeight();
	float GetHUDWidth();

protected:	
	float 	costePM;			// Coste de Mana del hechizo
	bool	lanzable;			// Se puede lanzar el hechizo? En Cooldown? Si/No

	float 	timeCasting;		// Tiempo que lleva casteando
	float 	casting;			// Tiempo total de casteo

	float 	currentCooldown;	// Tiempo actual de cooldown
	float	cooldown;			// Tiempo total de cooldown

	float 	optimHP;			// Coste optimo de vida para lanzar (IA)
	float 	optimMP;			// Coste optimo de mana para lanzar (IA)
	std::string HUDTexturePath; // Textura de la miniatura en el hud 

	SoundEvent* shotEvent;		//Shot sound event
	SoundEvent* voiceEvent;		//Voice sound event

	SPELLCODE type;

	//Sound Functions
	virtual void createSoundEvent();
	virtual void playSoundEvent(SoundEvent* event,vector3df pos);

	GSprite* m_bkg;
	GRect* m_rect;
	GRect* m_cast_cd;
	GSprite* m_sprite;
	
};

#endif