#include "WatcherCamera.h"

#include <GraphicEngine/GraphicEngine.h>
#include <PhysicsEngine/BT_Body.h>
#include <GraphicEngine/GCamera.h>

float lastDistance = 0;

WatcherCamera::WatcherCamera(vector3df lookat):Camera(){
	// intialize variables
	XAngle = 180;
	YAngle = 0;
	maskActivated = true;
	colliding = false;
	mouseVelocity = 6;
	distanceTarget = 0;
	maxCamDistance = 3;
	minCamDistance = 1;
	cam_distance = maxCamDistance;
	updateCamDistancia = 0.01;

	vector3df position = lookat;
	position.Z--;

	// Graphic engine
	p_Camera = GraphicEngine::getInstance()->addCameraSceneNode(position, lookat);

	// LastPos
	lastPos =  GraphicEngine::getInstance()->GetCursorPosition();

	//Bullet Physics
	p_BtBody = new BT_Body();
	clase = EENUM_CAMERA;
	p_BtBody->CreateBox(position, vector3df(0.5, 0.5, 0.5), 1, 0, vector3df(0,0,0), C_CAMERA, cameraCW);
	p_BtBody->SetGravity(vector3df(0.0f,0.0f,0.0f));
	p_BtBody->AssignPointer(this);
	clase = EENUM_CAMERA;

}

WatcherCamera::~WatcherCamera(){
	if(p_BtBody!=nullptr){
		delete p_BtBody;
		p_BtBody = nullptr;
	}

}

void WatcherCamera::SetPosition(vector3df target){
	p_BtBody->SetPosition(target);
	p_Camera->setPosition(target);
}

vector3df WatcherCamera::GetTarget(float AngleX, float AngleY){
	float tx = sin(AngleX * M_PI /180) * cos(AngleY * (M_PI /180)) * cam_distance;
	float ty = sin(AngleY * (M_PI /180)) * cam_distance;
	float tz = cos(AngleX * M_PI /180) * cos(AngleY * (M_PI /180)) * cam_distance;

	return vector3df(tx, ty, tz);
}

void WatcherCamera::UpdateCamera(vector3df target, float deltaTime){
	p_BtBody->Update();
	p_BtBody->SetLinearVelocity(vector3df(0,0,0));
	target.Y+=0.5;
	p_Camera->setTarget(target);
	
	// Get the new position of the camera
	vector3df newPosition = GetTarget(XAngle, YAngle);
	newPosition.X += target.X;
	newPosition.Y += target.Y;
	newPosition.Z += target.Z;

	vector3df camPos = p_BtBody->GetPosition();
	float angle = atan2(camPos.X - target.X, camPos.Z - target.Z);
	angle = angle * 180/M_PI;
	angle = angle<0? angle+=360: angle;

	// Apply the force
	float distance = (newPosition-camPos).length();
	newPosition = newPosition - camPos;
	newPosition.normalize();
	p_BtBody->ApplyCentralImpulse(newPosition*distance*10);

	// Move the visual object
	p_Camera->setPosition(p_BtBody->GetPosition());

	distanceTarget = (camPos-target).length();

	UpdateAngles();
	CheckMaxVelocity();
	CheckCollision();
	CheckDistance();
}

int WatcherCamera::sign(float value){
	if(value>=0) return 1;
	return -1;
}

void WatcherCamera::CheckCollision(){
	if(!colliding && cam_distance<maxCamDistance){
		cam_distance += updateCamDistancia*2;
	}
	colliding = false;
}

void WatcherCamera::Contact(void* punt, EntityEnum tipo){
	if(tipo == EENUM_FLOOR){
		if(cam_distance>minCamDistance) cam_distance -= updateCamDistancia;
		colliding = true;
	}
}

void WatcherCamera::CheckDistance(){
	if(maskActivated && distanceTarget>maxCamDistance*2){
		p_BtBody->SetFlags(C_NOTHING, 0);
		maskActivated = false;
	}
	if(!maskActivated && distanceTarget<=maxCamDistance+0.5f){
		p_BtBody->SetFlags(C_CAMERA, cameraCW);
		maskActivated = true;
	}
}

void WatcherCamera::UpdateAngles(){
	GraphicEngine* g_engine = GraphicEngine::getInstance();
	int screenW = g_engine->GetScreenWidth();
	int screenH = g_engine->GetScreenHeight();
	
	vector2di mousePos = g_engine->GetCursorPosition();
	vector2di dirMouse = lastPos - mousePos;

	lastPos = mousePos;
	if(lastPos.X<=0 || lastPos.Y<=0 || lastPos.X>=screenW || lastPos.Y>=screenH){
		lastPos = vector2di(screenW/2, screenH/2);
		g_engine->SetCursorPosition(lastPos);
	}

	// Update the Y angle
	int yDiff = dirMouse.Y;
	if(abs(yDiff)>5){
		YAngle += 5*sign(yDiff)*-1;
		YAngle = YAngle>60? 60: YAngle<-60? -60: YAngle;
	}

	// Update the X angle
	int xDiff = dirMouse.X;
	if(abs(xDiff)>5){
		XAngle += 5*sign(xDiff)*-1;
		XAngle = XAngle>360? XAngle-360: XAngle<0? XAngle+360: XAngle;
	}

}

void WatcherCamera::CheckMaxVelocity(){
	float max_velocity = 10.0f;
	
	vector3df velocity = p_BtBody->GetLinearVelocity();
	vector3df auxVelocity(velocity.X, velocity.Y, velocity.Z);
	float speed = auxVelocity.length();
	if(speed > max_velocity) {
		auxVelocity *= max_velocity/speed;
		p_BtBody->SetLinearVelocity(auxVelocity);
	}
}

void WatcherCamera::SetWorking(bool work){
	m_working = work;
}