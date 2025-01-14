#ifndef ROOMGRAPH_H
#define ROOMGRAPH_H

#include <iostream>
#include <vector>

class RoomInfo;

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;

class RoomGraph{
public:
	RoomGraph();
	RoomGraph(RoomGraph* graph);
	~RoomGraph();
	void ChangeSecurityLevel(float value);

	// Generate RoomGraph
	RoomInfo* AddRoom(int id, vector3df position, vector3df firstSide, vector3df secondSide);
	bool AddConnection(int first, int second);
	void CopyGraph(RoomGraph* copyGraph);

	void InitRoom(vector3df pos, float deltaTime);

	// ACTUAL ROOMINFO
	vector3df RoomPos();
	bool RoomExplored();
	vector3df WhereExplore(vector3df pos);
	void UpdateExplore(vector3df pos);
	vector3df GetFirstCorner();
	vector3df GetSecondCorner();

	bool IntoSameRoom(vector3df pos);
	bool IntoNextRoom(vector3df pos);

	// NEXT ROOM
	RoomInfo* GetUnexploredRoom();
	bool NextRoom();
	vector3df NextRoomPos();
	vector3df GetEscapeRoom(vector3df player, vector3df target);
private:
	void ShuffleVector();
	bool CheckInside(float A, float B, float C);

	std::vector<RoomInfo*>	m_rooms;
	RoomInfo*				m_actualRoom;
	RoomInfo*				m_nextRoom;
};

#endif