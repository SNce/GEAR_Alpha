/*
 *  gxTexture.h
 *  BMXProRider
 *
 *  Created by arun on 16/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GXTEXTURE_H
#define GXTEXTURE_H

#include "stTexturePacket.h"
#include "matrix4x4f.h"

class gxTexture
{
public:
	enum ETEXTURE
	{
		TEX_NORMAL,
		TEX_ALPHA,
		TEX_UNDEFINED
	};
	
	gxTexture()
	{
		m_pTexTurePtr=NULL;
		m_eTextureType=TEX_UNDEFINED;
		m_iFileCRC=0;
	}
	
	~gxTexture()
	{
	}
	
	void reset()
	{
	}
	
	void setTexture(stTexturePacket* pTexTurePtr)		{	m_pTexTurePtr=pTexTurePtr;			}
	unsigned int getTextureID()	const
    {
        return (m_pTexTurePtr)?m_pTexTurePtr->iTextureID:0;
    }
    
	void setTextureType(ETEXTURE type)		{	m_eTextureType=type;	}
	ETEXTURE getTextureType() const			{	return m_eTextureType;	}
	
	matrix4x4f* getTextureMatrix()			{	return &m_cMatrix;		}

	void setFileCRC(int crc)	{	m_iFileCRC=crc;		}
	int getFileCRC()			{	return m_iFileCRC;	}

private:
	stTexturePacket* m_pTexTurePtr;     //must not delete this pointer
	ETEXTURE m_eTextureType;
	matrix4x4f m_cMatrix;
	int m_iFileCRC;
};

#endif