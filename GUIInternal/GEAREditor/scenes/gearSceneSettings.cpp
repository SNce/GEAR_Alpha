#include "gearSceneSettings.h"

gearSceneSettings::gearSceneSettings():
geWindow("Settings View")
{
	m_pSettingsAndroidParentNode=NULL;
	m_pSettingsAndroid=NULL;

	m_pSettingsGlobalParentNode=NULL;
	m_pSettingsGlobal=NULL;
}

gearSceneSettings::~gearSceneSettings()
{
}

void gearSceneSettings::onCreate()
{
	m_cszSprites[0].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[0].setClip(6, 256, 16, 16);
	m_cszSprites[1].loadTexture(&geGUIManager::g_cTextureManager, "res//icons16x16.png");
	m_cszSprites[1].setClip(173, 321, 16, 16);

	m_cSettingsTreeView.create(m_pRenderer, this, "SettingsTV", this);

	geTreeNode* rootNode=m_cSettingsTreeView.getRoot();

	m_pSettingsGlobalParentNode = new geTreeNode(m_pRenderer, rootNode, "Global", &m_cszSprites[0], 0);
	m_pSettingsGlobal = new geSettingsGlobal(m_pRenderer, m_pSettingsGlobalParentNode, "", NULL);

	m_pSettingsAndroidParentNode = new geTreeNode(m_pRenderer, rootNode, "Android", &m_cszSprites[1], 0);
	m_pSettingsAndroid = new geSettingsAndroid(m_pRenderer, m_pSettingsAndroidParentNode, "", NULL);
}

void gearSceneSettings::onDraw()
{
#ifdef LOG_GLERROR
	int err=glGetError();
	if(err!=GL_NO_ERROR)
	{
		printf("glGetError 0x%x\n", err);
	}
#endif

	m_cSettingsTreeView.draw();
}

void gearSceneSettings::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{
}

bool gearSceneSettings::onMouseLButtonDown(float x, float y, int nFlag)
{
	return geWindow::onMouseLButtonDown(x, y, nFlag);
}

bool gearSceneSettings::onMouseLButtonUp(float x, float y, int nFlag)
{
	return geWindow::onMouseLButtonUp(x, y, nFlag);
}

bool gearSceneSettings::onMouseMove(float x, float y, int flag)
{
	return geWindow::onMouseMove(x, y, flag);
}

void gearSceneSettings::onMouseWheel(int zDelta, int x, int y, int flag)
{
	geWindow::onMouseWheel(zDelta, x, y, flag);
}