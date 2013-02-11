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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

GUIStatusBar::GUIStatusBar()
{
	m_pEffect = NULL;
}

GUIStatusBar::~GUIStatusBar()
{
}

void GUIStatusBar::LoadVertexBuffer(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, float percentage,
		int leftOffset, int topOffset, int rightOffset, int bottomOffset, bool forwardTexture, float angle, DWORD color,
		float lastPercentage, float timePosition)
{
	IBaseTextureObject *bgTex, *barTex, *twoHTex, *threeHTex, *fourHTex, *ghostTex;
	bgTex = m_BGTex.GetMask();
	barTex = m_BarTex.GetMask();
	twoHTex = m_TwoHundTex.GetMask();
	threeHTex = m_ThreeHundTex.GetMask();
	fourHTex = m_FourHundTex.GetMask();
	ghostTex = m_GhostTex.GetMask();

	if (bgTex)
	{
		GUIVERTEXINFO entry;
		entry.iNumPrims = m_BGTex.LoadVertexData(entry.VertexData, x, y, width, height, angle, color, true);
		entry.pTexture = bgTex;
		vertexData.push_back(entry);
	}

	float barX, barY, barWidth, barHeight;
	barX = x + (float)leftOffset;
	barY = y + (float)topOffset;
	barWidth = width - (float)leftOffset + (float)rightOffset;
	barHeight = height - (float)topOffset + (float)bottomOffset;

	float u1, u2;
	float fractionPercentage, ghostPercentage = 0.0f;
	if (timePosition == 1.0f)
	{
		fractionPercentage = percentage - floor(percentage);	
	}
	else
	{
		if (lastPercentage > percentage)
		{
			fractionPercentage = percentage - floor(percentage);				
			ghostPercentage = CalcSquaredPercentage(lastPercentage, percentage, timePosition);
		}
		else
		{
			ghostPercentage = percentage - floor(percentage);			
			fractionPercentage = CalcSquaredPercentage(lastPercentage, percentage, timePosition);
		}
	}
	if ((percentage > 0.0f) && (fractionPercentage == 0.0f))
	{
		fractionPercentage = 1.0f;
	}
	if ((percentage > 0.0f) && (ghostPercentage == 0.0f))
	{
		ghostPercentage = 1.0f;
	}

	if (forwardTexture)
	{
		u2 = 1.f;
		u1 = 1.f - fractionPercentage;
	}
	else
	{
		u1 = 0.f;
		u2 = fractionPercentage;
	}

	if (percentage <= 1.f)
	{
		if (barTex)
		{
			GUIVERTEXINFO entry;
			entry.iNumPrims = m_BarTex.LoadVertexData(entry.VertexData, barX, barY, 
				barWidth*fractionPercentage, barHeight, angle, color, false, u1, 0.0f, u2, 1.0f);
			entry.pTexture = barTex;
			vertexData.push_back(entry);
		}
	}
	else if (percentage <= 2.f)
	{
		if (barTex)
		{
			GUIVERTEXINFO entry;
			entry.iNumPrims = m_BarTex.LoadVertexData(entry.VertexData, barX, barY, barWidth, barHeight, 
				angle, color, false);
			entry.pTexture = barTex;
			vertexData.push_back(entry);
		}
		if (twoHTex)
		{
			GUIVERTEXINFO entry;
			entry.iNumPrims = m_TwoHundTex.LoadVertexData(entry.VertexData, barX, barY, 
				barWidth*fractionPercentage, barHeight, angle, color, false, u1, 0.0f, u2, 1.0f);
			entry.pTexture = twoHTex;
			vertexData.push_back(entry);
		}
	}	
	else if (percentage <= 3.f)
	{
		if (twoHTex)
		{
			GUIVERTEXINFO entry;
			entry.iNumPrims = m_TwoHundTex.LoadVertexData(entry.VertexData, barX, barY, barWidth, barHeight, 
				angle, color, false);
			entry.pTexture = twoHTex;
			vertexData.push_back(entry);
		}
		if (threeHTex)
		{
			GUIVERTEXINFO entry;
			entry.iNumPrims = m_ThreeHundTex.LoadVertexData(entry.VertexData, barX, barY, 
				barWidth*fractionPercentage, barHeight, angle, color, false,
				u1, 0.0f, u2, 1.0f);
			entry.pTexture = threeHTex;
			vertexData.push_back(entry);
		}
	}	
	else if (percentage <= 4.f)
	{
		if (threeHTex)
		{
			GUIVERTEXINFO entry;
			entry.iNumPrims = m_ThreeHundTex.LoadVertexData(entry.VertexData, barX, barY, barWidth, barHeight, 
				angle, color, false);
			entry.pTexture = threeHTex;
			vertexData.push_back(entry);
		}
		if (fourHTex)
		{
			GUIVERTEXINFO entry;
			entry.iNumPrims = m_FourHundTex.LoadVertexData(entry.VertexData, barX, barY, 
				barWidth*fractionPercentage, barHeight, angle, color, false,
				u1, 0.0f, u2, 1.0f);
			entry.pTexture = fourHTex;
			vertexData.push_back(entry);
		}
	}

	if ((timePosition != 1.0f) && (ghostTex))
	{
		float x, width;
		x = barX + barWidth*fractionPercentage;
		width = (barWidth*ghostPercentage) - (barWidth*fractionPercentage);
		
		GUIVERTEXINFO entry;
		entry.iNumPrims = m_GhostTex.LoadVertexData(entry.VertexData, x, barY, 
			width, barHeight, angle, color, true);
		entry.pTexture = ghostTex;
		vertexData.push_back(entry);
	}
}

void GUIStatusBar::SetTextures(StdString bgTex, StdString barTex, StdString twoHndTex, StdString threeHndTex, StdString fourHndTex)
{
	if (_tcscmp((const TCHAR*)bgTex, _T("")) != 0)
	{
		m_BGTex.LoadTexture(bgTex);
	}
	if (_tcscmp((const TCHAR*)barTex, _T("")) != 0)
	{
		m_BarTex.LoadTexture(barTex);
	}	
	if (_tcscmp((const TCHAR*)twoHndTex, _T("")) != 0)
	{
		m_TwoHundTex.LoadTexture(twoHndTex);
	}
	if (_tcscmp((const TCHAR*)threeHndTex, _T("")) != 0)
	{
		m_ThreeHundTex.LoadTexture(threeHndTex);
	}
	if (_tcscmp((const TCHAR*)fourHndTex, _T("")) != 0)
	{
		m_FourHundTex.LoadTexture(fourHndTex);
	}
}

void GUIStatusBar::SetGhostTexture(StdString ghostTex)
{
	if (_tcscmp(ghostTex, _T("")) != 0)
	{
		m_GhostTex.LoadTexture(ghostTex);	
	}
}

__inline float GUIStatusBar::CalcSquaredPercentage(float lastPercent, float currentPercent, float time)
{
	float val = time * time;
	float diff = currentPercent - lastPercent;
	val *= diff;
	return lastPercent + val;
}