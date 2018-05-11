#ifndef BULLETDEBUG_H
#define BULLETDEBUG_H

#include <iostream>
#include <bullet/LinearMath/btIDebugDraw.h>

class BulletDebug: public btIDebugDraw {
public:

	void drawLine( const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3&  normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int getDebugMode() const;

};


#endif