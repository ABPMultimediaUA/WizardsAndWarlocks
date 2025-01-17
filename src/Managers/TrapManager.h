#ifndef TRAPMANAGER_H
#define TRAPMANAGER_H

#include <map>
#include <vector>
#include <TrapCodes.h>

class Trap;
class GRect;
class Player;
class GSprite;

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;

class TrapManager{
	friend class PlayerHUD;
public:
	static TrapManager* GetInstance();
	~TrapManager();
	void InitObject();
	void EmptyObject();

	void Update(float deltaTime);
	void DeleteTrap(Trap* t);
	
	void AddTrapToPlayer(Player*,TrapEnum);
	bool PlayerDeployTrap(Player*, vector3df, vector3df);

	Trap* AddTrap(vector3df pos,vector3df normal, TrapEnum type);
	bool DeployTrap(TrapEnum type,vector3df Start, vector3df End, int playerId);
	void UpdateTrap(float deltaTime);

	int getPlayerUsings(Player*);
	TrapEnum getPlayerTrap(Player*);
	std::string GetPathFromEnum(TrapEnum tKind);
	std::vector<Trap*> GetAllTraps();

	bool setPlayerUsings(Player*, int);
	bool setPlayerTrap(Player*, TrapEnum, bool broadcast = true);

	void ClearTraps();
	void SendAllSignal();

	// FOR NETWORK
	void DirectDeploy(int playerId, vector3df position, vector3df normal, int id);
	void NoPlayerDeploy(vector3df position, vector3df normal, TrapEnum type, int id);

	void IdErase(int id);
	Trap* GetTrapWithId(int id);

	void ErasePlayer(Player* player);
	void RefreshServerAll();

	std::map<std::string, TrapEnum> GetTRAPENUM_StrMap();
	std::string GetTrapEffect(TrapEnum trap);
	std::string GetTrapName(TrapEnum trap);
	std::string GetTrapDescription(TrapEnum trap);
	float GetTrapDamage(TrapEnum trap);

private:
	void loadJSONTrapData();

	std::vector<Trap*> traps;
	std::map<Player*,TrapEnum> playerTrap;
	std::map<Player*, int> playerUsings;
	int lastTrapId;
	int MaxUsings = 4;

	std::map<TrapEnum,std::string> trap_EFFECT;
	std::map<TrapEnum,std::string> trap_name;
	std::map<TrapEnum,std::string> trap_description;
	std::map<TrapEnum,float> trap_damage;

	GSprite* trapHud;
	GSprite* numberTrap;
	GRect*	 rectHud;

	std::string GetPathFromUsings(int usings);
	TrapManager();
	TrapManager(TrapManager&);
	TrapManager operator =(TrapManager&);
};

#endif