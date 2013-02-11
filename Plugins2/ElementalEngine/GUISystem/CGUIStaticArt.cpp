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
#include "CGUIStaticArt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CGUIStaticArt);

REGISTER_MESSAGE_HANDLER(SetStaticImage, OnSetStaticImage, CGUIStaticArt);

CGUIStaticArt::CGUIStaticArt(IHashString *parentName, IHashString *name) :
	OBJECTTEMPLATE(CGUIManager, CGUIStaticArt, IGUIReference, parentName, name)	
{
	m_pArtItem = new GUIButton();

	//set all members to default vals
	m_szArtTex = _T("");
	
	m_bSmartScale = false;

	m_fCursorPosX = m_fCursorPosY = 0.f;

	m_fSaveVersion = 1.0f;
}

CGUIStaticArt::~CGUIStaticArt()
{
	delete m_pArtItem;
}

 IComponent *CGUIStaticArt::Create(int nArgs, va_list argptr)
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
		self = new CGUIStaticArt(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}
	return self;
}

bool CGUIStaticArt::Update()
{
	return true;
}

IHashString *CGUIStaticArt::GetComponentType()
{
	static CHashString CGUIStaticArtCT(_T("CGUIStaticArt"));
	return &CGUIStaticArtCT;
}

bool CGUIStaticArt::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

void CGUIStaticArt::Serialize( IArchive &ar )
{
	if(ar.IsReading())
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

		ar.Read(m_szArtTex);
		ar.Read(m_bSmartScale);
	
		CreateItem();
	}
	else
	{
		ar.Write(m_fSaveVersion, _T("Version"));
		ar.Write(m_szArtTex, _T("NormTexture"));
		ar.Write(m_bSmartScale, _T("SmartScale"));
	}
}

DWORD CGUIStaticArt::OnSetStaticImage(DWORD msgSize, void *param)
{
	IHashString *imagePath;
	const TCHAR *image;
	VERIFY_MESSAGE_SIZE(msgSize, sizeof(IHashString));

	imagePath = (IHashString*)param;
	image = imagePath->GetString();
	
	// set a new image
	m_pArtItem->SetScreenMasks(image, image, image, image);

	return MSG_HANDLED_STOP;
}

void CGUIStaticArt::CreateItem()
{
	m_pArtItem->SetScreenMasks(m_szArtTex, m_szArtTex, m_szArtTex, m_szArtTex);
	m_pArtItem->SetSmartScale(m_bSmartScale);
}

void CGUIStaticArt::GetRenderParams(vector<GUIVERTEXINFO> &vertexData, float x, float y, float width, float height, 
									DWORD state, float angle, DWORD color)
{
	if (m_pArtItem)
	{
		m_pArtItem->LoadVertexBuffer(vertexData, x, y, width, height, state, angle, color);
	}
}