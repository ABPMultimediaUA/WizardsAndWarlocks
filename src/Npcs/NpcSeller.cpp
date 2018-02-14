#include "./NpcSeller.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/TrapManager.h"

NpcSeller::NpcSeller(vector3df TPosition, vector3df TTScale, vector3df TRotation):Npc(){
	CreatePhysical(TPosition, TTScale, TRotation);
	hp = NULL;
}

NpcSeller::~NpcSeller(){
}

void NpcSeller::CreatePhysical(vector3df TPosition, vector3df TScale, vector3df TRotation){
	GraphicEngine* engine = GraphicEngine::getInstance();
	clase = EENUM_NPC;

	gBody = engine->addObjMeshSceneNode("./../assets/modelos/npc.obj");
	gBody->setPosition(TPosition);
	gBody->setScale(TScale);
	gBody->setRotation(TRotation);
	gBody->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
	if (gBody) {
		gBody->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
        gBody->setMaterialTexture(0, "./../assets/textures/npc.png");
    }

	vector3df HalfExtents(TScale.X * 0.15f, TScale.Y * 0.35, TScale.Z * 0.15f);
	pBody = new BT_Body();
	pBody->CreateBox(TPosition, HalfExtents,0,0,vector3df(0,0,0), C_NPC, npcCW);
	pBody->SetRotation(TRotation);
	pBody->AssignPointer(this);
}

void NpcSeller::Interact(Player* p){
	if(p->IsPlayerOne()){
		GraphicEngine::getInstance()->InitReceiver();
		MenuManager::GetInstance()->CreateMenu(SELLER_M);
		active = true;
		hp = (HumanPlayer*) p;
		hp->ToggleMenu(true);
		hp->SetAllInput(UP);
	}
}

/*
void NpcSeller::StopInteraction(){
	if(active){
		ChangeChangingSpell(1);
		MenuManager::GetInstance()->ClearMenu();
		if(hp != NULL){
			hp->ToggleMenu(false);
			hp = NULL;
			active = false;
		}
	}
}*/