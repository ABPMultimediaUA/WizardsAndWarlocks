#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "Player.h"

class HumanPlayer: public Player{
public:
	HumanPlayer(bool isPlayer1 = true);
	~HumanPlayer();

	void CheckInput();

	void GetNetInput();
	void SetNetInput();

	void Update(float deltaTime);

	void ToggleMenu(bool newState);

private:
	bool menuActivated;
	bool m_interacting;
	void LookingAtObject();
	float showObjInfo;

};

#endif