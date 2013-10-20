#ifndef GEPROPERTYLIGHT_H
#define GEPROPERTYLIGHT_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../gui/geHorizontalSlider.h"
#include "../../gui/geColorControl.h"
#include "../../../../GEAREngine/src/core/gxLight.h"

class gePropertyLight : public geTreeNode, public MGUIObserver
{
public:
	gxLight* m_pLightPtr;
	//geHorizontalSlider* m_pHorizontalSlider_LightColor[3];
	geColorControl* m_pColorControl;

	//geHorizontalSlider* m_pHorizontalSlider_LightAmbientColor[3];
	geColorControl* m_pColorControlAmbient;
	//geHorizontalSlider* m_pHorizontalSlider_LightSpecularColor[3];
	geColorControl* m_pColorControlSpecular;

	gePropertyLight(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite):
	  geTreeNode(renderer, parent, name, sprite, 10)
	{
		setSize(m_cSize.x, 85.0f);

		//diffuse
		m_pColorControl = new geColorControl();
		m_pColorControl->create(renderer, this, 10, 10);
		m_pColorControl->setControlColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pColorControl->setGUIObserver(this);
		//for(int x=0;x<3;x++)
		//{
		//	m_pHorizontalSlider_LightColor[x] = new geHorizontalSlider();
		//	m_pHorizontalSlider_LightColor[x]->create(renderer, this, "slider", 30, 10+x*15, 70);
		//	m_pHorizontalSlider_LightColor[x]->setSliderValue(0.2f);
		//	m_pHorizontalSlider_LightColor[x]->setGUIObserver(this);
		//}

		//ambient
		m_pColorControlAmbient = new geColorControl();
		m_pColorControlAmbient->create(renderer, this, 10, 35);
		m_pColorControlAmbient->setControlColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pColorControlAmbient->setGUIObserver(this);
		//for(int x=0;x<3;x++)
		//{
		//	m_pHorizontalSlider_LightAmbientColor[x] = new geHorizontalSlider();
		//	m_pHorizontalSlider_LightAmbientColor[x]->create(renderer, this, "slider", 140, 10+x*15, 70);
		//	m_pHorizontalSlider_LightAmbientColor[x]->setSliderValue(0.2f);
		//	m_pHorizontalSlider_LightAmbientColor[x]->setGUIObserver(this);
		//}

		//specular
		m_pColorControlSpecular = new geColorControl();
		m_pColorControlSpecular->create(renderer, this, 10, 60);
		m_pColorControlSpecular->setControlColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_pColorControlSpecular->setGUIObserver(this);
		//for(int x=0;x<3;x++)
		//{
		//	m_pHorizontalSlider_LightSpecularColor[x] = new geHorizontalSlider();
		//	m_pHorizontalSlider_LightSpecularColor[x]->create(renderer, this, "slider", 250, 10+x*15, 70);
		//	m_pHorizontalSlider_LightSpecularColor[x]->setSliderValue(0.2f);
		//	m_pHorizontalSlider_LightSpecularColor[x]->setGUIObserver(this);
		//}

		m_pLightPtr=NULL;

		setNodeColor(0.21f, 0.21f, 0.21f);
		setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setColor(&m_cVBClientArea, 0.21f, 0.21f, 0.21f, 1.0f);
	}

	virtual ~gePropertyLight()
	{

	}

	void populatePropertyOfLight(object3d* obj);

	virtual void drawNode();

	virtual void onTextChange(geGUIBase* textbox);
	virtual void onSliderChange(geGUIBase* slider);
	virtual void onColorChange(geGUIBase* colorControl);
	virtual void onDragDrop(int x, int y, MDataObject* dropObject);
};

#endif