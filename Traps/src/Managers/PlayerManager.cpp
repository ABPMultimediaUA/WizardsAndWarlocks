#include "PlayerManager.h"

PlayerManager* PlayerManager::instance = 0;

PlayerManager* PlayerManager::GetInstance(){
	if(instance==0){
		instance = new PlayerManager();
	}
	return instance;
}

PlayerManager::PlayerManager(){}

PlayerManager::~PlayerManager(){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		delete p;
	}
	players.clear();
}

Player* PlayerManager::AddPlayer(bool one){
	Player* p = new Player(one);
	players.push_back(p);
	return p;
}

void PlayerManager::UpdatePlayers(){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		p->Update();
	}
}