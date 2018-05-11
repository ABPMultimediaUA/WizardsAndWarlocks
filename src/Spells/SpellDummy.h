#ifndef SPELLDUMMY_H
#define SPELLDUMMY_H

#include "./Hechizo.h"

class SpellDummy: public Hechizo{
public:
	SpellDummy(float costPM, float tCast, float tCoolDown, float optHP, float optMP);
	~SpellDummy();
	void Lanzar(Player* p);

protected:	
	void createSoundEvent();
private:
	SoundEvent* spawnEvent;
};


#endif
