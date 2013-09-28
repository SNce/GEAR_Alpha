#include "gePropertySaveMetaData.h"
#include "../../AssetImporter.h"
#include "../../EditorApp.h"

void gePropertySaveMetaData::onButtonClicked(geGUIBase* btn)
{
	if(btn==m_pButtonSave)
	{
		if(m_pButtonSave->isButtonPressed())
		{
			object3d* obj=m_pObject3dPtr;
			if(obj && obj->getParent()==monoWrapper::mono_engine_getWorld(0) && obj->getFileCRC()!=0)
			{
				//save metadata
				stMetaHeader metaHeader;
				struct stat fst;

				if(AssetImporter::readMetaHeader(obj->getFileCRC(), metaHeader, fst))
				{
					char crcFileName[512];
					sprintf(crcFileName, "%s/%s/%x", EditorApp::getProjectHomeDirectory(), "MetaData", obj->getFileCRC());
					AssetImporter::saveObject3DToMetaData(crcFileName, obj, fst);
					saveMaterialRecursiveToMeta(obj);
				}
			}
		}
	}
}

void gePropertySaveMetaData::saveMaterialRecursiveToMeta(object3d* obj)
{
	if(obj->getID()==100)
	{
		gxMesh* mesh=(gxMesh*)obj;
		for(int x=0;x<mesh->getNoOfTriInfo();x++)
		{
			gxTriInfo* triinfo=mesh->getTriInfo(x);
			gxMaterial* material=triinfo->getMaterial();
			if(material)
			{
				stMetaHeader metaHeader;
				struct stat fst;
				if(AssetImporter::readMetaHeader(material->getFileCRC(), metaHeader, fst))
				{
					char crcFileName[512];
					sprintf(crcFileName, "%s/%s/%x", EditorApp::getProjectHomeDirectory(), "MetaData", material->getFileCRC());
					AssetImporter::saveMaterialToMetaData(crcFileName, material, fst);
				}
			}
		}
	}

	for(std::vector<object3d*>::iterator it = obj->getChildList()->begin(); it != obj->getChildList()->end(); ++it)
	{
		object3d* child = *it;
		saveMaterialRecursiveToMeta(child);
	}
}