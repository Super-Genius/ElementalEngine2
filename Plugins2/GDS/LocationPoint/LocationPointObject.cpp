///============================================================================
/// \file		LocationPointObject.cpp
/// \brief		Implentation of CLocationPointObject Class
/// \date		12/13/2006
/// \author		Nick Tourte
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
/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CLocationPointObject);

REGISTER_MESSAGE_HANDLER(GetGlobalPosition, OnGetGlobalPosition, CLocationPointObject);
REGISTER_MESSAGE_HANDLER(GetGlobalEuler, OnGetGlobalEuler, CLocationPointObject);
REGISTER_MESSAGE_HANDLER(GetGlobalScale, OnGetGlobalScale, CLocationPointObject);
REGISTER_MESSAGE_HANDLER(GetGlobalTransform, OnGetGlobalTransform, CLocationPointObject);
//REGISTER_MESSAGE_HANDLER(GetBoundingBox, OnGetBoundingBox, CLocationPointObject);

REGISTER_MESSAGE_HANDLER(ParentUpdateBoundingBox, OnParentUpdateBoundingBox, CLocationPointObject);
REGISTER_MESSAGE_HANDLER(ParentRemoveBoundingBox, OnParentRemoveBoundingBox, CLocationPointObject);

REGISTER_MESSAGE_HANDLER(SetDirtyTransformFlag, OnSetDirtyTransformFlag, CLocationPointObject);


// Default Constructor
CLocationPointObject::CLocationPointObject( IHashString *parentName, IHashString *name, bool bAddToHier ) :
	OBJECTTEMPLATE( CLocationPointManager, CLocationPointObject, IObject, parentName, name )
{
	m_bAddToHierarchy = bAddToHier;
	if (bAddToHier)
	{
//		AddToHierarchy();
	}
    m_RendObj = NULL;
	m_szROFileName = _T("");

	m_Scale.Set(1.0f, 1.0f, 1.0f);
}

// Default Destructor
CLocationPointObject::~CLocationPointObject()
{
	DeInit();
	if (m_bAddToHierarchy)
	{
//		RemoveFromHierarchy();
	}
}

IComponent *CLocationPointObject::Create( int nArgs, va_list argptr )
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
			self = new CLocationPointObject(parentName, name, true);
		}
		else if (nArgs == 3)
		{
			bAddToHier = va_arg(argptr, bool);
			self = new CLocationPointObject(parentName, name, bAddToHier);
		}
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}

void CLocationPointObject::Init()
{
	static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &m_Position, GetParentName());
	static DWORD msgHash_GetGlobalEuler = CHashString(_T("GetGlobalEuler")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalEuler, sizeof(Vec3), &m_Orientation, GetParentName());
	static DWORD msgHash_GetGlobalScale = CHashString(_T("GetGlobalScale")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalScale, sizeof(Vec3), &m_Scale, GetParentName());

//	CreateRenderObject();
	CreateEditorEntityMunge();
}

void CLocationPointObject::DeInit()
{
//	DestroyRenderObject();
}

void CLocationPointObject::Serialize(IArchive &ar)
{
	if (ar.IsReading())
	{
		ar.Read(m_szROFileName);
	}
	else
	{
		ar.Write(m_szROFileName, _T("RenderObjectFilename"));
	}
}

void CLocationPointObject::CreateRenderObject()
{	
	IHashString * pname = GetName();
	if( !pname )
	{
		return;
	}

	CHashString hszParentName(GetName());
	CHashString hszTypeName(_T("Cal3DRenderObject"));	
	CHashString hszName;
	StdString newname;
	newname = pname->GetString();
	newname += _T("_RO");
	hszName = newname;

	IComponent *rendComp = m_ToolBox->CreateComponent(&hszTypeName, 3, &hszName, &hszParentName, false);
	assert (rendComp);

	OFACTORYADDPARAMS ofap;
	ofap.name = &hszName;
	ofap.component = rendComp;
	static DWORD msgHash_AddObjectToFactory = CHashString(_T("AddObjectToFactory")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddObjectToFactory, sizeof(OFACTORYADDPARAMS), &ofap);

	m_RendObj = dynamic_cast<IObject*>(rendComp);

	IArchive *MemArchive;
	CHashString memType(_T("Memory"));
					
	CREATEARCHIVE caOut;
	caOut.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	caOut.streamData = NULL;
	caOut.streamSize = 0;
	caOut.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caOut) != MSG_HANDLED)
	{
		return;
	}
	
	MemArchive = caOut.archive;
	MemArchive->Write( m_szROFileName, _T("FileName") );
	
	Vec3 pos, rot;
	static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
	//m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &pos, GetParentName());
	static DWORD msgHash_GetGlobalEuler = CHashString(_T("GetGlobalEuler")).GetUniqueID();
	//m_ToolBox->SendMessage(msgHash_GetGlobalEuler, sizeof(Vec3), &rot, GetParentName());
	MemArchive->Write( m_Position, _T("Position"));
	MemArchive->Write( m_Orientation, _T("Rotation") );
	MemArchive->Write( Vec3(1.f, 1.f, 1.f), _T("Scaling") );		
	MemArchive->Write( _T("Shaders/Cal3DEffect_Noskin.efx") , _T("Shader"));

	MemArchive->SetIsWriting(false);
	MemArchive->SeekTo(0);
	
	m_RendObj->Serialize(*MemArchive);
	m_RendObj->Init();	

	// send out a message that an object needs to be added
	CREATEOBJECTPARAMS cop;
	cop.name = &hszName;
	cop.typeName = &hszTypeName;
	cop.parentName = &hszParentName;
	static DWORD msgHash_AddObject = CHashString(_T("AddObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_AddObject, sizeof(CREATEOBJECTPARAMS), &cop);

	MemArchive->Close();
}

void CLocationPointObject::DestroyRenderObject()
{
	StdString roName = GetName()->GetString();
	roName += _T("_RO");
	CHashString hszRoName(roName);
	
	DELETEOBJECTPARAMS dop;
	dop.name = &hszRoName;
	static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop);
}

DWORD CLocationPointObject::OnSetDirtyTransformFlag(DWORD size, void *param)
{
	static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &m_Position, GetParentName());
	static DWORD msgHash_GetGlobalEuler = CHashString(_T("GetGlobalEuler")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalEuler, sizeof(Vec3), &m_Orientation, GetParentName());
	static DWORD msgHash_GetGlobalScale = CHashString(_T("GetGlobalScale")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetGlobalScale, sizeof(Vec3), &m_Scale, GetParentName());

	static DWORD msgHash_SetDirtyTransformFlag = CHashString(_T("SetDirtyTransformFlag")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SetDirtyTransformFlag, 0, NULL, m_RendObj->GetName());

	return MSG_HANDLED_PROCEED;
}

DWORD CLocationPointObject::OnParentUpdateBoundingBox(DWORD size, void *param)
{
	static DWORD msgHash_ParentUpdateBoundingBox = CHashString(_T("ParentUpdateBoundingBox")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_ParentUpdateBoundingBox, size, param, GetParentName());
	return MSG_HANDLED_STOP;
}
	
DWORD CLocationPointObject::OnParentRemoveBoundingBox(DWORD size, void *param)
{
	static DWORD msgHash_ParentRemoveBoundingBox = CHashString(_T("ParentRemoveBoundingBox")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_ParentRemoveBoundingBox, size, param, GetParentName());
	return MSG_HANDLED_STOP;
}

DWORD CLocationPointObject::OnGetGlobalPosition(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(Vec3));
	Vec3 *pos = (Vec3*)param;

	pos->Set(m_Position);

	return MSG_HANDLED_STOP;
}
	
DWORD CLocationPointObject::OnGetGlobalEuler(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(Vec3));
	Vec3 *euler = (Vec3*)param;

	euler->Set(m_Orientation);

	return MSG_HANDLED_STOP;
}
	
DWORD CLocationPointObject::OnGetGlobalScale(DWORD size, void *param)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(Vec3));
	Vec3 *scale = (Vec3*)param;

	scale->Set(m_Scale);

	return MSG_HANDLED_STOP;
}

DWORD CLocationPointObject::OnGetBoundingBox(DWORD size, void *param)
{
	static DWORD msgHash_GetBoundingBox = CHashString(_T("GetBoundingBox")).GetUniqueID();
	return m_ToolBox->SendMessage(msgHash_GetBoundingBox, size, param, m_RendObj->GetName());
}

DWORD CLocationPointObject::OnGetGlobalTransform(DWORD size, void *param)
{
	static DWORD msgHash_GetGlobalTransform = CHashString(_T("GetGlobalTransform")).GetUniqueID();
	return m_ToolBox->SendMessage(msgHash_GetGlobalTransform, size, param, GetParentName());
}

void CLocationPointObject::CreateEditorEntityMunge()
{
	CREATEOBJECTPARAMS cop;
	SERIALIZEOBJECTPARAMS sop;
	INITOBJECTPARAMS iop;

	IArchive *MemArchive;
	CHashString memType(_T("Memory"));
					
	CREATEARCHIVE caOut;
	caOut.mode = STREAM_MODE_WRITE | STREAM_MODE_READ;
	caOut.streamData = NULL;
	caOut.streamSize = 0;
	caOut.streamType = &memType;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &caOut) != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGWARNING, _T("Location point object: could not create mem archive for editor entity!"));
		return;
	}

	MemArchive = caOut.archive;
	if (MemArchive == NULL)
	{
		m_ToolBox->Log(LOGWARNING, _T("Location point object: could not create mem archive for editor entity!"));
		return;
	}

	CHashString editorEntType(_T("CEditorEntity"));
	CHashString renderObjType(_T("Cal3DRenderObject"));
	IHashString *myName = GetName();
	IHashString *myParentName = GetParentName();

	myName->GetString();
	CHashString hsEditorEntName;

	GENERATEUNIQUEOBJECTNAMEPARAMS guonp;
	guonp.name = myName;
	guonp.newname = &hsEditorEntName;

	static DWORD msgHash_GenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(GENERATEUNIQUEOBJECTNAMEPARAMS), &guonp);

	StdString roName = hsEditorEntName.GetString();
	roName+= _T("_RO");
	CHashString hsRendObjName(roName);

	cop.name = &hsEditorEntName;
	cop.parentName = myParentName;
	cop.typeName = &editorEntType;

	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

	if (retval != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGWARNING, _T("Location point object: could not create editor entity to munge!"));
		return;
	}

	// entity serialization
	MemArchive->SetIsWriting(true);
	MemArchive->Write(_T("dummyObject"), _T("EntityType"));
	MemArchive->Write(Vec3(0.0f, 0.0f, 0.0f), _T("Position"));
	MemArchive->Write(Vec3(0.0f, 0.0f, 0.0f), _T("Orientation"));
	MemArchive->Write(Vec3(1.0f, 1.0f, 1.0f), _T("Scale"));
	MemArchive->SetIsWriting(false);

	sop.name = &hsEditorEntName;
	sop.archive = MemArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

	iop.name = &hsEditorEntName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);

	// now for the underlying render object...
	cop.name = &hsRendObjName;
	cop.parentName = &hsEditorEntName;
	cop.typeName = &renderObjType;
	
	retval = m_ToolBox->SendMessage(msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &cop);

	if (retval != MSG_HANDLED)
	{
		m_ToolBox->Log(LOGWARNING, _T("Location point object: could not create render object to munge!"));
		return;
	}

	// ro serialization
	MemArchive->SeekTo(0, SEEK_SET);
	MemArchive->SetIsWriting(true);
	MemArchive->Write(m_szROFileName, _T("FileName"));
	MemArchive->SetIsWriting(false);

	sop.name = &hsRendObjName;
	sop.archive = MemArchive;
	m_ToolBox->SendMessage(msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &sop);

	iop.name = &hsRendObjName;
	m_ToolBox->SendMessage(msgHash_InitObject, sizeof(INITOBJECTPARAMS), &iop);
}
*/