#include "GParticle.h"
#include "GraphicEngine.h"
#include <NetworkEngine/NetworkEngine.h>
#include <TOcularEngine/TOcularEngine.h>
#include "./../../Managers/StateManager.h"
#include <ParticleData.h>

GParticle::GParticle(vector3df position){
	SceneManager* sm = GraphicEngine::getInstance()->privateSManager;
	TOEvector3df pos =  TOEvector3df(position.X, position.Y, position.Z);
	ps = nullptr;
	if(!NetworkEngine::GetInstance()->IsServerInit()){
		ps = sm->AddParticleSystem(pos,  TOEvector3df(0,0,0),  TOEvector3df(1,1,1));
	}
}

GParticle::~GParticle(){
	SceneManager* sm = GraphicEngine::getInstance()->privateSManager;
	sm->DeleteMesh(ps);
}

void GParticle::SetQuantityPerSecond(int particles){
	if(ps!=nullptr) ps->SetNewPerSecond(particles);
}

void GParticle::SetTexture(std::string path){
	if(ps!=nullptr) ps->SetTexture(path);
}

void GParticle::SetType(PARTICLE_TYPE type){
	if(ps==nullptr) return;
	
	switch(type){
		case TRAP_PARTICLE:
			pm = new TrapParticle();
			ps->SetManager(pm);
			break;
		case INVISIBLE_PARTICLE:
			pm = new InvisibleParticle();
			ps->SetManager(pm); 
			break;
		case PROJECTILE_PARTICLE:
			pm = new ProjectileParticle();
			ps->SetManager(pm); 
			break;
		case EFFECT_PARTICLE:
			pm = new EffectParticle();
			ps->SetManager(pm); 
			break;
		case POISON_PARTICLE:
			pm = new PoisonParticle();
			ps->SetManager(pm); 
			break;
		case BLOOD_PARTICLE:
			pm = new BloodParticle();
			ps->SetManager(pm); 
			break;
		case WIND_PARTICLE:
			pm = new WindParticle();
			ps->SetManager(pm); 
			break;
		case GRAIL_PARTICLE:
			pm = new GrailParticle();
			ps->SetManager(pm); 
			break;
		case FOUNTAIN_PARTICLE:
			pm = new FountainParticle();
			ps->SetManager(pm); 
			break;
		default:;
	}
}

void GParticle::SetPos(vector3df position){
	TOEvector3df pos =  TOEvector3df(position.X, position.Y, position.Z);
	if(ps!=nullptr) ps->SetTranslateSmooth(pos);
}

void GParticle::SetPosSpecial(vector3df position){
	TOEvector3df pos =  TOEvector3df(position.X, position.Y, position.Z);
	if(ps!=nullptr) ps->SetTranslate(pos);
}

void GParticle::SetRot(vector3df rotation){
	TOEvector3df rot =  TOEvector3df(rotation.X, rotation.Y, rotation.Z);
	if(ps!=nullptr) ps->SetRotation(rot);
}

void GParticle::Update(){
	float dt = StateManager::GetInstance()->GetDeltaTime();
	if(ps!=nullptr) ps->Update(dt);
}

ParticleManager* GParticle::GetPM(){
	return pm;
}