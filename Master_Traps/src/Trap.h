#ifndef TRAP_H
#define TRAP_H

#include "vector3d.h"
#include "./PhysicsEngine/BT_GhostObject.h"

enum TrapEnum: int  {
    TENUM_DEATH_CLAWS = 0,
    TENUM_SPIRITS = 1,
    TENUM_SILENCE = 2,
    TENUM_TAXES = 3,
    TENUM_DISTURBANCE = 4
};


class Trap{
public:

    Trap();
    Trap(vector3df, vector3df,TrapEnum type);

    void Deactivate();
    void Activate();

    void SetPosition(vector3df);
    void SetDimensions(vector3df);
    void SetType(TrapEnum);

    vector3df* GetPosition();
    vector3df* GetDimensions();
    TrapEnum GetType();

   // virtual void SetEffect() = 0;
    


protected:

    vector3df* m_position;
    vector3df* m_dimensions;
    //clock m_deactivation;
    bool m_deactivated;
    BT_GhostObject* m_body;
    //effect m_effect;
    //string m_texturePath;
    TrapEnum m_trapType;

};

#endif