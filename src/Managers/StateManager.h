#ifndef STATEMANAGER_H
#define STATEMANAGER_H

// Estados
#include "./../States/MultiPlayer/MultiPlayerGame.h"
#include "./../States/OnePlayer/SinglePlayerGame.h"
#include "./../States/MenuPrincipal.h"
#include "./../States/State.h"
// Motores
#include <PhysicsEngine/BulletEngine.h>
#include <GraphicEngine/GraphicEngine.h>
#include <NetworkEngine/NetworkEngine.h>
#include <SoundEngine/SoundSystem.h>
#include <GUIEngine/GUIEngine.h>

#include "./../ResourceManager.h"
#include <iostream>
#include <StateCodes.h>
#include <NetworkStructs.h>

class StateManager{
public:
	static StateManager* GetInstance(ServerInfo* serverInfo = nullptr);

	~StateManager();

	void PrepareStatus(State_Code status);
	bool Update();
	void LoadState(State_Code code, bool* end = nullptr);
	void CloseGame();
	float GetDeltaTime();

private:
	void UpdateDelta();

	State_Code 	preparedStatus;
	State*		currentState; 

	BulletEngine*	f_engine;
	GraphicEngine*	g_engine;
	SoundSystem*	s_engine;
	NetworkEngine*	n_engine;
	GUIEngine* 		gui_engine;

	float deltaTime;
	float timeStart;
	float minFrameTime;

	bool resourcesLoaded;

	StateManager(ServerInfo* serverInfo = nullptr);
};

#endif