#include "Entidad.h"

int Entidad::ITERADORENTIDAD = 0;

Entidad::Entidad(){
	id = ITERADORENTIDAD;
	ITERADORENTIDAD++;
}

Entidad::~Entidad(){
}

EntityEnum Entidad::GetClase(){
	return clase;
}

int Entidad::GetId(){
	return id;
}
#include <iostream>
void Entidad::Contact(void* punt, EntityEnum tipo){}
void Entidad::Interact(Player* p){}
void Entidad::ShowInteractInfo(Player* p){}
