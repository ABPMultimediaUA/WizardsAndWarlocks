#ifndef GRECT_H
#define GRECT_H

#include <vector2d.h>

class GraphicEngine;
class TFRect;

class GRect{
	friend class GraphicEngine;
public:
	~GRect();
	void SetColor(float r, float g, float b, float a = 1.0f);
	void SetMask(std::string path = "");
	void SetRotation(float rot);
	void SetWidth(float width);
	void SetHeight(float height);
	void SetPosition(float x, float y);
	void SetXPos(float x);
	void SetYPos(float y);
	float GetWidth();
	float GetHeight();

private:
	GRect(TFRect* rect);

	TFRect* m_rect;
};


#endif