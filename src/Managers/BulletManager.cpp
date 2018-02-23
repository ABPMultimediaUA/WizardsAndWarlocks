#include "BulletManager.h"
#include "./../Projectiles/BasicProjectile.h"
#include "./../Projectiles/FireProjectile.h"
#include "./../Projectiles/ThunderProjectile.h"
#include "./../Projectiles/PoisonBomb.h"

static BulletManager* instance = nullptr;

BulletManager* BulletManager::GetInstance(){
	static BulletManager localInstance;
	if(instance == nullptr){
		localInstance.InitObject();
		instance = &localInstance;
	}
	return instance;
}

BulletManager::BulletManager(){}

BulletManager::~BulletManager(){
	EmptyObject();
}

void BulletManager::InitObject(){}

void BulletManager::EmptyObject(){
	DeleteAllProyectiles();
	instance = nullptr;
}

void BulletManager::AddProyectil(vector3df pos, vector3df dir, int emi, float dmgMult, BULLETCODE b, EFFECTCODE bulletEffect){
	switch(b){
		case BULLET_BASIC:
			proyectiles.push_back(new BasicProjectile(pos, dir, emi, bulletEffect, dmgMult));
			break;
		case BULLET_FIRE:
			proyectiles.push_back(new FireProjectile(pos, dir, emi, dmgMult));
			break;
		case BULLET_THUNDER:
			proyectiles.push_back(new ThunderProjectile(pos, dir, emi, dmgMult));
			break;
		case POISON_BOMB:
			proyectiles.push_back(new PoisonBomb(pos, dir, emi, dmgMult));
			break;
	}
}

void BulletManager::Update(){
	DeleteProyectiles();
	int size = proyectiles.size();
	for(int i=size-1; i>=0; i--){
		proyectiles[i]->Update();
	}
}

/**
 * @brief [Mueve los objetos proyectil del vector de Update al de delete]
 * @details [long description]
 */
void BulletManager::AddToDeleteProyecil(Projectile* pro){
	// Comprueba que el proyectil no sea nullptr
	if(pro!=nullptr){
		// Nos guardamos el tamanyo el vector
		int size = proyectiles.size();
		// Recorremos el vector de proyectil en update
		for(int i=size-1; i>=0; i--){
			Projectile* p = proyectiles[i];
			// Comprobamos si el proyectil esta en el vector
			if(p == pro){
				// En el caso de que este lo borramos del primer vector y lo anyadimos en el de eliminar
				proyectiles.erase(proyectiles.begin()+i);
				proyectilesToDelete.push_back(pro);
				// Salimos del bucle
				break;
			}
		}
	}
}

/**
 * @brief [Eliminamos todos los proyectiles que esten en el vector]
 * @details [long description]
 */
void BulletManager::DeleteProyectiles(){
	// Nos guardamos el tamanyo del vector
	int size = proyectilesToDelete.size();
	// Recorremos el vector de proyectiles por eliminar
	for(int i=0; i<size; i++){
		// Cargamos el proyectil a eliminar
		Projectile* p = proyectilesToDelete[i];
		// Lo eliminamos del mundo de Irrlicht y Bullet3D
		// Delete del proyectil
		delete p;
	}
	// Limpiamos el vector de proyectiles por eliminar
	proyectilesToDelete.clear();
}

void BulletManager::DeleteAllProyectiles(){
	// =====================================
	//  PROYECTILES
	// =====================================
	int size = proyectiles.size();							// Nos guardamos el tamanyo del vector
	for(int i=0; i<size; i++){								// Recorremos el vector de proyectiles por eliminar
		Projectile* p = proyectiles[i];						// Cargamos el proyectil a eliminar
		delete p;											// Lo eliminamos del mundo de Irrlicht y Bullet3D
	}
	proyectiles.clear();									// Limpiamos el vector
	// =====================================
	// PROYECTILESTODELETE
	// =====================================
	size = proyectilesToDelete.size();					// Nos guardamos el tamanyo del vector
	for(int i=0; i<size; i++){								// Recorremos el vector de proyectiles por eliminar
		Projectile* p = proyectilesToDelete[i];				// Cargamos el proyectil a eliminar
		delete p;											// Delete del proyectil
	}
	proyectilesToDelete.clear();							// Limpiamos el vector
}
