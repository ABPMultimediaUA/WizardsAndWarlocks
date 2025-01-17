#include "MultiMatch.h"

#include "./../Managers/NetworkManager.h"
#include "./../Managers/BulletManager.h"
#include "./../Managers/EffectManager.h"
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/StateManager.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/TrapManager.h"
#include "./../Players/Player.h"
#include "./../LevelLoader.h"


#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/MenuManager.h>
#include <GraphicEngine/GCamera.h>
#include <PhysicsEngine/BulletEngine.h>
#include <SoundEngine/SoundSystem.h>
#include <NetworkEngine/NetworkEngine.h>
#include <NetworkEngine/NetworkObject.h>

MultiMatch::MultiMatch(MultiPlayerGame* fat){
	father = fat;

	f_engine 		= BulletEngine::GetInstance();
	g_engine 		= GraphicEngine::getInstance();
	s_engine 		= SoundSystem::getInstance();
	n_engine 		= NetworkEngine::GetInstance();

	spellManager 	= SpellManager::GetInstance();
	bulletManager 	= BulletManager::GetInstance();
	effectManager 	= EffectManager::GetInstance();
	objectManager	= ObjectManager::GetInstance();
	playerManager	= PlayerManager::GetInstance();
	trapManager		= TrapManager::GetInstance();
	networkManager	= NetworkManager::GetInstance();

	// Level
	LevelLoader::LoadLevel("../assets/json/castle_final.json");

	objectManager->SetWarlockSpawn();

	// Ponemos a true el inicio de la partida de los players
	playerManager->ManageMatchStatus(true);
	gameEnded = false;
	winnerMenuCreated = false;

	if(n_engine->IsServerInit()) isServer = true;
	else if(n_engine->IsClientInit()) isServer = false;

	// Asignamos el playerOne
	playerOne = playerManager->GetPlayerOne();
	networkObject = networkManager->GetMultiGame();

	if(networkObject != nullptr) networkObject->SetBoolVar(MULTIGAME_CHANGE, false, false, false);

	// Si estaba algun menu activado lo desactivamos
	g_engine->ToggleMenu(false);
}

MultiMatch::~MultiMatch(){
	// Los motores los eliminara el StateManager
	// Los managers los eliminara el SinglePlayerManager
}

void MultiMatch::CheckIfWon(){
	Alliance whosWon = NO_ALLIANCE;

	if(isServer){
		if(objectManager->CheckIfWon() || playerManager->CheckIfWon(ALLIANCE_WIZARD)) whosWon = ALLIANCE_WIZARD;
		else if (playerManager->CheckIfWon(ALLIANCE_WARLOCK)) whosWon = ALLIANCE_WARLOCK;
		if(whosWon != NO_ALLIANCE && whosWon != ERR_ALLIANCE) MatchEnded(whosWon);
	}
}

void MultiMatch::MatchEnded(Alliance winnerAlliance){
	if(isServer) networkObject->SetIntVar(MULTIGAME_WINNER_ALLIANCE, (int)winnerAlliance, true, false);
}

bool MultiMatch::Input(){
	return false;
}

void MultiMatch::Update(float deltaTime){

	n_engine->Update();
	f_engine->UpdateWorld(deltaTime);

	if(g_engine->getActiveCamera() != nullptr){
		s_engine->Update(g_engine->getActiveCamera()->getPosition(), g_engine->getActiveCamera()->getRotation());
	}

	playerManager->UpdatePlayers(deltaTime, true);
	networkManager->Update();
	bulletManager->Update();
	spellManager->UpdateCooldown(deltaTime);
	effectManager->UpdateEffects(deltaTime);
	objectManager->Update(deltaTime);
	trapManager->Update(deltaTime);

	g_engine->UpdateReceiver();

	CheckIfWon();

	// Comprobamos si terminamos la partida
	Alliance winnerAlliance = (Alliance) networkObject->GetIntVar(MULTIGAME_WINNER_ALLIANCE);
	if(networkObject != nullptr &&  winnerAlliance != NO_ALLIANCE && winnerAlliance != ERR_ALLIANCE){

		if(!isServer && !winnerMenuCreated){
			networkObject->SetBoolVar(MULTIGAME_WINNER_ALLIANCE, (int)NO_ALLIANCE, false, false);
			winnerMenuCreated = true;

			if(playerOne != nullptr) {
				if (playerOne->GetAlliance() != winnerAlliance) father->PlayEvent("defeat");
				else father->PlayEvent("victory");
				playerOne->SetAllInput(UP);
			}

			g_engine->ToggleCameraMovement(false);
			g_engine->ToggleMenu(true);
			MenuManager::GetInstance()->CreateMenu(ENDMATCH_M, winnerAlliance);
		}
		//if(MenuManager::GetInstance()->GetCurrentMenu() == nullptr)
		father->ReturnLobby();
	}
}

void MultiMatch::Draw(){
	if(playerOne != nullptr){
		g_engine->drawAim(playerOne->GetMoving());
		playerOne->Draw();
	}
}
