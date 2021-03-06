#include "geGUIManager.h"

geFontManager geGUIManager::g_cFontManager;
geFont* geGUIManager::g_pFontArial10_84Ptr=NULL;
geFont* geGUIManager::g_pFontArial10_80Ptr=NULL;
CGETextureManager geGUIManager::g_cTextureManager;

geGUIManager::geGUIManager()
{
}

geGUIManager::~geGUIManager()
{
	reset();
}

void geGUIManager::reset()
{
	for(std::vector<geWindow*>::iterator it = m_vWindowObjects.begin(); it != m_vWindowObjects.end(); ++it)
	{
		geWindow* obj = *it;
		GE_DELETE(obj);
	}
	m_vWindowObjects.clear();
}

void geGUIManager::init(rendererGL10* renderer)
{
	g_cFontManager.init();
	g_pFontArial10_84Ptr=g_cFontManager.loadFont("res//fonts//arial_iphone10_84.ecf");
	g_pFontArial10_80Ptr=g_cFontManager.loadFont("res//fonts//arial_iphone10_80.ecf");

	g_pFontArial10_84Ptr->setRGBA(0.7f, 0.7f, 0.7f);
	g_pFontArial10_80Ptr->setRGBA(0.5f, 0.5f, 0.5f);

	m_cLayoutManager.create(renderer, 0, 0, 1184, 567);
}

void geGUIManager::appendWindow(geWindow* window)
{
	m_vWindowObjects.push_back(window);
}

void geGUIManager::size(int cx, int cy)
{
	m_cLayoutManager.setSize(cx, cy);
}

void geGUIManager::update(float dt)
{
	for(std::vector<geWindow*>::iterator it = m_vWindowObjects.begin(); it != m_vWindowObjects.end(); ++it)
	{
		geWindow* obj = *it;
		obj->update(dt);
	}
}

void geGUIManager::draw()
{
	m_cLayoutManager.draw();
}

void geGUIManager::MouseLButtonDown(float x, float y, int nFlag)
{
	m_cLayoutManager.MouseLButtonDown(x, y, nFlag);
}

void geGUIManager::MouseLButtonUp(float x, float y, int nFlag)
{
	m_cLayoutManager.MouseLButtonUp(x, y, nFlag);
}

void geGUIManager::MouseMove(float x, float y, int flag)
{
	m_cLayoutManager.MouseMove(x, y, flag);
}

void geGUIManager::MouseWheel(int zDelta, int x, int y, int flag)
{
	m_cLayoutManager.MouseWheel(zDelta, x, y, flag);
}

void geGUIManager::MouseRButtonDown(float x, float y, int nFlag)
{
	m_cLayoutManager.MouseRButtonDown(x, y, nFlag);
}

void geGUIManager::MouseRButtonUp(float x, float y, int nFlag)
{
	m_cLayoutManager.MouseRButtonUp(x, y, nFlag);
}

void geGUIManager::DragEnter(int x, int y)
{
	m_cLayoutManager.DragEnter(x, y);
}

void geGUIManager::DragDrop(int x, int y, MDataObject* dropObject)
{
	m_cLayoutManager.DragDrop(x, y, dropObject);
}

void geGUIManager::DragLeave()
{
	m_cLayoutManager.DragLeave();
}

bool geGUIManager::KeyDown(int charValue, int flag)
{
	return m_cLayoutManager.KeyDown(charValue, flag);
}

bool geGUIManager::KeyUp(int charValue, int flag)
{
	return m_cLayoutManager.KeyUp(charValue, flag);
}

void geGUIManager::DoCommand(int cmd)
{
	m_cLayoutManager.DoCommand(cmd);

}