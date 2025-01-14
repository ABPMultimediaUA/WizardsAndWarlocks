#include "Secuencia.h"

Secuencia::Secuencia(){}

Secuencia::~Secuencia(){
	int size = children.size();
	for(int i=0; i<size; i++){
		Task* t = children[i];
		delete t;
	}
	children.size();
}

void Secuencia::addChild(Task* task){
	children.push_back(task);
}

void Secuencia::addChildren(std::vector<Task*> tasks){
	int tamanyo = tasks.size();
	for(int i=0; i<tamanyo; i++){
		addChild(tasks.at(i));
	}
}

bool Secuencia::run(Blackboard* bb){
	int tamanyo = children.size();
	for(int i=0; i< tamanyo; i++){
		Task* child = children.at(i);
		if(!child->run(bb)){
			return false;
		}		
	}
	return true;
}

bool Secuencia::terminate(){
	//std::cout<< "Terminate de la tarea"<<std::endl;
	return true;
}

RandomSecuencia::RandomSecuencia(){}

bool RandomSecuencia::run(Blackboard* bb){
	int tamanyo = children.size();
	shuffleVector();
	for(int i=0; i<tamanyo; i++){
		Task* child = children.at(i);
		if(!child->run(bb)){
			return false;
		}
	}
	return true;
}

void RandomSecuencia::shuffleVector(){
	int n = children.size();
	while(n>1){
		int k = rand() % n;
		n--;
		Task* temp = children.at(k);
		children.at(k) = children.at(n);
		children.at(n) = temp;
	}
}

