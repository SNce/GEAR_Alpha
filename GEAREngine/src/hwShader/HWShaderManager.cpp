#include "HWShaderManager.h"

HWShaderManager::HWShaderManager()
{
}

HWShaderManager::~HWShaderManager()
{
	Reset();
}

void HWShaderManager::Init()
{
#if defined (USE_ProgrammablePipeLine)
        LoadDefaultShaders();
#endif
}

void HWShaderManager::Reset()
{
#if defined (USE_ProgrammablePipeLine)
	for(std::vector<gxHWShader*>::iterator it = m_cvHWShaderLst.begin(); it != m_cvHWShaderLst.end(); ++it)
	{
		gxHWShader* shader = *it;
		GX_DELETE(shader);
	}
	m_cvHWShaderLst.clear();

	std::vector<stHWShaderSnippet*>* submaplist=&m_cvHWShaderSnippets;
	for(std::vector<stHWShaderSnippet*>::iterator it = submaplist->begin(); it != submaplist->end(); ++it)
	{
		stHWShaderSnippet* snippet = *it;
		GX_DELETE(snippet);
	}
	m_cvHWShaderSnippets.clear();
#endif
}

void HWShaderManager::LoadDefaultShaders()
{
#if defined (USE_ProgrammablePipeLine)
#if defined(WIN32)

	//load code snippets
	stHWShaderSnippet* snippet=LoadCodeSnippet("res/shadersWin32/snippets/matrices_uniform_vars.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("res/shadersWin32/snippets/vertex_attrib_vars.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("res/shadersWin32/snippets/vertex_varying_vars.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("res/shadersWin32/snippets/vertex_main.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);
	snippet=LoadCodeSnippet("res/shadersWin32/snippets/fragment_main.snippet");
	if(snippet)
		m_cvHWShaderSnippets.push_back(snippet);

	gxHWShader* pShader=new gxHWShader();
    if(pShader->loadShader("res/shadersWin32/hwshader/only_diffuse.glsl"))	//0
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader("res/shadersWin32/hwshader/diffusemapunlit.glsl"))	//0
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader("res/shadersWin32/hwshader/only_diffuse_with_color_pointer.glsl"))	//0
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader("res/shadersWin32/hwshader/pvLightingOnlyShaderFirstPass.glsl"))	//0
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader("res/shadersWin32/hwshader/guishader.glsl"))	//0
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
	pShader=new gxHWShader();
    if(pShader->loadShader("res/shadersWin32/hwshader/blurshader.glsl"))	//0
		m_cvHWShaderLst.push_back(pShader);
	else
		GX_DELETE(pShader);
#endif
#endif
}

gxHWShader* HWShaderManager::LoadShaderFromBuffer(const char* name, const char* buffer, int size)
{
	for(std::vector<gxHWShader*>::iterator it = m_cvHWShaderLst.begin(); it != m_cvHWShaderLst.end(); ++it)
	{
		gxHWShader* hwshaderNode = *it;
		if(strcmp(hwshaderNode->getShaderName(), name)==0)
			return hwshaderNode;
	}

	if(buffer==NULL || size==0) return NULL;

	gxHWShader* shader = new gxHWShader();
	if(!shader->loadShaderFromBuffer(name, buffer, size))
	{
		GX_DELETE(shader);
	}
	m_cvHWShaderLst.push_back(shader);
	return shader;
}

HWShaderManager::stHWShaderSnippet* HWShaderManager::LoadCodeSnippet(const char* filename)
{
	//read snippet code
	int fileSz=0;
	FILE* fp=fopen(filename, "r");
	if(fp==NULL) return NULL;

	fseek(fp, 0, SEEK_END);
	fileSz=ftell(fp);
	fclose(fp);
	
	if(!fileSz) return NULL;
	
	//vertex shader source
	fp=fopen(filename, "r");
	if(fp==NULL) return NULL;

	stHWShaderSnippet* newSnippetCode = new stHWShaderSnippet();

	newSnippetCode->size=fileSz;
	newSnippetCode->snippet=new char[newSnippetCode->size];
	memset((void*)newSnippetCode->snippet, 0, newSnippetCode->size);
	fread((void*)newSnippetCode->snippet, 1, newSnippetCode->size, fp);
	fclose(fp);
	//

	return newSnippetCode;
}

#if defined (USE_ProgrammablePipeLine)
gxHWShader* HWShaderManager::GetHWShader(int index)
{
	if(index>=(int)m_cvHWShaderLst.size()) return NULL;
	return m_cvHWShaderLst[index];
}
#endif

void HWShaderManager::update(float dt)
{
//#if defined (USE_ProgrammablePipeLine)
//	for(std::vector<gxHWShader*>::iterator it = m_cvHWShaderLst.begin(); it != m_cvHWShaderLst.end(); ++it)
//	{
//		gxHWShader* hwshaderNode = *it;
//		hwshaderNode->updateShaderVars(dt);
//	}
//#endif
}
