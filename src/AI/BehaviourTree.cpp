#include "BehaviourTree.h"

BehaviourTree::BehaviourTree(){
    informacion = new Blackboard();

    PrepareSubTrees();
    CreateReceive();
    CreateAction();
    CreateMovement();
}

BehaviourTree::~BehaviourTree(){
	delete informacion;
    int size = tasks.size();
    for(int i=0; i<size; i++){
        Task* t = tasks[i];
        delete t;
    }
    tasks.clear();
}

void BehaviourTree::run(){
    rootRecive->run(informacion);
    rootAction->run(informacion);
    rootMove->run(informacion);
}

void BehaviourTree::SetRootReceive(Task* t){
    rootRecive = t;
}

void BehaviourTree::SetRootAction(Task* t){
    rootAction = t;
}

void BehaviourTree::SetRootMove(Task* t){
    rootMove = t;
}

void BehaviourTree::ResetInformacion(){
    informacion->CleanPuntero(AI_TARGET);
    informacion->CleanSense();
}

void BehaviourTree::SetInformation(Blackboard* bb){
    informacion->SaveParent(bb);
}

void BehaviourTree::AnyadirInformacion(AI_code name, void* value){
    informacion->SetPuntero(name, value);
}

void BehaviourTree::AnyadirInformacion(AI_code name, int value){
    informacion->SetInt(name, value);
}

Blackboard* BehaviourTree::GetBlackboard(){
	return informacion;
}

void BehaviourTree::SetPlayer(AIPlayer* p){
    informacion->SetPlayer(p);
}

// ====================================================================================== //
//
//  SUBARBOLES
//
// ====================================================================================== //

void BehaviourTree::CreateReceive(){

    Selector* sc_checkActions = new Selector();
    sc_checkActions->addChild(new CheckUsePotion());
    sc_checkActions->addChild(new CheckPlayerSight());
    sc_checkActions->addChild(new CheckSawTrap());
    sc_checkActions->addChild(new CheckSawFountain());
    sc_checkActions->addChild(new CheckSawPotion());
    sc_checkActions->addChild(new PutDefaultAction());

    SetRootReceive(sc_checkActions);
}

void BehaviourTree::CreateAction(){
    Task* t_masterAction = new MasterAction();
    SetRootAction(t_masterAction);
}

void BehaviourTree::CreateMovement(){
    Task* t_masterMovement = new MasterMovement();
    SetRootMove(t_masterMovement);
}

void BehaviourTree::PrepareSubTrees(){
    // No movimiento
    CreateNoMove();
    // Movimiento Default
    CreateMoveDefault();
    // Movimiento a un target
    CreateMoveToTarget();
    // Movimiento de los Hechizos
    CreateMoveSpell();
    // Lanzamiento de los hechizos
    CreateShootSpell();
    // Coger Pocion
    CreateCathPotion();
    // Beber Pocion
    CreateDrinkPotion();
    // Usar fuente
    CreateUseFountain();
    // Desactivar Trampa
    CreateDefuseTrap();

    // DECLARANDO FUNCIONES DE ATAQUE
    Task* t_shootBasic = new UseSpell();
    informacion->SetPuntero(AI_TASK_SPELL00, t_shootBasic);
    informacion->SetPuntero(AI_TASK_SPELL01, t_shootBasic);
    informacion->SetPuntero(AI_TASK_SPELL02, t_shootBasic);

    Secuencia* sc_distance_spell = new Secuencia();
    sc_distance_spell->addChild(new CheckDistance(1.0f));
    sc_distance_spell->addChild(new UseGuivernum());
    informacion->SetPuntero(AI_TASK_SPELL03, sc_distance_spell);

    tasks.push_back(t_shootBasic);
    tasks.push_back(sc_distance_spell);
}

void BehaviourTree::CreateShootSpell(){
    Secuencia* sc_attack = new Secuencia();
    sc_attack->addChild(new CheckPlayerSight());   
    sc_attack->addChild(new SpellSecuencia());  

    Decorador* d_attack = new ReleaseSpell();
    d_attack->setChild(sc_attack);

    informacion->SetPuntero(AI_TASK_SHOOT_SPELL, d_attack);

    tasks.push_back(d_attack);
}

void BehaviourTree::CreateMoveDefault(){
    Secuencia* sc_sight = new Secuencia();
    sc_sight->addChild(new CheckPlayerSight());
    sc_sight->addChild(new GoToTarget());
    sc_sight->addChild(new HasArrived());

    Secuencia* sc_hearing = new Secuencia();
    sc_hearing->addChild(new CheckPlayerHearing());
    sc_hearing->addChild(new FaceTarget());
    
    Selector* sl_movement = new Selector();
    sl_movement->addChild(sc_sight);
    sl_movement->addChild(sc_hearing);
    sl_movement->addChild(new T_Wander());

    informacion->SetPuntero(AI_MOVE_DEFAULT, sl_movement);

    tasks.push_back(sl_movement);
}

void BehaviourTree::CreateMoveSpell(){
    Secuencia* sc_moveToTarget = new Secuencia();
    sc_moveToTarget->addChild(new CheckDistance(8.0f));
    sc_moveToTarget->addChild(new FleeFromTarget());

    Selector* sl_moveShoot = new Selector();
    sl_moveShoot->addChild(sc_moveToTarget);
    sl_moveShoot->addChild(new GoToTarget());

    informacion->SetPuntero(AI_MOVE_SPELL00, sl_moveShoot);
    informacion->SetPuntero(AI_MOVE_SPELL01, sl_moveShoot);
    informacion->SetPuntero(AI_MOVE_SPELL02, sl_moveShoot);
    informacion->SetPuntero(AI_MOVE_SPELL03, sl_moveShoot);

    tasks.push_back(sl_moveShoot);
}

void BehaviourTree::CreateCathPotion(){
    Secuencia* sc_catchPotion = new Secuencia();
    sc_catchPotion->addChild(new CheckDistance(2.0f));  // Distancia del raycast
    sc_catchPotion->addChild(new CatchPotion());

    informacion->SetPuntero(AI_TASK_CATCH_POT, sc_catchPotion);

    tasks.push_back(sc_catchPotion);
}

void BehaviourTree::CreateMoveToTarget(){
    Task* t = new GoToTarget();
    informacion->SetPuntero(AI_MOVE_GOTARGET, t);

    tasks.push_back(t);
}

void BehaviourTree::CreateDrinkPotion(){
    Task* t = new UsePotion();
    informacion->SetPuntero(AI_TASK_DRINK_POT, t);

    tasks.push_back(t);
}

void BehaviourTree::CreateUseFountain(){
    Secuencia* sc_useFountain = new Secuencia();
    sc_useFountain->addChild(new CheckDistance(2.0f));  // Distancia del raycast
    sc_useFountain->addChild(new UseFountain());

    informacion->SetPuntero(AI_TASK_USE_FOUNT, sc_useFountain);

    tasks.push_back(sc_useFountain);
}

void BehaviourTree::CreateDefuseTrap(){
    Secuencia* sc_defuseTrap = new Secuencia();
    sc_defuseTrap->addChild(new CheckDistance(2.0f));   // Distancia del raycast
    sc_defuseTrap->addChild(new DefuseTrap());

    informacion->SetPuntero(AI_TASK_DEFUSE_TRAP, sc_defuseTrap);

    tasks.push_back(sc_defuseTrap);
}

void BehaviourTree::CreateNoMove(){
    Task* t = new EmptyTask();

    informacion->SetPuntero(AI_MOVE_NO, t);

    tasks.push_back(t);
}