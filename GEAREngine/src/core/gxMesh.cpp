#include "gxMesh.h"
#include "../GEAREngine.h"

gxMesh::gxMesh(int ID):
	object3d(ID)
{
	m_nTriInfoArray=0;
	m_pszTriInfoArray=NULL;

	m_pszVertexBuffer=NULL;
	m_pszColorBuffer=NULL;
	m_pszNormalBuffer=NULL;
	m_nUVChannels=0;
	m_pszUVChannels=NULL;
	m_nTris_For_Internal_Use=0;
}

gxMesh::gxMesh():
	object3d(100)
{
	m_nTriInfoArray=0;
	m_pszTriInfoArray=NULL;

	m_pszVertexBuffer=NULL;
	m_pszColorBuffer=NULL;
	m_pszNormalBuffer=NULL;
	m_nUVChannels=0;
	m_pszUVChannels=NULL;
	m_nTris_For_Internal_Use=0;
}

gxMesh::~gxMesh()
{
	m_nTriInfoArray=0;
	GX_DELETE_ARY(m_pszTriInfoArray);
	GX_DELETE_ARY(m_pszVertexBuffer);
	GX_DELETE_ARY(m_pszColorBuffer);
	GX_DELETE_ARY(m_pszNormalBuffer);

	GX_DELETE_ARY(m_pszUVChannels);
}

void gxMesh::update(float dt)
{
	object3d::update(dt);
}

void gxMesh::render(gxRenderer* renderer, object3d* light)
{
	if(!isBaseFlag(eObject3dBaseFlag_Visible))
		return;

#if defined (USE_ProgrammablePipeLine)
	if(renderer->getRenderPassType()==gxRenderer::RENDER_LIGHTING_ONLY)
		renderWithLight(renderer, light);
	else if(renderer->getRenderPassType()==gxRenderer::RENDER_NORMAL)
		renderWithHWShader(renderer);
#else
	renderNormal(renderer);
#endif

	object3d::render(renderer, light);
}

void gxMesh::renderNormal(gxRenderer* renderer)
{
	glPushMatrix();
	glMultMatrixf(getWorldMatrix()->getMatrix());

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, m_pszVertexBuffer);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, m_pszNormalBuffer);

	for(int x=0;x<m_nTriInfoArray;x++)
	{
		gxTriInfo* triInfo=&m_pszTriInfoArray[x];
		if(!triInfo->getTriList()) continue;

		if(triInfo->getMaterial())
			glColor4fv(&triInfo->getMaterial()->getDiffuseClr().x);

		int nTexUsed=0;
		for(int m=0;m<m_nUVChannels;m++)
		{
			gxUV* uv=&m_pszUVChannels[m];
			if(triInfo->getMaterial() && applyStageTexture(renderer, nTexUsed, triInfo, uv, GL_TEXTURE_ENV_MODE, GL_MODULATE, 2))
				nTexUsed++;
		}
		glDrawElements(GL_TRIANGLES, triInfo->getVerticesCount(), GL_UNSIGNED_INT, triInfo->getTriList());
		renderer->m_nTrisRendered+=(triInfo->getVerticesCount()/3);
		renderer->m_nDrawCalls++;

		disableTextureOperations(nTexUsed, NULL, NULL);
	}

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
	glPopMatrix();
}

void gxMesh::renderWithLight(gxRenderer* renderer, object3d* light)
{
	if(light->getID()!=3)
		return;

	gxLight* light_ob=(gxLight*)light;

	matrix4x4f cMV = *renderer->getViewMatrix() * *getWorldMatrix();
	const float* u_modelview_m4x4=cMV.getMatrix();

	matrix4x4f noscaleMV(cMV);
	noscaleMV.noScale();
	matrix4x4f cMVInverse = noscaleMV.getInverse();
	cMVInverse.transpose();
	const float* u_modelview_inverse_m4x4=cMVInverse.getMatrix();

	matrix4x4f cMVP = *renderer->getViewProjectionMatrix() * *getWorldMatrix();
    const float* u_mvp_m4x4=cMVP.getMatrix();

	for(int x=0;x<m_nTriInfoArray;x++)
	{
		gxTriInfo* triInfo=&m_pszTriInfoArray[x];
		if(!triInfo->getTriList()) continue;

		gxMaterial* material=triInfo->getMaterial();
		if(!material) continue;
		gxHWShader* shader=material->getLightingSurfaceShader()->getMainShader();
		shader->enableProgram();

		light_ob->renderPass(renderer, shader);

		shader->sendUniformTMfv("GEAR_M", getMatrix(), false, 4);
		matrix4x4f inv_model=*this;
		inv_model.inverse();
		shader->sendUniformTMfv("GEAR_M_INVERSE", inv_model.getMatrix(), false, 4);

		//shader->sendUniformTMfv("GEAR_MV", u_modelview_m4x4, false, 4);
		shader->sendUniformTMfv("GEAR_MVP", u_mvp_m4x4, false, 4);
		//shader->sendUniformTMfv("GEAR_NORMAL_MATRIX", u_modelview_inverse_m4x4, false, 4);

		glVertexAttribPointer(shader->getAttribLoc("vIN_Position"), 3, GL_FLOAT, GL_FALSE, 0, getVertexBuffer());
		glEnableVertexAttribArray(shader->getAttribLoc("vIN_Position"));

		glVertexAttribPointer(shader->getAttribLoc("vIN_Normal"), 3, GL_FLOAT, GL_FALSE, 0, getNormalBuffer());
		glEnableVertexAttribArray(shader->getAttribLoc("vIN_Normal"));

		if(triInfo->getMaterial())
		{
			shader->sendUniform4fv("material.diffuse", &material->getDiffuseClr().x);
			shader->sendUniform4fv("material.ambient", &material->getAmbientClr().x);
			shader->sendUniform4fv("material.specular", &material->getSpecularClr().x);
			shader->sendUniform1f("material.shininess", 10.0f/*material->getShininess()*/);
		}
		else
		{
			shader->sendUniform4f("material.diffuse", 0.5f, 0.5f, 0.5f, 1.0f);
			shader->sendUniform4f("material.ambient", 0.2f, 0.2f, 0.2f, 1.0f);
			shader->sendUniform4f("material.specular", 0.2f, 0.2f, 0.2f, 1.0f);
			shader->sendUniform1f("material.shininess", 10.0f/*material->getShininess()*/);
		}


		glDrawElements(GL_TRIANGLES, triInfo->getVerticesCount(), GL_UNSIGNED_INT, triInfo->getTriList());
		renderer->m_nTrisRendered+=(triInfo->getVerticesCount()/3);
		renderer->m_nDrawCalls++;

		glDisableVertexAttribArray(shader->getAttribLoc("vIN_Normal"));
		glDisableVertexAttribArray(shader->getAttribLoc("vIN_Position"));

		shader->disableProgram();
	}
}

void gxMesh::renderWithHWShader(gxRenderer* renderer)
{
	matrix4x4f cMVP = *renderer->getViewProjectionMatrix() * *getWorldMatrix();
    const float* u_mvp_m4x4=cMVP.getMatrix();

	for(int x=0;x<m_nTriInfoArray;x++)
	{
		gxTriInfo* triInfo=&m_pszTriInfoArray[x];
		if(!triInfo->getTriList()) continue;

		gxMaterial* material=triInfo->getMaterial();
		if(!material) continue;
		gxHWShader* shader=material->getMainShader();

		shader->enableProgram();
		shader->sendUniformTMfv("GEAR_MVP", u_mvp_m4x4, false, 4);

		glVertexAttribPointer(shader->getAttribLoc("vIN_Position"), 3, GL_FLOAT, GL_FALSE, 0, getVertexBuffer());
		glEnableVertexAttribArray(shader->getAttribLoc("vIN_Position"));

		int nTexUsed=0;
		int cntr=0;
		std::vector<gxSubMap*>* maplist=material->getSubMapList();
		gxUV* base_uv=(m_nUVChannels)?&m_pszUVChannels[0]:NULL;
		stShaderProperty_Texture2D* base_tex_var=NULL;
		for(std::vector<gxSubMap*>::iterator it = maplist->begin(); it != maplist->end(); ++it, ++cntr)
		{
			gxSubMap* submap = *it;
			stShaderProperty_Texture2D* shader_var=submap->getShaderTextureProperty();
			if(!base_tex_var)
				base_tex_var=shader_var;
			gxUV* uv=&m_pszUVChannels[0];	//base uv
			if(applyStageTexture(renderer, nTexUsed, triInfo, base_uv, GL_TEXTURE_ENV_MODE, GL_REPLACE, 2, shader, base_tex_var->texture_uv_in_name.c_str()))
			{
				shader->sendUniform1i(shader_var->texture_sampler2d_name.c_str(), nTexUsed);
				nTexUsed++;
			}
		}
		glDrawElements(GL_TRIANGLES, triInfo->getVerticesCount(), GL_UNSIGNED_INT, triInfo->getTriList());
		renderer->m_nTrisRendered+=(triInfo->getVerticesCount()/3);
		renderer->m_nDrawCalls++;

		if(base_tex_var)
		{
			disableTextureOperations(nTexUsed, shader, base_tex_var->texture_uv_in_name.c_str());
		}

		glDisableVertexAttribArray(shader->getAttribLoc("vIN_Position"));
		shader->disableProgram();
	}
}

bool gxMesh::applyStageTexture(gxRenderer* renderer, int stage, gxTriInfo* triInfo, gxUV* uv, int aTexEnv1, int aTexEnv2, unsigned int texCoordSz)
{
	if(!triInfo->getMaterial()) return false;

	gxSubMap* subMap=NULL;
	subMap=triInfo->getMaterial()->getSubMap(stage);

	if(!subMap)	return false;
	if(!subMap->getTexture()) return false;
	if(subMap->getTexture()->getTextureID()==0 || !uv) return false;

	glActiveTexture(GL_TEXTURE0+stage);
	glClientActiveTexture(GL_TEXTURE0+stage);
	glTexCoordPointer(texCoordSz, GL_FLOAT, 0, uv->m_pszfGLTexCoordList);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, subMap->getTexture()->getTextureID() );
	glTexEnvf(GL_TEXTURE_ENV, aTexEnv1, (float)aTexEnv2);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glMatrixMode(GL_TEXTURE);
	glLoadMatrixf(subMap->getTexture()->getTextureMatrix()->getMatrix());
	glMatrixMode(GL_MODELVIEW);

	if(subMap->getTexture()->getTextureType()==gxTexture::TEX_ALPHA)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	return true;
}

bool gxMesh::applyStageTexture(gxRenderer* renderer, int stage, gxTriInfo* triInfo, gxUV* uv, int aTexEnv1, int aTexEnv2, unsigned int texCoordSz, gxHWShader* shader, const char* texCoordAttribName)
{
	if(!shader) return false;

	gxSubMap* subMap=NULL;
	bool bUse1x1Texture=true;
	if(triInfo->getMaterial())
	{
		subMap=triInfo->getMaterial()->getSubMap(stage);
		if(subMap && subMap->getTexture())
			bUse1x1Texture=false;
	}

	gxHWShader* hwShader=(gxHWShader*)shader;
    glActiveTexture(GL_TEXTURE0+stage);
    //if(m_bVBO)
    //{
    //    glBindBuffer(GL_ARRAY_BUFFER, uv->m_cVBO_texID);
    //    glVertexAttribPointer(hwShader->getAttribLoc(texCoordAttribName), 2, GL_FLOAT, GL_FALSE, 0, 0);
    //}
    //else
    {
        glVertexAttribPointer(hwShader->getAttribLoc(texCoordAttribName), 2, GL_FLOAT, GL_FALSE, 0, uv->m_pszfGLTexCoordList);
    }
	glEnableVertexAttribArray(hwShader->getAttribLoc(texCoordAttribName));
	//glEnable(GL_TEXTURE_2D);
	if(bUse1x1Texture)
		glBindTexture(GL_TEXTURE_2D, renderer->getGEARTexture1x1()->iTextureID);	
	else
	{
		glBindTexture(GL_TEXTURE_2D, subMap->getTexture()->getTextureID());	
		glTexEnvf(GL_TEXTURE_ENV, aTexEnv1, (float)aTexEnv2);
		if(subMap->getTexture()->getTextureType()==gxTexture::TEX_ALPHA)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}

	return true;
}

void gxMesh::disableTextureOperations(int nMultiTextureUsed, gxHWShader* shader, const char* texCoordAttribName)
{
#if defined (USE_ProgrammablePipeLine)
	if(nMultiTextureUsed && shader)
	{
		glDisableVertexAttribArray(shader->getAttribLoc(texCoordAttribName));
		//glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
#else
	//Disabling all texture operations
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	for(int i=0;i<nMultiTextureUsed;i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_TEXTURE_2D);	
	}
#endif
}

float* gxMesh::allocateVertexBuffer(int nTris)
{
	m_nTris_For_Internal_Use=nTris;
	GX_DELETE_ARY(m_pszVertexBuffer);
	m_pszVertexBuffer = new float[nTris*3*3];
	return m_pszVertexBuffer;
}

float* gxMesh::allocateColorBuffer(int nTris)
{
	GX_DELETE_ARY(m_pszColorBuffer);
	m_pszColorBuffer = new float[nTris*3*3];
	return m_pszColorBuffer;
}

float* gxMesh::allocateNormalBuffer(int nTris)
{
	GX_DELETE_ARY(m_pszNormalBuffer);
	m_pszNormalBuffer = new float[nTris*3*3];
	return m_pszNormalBuffer;
}

int gxMesh::getVerticesCount()
{
	int nTris=0;
	for(int x=0;x<m_nTriInfoArray;x++)
	{
		nTris+=m_pszTriInfoArray[x].getVerticesCount();
	}

	return nTris;
}

void gxMesh::writeMeshData(gxFile& file)
{
	file.Write(m_iObjectID);
	file.Write(m_eBaseFlags);
	file.Write(m_cszName);
	file.WriteBuffer((unsigned char*)m, sizeof(m));
	file.WriteBuffer((unsigned char*)&m_cOOBB, sizeof(m_cOOBB));
	file.Write(m_iFileCRC);
	writeAnimationController(file);

	file.Write(m_nTriInfoArray);
	for(int x=0;x<m_nTriInfoArray;x++)
	{
		m_pszTriInfoArray[x].write(file);
	}

	file.Write(m_nTris_For_Internal_Use);
	if(m_pszVertexBuffer)
	{
		file.Write(true);
		file.WriteBuffer((unsigned char*)m_pszVertexBuffer, sizeof(float)*m_nTris_For_Internal_Use*3*3);
	}
	else
	{
		file.Write(false);
	}

	if(m_pszColorBuffer)
	{
		file.Write(true);
		file.WriteBuffer((unsigned char*)m_pszColorBuffer, sizeof(float)*m_nTris_For_Internal_Use*3*3);
	}
	else
	{
		file.Write(false);
	}

	if(m_pszNormalBuffer)
	{
		file.Write(true);
		file.WriteBuffer((unsigned char*)m_pszNormalBuffer, sizeof(float)*m_nTris_For_Internal_Use*3*3);
	}
	else
	{
		file.Write(false);
	}

	file.Write(m_nUVChannels);
	for(int x=0;x<m_nUVChannels;x++)
	{
		if(m_pszUVChannels[x].m_pMaterialPtr)
			file.Write(m_pszUVChannels[x].m_pMaterialPtr->getFileCRC());
		else
			file.Write((int)0);
		file.WriteBuffer((unsigned char*)m_pszUVChannels[x].m_pszfGLTexCoordList, sizeof(float)*m_nTris_For_Internal_Use*3*2);
	}
}

void gxMesh::write(gxFile& file)
{
	writeMeshData(file);

	file.Write((int)m_cChilds.size());
	for(std::vector<object3d*>::iterator it = m_cChilds.begin(); it != m_cChilds.end(); ++it)
	{
		object3d* obj = *it;
		obj->write(file);
	}
}

void gxMesh::read(gxFile& file)
{
	file.Read(m_eBaseFlags);
	char* temp=file.ReadString();
	GX_STRCPY(m_cszName, temp);
	GX_DELETE_ARY(temp);
	file.ReadBuffer((unsigned char*)m, sizeof(m));
	file.ReadBuffer((unsigned char*)&m_cOOBB, sizeof(m_cOOBB));
	file.Read(m_iFileCRC);
	readAnimationController(file);

	file.Read(m_nTriInfoArray);
	if(m_nTriInfoArray)
		m_pszTriInfoArray = new gxTriInfo[m_nTriInfoArray];

	for(int x=0;x<m_nTriInfoArray;x++)
	{
		m_pszTriInfoArray[x].read(file);
	}

	file.Read(m_nTris_For_Internal_Use);

	bool bVertexBuffer=false;
	file.Read(bVertexBuffer);
	if(bVertexBuffer)
	{
		float* buffer=allocateVertexBuffer(m_nTris_For_Internal_Use);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*m_nTris_For_Internal_Use*3*3);
	}

	bool bColorBuffer=false;
	file.Read(bColorBuffer);
	if(bColorBuffer)
	{
		float* buffer=allocateColorBuffer(m_nTris_For_Internal_Use);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*m_nTris_For_Internal_Use*3*3);
	}

	bool bNormalBuffer=false;
	file.Read(bNormalBuffer);
	if(bNormalBuffer)
	{
		float* buffer=allocateNormalBuffer(m_nTris_For_Internal_Use);
		file.ReadBuffer((unsigned char*)buffer, sizeof(float)*m_nTris_For_Internal_Use*3*3);
	}


	file.Read(m_nUVChannels);
	if(m_nUVChannels)
		m_pszUVChannels = new gxUV[m_nUVChannels];
	for(int x=0;x<m_nUVChannels;x++)
	{
		int materialCRC=0;
		file.Read(materialCRC);
		m_pszUVChannels[x].m_pszfGLTexCoordList =new float[m_nTris_For_Internal_Use*3*2];
		file.ReadBuffer((unsigned char*)m_pszUVChannels[x].m_pszfGLTexCoordList, sizeof(float)*m_nTris_For_Internal_Use*3*2);
	}
}

void gxMesh::transformationChangedf()
{
	object3d::transformationChangedf();
}