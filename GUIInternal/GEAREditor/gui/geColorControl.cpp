#include "geColorControl.h"
#include "geGUIManager.h"

geColorControl::geColorControl():
	geGUIBase(GEGUI_COLOR_CONTROL, "Color Control")
{
}

geColorControl::~geColorControl()
{
}

void geColorControl::create(geGUIBase* parent, float x, float y)
{
	createBase(parent);

	setSize(16, 16);
	setPos(x, y);

	setColor(&m_cVBClientArea, 0.21f, 0.21f, 0.21f, 1.0f);
}

void geColorControl::setControlColor(float r, float g, float b, float a)
{
	setColor(&m_cVBClientArea, r, g, b, a);
}

void geColorControl::draw()
{
	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y, 0);
	drawRect(&m_cVBClientArea);
	drawLine(m_cVBClientAreaLine, 0.13f, 0.13f, 0.13f, 1.0f, 3, false);
	drawLine(&m_cVBClientAreaLine[4], 0.3f, 0.3f, 0.3f, 1.0f, 3, false);
	glPopMatrix();
}
	
void geColorControl::onPosition(float x, float y, int flag)
{
}

void geColorControl::onSize(float cx, float cy, int flag)
{
	const float title_vertLst[8] =
	{
		cx,	0,
		0,		0,
		cx,	cy,
		0,		cy,
	};
	memcpy(m_cVBClientArea.m_cszVertexList, title_vertLst, sizeof(title_vertLst));

	const float clientarea_linevertLst[10] =
	{
		cx,	0,
		0,	0,
		0,	cy-0.5f,
		cx,	cy-0.5f,
		cx,	0,
	};
	memcpy(m_cVBClientAreaLine, clientarea_linevertLst, sizeof(clientarea_linevertLst));
}

bool geColorControl::onMouseLButtonDown(float x, float y, int nFlag)
{
	return true;
}

bool geColorControl::onMouseLButtonUp(float x, float y, int nFlag)
{
	return true;
}

bool geColorControl::onMouseMove(float x, float y, int flag)
{
	return true;
}