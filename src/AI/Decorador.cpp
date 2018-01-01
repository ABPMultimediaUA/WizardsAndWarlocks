#include "Decorador.h"

Decorador::Decorador(){

}

Decorador::~Decorador(){

}

bool Decorador::run(Blackboard* bb){
    std::cout<<"Run del Decorador básico"<<std::endl;
    if(child != NULL){
        return child->run(bb);
    }
    return false;
}

bool Decorador::terminate(){
    std::cout<<"Terminate del decorador básico"<<std::endl;
    return true;
}

void Decorador::setChild(Task* newChild){
    child = newChild;
}
