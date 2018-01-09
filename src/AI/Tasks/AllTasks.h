#ifndef ALLTASKS_H
#define ALLTASKS_H

#include <iostream>

#include "./../Secuencia.h"
#include "./../Decorador.h"
#include "./../Selector.h"
#include "./../Task.h"

// ================================================================================================= //
//
//	MASTER MOVEMENT
//
// ================================================================================================= //

class MasterAction: public Task{
public:
	MasterAction();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	MASTER MOVEMENT
//
// ================================================================================================= //

class MasterMovement: public Task{
public:
	MasterMovement();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	PUT DEFAULT ACTION
//
// ================================================================================================= //

class PutDefaultAction: public Task{
public:
	PutDefaultAction();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	USE POTION
//
// ================================================================================================= //

class UsePotion: public Task{
public:
	UsePotion();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	CHECK USE POTION
//
// ================================================================================================= //

class CheckUsePotion: public Task{
public:
	CheckUsePotion();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	RELEASE SPELL IF FALSE
//
// ================================================================================================= //

class ReleaseSpell: public Decorador{
public:
	ReleaseSpell();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	SHOOT PROYECTILE BASIC
//
// ================================================================================================= //
 
 class UseSpell: public Task{
 public:
 	UseSpell();
 	bool run(Blackboard* bb);
 };

// ================================================================================================= //
//
//	SEND ALL SIGNALS
//
// ================================================================================================= //

class SendAllSignals: public Task{
public:
	SendAllSignals();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	SEND PLAYER SIGNALS
//
// ================================================================================================= //

class SendPlayerSignals: public Task{
public:
	SendPlayerSignals();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	CHECK DISTANCE
//
// ================================================================================================= //

class CheckDistance: public Task{
public:
	CheckDistance(float dist);
	bool run(Blackboard* bb);
private:
	float distance;
};

// ================================================================================================= //
//
//	CHECK PLAYER SIGHT
//
// ================================================================================================= //

class CheckPlayerSight: public Task{
public:
	CheckPlayerSight();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	CHECK PLAYER HEARING
//
// ================================================================================================= //

class CheckPlayerHearing: public Task{
public:
	CheckPlayerHearing();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	HAS ARRIVED?
//
// ================================================================================================= //

class HasArrived: public Task{
public:
	HasArrived();
	bool run(Blackboard* bb);
private:
	float arrivedTarget;
};

// ================================================================================================= //
//
//	FACE THE TARGET
//
// ================================================================================================= //

class FaceTarget: public Task{
public:
	FaceTarget();
	bool run(Blackboard* bb);
};

// ================================================================================================= //
//
//	GO TO TARGET
//
// ================================================================================================= //

class GoToTarget: public Task{
public:
	GoToTarget();
	bool run(Blackboard* bb);
private:
	float maxAcceleration;
};

// ================================================================================================= //
//
//	FLEE FROM TARGET
//
// ================================================================================================= //

class FleeFromTarget: public Task{
public:
	FleeFromTarget();
	bool run(Blackboard* bb);
private:
	float maxAcceleration;
};

// ================================================================================================= //
//
//	DEAMBULAR
//
// ================================================================================================= //

class T_Wander: public Task{
public:
	T_Wander();
	bool run(Blackboard* bb);
private:
	float maxAcceleration;
};


// ================================================================================================= //
//
//	ORDER SPELLS BY UTILITY
//
// ================================================================================================= //

class SpellSecuencia: public Selector{
public:
	SpellSecuencia();
	bool run(Blackboard* bb);
	void SortVector(Blackboard* bb);
private:
	std::vector<int> spellsOrder;
};

#endif