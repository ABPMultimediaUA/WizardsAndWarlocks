#ifndef ENTIDAD_H
#define ENTIDAD_H

enum EntityEnum{
	EENUM_PLAYER 		= 0x01,		//1
	EENUM_PROJECTILE 	= 0x02,		//2
	EENUM_TRAP 			= 0x03,		//3
	EENUM_SWITCH 		= 0x04,		//4
	EENUM_DOOR 			= 0x05,		//5
	EENUM_POTION 		= 0x06,		//6
	EENUM_FOUNTAIN 		= 0x07,		//7
	EENUM_FLOOR 		= 0x08,		//8
	EENUM_NPC 			= 0x09,		//9
	EENUM_INVOCATION 	= 0x0A,		//10
	EENUM_PROP			= 0x0B,		//11
	EENUM_CAMERA		= 0x0C,		//12
	EENUM_DAMAGEAREA 	= 0x0D,		//13
	EENUM_GRAIL			= 0x0E		//14
};

class Player;

class Entidad{
public:
	Entidad();
	virtual void Contact(void* punt, EntityEnum tipo);
	virtual void Interact(Player* p);
	virtual void ShowInteractInfo(Player* p);
	EntityEnum GetClase();
	int GetId();
	virtual ~Entidad();
protected:
	static int ITERADORENTIDAD;
	EntityEnum clase;
	int id;
private:

};

#endif