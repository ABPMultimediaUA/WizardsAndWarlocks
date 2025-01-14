#include "GEntity.h"
#include "GraphicEngine.h"

void GEntity::setPosition(vector3df pos){
     TOEvector3df matrix =  TOEvector3df(pos.X, pos.Y, pos.Z);
    privateNode->SetTranslate(matrix);
}

void GEntity::setRotation(vector3df rot){
     TOEvector3df matrix =  TOEvector3df(rot.X, rot.Y, rot.Z);
    privateNode->SetRotation(matrix);
}

void GEntity::Rotate(vector3df rot){
     TOEvector3df matrix =  TOEvector3df(rot.X, rot.Y, rot.Z);
    privateNode->Rotate(matrix);
}

void GEntity::setScale(vector3df scale){
     TOEvector3df matrix =  TOEvector3df(scale.X, scale.Y, scale.Z);
    privateNode->SetScale(matrix);
}

vector3df GEntity::getPosition(){
     TOEvector3df position = privateNode->GetTranslation();
    return vector3df(position.X, position.Y, position.Z);
}

vector3df GEntity::getRotation(){
     TOEvector3df rotation = privateNode->GetRotation();
    return vector3df(rotation.X, rotation.Y, rotation.Z);
}

vector3df GEntity::getScale(){
     TOEvector3df scale = privateNode->GetScale();
    return vector3df(scale.X, scale.Y, scale.Z);
}

void GEntity::updateAbsolutePosition(){
    //privateNode->updateAbsolutePosition();
}

void GEntity::setAutomaticCulling(){
    //privateNode->setAutomaticCulling(irr::scene::EAC_FRUSTUM_BOX);
}

void GEntity::Erase(){
    //IrrEngine::getInstance()->privateSManager->addToDeletionQueue(privateNode);
}