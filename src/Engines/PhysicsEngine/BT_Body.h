#ifndef BT_BODY_H
#define BT_BODY_H

#include <iostream>
#include <vector3d.h>
#include <ColliderMasks.h>

class btCollisionShape;
struct btDefaultMotionState;
class btRigidBody;
class btTransform;

class BT_Body{
public:

	BT_Body();
	void CreateBox(vector3df position, vector3df dimensions, float Tmass, float roz, vector3df center = vector3df(0.0f,0.0f,0.0f), CollisionTypes mask = C_NOTHING, int collideWith = 0);

	void Update();

	void ApplyCentralImpulse(vector3df);
	void SetAngularVelocity(vector3df);
	void ApplyCentralForce(vector3df);
	void Rotate(vector3df);
	void SetRotation(vector3df);

	void SetCenter(vector3df);
	void SetPosition(vector3df);
	void SetDimensions(vector3df);
	void SetLinearVelocity(vector3df);
	void SetLinearVelocity_Y(float value);
	void SetMass(float);
	void SetAngularFactor(vector3df v);
	void SetFlags(CollisionTypes mask, int collideWith);
	void SetGravity(vector3df);

	vector3df GetPosition();
	vector3df GetRotation();
	vector3df* GetDimensions();
	vector3df GetLinearVelocity();
	vector3df GetAngularVelocity();
	float GetMass();
	vector3df GetGravity();

	void AssignPointer(void*);
	void SetCollisionFlags(std::string);
	void SetCCD(float treshold, float size);

	void Erase();
	~BT_Body();

private:
	vector3df* m_position;
	vector3df* m_dimensions;
	vector3df* m_center;
	float m_Mass;
	float m_Friction;

	btCollisionShape* m_Shape;
	btDefaultMotionState* m_MotionState;
	btRigidBody* m_RigidBody;

	btTransform GetBodyTrans();
};

#endif