///============================================================================
/// \file		HeightmapObject.h
/// \brief		Implementation file for a Heightmap Object
/// \date		06-29-2005
/// \author		D. Patrick Ghiocel
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

#include "StdAfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


REGISTER_COMPONENT(CHeightmapObject);
REGISTER_MESSAGE_HANDLER(SetHeightmapData, OnSetHeightmapData, CHeightmapObject);
REGISTER_MESSAGE_HANDLER(GetHeightmapData, OnGetHeightmapData, CHeightmapObject);

IComponent *CHeightmapObject::Create(int nArgs, va_list argptr)
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
		self = new CHeightmapObject(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

CHeightmapObject::CHeightmapObject(IHashString *parentName, IHashString *name) :
		OBJECTTEMPLATE( CTerrainManager, CHeightmapObject, IObject, parentName, name)
{
	m_pManager = dynamic_cast<CTerrainManager*>(m_ToolBox->GetComponent(GetManagerName()));

	m_pHeightmapData = NULL;
	m_iWidth = 0;
	m_iHeight = 0;
}

CHeightmapObject::~CHeightmapObject()
{
	SAFE_DELETE_ARRAY( m_pHeightmapData );

	DeInit();
}

void CHeightmapObject::Init()
{

}

void CHeightmapObject::DeInit( void )
{
	CObjectTemplate<IObject>::DeInit();
}

void CHeightmapObject::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
		int iSize = 0;
		ar.Read( iSize );
		iSize = EE_ENDIANSWAP32( iSize );

		m_iWidth = iSize;
		m_iHeight = iSize;

		SAFE_DELETE_ARRAY( m_pHeightmapData );

		int iSamples = iSize * iSize;
		if( iSamples > 0 )
		{
			float f;
			m_pHeightmapData = new float[iSamples];
			for( int i = 0; i < iSamples; ++i )
			{
				ar.Read( f );
				m_pHeightmapData[i] = EE_ENDIANSWAP32F( f );
			}
		}
	}
	else
	{
		int iSize = m_iWidth;
		ar.Write( iSize );
		int iSamples = iSize * iSize;
		if( m_pHeightmapData != NULL )
		{
			for( int i = 0; i < iSamples; ++i )
			{
				ar.Write( m_pHeightmapData[i] );
			}
		}
		else
		{
			for( int i = 0; i < iSamples; ++i )
			{
				ar.Write( 0.0f );
			}
		}
	}
}

DWORD CHeightmapObject::OnSetHeightmapData(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(HEIGHTMAPDATAPARAMS));
	HEIGHTMAPDATAPARAMS *pParams = (HEIGHTMAPDATAPARAMS*)in_params;

	// If the Width Mismatch or Height MisMatch, re-allocate data
	if( (m_iWidth != pParams->iWidth) || (m_iHeight != pParams->iHeight) )
	{
		SAFE_DELETE_ARRAY( m_pHeightmapData );
		m_iWidth = pParams->iWidth;
		m_iHeight = pParams->iHeight;
		m_pHeightmapData = new float[m_iWidth * m_iHeight];
	}
	memcpy( m_pHeightmapData, pParams->pData, sizeof(float) * m_iWidth * m_iHeight );

	return MSG_HANDLED_STOP;
}

DWORD CHeightmapObject::OnGetHeightmapData(DWORD size, void *in_params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(HEIGHTMAPDATAPARAMS));
	HEIGHTMAPDATAPARAMS *pParams = (HEIGHTMAPDATAPARAMS*)in_params;

	if (m_pHeightmapData)
	{
		pParams->pData = m_pHeightmapData;
		pParams->iHeight = m_iHeight;
		pParams->iWidth = m_iWidth;
		return MSG_HANDLED_STOP;
	}
	else
	{
		return MSG_NOT_HANDLED;
	}
}
