#ifndef NODERECORD_H
#define NODERECORD_H

class Node;
class Connection;

//This structure is used to keep track of the information we need for each node
struct NodeRecord{
public:
	NodeRecord() : 
		m_node(nullptr),
		m_connection(nullptr),
		m_costSoFar(0),
		m_estimatedTotalCost(0),
		m_heuristic(0)
	{
	}
	~NodeRecord(){
		// Los nodos se eliminan con el navmesh
		// Las conexiones se eliminan en otro sitio
	}
    Node* m_node;
    Connection *m_connection;
    float m_costSoFar;
    float m_estimatedTotalCost;
    float m_heuristic;
};

#endif