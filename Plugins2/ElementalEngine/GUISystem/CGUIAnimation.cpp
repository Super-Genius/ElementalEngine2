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

REGISTER_COMPONENT(CGUIAnimation);
REGISTER_MESSAGE_HANDLER(StartGUIAnimation, OnStartGUIAnimation, CGUIAnimation);
REGISTER_MESSAGE_HANDLER(StopGUIAnimation, OnStopGUIAnimation, CGUIAnimation);
REGISTER_MESSAGE_HANDLER(GetGUIAnimationIsRunning, OnGetGUIAnimationIsRunning, CGUIAnimation);
REGISTER_MESSAGE_HANDLER(GetAnimationData, OnGetAnimationData, CGUIAnimation);
REGISTER_MESSAGE_HANDLER(ScrubGUIAnimation, OnScrubGUIAnimation, CGUIAnimation);
REGISTER_MESSAGE_HANDLER(AddKeysToAnimation, OnAddKeysToAnimation, CGUIAnimation);
REGISTER_MESSAGE_HANDLER(AddOneKey, OnAddOneKey, CGUIAnimation);
REGISTER_MESSAGE_HANDLER(DeleteOneKey, OnDeleteOneKey, CGUIAnimation);
REGISTER_MESSAGE_HANDLER(AddGUIAnimationCallback, OnAddGUIAnimationCallback, CGUIAnimation);
REGISTER_MESSAGE_HANDLER(ChangeGUIAnimKey, OnChangeGUIAnimKey, CGUIAnimation);
REGISTER_MESSAGE_HANDLER(IsGUIAnimationRelativePositions, OnIsGUIAnimationRelativePositions, CGUIAnimation);

CGUIAnimation::CGUIAnimation(IHashString *parentName, IHashString *name)
:OBJECTTEMPLATE(CGUIManager, CGUIAnimation, IObject, parentName, name)
{
	m_iObjectNum = 0;
	m_fTotalTime = 0.f;
	m_fCurrentTime = 0.f;
	m_bIsRunning = false;
	m_ToolBox = EngineGetToolBox();
	m_iFramesPerSec = 30;
	m_bIsLooping = false;
	m_bScrub = false;
	m_fSaveVersion = 1.1f;
	m_bRelativePositions = true;
	m_bInitialSerialize = true;
}

CGUIAnimation::~CGUIAnimation()
{}

IComponent *CGUIAnimation::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = new CGUIAnimation(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CGUIAnimation::Init()
{}

void CGUIAnimation::DeInit()
{}

bool CGUIAnimation::Update()
{
	if (m_bIsRunning)
	{
		if (m_bScrub)
		{
			m_bIsRunning = false;
			m_bScrub = false;
		}
		else
		{
			ITimer* timer = m_ToolBox->GetTimer();
			m_fCurrentTime = timer->GetTime();
			if ((m_fCurrentTime - m_fStartTime) > m_fTotalTime)
			{
				if (m_bIsLooping)
				{
					/// restart the animation
					OnStartGUIAnimation(0, NULL);
				}
				else
				{
					m_bIsRunning = false;				
				}

				// anim callbacks
				for (UINT i=0; i<m_AnimCallbacks.size(); i++)
				{
					static DWORD msgQueueGUIStateEvent = CHashString(_T("QueueGUIStateEvent")).GetUniqueID();
					m_ToolBox->SendMessage(msgQueueGUIStateEvent, sizeof(GUIEVENT), &m_AnimCallbacks[i]);						
				}
			}
		}
		float dispTime = m_fCurrentTime - m_fStartTime;

		// calc current frame, send info out
		UINT currentFrame = (UINT)(dispTime * (float)m_iFramesPerSec);
		static DWORD msgHash_UpdateGUIAnimBarPos = CHashString(_T("UpdateGUIAnimBarPos")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_UpdateGUIAnimBarPos, sizeof(UINT), &currentFrame);
		for (UINT i=0; i<m_AnimObjects.size(); i++)
		{
			int lastKey;
			int key = m_AnimObjects[i].m_chPosX.m_Channel.GetKeyframeIndex(m_AnimObjects[i].m_chPosX.m_iLastKey, dispTime);
					
			lastKey = key - 1;
			if (lastKey < 0)
			{
				lastKey = 0;
			}
			m_AnimObjects[i].m_chPosX.m_iLastKey = lastKey;
			m_AnimObjects[i].m_chPosY.m_iLastKey = lastKey;
			

			if (key == -1)
			{
				key = m_AnimObjects[i].m_chPosX.m_Channel.GetLastKeyIndex();
				lastKey = key;
			}

			if (m_AnimObjects[i].m_chPosX.m_iNumKeys != 0)
			{
				// xpos and ypos keys should always be linked
				float xPos = m_AnimObjects[i].m_chPosX.m_Channel.Interpolate(dispTime, lastKey, key);
				float yPos = m_AnimObjects[i].m_chPosY.m_Channel.Interpolate(dispTime, lastKey, key);
				POSITIONDATA pd;
				pd.m_fXPos = xPos;
				pd.m_fYPos = yPos;

				if (m_bRelativePositions)
				{
					static DWORD msgHash_SetPositionRelative = CHashString(_T("SetPositionRelative")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_SetPositionRelative, sizeof(POSITIONDATA), &pd, &m_AnimObjects[i].m_szObjName, &m_AnimObjects[i].m_szObjType);
				}
				else
				{
					static DWORD msgHash_SetPosition = CHashString(_T("SetPosition")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_SetPosition, sizeof(POSITIONDATA), &pd, &m_AnimObjects[i].m_szObjName, &m_AnimObjects[i].m_szObjType);
				}
			}

			key = m_AnimObjects[i].m_chSizeX.m_Channel.GetKeyframeIndex(m_AnimObjects[i].m_chSizeX.m_iLastKey, dispTime);
			
			lastKey = key - 1;
			if (lastKey < 0)
			{
				lastKey = 0;
			}
			m_AnimObjects[i].m_chSizeX.m_iLastKey = lastKey;
			m_AnimObjects[i].m_chSizeY.m_iLastKey = lastKey;

			if (key == -1)
			{
				key = m_AnimObjects[i].m_chSizeX.m_Channel.GetLastKeyIndex();
				lastKey = key;
			}

			if (m_AnimObjects[i].m_chSizeX.m_iNumKeys != 0)
			{
				// width and height keys should always be linked
				float width = m_AnimObjects[i].m_chSizeX.m_Channel.Interpolate(dispTime, lastKey, key);
				float height = m_AnimObjects[i].m_chSizeY.m_Channel.Interpolate(dispTime, lastKey, key);
				SIZEDATA sd;
				sd.m_fXSize = width;
				sd.m_fYSize = height;
				static DWORD msgHash_SetSize = CHashString(_T("SetSize")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_SetSize, sizeof(SIZEDATA), &sd, &m_AnimObjects[i].m_szObjName, &m_AnimObjects[i].m_szObjType);
			}

			key = m_AnimObjects[i].m_chAlphaCol.m_Channel.GetKeyframeIndex(m_AnimObjects[i].m_chAlphaCol.m_iLastKey, dispTime);
			
			lastKey = key - 1;
			if (lastKey < 0)
			{
				lastKey = 0;
			}

			m_AnimObjects[i].m_chAlphaCol.m_iLastKey = lastKey;
			m_AnimObjects[i].m_chRCol.m_iLastKey = lastKey;
			m_AnimObjects[i].m_chGCol.m_iLastKey = lastKey;
			m_AnimObjects[i].m_chBCol.m_iLastKey = lastKey;

			if (key == -1)
			{
				key = m_AnimObjects[i].m_chAlphaCol.m_Channel.GetLastKeyIndex();
				lastKey = key;
			}

			if (m_AnimObjects[i].m_chAlphaCol.m_iNumKeys != 0)
			{
				// color keys should always be linked
				float a = m_AnimObjects[i].m_chAlphaCol.m_Channel.Interpolate(dispTime, lastKey, key);
				float r = m_AnimObjects[i].m_chRCol.m_Channel.Interpolate(dispTime, lastKey, key);
				float g = m_AnimObjects[i].m_chGCol.m_Channel.Interpolate(dispTime, lastKey, key);
				float b = m_AnimObjects[i].m_chBCol.m_Channel.Interpolate(dispTime, lastKey, key);
				
				if (a > 255.f)
				{
					a = 255.f;
				}
				if (a < 0.f)
				{
					a = 0.f;
				}
				if (r > 255.f)
				{
					r = 255.f;
				}
				if (r < 0.f)
				{
					r = 0.f;
				}
				if (g > 255.f)
				{
					g = 255.f;
				}
				if (g < 0.f)
				{
					g = 0.f;
				}
				if (b > 255.f)
				{
					b = 255.f;
				}
				if (b < 0.f)
				{
					b = 0.f;
				}
				DWORD color = 0;
				color += (DWORD)a;
				color = color << 8;
				color += (DWORD)r;
				color = color << 8;
				color += (DWORD)g;
				color = color << 8;
				color += (DWORD)b;

				static DWORD msgHash_SetColor = CHashString(_T("SetColor")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_SetColor, sizeof(DWORD), &color, &m_AnimObjects[i].m_szObjName, &m_AnimObjects[i].m_szObjType);
			}		
			
			key = m_AnimObjects[i].m_chRot.m_Channel.GetKeyframeIndex(m_AnimObjects[i].m_chRot.m_iLastKey, dispTime);
			
			lastKey = key - 1;
			if (lastKey < 0)
			{
				lastKey = 0;
			}
			m_AnimObjects[i].m_chRot.m_iLastKey = lastKey;			

			if (key == -1)
			{
				key = m_AnimObjects[i].m_chRot.m_Channel.GetLastKeyIndex();
				lastKey = key;
			}

			if (m_AnimObjects[i].m_chRot.m_iNumKeys != 0)
			{
				float rot = m_AnimObjects[i].m_chRot.m_Channel.Interpolate(dispTime, lastKey, key);
				
				static DWORD msgHash_SetAngle = CHashString(_T("SetAngle")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_SetAngle, sizeof(float), &rot, &m_AnimObjects[i].m_szObjName, &m_AnimObjects[i].m_szObjType);
			}
		}
	}
	return true;
}

void CGUIAnimation::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
		CHashString versionName;
		static DWORD vers2_0a = CHashString(_T("2.0a")).GetUniqueID();
		static DWORD vers2_0b = CHashString(_T("2.0b")).GetUniqueID();
		static DWORD vers2_0c = CHashString(_T("2.0c")).GetUniqueID();
		static DWORD msgHash_GetFileVersion = CHashString(_T("GetFileVersion")).GetUniqueID();
		DWORD retval = EngineGetToolBox()->SendMessage(msgHash_GetFileVersion, sizeof(IHashString), &versionName);

		int verNum = 0;
		
		if (versionName.GetUniqueID() == vers2_0a)
		{
			verNum = 3;
		}
		else if (versionName.GetUniqueID() == vers2_0b)
		{
			verNum = 4;
		}
		else if (versionName.GetUniqueID() == vers2_0c)
		{
			verNum = 5;
		}
		else 
		{
			m_ToolBox->Log(LOGERROR, _T("Bad GUI file version!"));
			assert(0);
			return;
		}

		float loadVersion = 0.0f;
		
		if (verNum >= 4)
		{
			ar.Read(loadVersion);
			if (loadVersion == 0.0f)
			{
				// initial serialize from editor; set to save version (up to date schema)
				loadVersion = m_fSaveVersion;
			}
		}

		ar.Read(m_iObjectNum);
		ar.Read(m_fTotalTime);
		ar.Read(m_iFramesPerSec);		

		// ensure this is never 0!
		if (m_iFramesPerSec == 0)
		{
			m_iFramesPerSec = 30;
		}
		ar.Read(m_bIsLooping);

		if (loadVersion >= 1.1f)
		{
			bool relativePos;
			ar.Read(relativePos);
			if ((!m_bInitialSerialize) && (m_bRelativePositions != relativePos))
			{
				// positions switched, gotta recompute the keys
				ConvertPositions();
			}
			m_bRelativePositions = relativePos;
		}

		if (m_bInitialSerialize)
		{
			m_bInitialSerialize = false;
		}

		if (ar.IsEndOfFile())
		{
			return;
		}
		
		for (UINT i=0; i<m_iObjectNum; i++)
		{
			GUIANIMOBJ gao;
			StdString oName, oType;
			ar.Read(oName);
			ar.Read(oType);
			gao.m_szObjName = (const TCHAR*)oName;
			gao.m_szObjType = (const TCHAR*)oType;

			for (UINT j=0; j<4; j++)
			{
				StdString cType;
				ar.Read(cType);
				if (_tcscmp((const TCHAR*)cType, _T("Pos"))==0)
				{
					UINT keyNum;
					float time, xVal, yVal;
					ar.Read(keyNum);
					gao.m_chPosX.m_iNumKeys = keyNum;
					gao.m_chPosY.m_iNumKeys = keyNum;

					for (UINT k=0; k<keyNum; k++)
					{
						ar.Read(time);
						ar.Read(xVal);
						ar.Read(yVal);
						
						gao.m_chPosX.m_Channel.AddKeyframe(time, xVal);
						gao.m_chPosY.m_Channel.AddKeyframe(time, yVal);						
					}					
				}
				else if (_tcscmp((const TCHAR*)cType, _T("Size"))==0)
				{
					UINT keyNum;
					float time, xVal, yVal;
					ar.Read(keyNum);
					gao.m_chSizeX.m_iNumKeys = keyNum;
					gao.m_chSizeY.m_iNumKeys = keyNum;

					for (UINT k=0; k<keyNum; k++)
					{
						ar.Read(time);
						ar.Read(xVal);
						ar.Read(yVal);
						
						gao.m_chSizeX.m_Channel.AddKeyframe(time, xVal);
						gao.m_chSizeY.m_Channel.AddKeyframe(time, yVal);
					}					
				}
				else if (_tcscmp((const TCHAR*)cType, _T("Color"))==0)
				{
					UINT keyNum;
					float time;
					UINT a, r, g, b;
					ar.Read(keyNum);
					gao.m_chAlphaCol.m_iNumKeys = keyNum;
					gao.m_chRCol.m_iNumKeys = keyNum;
					gao.m_chGCol.m_iNumKeys = keyNum;
					gao.m_chBCol.m_iNumKeys = keyNum;

					for (UINT k=0; k<keyNum; k++)
					{
						ar.Read(time);
						ar.Read(r);
						ar.Read(g);
						ar.Read(b);
						ar.Read(a);
						
						gao.m_chAlphaCol.m_Channel.AddKeyframe(time, (float)a);
						gao.m_chRCol.m_Channel.AddKeyframe(time, (float)r);
						gao.m_chGCol.m_Channel.AddKeyframe(time, (float)g);
						gao.m_chBCol.m_Channel.AddKeyframe(time, (float)b);
					}					
				}
				else if (_tcscmp(cType, _T("Rotation")) == 0)
				{
					UINT keyNum;
					float time;
					float val;
					ar.Read(keyNum);
					gao.m_chRot.m_iNumKeys = keyNum;

					for (UINT k=0; k<keyNum; k++)
					{
						ar.Read(time);
						ar.Read(val);
						gao.m_chRot.m_Channel.AddKeyframe(time, val);
					}
				}
			}
			m_AnimObjects.push_back(gao);
		}
	}
	else
	{
		ar.Write(m_fSaveVersion, _T("Version"));
		ar.Write(m_iObjectNum, _T("ObjectNum"));
		ar.Write(m_fTotalTime, _T("TotalTime"));
		ar.Write(m_iFramesPerSec, _T("FramesPerSec"));
		ar.Write(m_bIsLooping, _T("IsLooping"));
		ar.Write(m_bRelativePositions, _T("PositionRelative"));
		for (UINT i=0; i<m_iObjectNum; i++)
		{
			ar.StartClass(_T("GUIAnimObject"));

			StdString oName, oType;
			oName = m_AnimObjects[i].m_szObjName.GetString();
			oType = m_AnimObjects[i].m_szObjType.GetString();
			ar.Write(oName, _T("ObjectName"));
			ar.Write(oType, _T("ObjectType"));

			// write each channel
			ar.StartClass(_T("GUIChannel"));
			GUIKEYLIST xPos, yPos;
			xPos = m_AnimObjects[i].m_chPosX.m_Channel.GetKeys();
			yPos = m_AnimObjects[i].m_chPosY.m_Channel.GetKeys();

			assert(xPos.size() == yPos.size());

			ar.Write(_T("Pos"), _T("Type"));
			ar.Write((int)xPos.size(), _T("KeyNum"));
			for (UINT j=0; j<xPos.size(); j++)
			{
				ar.StartClass(_T("GUIKey"));
				ar.Write(xPos[j].time, _T("Time"));
				ar.Write(xPos[j].value, _T("XPos"));
				ar.Write(yPos[j].value, _T("YPos"));
				ar.EndClass();
			}
			ar.EndClass(); 

			ar.StartClass(_T("GUIChannel"));
			GUIKEYLIST width, height;
			width = m_AnimObjects[i].m_chSizeX.m_Channel.GetKeys();
			height = m_AnimObjects[i].m_chSizeY.m_Channel.GetKeys();

			assert(width.size() == height.size());

			ar.Write(_T("Size"), _T("Type"));
			ar.Write((int)width.size(), _T("KeyNum"));
			for (UINT j=0; j<width.size(); j++)
			{
				ar.StartClass(_T("GUIKey"));
				ar.Write(width[j].time, _T("Time"));
				ar.Write(width[j].value, _T("Width"));
				ar.Write(height[j].value, _T("Height"));
				ar.EndClass();
			}
			ar.EndClass(); 

			ar.StartClass(_T("GUIChannel"));
			GUIKEYLIST a, r, g, b;
			a = m_AnimObjects[i].m_chAlphaCol.m_Channel.GetKeys();
			r = m_AnimObjects[i].m_chRCol.m_Channel.GetKeys();
			g = m_AnimObjects[i].m_chGCol.m_Channel.GetKeys();
			b = m_AnimObjects[i].m_chBCol.m_Channel.GetKeys();

			assert((a.size() == r.size()) && (a.size() == g.size()) && (a.size() == b.size()));

			ar.Write(_T("Color"), _T("Type"));
			ar.Write((int)a.size(), _T("KeyNum"));
			for (UINT j=0; j<a.size(); j++)
			{
				ar.StartClass(_T("GUIKey"));
				ar.Write(a[j].time, _T("Time"));
				ar.Write(r[j].value, _T("R"));
				ar.Write(g[j].value, _T("G"));
				ar.Write(b[j].value, _T("B"));
				ar.Write(a[j].value, _T("A"));
				ar.EndClass();
			}
			ar.EndClass(); 

			ar.StartClass(_T("GUIChannel"));
			GUIKEYLIST rot;
			rot = m_AnimObjects[i].m_chRot.m_Channel.GetKeys();			

			ar.Write(_T("Rotation"), _T("Type"));
			ar.Write((int)rot.size(), _T("KeyNum"));
			for (UINT j=0; j<rot.size(); j++)
			{
				ar.StartClass(_T("GUIKey"));
				ar.Write(rot[j].time, _T("Time"));
				ar.Write(rot[j].value, _T("Angle"));				
				ar.EndClass();
			}
			ar.EndClass(); 

			ar.EndClass();
		}
	}
}

DWORD CGUIAnimation::OnStartGUIAnimation(DWORD size, void *param)
{
	m_bIsRunning = true;
	ITimer* timer = m_ToolBox->GetTimer();
	m_fStartTime = timer->GetTime();
	for (UINT i=0; i<m_AnimObjects.size(); i++)
	{
		m_AnimObjects[i].m_chAlphaCol.m_iLastKey = 0;
		m_AnimObjects[i].m_chRCol.m_iLastKey = 0;
		m_AnimObjects[i].m_chRCol.m_iLastKey = 0;
		m_AnimObjects[i].m_chBCol.m_iLastKey = 0;
		m_AnimObjects[i].m_chSizeX.m_iLastKey = 0;
		m_AnimObjects[i].m_chSizeY.m_iLastKey = 0;
		m_AnimObjects[i].m_chPosX.m_iLastKey = 0;
		m_AnimObjects[i].m_chPosY.m_iLastKey = 0;
	}
	return MSG_HANDLED_STOP;
}

DWORD CGUIAnimation::OnStopGUIAnimation(DWORD size, void *param)
{
	m_bIsRunning = false;
	return MSG_HANDLED_STOP;
}

DWORD CGUIAnimation::OnGetGUIAnimationIsRunning(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	bool *ret = (bool*) param;
	*ret = m_bIsRunning;
	return MSG_HANDLED_STOP;
}

DWORD CGUIAnimation::OnGetAnimationData(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GUIANIMDATA));
	GUIANIMDATA *gad = (GUIANIMDATA*) param;
	gad->animName = GetName()->GetString();
	gad->iFrameRate = m_iFramesPerSec;
	for (UINT i=0; i<m_AnimObjects.size(); i++)
	{
		UINT j;
		GUIEDANIMOBJ geao;
		geao.objName = m_AnimObjects[i].m_szObjName.GetString();

		GUIKEYLIST xPos, yPos;
		xPos = m_AnimObjects[i].m_chPosX.m_Channel.GetKeys();
		yPos = m_AnimObjects[i].m_chPosY.m_Channel.GetKeys();

		for (j=0; j<xPos.size(); j++)
		{
			GUIEDKEY xKey, yKey;			
			xKey.value = xPos[j].value;
			xKey.key = (UINT)(xPos[j].time * (float)m_iFramesPerSec);
			yKey.value = yPos[j].value;
			yKey.key = (UINT)(yPos[j].time * (float)m_iFramesPerSec);
			geao.posChannel.xPos.push_back(xKey);
			geao.posChannel.yPos.push_back(yKey);
		}

		GUIKEYLIST width, height;
		width = m_AnimObjects[i].m_chSizeX.m_Channel.GetKeys();
		height = m_AnimObjects[i].m_chSizeY.m_Channel.GetKeys();

		for (j=0; j<width.size(); j++)
		{
			GUIEDKEY xKey, yKey;			
			xKey.value = width[j].value;
			xKey.key = (UINT)(width[j].time * (float)m_iFramesPerSec);
			yKey.value = height[j].value;
			yKey.key = (UINT)(height[j].time * (float)m_iFramesPerSec);
			geao.sizeChannel.width.push_back(xKey);
			geao.sizeChannel.height.push_back(yKey);
		}

		GUIKEYLIST a, r, g, b;
		a = m_AnimObjects[i].m_chAlphaCol.m_Channel.GetKeys();
		r = m_AnimObjects[i].m_chRCol.m_Channel.GetKeys();
		g = m_AnimObjects[i].m_chGCol.m_Channel.GetKeys();
		b = m_AnimObjects[i].m_chBCol.m_Channel.GetKeys();

		for (j=0; j<a.size(); j++)
		{
			GUIEDKEY aKey, rKey, gKey, bKey;
			aKey.value = a[j].value;
			aKey.key = (UINT)(a[j].time * (float)m_iFramesPerSec);
			rKey.value = r[j].value;
			rKey.key = (UINT)(r[j].time * (float)m_iFramesPerSec);
			gKey.value = g[j].value;
			gKey.key = (UINT)(g[j].time * (float)m_iFramesPerSec);
			bKey.value = b[j].value;
			bKey.key = (UINT)(b[j].time * (float)m_iFramesPerSec);			
			geao.colorChannel.alpha.push_back(aKey);
			geao.colorChannel.r.push_back(rKey);
			geao.colorChannel.g.push_back(gKey);
			geao.colorChannel.b.push_back(bKey);
		}
		GUIKEYLIST rot;
		rot = m_AnimObjects[i].m_chRot.m_Channel.GetKeys();
		for (j=0; j<rot.size(); j++)
		{
			GUIEDKEY rotKey;
			rotKey.value = rot[j].value;
			rotKey.key = (UINT)(rot[j].time * (float)m_iFramesPerSec);
			geao.rotationChannel.rot.push_back(rotKey);
		}
		gad->vObjects.push_back(geao);
	}
	return MSG_HANDLED_STOP;
}

DWORD CGUIAnimation::OnScrubGUIAnimation(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(float));
	float time = *(float*)param;
	m_fCurrentTime = time;
	m_fStartTime = 0.f;
	m_bIsRunning = true;
	m_bScrub = true;
	return MSG_HANDLED_STOP;
}

DWORD CGUIAnimation::OnAddKeysToAnimation(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ADDMULTICHANNELKEYTOGUIANIM));
	ADDMULTICHANNELKEYTOGUIANIM aktga = *(ADDMULTICHANNELKEYTOGUIANIM*)param;
	for (UINT i=0; i<m_AnimObjects.size(); i++)
	{
		if (_tcscmp(m_AnimObjects[i].m_szObjName.GetString(), aktga.objName) == 0)
		{
			float time = ((float)aktga.key / (float)m_iFramesPerSec);
			if (time > m_fTotalTime)
			{
				m_fTotalTime = time;
			}

			int key = m_AnimObjects[i].m_chPosX.m_Channel.GetKeyframeIndex(time);
			float keyTime, keyVal;
			m_AnimObjects[i].m_chPosX.m_Channel.GetKeyframeValues(key, &keyTime, &keyVal);
			if (keyTime < time)
			{
				if ((m_AnimObjects[i].m_chPosX.m_Channel.GetKeyframeValues(key+1, &keyTime, &keyVal)) && (keyTime == time))
				{
					// key already there! delete old key first
					m_AnimObjects[i].m_chPosX.m_Channel.RemoveKeyframe(key+1);
					m_AnimObjects[i].m_chPosX.m_iNumKeys--;
					m_AnimObjects[i].m_chPosY.m_Channel.RemoveKeyframe(key+1);
					m_AnimObjects[i].m_chPosY.m_iNumKeys--;
				}
			}
			m_AnimObjects[i].m_chPosX.m_Channel.AddKeyframe(time, aktga.xPos);
			m_AnimObjects[i].m_chPosY.m_Channel.AddKeyframe(time, aktga.yPos);
			m_AnimObjects[i].m_chPosX.m_iNumKeys++;
			m_AnimObjects[i].m_chPosY.m_iNumKeys++;

			key = m_AnimObjects[i].m_chSizeX.m_Channel.GetKeyframeIndex(time);			
			m_AnimObjects[i].m_chSizeX.m_Channel.GetKeyframeValues(key, &keyTime, &keyVal);
			if (keyTime < time)
			{
				if ((m_AnimObjects[i].m_chSizeX.m_Channel.GetKeyframeValues(key+1, &keyTime, &keyVal)) && (keyTime == time))
				{
					// key already there! delete old key first
					m_AnimObjects[i].m_chSizeX.m_Channel.RemoveKeyframe(key+1);
					m_AnimObjects[i].m_chSizeX.m_iNumKeys--;
					m_AnimObjects[i].m_chSizeY.m_Channel.RemoveKeyframe(key+1);
					m_AnimObjects[i].m_chSizeY.m_iNumKeys--;
				}
			}
			m_AnimObjects[i].m_chSizeX.m_Channel.AddKeyframe(time, aktga.width);
			m_AnimObjects[i].m_chSizeY.m_Channel.AddKeyframe(time, aktga.height);
			m_AnimObjects[i].m_chSizeX.m_iNumKeys++;
			m_AnimObjects[i].m_chSizeY.m_iNumKeys++;

			key = m_AnimObjects[i].m_chAlphaCol.m_Channel.GetKeyframeIndex(time);			
			m_AnimObjects[i].m_chAlphaCol.m_Channel.GetKeyframeValues(key, &keyTime, &keyVal);
			if (keyTime < time)
			{
				if ((m_AnimObjects[i].m_chAlphaCol.m_Channel.GetKeyframeValues(key+1, &keyTime, &keyVal)) && (keyTime == time))
				{
					// key already there! delete old key first
					m_AnimObjects[i].m_chAlphaCol.m_Channel.RemoveKeyframe(key+1);
					m_AnimObjects[i].m_chAlphaCol.m_iNumKeys--;
					m_AnimObjects[i].m_chRCol.m_Channel.RemoveKeyframe(key+1);
					m_AnimObjects[i].m_chRCol.m_iNumKeys--;
					m_AnimObjects[i].m_chGCol.m_Channel.RemoveKeyframe(key+1);
					m_AnimObjects[i].m_chGCol.m_iNumKeys--;
					m_AnimObjects[i].m_chBCol.m_Channel.RemoveKeyframe(key+1);
					m_AnimObjects[i].m_chBCol.m_iNumKeys--;
				}
			}
			m_AnimObjects[i].m_chAlphaCol.m_Channel.AddKeyframe(time, aktga.a);
			m_AnimObjects[i].m_chRCol.m_Channel.AddKeyframe(time, aktga.r);
			m_AnimObjects[i].m_chGCol.m_Channel.AddKeyframe(time, aktga.g);
			m_AnimObjects[i].m_chBCol.m_Channel.AddKeyframe(time, aktga.b);
			m_AnimObjects[i].m_chAlphaCol.m_iNumKeys++;
			m_AnimObjects[i].m_chRCol.m_iNumKeys++;
			m_AnimObjects[i].m_chGCol.m_iNumKeys++;
			m_AnimObjects[i].m_chBCol.m_iNumKeys++;

			key = m_AnimObjects[i].m_chRot.m_Channel.GetKeyframeIndex(time);			
			m_AnimObjects[i].m_chRot.m_Channel.GetKeyframeValues(key, &keyTime, &keyVal);
			if (keyTime < time)
			{
				if ((m_AnimObjects[i].m_chRot.m_Channel.GetKeyframeValues(key+1, &keyTime, &keyVal)) && (keyTime == time))
				{
					// key already there! delete old key first
					m_AnimObjects[i].m_chRot.m_Channel.RemoveKeyframe(key+1);
					m_AnimObjects[i].m_chRot.m_iNumKeys--;					
				}
			}
			m_AnimObjects[i].m_chRot.m_Channel.AddKeyframe(time, aktga.angle);
			m_AnimObjects[i].m_chRot.m_iNumKeys++;

			return MSG_HANDLED_STOP;
		}
	}

	// not in list

	// don't add if the key isn't at 0
	if (aktga.key != 0)
	{
		m_ToolBox->Log(LOGWARNING, _T("New objects in GUI animation must have keys set at key 0 first!\n"));
#ifdef WIN32
		MessageBox(NULL, _T("Must set key for new object in GUI animation at key 0 first!"), _T("Danger! Danger!"), MB_OK);
#endif
		return MSG_HANDLED_STOP;
	}

	if (((float)aktga.key / (float)m_iFramesPerSec) > m_fTotalTime)
	{
		m_fTotalTime = ((float)aktga.key / (float)m_iFramesPerSec);
	}

	GUIANIMOBJ gao;
	gao.m_szObjName = CHashString(aktga.objName);
	gao.m_szObjType = CHashString(aktga.objType);

	gao.m_chPosX.m_Channel.AddKeyframe((float)aktga.key / (float)m_iFramesPerSec, aktga.xPos);
	gao.m_chPosY.m_Channel.AddKeyframe((float)aktga.key / (float)m_iFramesPerSec, aktga.yPos);
	gao.m_chPosX.m_iNumKeys++;
	gao.m_chPosY.m_iNumKeys++;

	gao.m_chSizeX.m_Channel.AddKeyframe((float)aktga.key / (float)m_iFramesPerSec, aktga.width);
	gao.m_chSizeY.m_Channel.AddKeyframe((float)aktga.key / (float)m_iFramesPerSec, aktga.height);
	gao.m_chSizeX.m_iNumKeys++;
	gao.m_chSizeY.m_iNumKeys++;

	gao.m_chAlphaCol.m_Channel.AddKeyframe((float)aktga.key / (float)m_iFramesPerSec, aktga.a);
	gao.m_chRCol.m_Channel.AddKeyframe((float)aktga.key / (float)m_iFramesPerSec, aktga.r);
	gao.m_chGCol.m_Channel.AddKeyframe((float)aktga.key / (float)m_iFramesPerSec, aktga.g);
	gao.m_chBCol.m_Channel.AddKeyframe((float)aktga.key / (float)m_iFramesPerSec, aktga.b);
	gao.m_chAlphaCol.m_iNumKeys++;
	gao.m_chRCol.m_iNumKeys++;
	gao.m_chGCol.m_iNumKeys++;
	gao.m_chBCol.m_iNumKeys++;

	gao.m_chRot.m_Channel.AddKeyframe((float)(float)aktga.key / (float)m_iFramesPerSec, aktga.angle);
	gao.m_chRot.m_iNumKeys++;

	m_AnimObjects.push_back(gao);
	m_iObjectNum ++;
	
	return MSG_HANDLED_STOP;
}

DWORD CGUIAnimation::OnDeleteOneKey(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GUIDELETEONEKEY));
	GUIDELETEONEKEY gdok = *(GUIDELETEONEKEY*)param;

	for (UINT i=0; i<m_AnimObjects.size(); i++)
	{
		if (_tcscmp(m_AnimObjects[i].m_szObjName.GetString(), gdok.objName) == 0)
		{
			switch(gdok.channel)
			{
				case 0: // pos
				{
					m_AnimObjects[i].m_chPosX.m_Channel.RemoveKeyframe(gdok.keyIndex);
					m_AnimObjects[i].m_chPosX.m_iNumKeys--;

					m_AnimObjects[i].m_chPosY.m_Channel.RemoveKeyframe(gdok.keyIndex);
					m_AnimObjects[i].m_chPosY.m_iNumKeys--;
					break;
				}
				case 1: // size
				{
					m_AnimObjects[i].m_chSizeX.m_Channel.RemoveKeyframe(gdok.keyIndex);
					m_AnimObjects[i].m_chSizeX.m_iNumKeys--;

					m_AnimObjects[i].m_chSizeY.m_Channel.RemoveKeyframe(gdok.keyIndex);
					m_AnimObjects[i].m_chSizeY.m_iNumKeys--;
					break;
				}
				case 2: // color
				{
					m_AnimObjects[i].m_chAlphaCol.m_Channel.RemoveKeyframe(gdok.keyIndex);
					m_AnimObjects[i].m_chAlphaCol.m_iNumKeys--;

					m_AnimObjects[i].m_chRCol.m_Channel.RemoveKeyframe(gdok.keyIndex);
					m_AnimObjects[i].m_chRCol.m_iNumKeys--;

					m_AnimObjects[i].m_chGCol.m_Channel.RemoveKeyframe(gdok.keyIndex);
					m_AnimObjects[i].m_chGCol.m_iNumKeys--;

					m_AnimObjects[i].m_chBCol.m_Channel.RemoveKeyframe(gdok.keyIndex);
					m_AnimObjects[i].m_chBCol.m_iNumKeys--;
					break;
				}
				case 3: // rotation
				{
					m_AnimObjects[i].m_chRot.m_Channel.RemoveKeyframe(gdok.keyIndex);
					m_AnimObjects[i].m_chRot.m_iNumKeys--;
					break;
				}
				default:
				{
					break;
				}
			}
			// if no more keys, remove object from list
			if ((m_AnimObjects[i].m_chPosX.m_iNumKeys == 0) &&
				(m_AnimObjects[i].m_chSizeX.m_iNumKeys == 0) &&
				(m_AnimObjects[i].m_chAlphaCol.m_iNumKeys == 0) &&
				(m_AnimObjects[i].m_chRot.m_iNumKeys == 0))
			{
				m_AnimObjects.erase(m_AnimObjects.begin() + i);
				m_iObjectNum --;
			}
		}
	}
	RecalcTime();
	return MSG_HANDLED_STOP;
}

DWORD CGUIAnimation::OnAddOneKey(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ADDSINGLECHANNELKEYTOGUIANIM));
	ADDSINGLECHANNELKEYTOGUIANIM aktga = *(ADDSINGLECHANNELKEYTOGUIANIM*) param;
	
	for (UINT i=0; i<m_AnimObjects.size(); i++)
	{
		if (_tcscmp(m_AnimObjects[i].m_szObjName.GetString(), aktga.objName) == 0)
		{
			float time = (float)aktga.key / (float)m_iFramesPerSec;
			if (time > m_fTotalTime)
			{
				m_fTotalTime = time;
			}
			switch(aktga.channel)
			{
				case 0: // pos
				{					
					m_AnimObjects[i].m_chPosX.m_Channel.AddKeyframe(time, aktga.val1);
					m_AnimObjects[i].m_chPosX.m_iNumKeys++;

					m_AnimObjects[i].m_chPosY.m_Channel.AddKeyframe(time, aktga.val2);
					m_AnimObjects[i].m_chPosY.m_iNumKeys++;
					break;
				}
				case 1: // size
				{
					m_AnimObjects[i].m_chSizeX.m_Channel.AddKeyframe(time, aktga.val1);
					m_AnimObjects[i].m_chSizeX.m_iNumKeys++;

					m_AnimObjects[i].m_chSizeY.m_Channel.AddKeyframe(time, aktga.val2);
					m_AnimObjects[i].m_chSizeY.m_iNumKeys++;
					break;
				}
				case 2: // color
				{
					m_AnimObjects[i].m_chAlphaCol.m_Channel.AddKeyframe(time, aktga.val1);
					m_AnimObjects[i].m_chAlphaCol.m_iNumKeys++;

					m_AnimObjects[i].m_chRCol.m_Channel.AddKeyframe(time, aktga.val2);
					m_AnimObjects[i].m_chRCol.m_iNumKeys++;

					m_AnimObjects[i].m_chGCol.m_Channel.AddKeyframe(time, aktga.val3);
					m_AnimObjects[i].m_chGCol.m_iNumKeys++;

					m_AnimObjects[i].m_chBCol.m_Channel.AddKeyframe(time, aktga.val4);
					m_AnimObjects[i].m_chBCol.m_iNumKeys++;
					break;
				}
				case 3: // rotation
				{
					m_AnimObjects[i].m_chRot.m_Channel.AddKeyframe(time, aktga.val1);
					m_AnimObjects[i].m_chRot.m_iNumKeys++;
					break;
				}
				default:
				{
					break;
				}
			}			
		}
	}
	return MSG_HANDLED_STOP;
}

DWORD CGUIAnimation::OnAddGUIAnimationCallback(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GUIEVENT));
	GUIEVENT event = *(GUIEVENT*)param;

	m_AnimCallbacks.push_back(event);

	return MSG_HANDLED_STOP;
}

DWORD CGUIAnimation::OnChangeGUIAnimKey(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ADDSINGLECHANNELKEYTOGUIANIM));
	ADDSINGLECHANNELKEYTOGUIANIM ascktga = *(ADDSINGLECHANNELKEYTOGUIANIM*)param;

	for (UINT i=0; i<m_AnimObjects.size(); i++)
	{
		if (_tcscmp(m_AnimObjects[i].m_szObjName.GetString(), ascktga.objName) == 0)
		{
			float time = (float)ascktga.key / (float)m_iFramesPerSec;
			switch(ascktga.channel)
			{
				case 0: // pos
				{
					int key = m_AnimObjects[i].m_chPosX.m_Channel.GetKeyframeIndex(time);
					float keyTime, keyVal;
					m_AnimObjects[i].m_chPosX.m_Channel.GetKeyframeValues(key, &keyTime, &keyVal);
					if (keyTime < time)
					{
						if ((m_AnimObjects[i].m_chPosX.m_Channel.GetKeyframeValues(key+1, &keyTime, &keyVal)) && (keyTime >= time))
						{
							key++;
						}
					}
					if (key >= 0)
					{
						m_AnimObjects[i].m_chPosX.m_Channel.ChangeKeyframeValues(key, time, ascktga.val1);
						m_AnimObjects[i].m_chPosY.m_Channel.ChangeKeyframeValues(key, time, ascktga.val2);
					}
					break;
				}
				case 1: // size
				{
					int key = m_AnimObjects[i].m_chSizeX.m_Channel.GetKeyframeIndex(time);
					float keyTime, keyVal;
					m_AnimObjects[i].m_chSizeX.m_Channel.GetKeyframeValues(key, &keyTime, &keyVal);
					if (keyTime < time)
					{
						if ((m_AnimObjects[i].m_chSizeX.m_Channel.GetKeyframeValues(key+1, &keyTime, &keyVal)) && (keyTime >= time))
						{
							key++;
						}
					}
					if (key >= 0)
					{
						m_AnimObjects[i].m_chSizeX.m_Channel.ChangeKeyframeValues(key, time, ascktga.val1);
						m_AnimObjects[i].m_chSizeY.m_Channel.ChangeKeyframeValues(key, time, ascktga.val2);
					}
					break;
				}
				case 2: // color
				{
					int key = m_AnimObjects[i].m_chAlphaCol.m_Channel.GetKeyframeIndex(time);
					float keyTime, keyVal;
					m_AnimObjects[i].m_chAlphaCol.m_Channel.GetKeyframeValues(key, &keyTime, &keyVal);
					if (keyTime < time)
					{
						if ((m_AnimObjects[i].m_chAlphaCol.m_Channel.GetKeyframeValues(key+1, &keyTime, &keyVal)) && (keyTime >= time))
						{
							key++;
						}
					}
					if (key >= 0)
					{
						m_AnimObjects[i].m_chAlphaCol.m_Channel.ChangeKeyframeValues(key, time, ascktga.val1);
						m_AnimObjects[i].m_chRCol.m_Channel.ChangeKeyframeValues(key, time, ascktga.val2);
						m_AnimObjects[i].m_chGCol.m_Channel.ChangeKeyframeValues(key, time, ascktga.val3);
						m_AnimObjects[i].m_chBCol.m_Channel.ChangeKeyframeValues(key, time, ascktga.val4);
					}
					break;
				}
				case 3: // rot
				{
					int key = m_AnimObjects[i].m_chRot.m_Channel.GetKeyframeIndex(time);
					float keyTime, keyVal;
					m_AnimObjects[i].m_chRot.m_Channel.GetKeyframeValues(key, &keyTime, &keyVal);
					if (keyTime < time)
					{
						if ((m_AnimObjects[i].m_chRot.m_Channel.GetKeyframeValues(key+1, &keyTime, &keyVal)) && (keyTime >= time))
						{
							key++;
						}
					}
					if (key >= 0)
					{
						m_AnimObjects[i].m_chRot.m_Channel.ChangeKeyframeValues(key, time, ascktga.val1);
					}
					break;
				}
				default:
				{
					break;
				}
			}			
			break;
		}		
	}

	return MSG_HANDLED_STOP;
}

void CGUIAnimation::RecalcTime()
{
	float maxTime = 0.0f;
	for (UINT i=0; i<m_AnimObjects.size(); i++)
	{
		float time, val;
		int lastKey = m_AnimObjects[i].m_chPosX.m_Channel.GetLastKeyIndex();
		m_AnimObjects[i].m_chPosX.m_Channel.GetKeyframeValues(lastKey, &time, &val);
		if (time > maxTime)
		{
			maxTime = time;
		}

		lastKey = m_AnimObjects[i].m_chSizeX.m_Channel.GetLastKeyIndex();
		m_AnimObjects[i].m_chSizeX.m_Channel.GetKeyframeValues(lastKey, &time, &val);
		if (time > maxTime)
		{
			maxTime = time;
		}

		lastKey = m_AnimObjects[i].m_chAlphaCol.m_Channel.GetLastKeyIndex();
		m_AnimObjects[i].m_chAlphaCol.m_Channel.GetKeyframeValues(lastKey, &time, &val);
		if (time > maxTime)
		{
			maxTime = time;
		}

		lastKey = m_AnimObjects[i].m_chRot.m_Channel.GetLastKeyIndex();
		m_AnimObjects[i].m_chRot.m_Channel.GetKeyframeValues(lastKey, &time, &val);
		if (time > maxTime)
		{
			maxTime = time;
		}
	}

	m_fTotalTime = maxTime;
}

void CGUIAnimation::ConvertPositions()
{
	for (UINT i=0; i<m_AnimObjects.size(); i++)
	{
		CHashString hsParentName(_T(""));
		static DWORD msgGetGUIParentName = CHashString(_T("GetGUIParentName")).GetUniqueID();
		m_ToolBox->SendMessage(msgGetGUIParentName, sizeof(IHashString*), &hsParentName, &m_AnimObjects[i].m_szObjName, &m_AnimObjects[i].m_szObjType);

		if (_tcscmp(hsParentName.GetString(), _T("")) != 0)
		{
			POSITIONDATA pd;
			static DWORD msgGetPosition = CHashString(_T("GetPosition")).GetUniqueID();
			m_ToolBox->SendMessage(msgGetPosition, sizeof(POSITIONDATA), &pd, &hsParentName);
			// switching to relative positions
			if (!m_bRelativePositions)
			{
				for (UINT j=0; j<m_AnimObjects[i].m_chPosX.m_iNumKeys; j++)
				{
					float time=0.0f, value=0.0f;
					m_AnimObjects[i].m_chPosX.m_Channel.GetKeyframeValues(j, &time, &value);
					value -= pd.m_fXPos;
					m_AnimObjects[i].m_chPosX.m_Channel.ChangeKeyframeValues(j, time, value);

					m_AnimObjects[i].m_chPosY.m_Channel.GetKeyframeValues(j, &time, &value);
					value -= pd.m_fYPos;
					m_AnimObjects[i].m_chPosY.m_Channel.ChangeKeyframeValues(j, time, value);
				}
			}
			// switching from relative to global
			else
			{
				for (UINT j=0; j<m_AnimObjects[i].m_chPosX.m_iNumKeys; j++)
				{
					float time=0.0f, value=0.0f;
					m_AnimObjects[i].m_chPosX.m_Channel.GetKeyframeValues(j, &time, &value);
					value += pd.m_fXPos;
					m_AnimObjects[i].m_chPosX.m_Channel.ChangeKeyframeValues(j, time, value);

					m_AnimObjects[i].m_chPosY.m_Channel.GetKeyframeValues(j, &time, &value);
					value += pd.m_fYPos;
					m_AnimObjects[i].m_chPosY.m_Channel.ChangeKeyframeValues(j, time, value);
				}
			}
		}
	}
}

DWORD CGUIAnimation::OnIsGUIAnimationRelativePositions(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(bool));
	bool *retval = (bool*) param;
	*retval = m_bRelativePositions;

	return MSG_HANDLED_STOP;
}