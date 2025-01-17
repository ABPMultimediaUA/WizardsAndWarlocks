#include "IncludeEventChilds.h"

#include <fmod/fmod_studio.hpp>
#include <fmod/fmod_errors.h>

#include <math.h>
#include <Constants.h>

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

//Function that shows an error when something goes wrong
void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line){
	if (result != FMOD_OK) {
		std::cerr << file << "(" << line << "): FMOD error " << result << " - " << FMOD_ErrorString(result) << std::endl;
		exit(-1);
	}
}

/********************************************************************************************************
********************************************** Sound System *********************************************
*********************************************************************************************************/

SoundSystem* SoundSystem::getInstance() {
	static SoundSystem instance = SoundSystem();
	return &instance;
}

/******************************************************
*  Constructor
******************************************************/
SoundSystem::SoundSystem() {}

/******************************************************
*  Destructor
******************************************************/
SoundSystem::~SoundSystem() {

	//Delete the sound event instances
	std::map<std::string, SoundEvent*>::iterator itSe = soundEvents.begin();
	for(; itSe!=soundEvents.end(); itSe++){
		SoundEvent* even = itSe->second;
		delete even;		//Delete the events
	}
	soundEvents.clear();	//Clear the sound events map

	//Delete the event descriptions
	std::map<std::string, FMOD_STUDIO_EVENTDESCRIPTION*>::iterator iteven = eventDescriptions.begin();
	for(; iteven!=eventDescriptions.end(); iteven++){
		FMOD_STUDIO_EVENTDESCRIPTION* even = iteven->second;
		ERRCHECK(FMOD_Studio_EventDescription_ReleaseAllInstances(even));		//Delete all the  event instances of the event description
	}
	eventDescriptions.clear();		//Clear the event descriptions map

	//Delete the banks
	std::map<std::string, FMOD_STUDIO_BANK*>::iterator itBanks = banks.begin();
	for(; itBanks!=banks.end(); itBanks++){
		FMOD_STUDIO_BANK* ba = itBanks->second;
		if(ba!=nullptr) ERRCHECK(FMOD_Studio_Bank_Unload(ba));  //Delete every map
	}
	banks.clear();		//Clear the banks map
	
	delete listener;	//Delete the sound listener

	ERRCHECK(FMOD_Studio_System_Release(system));	//Delete the sound system
}

/******************************************************
*  Constructor
*  @param soundBanksPath directory path where are located the FMOD Studio generated banks
******************************************************/
void SoundSystem::createSystem(std::string soundBanksPath){
   
	// Init Variables
	system = nullptr;
	lowLevelSystem = nullptr;
	masterBank = nullptr;
	stringsBank = nullptr;
    busMaster = nullptr;

	//Variables needed for the banks' filename
	banksPath = soundBanksPath.c_str();

	//Initializing the fmod studio api
	//Create the object Studio System
	ERRCHECK(FMOD_Studio_System_Create(&system, FMOD_VERSION));
	
	//Initializing the fmod low level api
	lowLevelSystem = nullptr;
	ERRCHECK(FMOD_Studio_System_GetLowLevelSystem(system, &lowLevelSystem));
	
	ERRCHECK(FMOD_System_SetSoftwareFormat(lowLevelSystem, 44100, FMOD_SPEAKERMODE_STEREO, 0));
	ERRCHECK(FMOD_System_SetSoftwareChannels(lowLevelSystem, 32));
	ERRCHECK(FMOD_System_SetOutput(lowLevelSystem, FMOD_OUTPUTTYPE_AUTODETECT));

	//Initialize the system
	ERRCHECK(FMOD_Studio_System_Initialize(system, 32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));
	//ERRCHECK(FMOD_Studio_System_Initialize(system, 32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_NORMAL, 0));

	//Load the needed banks
	loadBanks();
	loadAvc();

	// Load the bus
	ERRCHECK(FMOD_Studio_System_GetBus(system, "bus:/", &busMaster));

	//Initialize other variables
	listener = new FMOD_3D_ATTRIBUTES();
	setPos(listener,vector3df(0.0f,0.0f,0.0f));
	setVel(listener,vector3df(0.0f,0.0f,0.0f));
	setForward(listener,vector3df(1.0f,0.0f,0.0f));
	setUp(listener,vector3df(0.0f,1.0f,0.0f));

	ERRCHECK(FMOD_Studio_Bus_SetVolume(busMaster, 10.0f));
}

/******************************************************
* Factory method that constucts a SoundEvent by the name of the FMOD Studio event 
*  f. ex. "event:/Ambience/Country"
*  \param eventPath name of the event according FMOD Studio nomenclature
******************************************************/
SoundEvent* SoundSystem::getEvent(const char * eventPath) {
	return soundEvents.find(eventPath)->second;
}

/******************************************************
* @brief Loads an specific bank
* @param string path of the bank to load
* @param string name of the bank to save it in the map 
******************************************************/
void SoundSystem::loadBank(std::string filePath, std::string bankName) {

	FMOD_STUDIO_BANK* bank = nullptr;

	std::string path = "";
	const char * finalPath = "";

	path = banksPath + filePath;     //Make a string with the filename
	finalPath = path.c_str();        //Convert to const char*
	
	ERRCHECK(FMOD_Studio_System_LoadBankFile(system, finalPath, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));   //Load the bank

	if(bank != nullptr){
		banks[bankName] = bank;
	}
}

/******************************************************
*  Loads all the needed banks
******************************************************/
void SoundSystem::loadBanks() {
	//Load the banks
	loadBank("Master Bank.bank", "masterBank");
	loadBank("Master Bank.strings.bank", "stringsBank");
	loadBank("Character.bank", "characterBank");
	loadBank("Spells.bank", "spellsBank");
	loadBank("Ambience.bank", "AmbienceBank");
	loadBank("HUD.bank", "HUDBank");
	loadBank("Music.bank", "MusicBank");
	loadBank("CommonSound.bank", "CommonSoundBank");
}

/******************************************************
*  Loads all the needed avc
******************************************************/
void SoundSystem::loadAvc(){
	loadAvc("Music");
	loadAvc("SFX");
	loadAvc("Voices");
}

/******************************************************
* @brief Loads an specific vca
* @param string path of the vca to load
******************************************************/
void SoundSystem::loadAvc(std::string path){
	std::string completPath = "vca:/" + path;
	FMOD_STUDIO_VCA* createVCA = nullptr;

	ERRCHECK(FMOD_Studio_System_GetVCA(system, completPath.c_str(), &createVCA));

	if(createVCA != nullptr){
		vcas[path] = createVCA;
	}
}

/******************************************************
*  Modifies the general volume of the engine
******************************************************/
void SoundSystem::setVolume(float vol) {
	ERRCHECK(FMOD_Studio_Bus_SetVolume(busMaster, vol));
}

/******************************************************
*  Returns the general volume of the engine
******************************************************/
float SoundSystem::getVolume() {
	float vol, finalvol;
	ERRCHECK(FMOD_Studio_Bus_GetVolume(busMaster, &vol, &finalvol));
	return vol;
}


/******************************************************
 *  @brief Modifies the vca volume
 *  @param string name of the vca
 *  @param vol Volume required
 ******************************************************/
void SoundSystem::setVolumeVCA(std::string name, float vol){
	if(vcas.find(name) != vcas.end()){
		FMOD_STUDIO_VCA* currentVca = vcas[name];
		ERRCHECK(FMOD_Studio_VCA_SetVolume(currentVca, vol));
	}
}

/******************************************************
 * @brief Get the volume of the vca
 * @param string name of the vca
 * @return volume of the required vca
 ******************************************************/
float SoundSystem::getVolumeVCA(std::string name){
	float output = -1.0f;
	if(vcas.find(name) != vcas.end()){
		FMOD_STUDIO_VCA* currentVca = vcas[name];
		float finalVolume = 0.0f;
		ERRCHECK(FMOD_Studio_VCA_GetVolume(currentVca, &output, 0));
		ERRCHECK(FMOD_Studio_VCA_GetVolume(currentVca, &output, &finalVolume));
	}
	return output;
}

/******************************************************
*  Modifies the position and rotation of the listening point
******************************************************/
void SoundSystem::setListenerPosRot(vector3df pos, vector3df rot) {

	rot = rot * M_PI/180;		//Transform to degrees
	rot.X = 0; rot.Z = 0;
	setPos(listener, pos);		//Set the listener position

	//Calculate the forward vector
	vector3df frwd = vector3df (sin(rot.Y)*cos(rot.X), sin(rot.X), cos(rot.Y)*cos(rot.X));
	setForward(listener, frwd);	// Ponemos el FORWARD

	//Calculate the up vector from the forward vector
	//Right vector x forward vector --> (forward x absolute up) x forward --> (forward x [0,1,0]) x forward
	vector3df A = frwd; vector3df B = vector3df(0, 1, 0);
	vector3df aux = vector3df(A.Y * B.Z - B.Y * A.Z, A.Z * B.X - B.Z * A.X, A.X * B.Y - B.X * A.Y);
	A = aux; B = frwd;
	vector3df up = vector3df(A.Y * B.Z - B.Y * A.Z, A.Z * B.X - B.Z * A.X, A.X * B.Y - B.X * A.Y);
	setUp(listener, up);
	
	//Finally, set the attributes
	ERRCHECK(FMOD_Studio_System_SetListenerAttributes(system, 0, listener));
}

/******************************************************
*  Return the position of the listener
******************************************************/
vector3df SoundSystem::getListenerPosition() {
	vector3df toRet;
	toRet = vector3df(listener->position.x, listener->position.y, listener->position.z);
	return toRet;
}

/******************************************************
*  Updates the audio engine
*  \param paused tells if has to pause the engine or not
******************************************************/
void SoundSystem::Update(vector3df headPos, vector3df headRot) {
	setListenerPosRot(headPos, headRot);	//Position and rotation of the listener
	ERRCHECK(FMOD_Studio_System_Update(system));

}

void SoundSystem::Update(){
	ERRCHECK(FMOD_Studio_System_Update(system));
}

/******************************************************
*  @brief Creates the a FMOD eventDescription
*  @param string name path of the description
*  @param FMOD_STUDIO_EVENTDESCRIPTION* event description pointer
******************************************************/
FMOD_STUDIO_EVENTDESCRIPTION* SoundSystem::createDescription(const char* path, FMOD_STUDIO_EVENTDESCRIPTION* desc){
	ERRCHECK(FMOD_Studio_System_GetEvent(system, path, &desc));		//Create the event
	eventDescriptions[path] = desc;									//Add the descriptions to the event descriptions map
	return desc;
}

/******************************************************
 * @brief Creates a sound event
 * @param std::string path of the event
 ******************************************************/
SoundEvent* SoundSystem::createEvent(std::string eventPath) {
	
	FMOD_STUDIO_EVENTDESCRIPTION* eventDesc  = nullptr;					//Initialize the event description
	FMOD_STUDIO_EVENTINSTANCE* eventInst     = nullptr;					//Initialize the event instance
	SoundEvent* newEvent					 = nullptr; 					//Initialize the event
	//Search the description to know if it's already created
	if (eventDescriptions.find(eventPath) != eventDescriptions.end()){
		eventDesc = eventDescriptions[eventPath];				 //Set it to the eventDesc var
	} else {
		eventDesc = createDescription(eventPath.c_str(), eventDesc); //Else set a new event description
	}
	
	ERRCHECK(FMOD_Studio_EventDescription_CreateInstance(eventDesc, &eventInst));		//Set the event instance
	// GEt channel group
	// GET NUM CHANNELS
	//


	//Dertermine wich type of sound event will create
	if (eventPath.find("Character") != -1)  		newEvent = new CharacterSound();	//Set the event
	else if (eventPath.find("Spells") != -1)   		newEvent = new SpellSound();		//Set the event		
	else if (eventPath.find("Ambience") != -1) 		newEvent = new AmbienceSound();		//Set the event
	else if (eventPath.find("HUD") != -1) 	  		newEvent = new HUDSound();			//Set the event
	else if (eventPath.find("Music") != -1)   		newEvent = new Music();				//Set the event
	else if (eventPath.find("CommonSound") != -1)	newEvent = new CommonSoundEvent();	//Set the event
	
	newEvent->setInstance(eventInst);	//Set the event instance
	newEvent->setDescription(eventDesc);
	//soundEvents[eventPath] = newEvent;  //Store the event in the sound events map



	return newEvent;
}

/******************************************************
*  Sets the property for the listener
*  \param vec that has to be assigned
******************************************************/
void SoundSystem::setPos(FMOD_3D_ATTRIBUTES * var,vector3df vec) {   
	var->position.x = vec.X;
	var->position.y = vec.Y;
	var->position.z = vec.Z;
}

void SoundSystem::setVel(FMOD_3D_ATTRIBUTES * var,vector3df vec) {
	var->velocity.x = vec.X;
	var->velocity.y = vec.Y;
	var->velocity.z = vec.Z;
}

void SoundSystem::setForward(FMOD_3D_ATTRIBUTES * var,vector3df vec) {
	var->forward.x = vec.X;
	var->forward.y = vec.Y;
	var->forward.z = vec.Z;

}

void SoundSystem::setUp(FMOD_3D_ATTRIBUTES * var,vector3df vec) {
	var->up.x = vec.X;
	var->up.y = vec.Y;
	var->up.z = vec.Z;
}

/******************************************************
* @brief Checks if an event is playing and plays it
* @param eventPath path of the event to play
* @param playerPos position where should play the event and/or of the listener
* @param playerRot rotation where should play the event and/or of the listener
******************************************************/
void SoundSystem::checkAndPlayEvent(SoundEvent* event, vector3df playerPos) {
	//Checks if the event exists and is being played
	if (!event->isPlaying()) {
		playEvent(event, playerPos); //Plays the event
	}
}

/******************************************************
* @brief Plays a sound event
* @param eventPath path of the event to play
* @param playerPos position where should play the event and/or of the listener
* @param playerRot rotation where should play the event and/or of the listener
******************************************************/
void SoundSystem::playEvent(SoundEvent* event, vector3df playerPos) {
	event->setPosition(playerPos);	//Position the event
	event->start();					//Start the event
}

//Used for events without position as the events from the menu
void SoundSystem::playEvent(SoundEvent* event) {
	event->start();					//Start the event
}

/******************************************************
 * @brief Stops an event that is being played
 * @param eventPath path of the event to stop
 ******************************************************/
void SoundSystem::stopEvent(SoundEvent* event) {   
    event->stop();
}

/******************************************************
 * @brief Stops an event if it's being played
 * @param eventPath path of the event to stop
 ******************************************************/
void SoundSystem::checkAndStopEvent(SoundEvent* event) {
    if (event->isPlaying()) event->stop();
}

/******************************************************
 * @brief Erase an soundEvent from the map
 * @param event to erase
 ******************************************************/
void SoundSystem::eraseSoundEvent(SoundEvent* event){
	//Delete the sound event instances
	std::map<std::string, SoundEvent*>::iterator itSe = soundEvents.begin();
	for(; itSe!=soundEvents.end(); itSe++){
		SoundEvent* even = itSe->second;
		if(even == event){
			soundEvents.erase(itSe);
		}
	}
}

/********************************************************************************************************
********************************************** Sound Event *********************************************
********************************************************************************************************/

/******************************************************
*  Constructor
******************************************************/
SoundEvent::SoundEvent() {
	soundInstance = nullptr;
	soundDescription = nullptr;
}

/******************************************************
*  Destructor
******************************************************/
SoundEvent::~SoundEvent() {
	//ERRCHECK(soundInstance->release());
}

/******************************************************
*  Starts to reproduce the event
******************************************************/
void SoundEvent::start() {
	if(soundInstance!=nullptr)ERRCHECK(FMOD_Studio_EventInstance_Start(soundInstance));   
}

/******************************************************
*  Stops the event reproduction inmediately
******************************************************/
void SoundEvent::stop() {
	if(soundInstance!=nullptr)ERRCHECK(FMOD_Studio_EventInstance_Stop(soundInstance, FMOD_STUDIO_STOP_IMMEDIATE));
}

/******************************************************
*  Pauses the event reproduction
******************************************************/
void SoundEvent::pause() {
	FMOD_BOOL paused = false;
	if(soundInstance!=nullptr){
		ERRCHECK(FMOD_Studio_EventInstance_GetPaused(soundInstance, &paused));
		ERRCHECK(FMOD_Studio_EventInstance_SetPaused(soundInstance, !paused));
	}
}


/******************************************************
*  Modifies the sound event volume
*  \param vol event volume, 0 = silence, 1 = maximun volume
******************************************************/
void SoundEvent::setVolume(float vol) {
	if(soundInstance!=nullptr) ERRCHECK(FMOD_Studio_EventInstance_SetVolume(soundInstance, vol));
}

/******************************************************
*  Modifies the event volume multiplying it with a gain factor
*  \param gain factor, 0 = silence, 1 = keep volume
******************************************************/
void SoundEvent::setGain(float gain) {
	if(soundInstance!=nullptr) ERRCHECK(FMOD_Studio_EventInstance_SetPitch(soundInstance, gain));

}

/******************************************************
*  Modifies the 3D position of the sound event
*  @param x, y, and z, new 3D position
******************************************************/
void SoundEvent::setPosition(vector3df pos) {
	
	if(soundInstance==nullptr) return;

	if(isnan(pos.X)){
		pos.X = 0; pos.Y = 0; pos.Z = 0;
	}

	FMOD_3D_ATTRIBUTES* attributes = new FMOD_3D_ATTRIBUTES();
	

	// Raised by 0.5 to not get the same position as the listener
	vector3df newPos(pos.X, pos.Y + 0.5, pos.Z);
	SoundSystem::getInstance()->setPos(attributes, newPos);	//Set the position
	
	//Set the forward vector
	vector3df frwd = vector3df(0,0.5,0);
	frwd.normalize();
	SoundSystem::getInstance()->setForward(attributes, frwd);
	
	//Calculate the up vector from the forward vector
	//Right vector x forward vector --> (forward x absolute up) x forward --> (forward x [0,1,0]) x forward
	vector3df A = frwd; vector3df B = vector3df(0, 0, 1);
	vector3df aux = vector3df(A.Y * B.Z - B.Y * A.Z, A.Z * B.X - B.Z * A.X, A.X * B.Y - B.X * A.Y);
	A = aux; B = frwd;
	vector3df up = vector3df(A.Y * B.Z - B.Y * A.Z, A.Z * B.X - B.Z * A.X, A.X * B.Y - B.X * A.Y);
	SoundSystem::getInstance()->setUp(attributes, up);

	//Finally, set the attributes
	if (soundInstance != nullptr) ERRCHECK(FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, attributes));
	
	delete attributes;
}

/*******************************************************
* Checks if the sound is playing
*  @return returns true if the sound is playing
*******************************************************/
bool SoundEvent::isPlaying() {
	bool res = false;
	FMOD_STUDIO_PLAYBACK_STATE state;
	if(soundInstance!=nullptr){
		ERRCHECK(FMOD_Studio_EventInstance_GetPlaybackState(soundInstance, &state)); //Checks the state (0 = true, 3 = false)
		if (state == 0) res = true;
	}
	return res;
}

/*******************************************************
 * Releases the event and destroys it after it has stop ped
 *******************************************************/
void SoundEvent::release() {
	if (soundInstance != nullptr) {
		ERRCHECK(FMOD_Studio_EventInstance_Release(soundInstance));
		SoundSystem::getInstance()->eraseSoundEvent(this);
	}
}

/*******************************************************
* Sets the value of the pararmeter called as the parameter name
* @param string name of the parameter to modify
* @param float value of the parameter to modify
*******************************************************/
void SoundEvent::setParamValue(std::string name, float value) {
	if(soundInstance!=nullptr) ERRCHECK(FMOD_Studio_EventInstance_SetParameterValue(soundInstance, name.c_str(), value));
}   

/*******************************************************
 * @brief Returns the event instance
 * @return FMOD_STUDIO_EVENTINSTANCE* 
 *******************************************************/
FMOD_STUDIO_EVENTINSTANCE* SoundEvent::getInstance() {
	return soundInstance;
}

/*******************************************************
 * @brief Sets the event instance
 * @return FMOD_STUDIO_EVENTINSTANCE* 
 *******************************************************/
void SoundEvent::setInstance(FMOD_STUDIO_EVENTINSTANCE * instance) {
	soundInstance = instance;
}

void SoundEvent::setDescription(FMOD_STUDIO_EVENTDESCRIPTION* description){
	soundDescription = description;
}