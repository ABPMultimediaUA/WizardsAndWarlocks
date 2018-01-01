#include "Effect.h"

/**
 * @brief Constructor basico de hechizo
 * 
 * @param time - tiempo de duracion del efecto
 */
Effect::Effect(float time, EFFECTCODE cod){
	currentTime = time;
	code = cod;
}

Effect::~Effect(){}

/**
 * @brief Compruebo si ha llegado el final del efecto
 * 
 * @param deltaTime Tiempo pasado desde la ultima vez que se llamo al metodo
 * @return True->El efecto ha terminado
 */
bool Effect::CheckFinish(float deltaTime){
	currentTime -= deltaTime;

	if(currentTime<=0){
		return true;
	}
	return false;
}

/**
 * @brief Aplico por primera vez el efecto
 * 
 * @param p Jugador al que se le aplicara
 */
void Effect::ApplyEffect(Player* p){

}

/**
 * @brief Aplico el efecto continuo
 * 
 * @param p Jugador al que se le aplicara
 */
void Effect::UpdateEffect(Player* p){
	
}

/**
 * @brief Quito el efecto al jugador
 * 
 * @param p Jugador al que se le quita el efecto
 */
void Effect::RemoveEffect(Player* p){

}

/**
 * @brief Codigo que identifica al efecto
 * 
 * @return EFFECTCODE codigo del efecto
 */
EFFECTCODE Effect::getCode(){
	return code;
}