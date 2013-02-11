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

REGISTER_COMPONENT(ObjectCubeMap);

REGISTER_MESSAGE_HANDLER(SetCubemapData, OnSetCubemapData, ObjectCubeMap);
REGISTER_MESSAGE_HANDLER( GetModelFileName, OnGetModelFileName, ObjectCubeMap );

void	ObjectCubeMap::Serialize( IArchive &ar )
{
	if( ar.IsReading() )
	{
		StdString TypeName;
		StdString CurrentCubeFaceName;
		//Normalize the filename
		ar.Read( TypeName, _T("TypeName") );
		TypeName.MakeSafeFileName();

		// convert to hashstring
		m_Type.Init( TypeName );
		// read all six cube textures (I assume it is safe to require 6 textures for a cubemap)
		CHashString image;
		for (int i = 0; i < CUBEMAP_TOTAL_DIR; ++i)
		{
			TCHAR buffer[1024];
			_stprintf( buffer, "Face_%d", i );
			ar.Read( TypeName, buffer );
			if( _tcslen( (const TCHAR*)TypeName ) > 0 )
			{
				// Get the CubeTexture node
				image.Init( (const TCHAR *)TypeName );
				// add to array
				m_Images[i] = image.GetUniqueID();
			}else
			{
				m_ToolBox->Log( LOGERROR, _T("Error loading Object cubemap face, empty string") );
				m_Images[i] = 0;
			}
		}
	}
	else
	{
	}
}


ObjectCubeMap::ObjectCubeMap(IHashString *parentName, IHashString *name, bool bAddToHier):
	OBJECTTEMPLATE(CObjectCubeMapManager /*NULL??*/, ObjectCubeMap, IObject, parentName, name)
{
	/*
	//Add cubemap message actually inserts it into a list with the model filename this cubemap
	//references. This is not yet known... not sure why it was doing this
	ADDCUBEMAPMESSAGE msg;
	msg.CubeMapObject = this;
	static DWORD msgHash_AddCubeMap = CHashString(_T("AddCubeMap")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddCubeMap, sizeof( msg ), &msg );*/
}

ObjectCubeMap::~ObjectCubeMap()
{
}

IComponent *ObjectCubeMap::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;
	bool bAddToHier;
	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		if (nArgs == 2)
		{
			self = new ObjectCubeMap(parentName, name, true); // not sure if this is supposed to default to true or false
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new ObjectCubeMap(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

DWORD  ObjectCubeMap::OnSetCubemapData( DWORD size, void * params )
{
	CHashString image;
	VERIFY_MESSAGE_SIZE( size, sizeof( SETOBJECTCUBEMAPMSG ) );
	SETOBJECTCUBEMAPMSG * msg = (SETOBJECTCUBEMAPMSG*)params;
	if( msg )
	{
		// convert to hashstring
		m_Type.Init( (const TCHAR*) msg->CubemapTypename );
		// read all six cube textures (I assume it is safe to require 6 textures for a cubemap)
		for (int i = 0; i < CUBEMAP_TOTAL_DIR; ++i)
		{
			if( _tcslen( (const TCHAR*) msg->CubeNames[ i ] ) > 0 )
			{
				// Get the CubeTexture node
				image.Init((const TCHAR *)msg->CubeNames[ i ]);
				// add to array
				m_Images[i] = image.GetUniqueID();
			}else
			{
				m_ToolBox->Log( LOGERROR, _T("Error loading Object cubemap face, empty string") );
				m_Images[i] = 0;
			}
		}
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;	
}

void ObjectCubeMap::Init()
{
	static DWORD msgHash_LoadObjectCubeMap = CHashString(_T("LoadObjectCubeMap")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_LoadObjectCubeMap, sizeof(IHashString),
		GetName());
}

bool ObjectCubeMap::Update()
{
	return true;
}
void ObjectCubeMap ::DeInit()
{
}


DWORD ObjectCubeMap::OnGetModelFileName( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE( size, sizeof( CHashString ) );
	CHashString * parms = (CHashString *)params;
	if( parms &&
		_tcslen( m_Type.GetString() ) > 0 )
	{
		parms->Init( m_Type.GetString() );
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}