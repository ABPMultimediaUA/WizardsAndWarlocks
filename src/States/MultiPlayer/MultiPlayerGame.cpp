#include "MultiPlayerGame.h"

#include "MultiLobby.h"
#include "MultiMatch.h"

#include "./../Managers/NetworkManager.h"
#include "./../Managers/BulletManager.h"
#include "./../Managers/EffectManager.h"
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/StateManager.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/TrapManager.h"
#include <SoundEngine/SoundSystem.h>
#include <GraphicEngine/MenuManager.h>
#include <PhysicsEngine/BulletEngine.h>
#include <NetworkEngine/NetworkEngine.h>
#include <NetworkEngine/Server.h>
#include "./../../Players/HumanPlayer.h"

MultiPlayerGame::MultiPlayerGame(){
	// Motores
	n_engine		= NetworkEngine::GetInstance();
	g_engine		= GraphicEngine::getInstance();
	f_engine		= BulletEngine::GetInstance();
	s_engine		= SoundSystem::getInstance();
	
	// Managers
	spellManager 	= SpellManager::GetInstance();
	bulletManager 	= BulletManager::GetInstance();
	effectManager 	= EffectManager::GetInstance();
	objectManager	= ObjectManager::GetInstance();
	trapManager		= TrapManager::GetInstance();
	playerManager	= PlayerManager::GetInstance();
	networkManager	= NetworkManager::GetInstance();

	// Creamos un nuevo MultiLobby
	m_stateGame 	= new MultiLobby(this);
	
	// Si estaba algun menu activado lo desactivamos
	g_engine->ToggleCameraMovement(false);
	g_engine->ToggleMenu(false);

	// Inicializamos nuestras variables
	debug 		= false;
	mute 		= false;
	captured 	= false;
	m_changeMode = 0;

	// Y creamos los eventos de sonido
	CreateSoundEvents();

	m_isServer = false;

	// Si es un servidor creado INGAME lo muteamos
	Server* myServer = n_engine->GetServer();
	if(myServer != nullptr){
		m_isServer = true;
		if(myServer->GetCreatedFromGame()){
			mute = true;
			SoundSystem::getInstance()->setVolume(0);
		}
	}
}

MultiPlayerGame::~MultiPlayerGame(){
	if(m_stateGame!=nullptr)delete m_stateGame;

	networkManager->EmptyObject();	
	playerManager->EmptyObject();
	trapManager->EmptyObject();
	objectManager->EmptyObject();
	effectManager->EmptyObject();
	bulletManager->EmptyObject();
	spellManager->EmptyObject();	// Tiene que eliminarse despues de el playerManager NECESARIO

	// Eliminamos las habitaciones
	g_engine->CleanRooms();

	n_engine->EndService();

	std::map<std::string, SoundEvent*>::iterator it = soundEvents.begin();
	for(; it!=soundEvents.end(); it++){
		SoundEvent* even = it->second;
		even->release();
		delete even;
	}
}

// Ponemos el modo LOBBY en el siguiente Update
void MultiPlayerGame::ReturnLobby(){
	m_changeMode = 1;
}

// Ponemos el modo PARTIDA en el sigueinte Update
void MultiPlayerGame::StartGame(){
	m_changeMode = 2;
}

void MultiPlayerGame::ChangeMode(){
	switch(m_changeMode){
		case 1:
			// Eliminamos el estado que haya actualmente
			delete m_stateGame;
			CleanGame();

			// Creamos una nueva instancia del modo al que vamos a entrar
			m_stateGame = new MultiLobby(this);
			playerManager->RestartMatchStatus();

			// Reinicializamos el changeMode
			m_changeMode = 0;
			break;
		case 2:
			// Eliminamos el estado que haya actualmente
			delete m_stateGame;
			CleanGame();

			// Creamos una nueva instancia del modo al que vamos a entrar
			m_stateGame = new MultiMatch(this);
			playerManager->InitGame();

			// Reinicializamos el changeMode
			m_changeMode = 0;
			break;
	}
}

void MultiPlayerGame::CleanGame(){
	// Limpiamos los objetos
	objectManager->ClearMap();
	// Limpiamos los proyectiles
	bulletManager->DeleteAllProyectiles();
	// Limpiamos las trampas
	trapManager->ClearTraps();
	// Eliminamos las habitaciones
	g_engine->CleanRooms();
	// Limpiamos los hechizos
	spellManager->ResetAllDieHechizo();
}

bool MultiPlayerGame::Input(){
	if(!g_engine->run()) return true;

	if(g_engine->IsKeyPressed(Key_Escape)){
		GraphicEngine::getInstance()->InitReceiver();
		MenuManager::GetInstance()->CreateMenu(EXIT_MATCH_M);
		HumanPlayer* hp = (HumanPlayer*) PlayerManager::GetInstance()->GetPlayerOne();
		hp->ToggleMenu(true);
		hp->SetAllInput(UP);
	}

	// DEBUG
	if(g_engine->IsKeyPressed(Key_F1)) {
		MenuManager::GetInstance()->CreateMenu(NETDEBUG_M);
		debug = !debug;
	}

	if(g_engine->IsKeyPressed(Key_F2)){
		Server* myServer = n_engine->GetServer();
		if(myServer == nullptr || (myServer != nullptr && !myServer->GetCreatedFromGame())){	
			float vol = 1;
			if(!mute) vol = 0;
			SoundSystem::getInstance()->setVolume(vol);
			mute = !mute;
		}
	}

	if(g_engine->IsKeyPressed(Key_F3)){
		captured = !captured;
		g_engine->ToggleCameraMovement(captured);
		g_engine->setCursorVisible(captured);
	}

	return m_stateGame->Input();
}

void MultiPlayerGame::Update(float deltaTime){
	if(m_changeMode != 0) ChangeMode();
	m_stateGame->Update(deltaTime);
	if(!m_isServer) MenuManager::GetInstance()->Update(deltaTime);
}


void MultiPlayerGame::Draw(){
	if(n_engine != nullptr){
		bool draw = true;
		
		if(m_isServer){
			Server* myServer = n_engine->GetServer();
			if(myServer != nullptr && myServer->GetCreatedFromGame()) draw = false;
		}

		if(draw){
			g_engine->BeginDraw();
			m_stateGame->Draw();
			if(debug) f_engine->DebugDrawWorld();
			if(!m_isServer) MenuManager::GetInstance()->Draw();
			g_engine->EndDraw();
		}
	}
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/

void MultiPlayerGame::CreateSoundEvents() {
	//Create the sound events
	SoundEvent* defeat  = s_engine->createEvent("event:/Music/Defeat");
	SoundEvent* victory = s_engine->createEvent("event:/Music/Victory");
	SoundEvent* ghosts  = s_engine->createEvent("event:/Ambience/Ghosts");
	SoundEvent* waterDrops  = s_engine->createEvent("event:/Ambience/Water Drops");

	//Store them at the map
	soundEvents["defeat"]  = defeat;
	soundEvents["victory"] = victory;
	soundEvents["ghosts"]  = ghosts;
	soundEvents["waterdrops"]  = waterDrops;
}

void MultiPlayerGame::PlayEvent(std::string event, vector3df pos) {
	s_engine->playEvent(soundEvents[event], pos);
}

void MultiPlayerGame::PlayEvent(std::string event) {
	s_engine->playEvent(soundEvents[event]);
}

