#include "TrapManager.h"
#include <vector3d.h>

TrapManager* TrapManager::instance = 0;

TrapManager* TrapManager::GetInstance(){
	if(instance==0){
		instance = new TrapManager();
	}
	return instance;
}

void TrapManager::Update(float deltaTime){
	UpdateTrap(deltaTime);
}

TrapManager::TrapManager(){}

TrapManager::~TrapManager(){
	ClearTraps();
	playerTrap.clear();
	playerUsings.clear();
}

void TrapManager::ClearTraps(){
	int size = traps.size();
	for(int i=0; i<size; i++){
		Trap* t = traps[i];
		delete t;
	}
	traps.clear();
}

Trap* TrapManager::AddTrap(vector3df pos, vector3df normal, TrapEnum type){
	Trap* t = new Trap(pos,normal, type);
	traps.push_back(t);
	return t;
}

void TrapManager::AddTrapToPlayer(Player* player, TrapEnum trap){
	TrapEnum actualTrap = getPlayerTrap(player);
	if(actualTrap != TENUM_NO_TRAP){ //si el jugador ya tiene una trampa asignada se cambia por una nueva
		//std::cout<<"I found the player!!"<<std::endl;
		setPlayerTrap(player,trap);
		setPlayerUsings(player,MaxUsings);
	}
	else{//si no la tiene se la asignamos
		//std::cout<<"Creating a new player..."<<std::endl;
		playerTrap.insert(std::pair<Player*, TrapEnum>(player,trap));
		playerUsings.insert(std::pair<Player*, int>(player,MaxUsings));
	}
}

bool TrapManager::PlayerDeployTrap(Player* player,vector3df Start, vector3df End){
	//std::cout<<"Checking usings..."<<std::endl;
	int uses = getPlayerUsings(player);
	//std::cout<< uses << " uses"<<std::endl;
	if(uses == 0) return false;
	if(DeployTrap(getPlayerTrap(player),Start,End)){
		--uses;
		//std::cout<<"You have "<<uses<<" more uses"<<std::endl;
		setPlayerUsings(player,uses);
		return true;
	}
	return false;
}

bool TrapManager::DeployTrap(TrapEnum type,vector3df Start, vector3df End){
	vector3df point(0,0,0);
	vector3df normal(0,0,0);
	GraphicEngine::getInstance()->Raycast(Start,End, &point, &normal);

	if(!(normal.X == 0 && normal.Y != 0 && normal.Z == 0) //paredes
	&& !(normal.X == 0 && normal.Y == 0 && normal.Z == 0)
	&& !(normal.X == 90 && normal.Y == 0 && normal.Z == 0)
	&& (normal.Y >=0 && normal.Y <=180)
	){
		AddTrap(point,normal,type);
		return true;
	}

	return false;
}


void TrapManager::DeleteTrap(Trap* trap){
	int size = traps.size();
	for(int i=size-1; i>=0; i--){
		Trap* t = traps[i];
		if(t == trap){
			traps.erase(traps.begin() + i);
			delete t;
		}
	}
}

void TrapManager::UpdateTrap(float deltaTime){
	int size = traps.size();
	std::cout<<size<<std::endl;
	for(int i=0; i<size; i++){
		Trap* t = traps[i];
		t->Update(deltaTime);
	}
}

TrapEnum TrapManager::getPlayerTrap(Player* player){
	std::map<Player*, TrapEnum>::iterator it = playerTrap.begin();
	for(;it!=playerTrap.end();++it){
		if(it->first == player){
			return it->second;
		}
	}
	return TENUM_NO_TRAP;
}

int TrapManager::getPlayerUsings(Player* player){
	std::map<Player*, int>::iterator it = playerUsings.begin();
	for(;it!=playerUsings.end();++it){
		if(it->first == player){
			return it->second;
		}
	}
	return -1;
}

bool TrapManager::setPlayerTrap(Player* player, TrapEnum trap){
	std::map<Player*, TrapEnum>::iterator it = playerTrap.begin();
	for(;it!=playerTrap.end();++it){
		if(it->first == player){
			it->second = trap;
			return true;
		}
	}
	return false;
}
bool TrapManager::setPlayerUsings(Player* player,int uses){
	std::map<Player*, int>::iterator it = playerUsings.begin();
	for(;it!=playerUsings.end();++it){
		if(it->first == player){
			it->second = uses;
			return true;
		}
	}
	return false;
}