#include "AllTasks.h"
// Engines
#include "./../../GraphicEngine/GraphicEngine.h"
// Managers
#include "./../../Managers/SpellManager.h"
#include "./../../Managers/PlayerManager.h"
// Steerings
#include "./../SteeringBehaviour/Face.h"
#include "./../SteeringBehaviour/Wander.h"
#include "./../SteeringBehaviour/Arrive.h"
#include "./../SteeringBehaviour/ObstacleAvoidance.h"
#include "./../SteeringBehaviour/LookWhereYoureGoing.h"
// IAPlaye
#include "./../../Players/AIPlayer.h"
// New information classes
#include <vector2d.h>
#include <kinematicTypes.h>
// Enums
#include <KeyStates.h>

bool DEBUG = false;

// ================================================================================================= //
//
//	SHOOT PROYECTILE BASIC
//
// ================================================================================================= //

ShootBasic::ShootBasic(){}

bool ShootBasic::run(Blackboard* bb){
	if(DEBUG) std::cout<<"shootBasic"<<std::endl;

	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		SpellManager* spellMaster = SpellManager::GetInstance();

		int status = bb->GetInt(AI_SPELL_STATUS);
		if(status==-1){status = UP;}

		if(status == UP){
			if(spellMaster->StartHechizo(0, character)){
				bb->SetInt(AI_SPELL_STATUS, PRESSED);
				return true;
			}
		}else if(status == PRESSED || status == DOWN){
			bb->SetInt(AI_SPELL_STATUS, DOWN);
			if(spellMaster->LanzarHechizo(0, character)){
				spellMaster->ResetHechizo(0, character);
				bb->SetInt(AI_SPELL_STATUS, UP);
			}
			return true;
		}

	}
	return false;
}

// ================================================================================================= //
//
//	SEND PLAYER SIGNALS
//
// ================================================================================================= //

SendPlayerSignals::SendPlayerSignals(){}

bool SendPlayerSignals::run(Blackboard* bb){
	if(DEBUG) std::cout<<"sendPlayers"<<std::endl;

	PlayerManager* masterPlayer = PlayerManager::GetInstance(); 
	masterPlayer->SendVisualSignal();	// Cambiar nombre del metodo o modo de uso
	return true;
}

// ================================================================================================= //
//
//	CHECK PLAYER HEARING
//
// ================================================================================================= //

CheckPlayerHearing::CheckPlayerHearing(){}

bool CheckPlayerHearing::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CheckPlayerHearing"<<std::endl;

	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		int number = bb->GetNumberSound(AI_PLAYER);

		if(number>0){
			bb->SetTargetSound(AI_PLAYER, AI_TARGET);
			return true;
		}
	}
	return false;
}

// ================================================================================================= //
//
//	CHECK PLAYER SIGHT
//
// ================================================================================================= //

CheckPlayerSight::CheckPlayerSight(){}

bool CheckPlayerSight::run(Blackboard* bb){
	if(DEBUG) std::cout<<"CheckPlayerSight"<<std::endl;

	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		int number = bb->GetNumberSight(AI_PLAYER);

		if(number>0){
			bb->SetTargetSight(AI_PLAYER, AI_TARGET);
			return true;
		}
	}
	return false;
}

// ================================================================================================= //
//
//	HAS ARRIVED?
//
// ================================================================================================= //

HasArrived::HasArrived(){
	arrivedTarget = 1.0f;
}

bool HasArrived::run(Blackboard* bb){
	if(DEBUG) std::cout<<"HasArrived"<<std::endl;

	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
		if(target!=NULL){


			Kinematic cKin;
        	Kinematic tKin;

        	cKin = character->GetKinematic();
        	tKin = target->kinematic;

        	// La altura a la que se encuentren los personajes para nuestro juego
        	// en el que no podran haber dos personajes a diferentes alturas da igual
        	tKin.position.Y = cKin.position.Y;

        	vector3df dir = tKin.position - cKin.position;
        	float length = dir.length();

        	if(length<arrivedTarget){
        		bb->CleanPuntero(AI_TARGET);
				bb->CleanSense(target->id);
        	}

			return true;
		}
	}
	return true;
}

// ================================================================================================= //
//
//	FACE THE TARGET
//
// ================================================================================================= //

FaceTarget::FaceTarget(){}

bool FaceTarget::run(Blackboard* bb){
	if(DEBUG) std::cout<<"FaceTarget"<<std::endl;

	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
		if(target!=NULL){

			Kinematic cKin;
        	Kinematic tKin;

        	cKin = character->GetKinematic();
        	tKin = target->kinematic;

			SteeringOutput steering = ObstacleAvoidance::GetSteering(cKin);
        	if(steering.linear.length() == 0){
        		steering = Wander::GetSteering(cKin);

        		SteeringOutput steering2 = Face::GetSteering(cKin, tKin);
        		steering.angular = steering2.angular;
        	}else{
        		SteeringOutput steering2 = Face::GetSteering(cKin, tKin);
        		steering.angular = steering2.angular;
        	}

			character->Steering2Controller(steering);

			return true;
		}
	}
	return false;
}

// ================================================================================================= //
//
//	GO TO TARGET
//
// ================================================================================================= //

GoToTarget::GoToTarget(){
	maxAcceleration = 30.0f;
}

bool GoToTarget::run(Blackboard* bb){
	if(DEBUG) std::cout<<"GoToTarget"<<std::endl;

	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
		Sense_struct* target = (Sense_struct*)bb->GetPuntero(AI_TARGET);
		if(target!=NULL){

			Kinematic cKin;
        	Kinematic tKin;

        	cKin = character->GetKinematic();
        	tKin = target->kinematic;

			SteeringOutput steering = Arrive::GetSteering(cKin, tKin);

			SteeringOutput steering2 = Face::GetSteering(cKin, tKin);
			steering.angular = steering2.angular;

			character->Steering2Controller(steering);

			return true;
		}
	}
	return false;
}

// ================================================================================================= //
//
//	DEAMBULAR
//
// ================================================================================================= //

T_Wander::T_Wander(){
	maxAcceleration = 30.0f;
}

bool T_Wander::run(Blackboard* bb){
	if(DEBUG) std::cout<<"T_Wander"<<std::endl;

	AIPlayer* character = (AIPlayer*)bb->GetPuntero(AI_CHARACTER);
	if(character!=NULL){
			Kinematic cKin;
        	Kinematic tKin;

        	cKin = character->GetKinematic();

        	SteeringOutput steering = ObstacleAvoidance::GetSteering(cKin);
        	if(steering.linear.length() == 0){
        		steering = Wander::GetSteering(cKin);
        	}else{
        		SteeringOutput steering2 = LookWhereYoureGoing::GetSteering(cKin);
        		steering.angular = steering2.angular;
        	}
        	
        	character->Steering2Controller(steering);
			return true;
	}
	return false;
}