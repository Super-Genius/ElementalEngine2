///============================================================================
/// \note   Elemental Engine
///         Copyright (c)  2005-2008 Signature Devices, Inc.
///
///         This code is redistributable under the terms of the EE License.
///
///         This code is distributed without warranty or implied warranty of
///			merchantability or fitness for a particular purpose. See the 
///			EE License for more details.
///
///         You should have received a copy of the EE License along with this
///			code; If not, write to Signature Devices, Inc.,
///			3200 Bridge Parkway Suite 102, Redwood City, CA 94086 USA.
///============================================================================

#include "stdafx.h"
#include "IAnimatedTexture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace ElementalEngine;

CLayeredMask::CLayeredMask()
{
	m_pMask = NULL;
	
	m_szTexFile = _T("");
	

	// TODO: switch to get time delta?
	ITimer* timer = EngineGetToolBox()->GetTimer();
	m_fStartTime = timer->GetTime();
	m_bIsAnimatedTexture = false;
}

CLayeredMask::~CLayeredMask()
{
}

void CLayeredMask::LoadTexture(StdString &filename)
{
	m_szTexFile = filename;
	CHashString fullname( filename );
	static DWORD dwEmpty = CHashString(_T("")).GetUniqueID();
	if (fullname.GetUniqueID() != dwEmpty)
	{
		TEXTUREOBJECTPARAMS tobj;
		tobj.bLoad = true;
		tobj.Name = &fullname;
		tobj.TextureObjectInterface = NULL;
		static DWORD msgHash_AddTexture = CHashString(_T("AddTexture")).GetUniqueID();
		DWORD res = EngineGetToolBox()->SendMessage(msgHash_AddTexture, sizeof(tobj), &tobj );
		if( res == MSG_HANDLED )
		{
			m_pMask = tobj.TextureObjectInterface;
			if (m_pMask)
			{
				if (m_pMask->GetTextureType() == EE_TEXTUREANIMATED2D)
				{
					m_bIsAnimatedTexture = true;
				}
				else
				{
					m_bIsAnimatedTexture = false;
				}
			}
		}
		else
		{
			m_pMask = NULL;
		}
	}
	else
	{
		m_pMask = NULL;
	}
}

IBaseTextureObject* CLayeredMask::GetMask()
{
	if (m_bIsAnimatedTexture)
	{
		return GetSubTex();
	}
	else
	{
		return m_pMask;
	}
}

IBaseTextureObject* CLayeredMask::GetSubTex()
{
	if (m_pMask)
	{
		IAnimatedTextureObject *pAnimTex = NULL;
		pAnimTex = dynamic_cast<IAnimatedTextureObject*>(m_pMask);

		if (pAnimTex)
		{
			ITimer* timer = EngineGetToolBox()->GetTimer();
			float currTime = timer->GetTime();

			float timeDelta = currTime - m_fStartTime;
			float fFPS = pAnimTex->GetFrameRate();
			int iTextureFrame = (int)(timeDelta * fFPS);
			iTextureFrame %= pAnimTex->GetNumFrames();
			
			ITextureObject *texOut = NULL;
			float u1, v1, u2, v2;
			// TODO: allow for different uv sets, model can't use dynamic uvs
            pAnimTex->GetFrame(iTextureFrame, u1, v1, u2, v2, &texOut);

			return texOut;
		}
		else
		{
			return m_pMask;
		}
	}
	return NULL;
}

UINT CLayeredMask::LoadVertexData(GUIVERTEX vertexData[], float x, float y, float width, float height, float angle, 
					DWORD color, bool smartScaled, float uStart, float vStart, float uEnd, float vEnd)
{
	UINT numPrims = 0;
	float centerX = x + (width / 2.0f);
	float centerY = y + (height / 2.0f);
	// NOTE: smart scaled textures can't use start and end
	if (smartScaled)
	{
		IBaseTextureObject *curTex = GetMask();
		if (curTex)
		{
			UINT tWidth = curTex->GetWidth();
			UINT tHeight = curTex->GetHeight();

			if ((tWidth > (unsigned int)(width)) && (tHeight > (unsigned int)(height)))
			{
				SetQuad(vertexData, 0, x, y, width, height, 0.0f, 0.0f, 1.0f, 1.0f, angle, color, centerX, centerY);
				numPrims = 2;
			}
			else if (tWidth > (unsigned int)(width))
			{
				float midY = (float)tHeight / 2.0f;			
				float midV = (midY + 1) / (float)tHeight;

				SetQuad(vertexData, 0, x, y, width, midY, 0.0f, 0.0f, 1.0f, 0.5f, angle, color, centerX, centerY);
				SetQuad(vertexData, 6, x, y+midY, width, height-(float)tHeight, 0.0f, 0.5f, 1.0f, midV, angle, color, centerX, centerY);
				SetQuad(vertexData, 12, x, (y+height)-midY, width, midY, 0.0f, midV, 1.0f, 1.0f, angle, color, centerX, centerY);
				numPrims = 6;
			}
			else if (tHeight > (unsigned int)(height))
			{
				float midX = (float)tWidth / 2.0f;			
				float midU = (midX + 1) / (float)tWidth;

				SetQuad(vertexData, 0, x, y, midX, height, 0.0f, 0.0f, 0.5f, 1.0f, angle, color, centerX, centerY);
				SetQuad(vertexData, 6, x+midX, y, width-(float)tWidth, height, 0.5f, 0.0f, midU, 1.0f, angle, color, centerX, centerY);
				SetQuad(vertexData, 12, (x+width)-midX, y, midX, height, midU, 0.0f, 1.0f, 1.0f, angle, color, centerX, centerY);
				numPrims = 6;
			}
			else
			{
				float midX = (float)tWidth / 2.0f;
				float midY = (float)tHeight / 2.0f;

				float uOff = (midX + 1) / (float) tWidth;
				float vOff = (midY + 1) / (float) tHeight;
				
				float quadXPos[4], quadYPos[4];

				quadXPos[0] = x;
				quadXPos[1] = x + midX;
				quadXPos[2] = x + width - midX;
				quadXPos[3] = x + width;

				quadYPos[0] = y;
				quadYPos[1] = y + midY;
				quadYPos[2] = y + height - midY;
				quadYPos[3] = y + height;
				
				SetQuad(vertexData, 0, quadXPos[0], quadYPos[0], quadXPos[1]-quadXPos[0], quadYPos[1]-quadYPos[0], 0.0f, 0.0f, 0.5f, 0.5f, angle, color, centerX, centerY);
				SetQuad(vertexData, 6, quadXPos[1], quadYPos[0], quadXPos[2]-quadXPos[1], quadYPos[1]-quadYPos[0], 0.5f, 0.0f, uOff, 0.5f, angle, color, centerX, centerY);
				SetQuad(vertexData, 12, quadXPos[2], quadYPos[0], quadXPos[3]-quadXPos[2], quadYPos[1]-quadYPos[0], uOff, 0.0f, 1.0f, 0.5f, angle, color, centerX, centerY);				

				SetQuad(vertexData, 18, quadXPos[0], quadYPos[1], quadXPos[1]-quadXPos[0], quadYPos[2]-quadYPos[1], 0.0f, 0.5f, 0.5f, vOff, angle, color, centerX, centerY);
				SetQuad(vertexData, 24, quadXPos[1], quadYPos[1], quadXPos[2]-quadXPos[1], quadYPos[2]-quadYPos[1], 0.5f, 0.5f, uOff, vOff, angle, color, centerX, centerY);
				SetQuad(vertexData, 30, quadXPos[2], quadYPos[1], quadXPos[3]-quadXPos[2], quadYPos[2]-quadYPos[1], uOff, 0.5f, 1.0f, vOff, angle, color, centerX, centerY);

				SetQuad(vertexData, 36, quadXPos[0], quadYPos[2], quadXPos[1]-quadXPos[0], quadYPos[3]-quadYPos[2], 0.0f, vOff, 0.5f, 1.0f, angle, color, centerX, centerY);
				SetQuad(vertexData, 42, quadXPos[1], quadYPos[2], quadXPos[2]-quadXPos[1], quadYPos[3]-quadYPos[2], 0.5f, vOff, uOff, 1.0f, angle, color, centerX, centerY);
				SetQuad(vertexData, 48, quadXPos[2], quadYPos[2], quadXPos[3]-quadXPos[2], quadYPos[3]-quadYPos[2], uOff, vOff, 1.0f, 1.0f, angle, color, centerX, centerY);	

				numPrims = 18;
			}	
		}
		else
		{
			SetQuad(vertexData, 0, x, y, width, height, 0.0f, 0.0f, 1.0f, 1.0f, angle, color, centerX, centerY);	
			numPrims = 2;
		}
	}

	else
	{
		SetQuad(vertexData, 0, x, y, width, height, uStart, vStart, uEnd, vEnd, angle, color, centerX, centerY);	
		numPrims = 2;
	}	

	return numPrims;
}

void CLayeredMask::SetQuad(GUIVERTEX vertexData[], UINT startIndex, float x, float y, float width, float height, 
		float uStart, float vStart, float uEnd, float vEnd, float angle, DWORD color, float centerX, float centerY)
{
	Vec3 topLeft(-0.5f*width, -0.5f*height, 0.0f);
	Vec3 topRight(0.5f*width, -0.5f*height, 0.0f);
	Vec3 bottomLeft(-0.5f*width, 0.5f*height, 0.0f);
	Vec3 bottomRight(0.5f*width, 0.5f*height, 0.0f);

	Matrix4x4 mat;
	EulerAngle rot(angle, 0.0f, 0.0f);
	mat.SetRotation(rot);
	Vec3 trans((x+(width/2.0f))-centerX, (y+(height/2.0f))-centerY, 0.f);
	trans = mat * trans;
	trans.x = trans.x + centerX;
	trans.y = trans.y + centerY;
	mat.SetTranslation(trans);

	topLeft = mat * topLeft;
	topRight = mat * topRight;
	bottomLeft = mat * bottomLeft;
	bottomRight = mat * bottomRight;

	// NOTE: winding is NOW cw
	vertexData[startIndex].pos[0] = floor(topLeft.x)-0.5f;
	vertexData[startIndex].pos[1] = floor(topLeft.y)-0.5f;
	vertexData[startIndex].pos[2] = 0.0f;
	vertexData[startIndex].u = uStart;
	vertexData[startIndex].v = vStart;
	vertexData[startIndex].color = color;

	vertexData[startIndex+1].pos[0] = floor(topRight.x)-0.5f;
	vertexData[startIndex+1].pos[1] = floor(topRight.y)-0.5f;
	vertexData[startIndex+1].pos[2] = 0.0f;
	vertexData[startIndex+1].u = uEnd;
	vertexData[startIndex+1].v = vStart;
	vertexData[startIndex+1].color = color;

	vertexData[startIndex+2].pos[0] = floor(bottomLeft.x)-0.5f;
	vertexData[startIndex+2].pos[1] = floor(bottomLeft.y)-0.5f;
	vertexData[startIndex+2].pos[2] = 0.0f;
	vertexData[startIndex+2].u = uStart;
	vertexData[startIndex+2].v = vEnd;
	vertexData[startIndex+2].color = color;

	vertexData[startIndex+3].pos[0] = floor(topRight.x)-0.5f;
	vertexData[startIndex+3].pos[1] = floor(topRight.y)-0.5f;
	vertexData[startIndex+3].pos[2] = 0.0f;
	vertexData[startIndex+3].u = uEnd;
	vertexData[startIndex+3].v = vStart;
	vertexData[startIndex+3].color = color;

	vertexData[startIndex+4].pos[0] = floor(bottomRight.x)-0.5f;
	vertexData[startIndex+4].pos[1] = floor(bottomRight.y)-0.5f;
	vertexData[startIndex+4].pos[2] = 0.0f;
	vertexData[startIndex+4].u = uEnd;
	vertexData[startIndex+4].v = vEnd;
	vertexData[startIndex+4].color = color;

	vertexData[startIndex+5].pos[0] = floor(bottomLeft.x)-0.5f;
	vertexData[startIndex+5].pos[1] = floor(bottomLeft.y)-0.5f;
	vertexData[startIndex+5].pos[2] = 0.0f;
	vertexData[startIndex+5].u = uStart;
	vertexData[startIndex+5].v = vEnd;
	vertexData[startIndex+5].color = color;	
}