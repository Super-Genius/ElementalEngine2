///============================================================================
/// \file		LoadSaverBase.hpp
/// \brief		Header file for the load saver plugin base class
/// \date		11-15-2006
/// \author		Richard Nguyen
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

#ifndef _LOADSAVERBASE_H_
#define _LOADSAVERBASE_H_

#include "CManager.h"

typedef std::map< int, DWORD > NODEIDTOHASHIDMAP;
typedef std::map< DWORD, int > NAMETRACKERMAP;

/// Used to keep track of when the XML archive has move up or down
/// a depth level and maintains a stack of parent node positions.
/// These parent node positions can act as identifiers for specific
/// nodes which is useful when indexing data associated with them.
struct XMLParentNodeTracker
{
	int currentDepth;
	int currentPosition;
	int lastDepth;
	int lastPosition;
	int parentPosition;
	std::list<int> parentStack;

	XMLParentNodeTracker()
	{
		currentDepth = 0;
		currentPosition = 0;
		lastDepth = 0;
		lastPosition = 0;
		parentPosition = 0;
		std::list<int> parentStack;
	}

	void OnNodeMoved( IXMLArchive *ar )
	{
		lastDepth = currentDepth;
		lastPosition = currentPosition;

		currentDepth = ar->GetDepth();
		currentPosition = ar->GetCurrentNodePosition();

		if( currentDepth > 0 )
		{
			if( currentDepth > lastDepth )
			{
				parentStack.push_back( lastPosition );
				parentPosition = lastPosition;
			}
			else
			{
				int depthJump = lastDepth - currentDepth;
				for( int i = 0; i < depthJump; i++ )
				{
					parentStack.pop_back();
				}
				parentPosition = parentStack.back();
			}
		}
		else
		{
			parentStack.clear();
			parentPosition = 0;
		}
	}
};

class LoadSaverBase : public IComponent
{
public:
	LoadSaverBase( const TCHAR *classNameChar, const TCHAR *managerName, const TCHAR *startNodeName )
		: m_HashName( classNameChar ), m_ManagerName( managerName ), m_StartNodeName( startNodeName )
	{
		m_ToolBox = EngineGetToolBox();
	}

	~LoadSaverBase()
	{
	}

	/// We are the serializer, so we don't do anything with this.
	virtual void Serialize( IArchive &ar ) {};

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType()
	{
		return dynamic_cast<IHashString *>(&m_HashName);
	}

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf( IHashString *compType )
	{
		return ( compType->GetUniqueID() == GetComponentType()->GetUniqueID() );
	}

	/// Registers the component based on extension description provided by overriden functions in subclass
	void RegisterLoadSaveComponent()
	{
		m_LoadSaveRegisterData.m_ComponentName = GetComponentType();
		m_LoadSaveRegisterData.m_FilterStrings.m_LoadFilter = GetLoadDescription();
		m_LoadSaveRegisterData.m_FilterStrings.m_SaveFilter = GetSaveDescription();
		m_LoadSaveRegisterData.m_bPrecachable = GetPrecachableBool();	// Precachable bool
		m_LoadSaveRegisterData.m_pfnCallback = GetPrecachableFunc();	// Precachable func

		static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
		DWORD result = m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE), &m_LoadSaveRegisterData, NULL, NULL );

		if( !result )
		{
			m_ToolBox->Log( LOGERROR, _T("%s: Failed to register load/saver!\n"), m_HashName ); 
		}
	}

	/// message to load a  file
	virtual DWORD OnLoadFile( DWORD size, void *params )
	{
		DWORD retVal = MSG_HANDLED_STOP;

		VERIFY_MESSAGE_SIZE( size, sizeof(TCHAR *) );
		TCHAR *fileName = (TCHAR*)params;
		CHashString hszFileName( fileName );

		IXMLArchive *ar = CreateXMLArchive( fileName );

		if( ar != NULL )
		{
			retVal = LoadFile( ar, &hszFileName );
			ar->Close();
		}
		return retVal;
	}

	/// message to save a file
	virtual DWORD OnSaveFile( DWORD size, void *params )
	{
		DWORD retVal = MSG_HANDLED_STOP;

		VERIFY_MESSAGE_SIZE( size, sizeof(TCHAR *) );
		TCHAR *fileName = (TCHAR*)params;
		CHashString hszFileName( fileName );

		IXMLArchive *ar = CreateXMLArchive( fileName, STREAM_MODE_WRITE );

		if( ar != NULL )
		{
			retVal = SaveFile( ar, &hszFileName );
			ar->Close();
		}
		return retVal;
	}

protected:
	IXMLArchive *CreateXMLArchive( TCHAR *fileName, unsigned int streamMode = STREAM_MODE_READ )
	{
		CREATEARCHIVE ca;

		CHashString streamType(_T("File"));
		ca.mode = streamMode;
		ca.streamData = fileName;
		ca.streamType = &streamType;
		static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
		DWORD result = m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca);

		if( result != MSG_HANDLED )
		{
			m_ToolBox->Log( LOGWARNING, _T("Failed creating XML archive for loading file %s\n"), fileName );
			return NULL;
		}

		return (IXMLArchive*)ca.archive;
	}

	/// Handles reading from the archive.
	/// We assume the root node follows the "version and name" standard, and that name is unique
	virtual DWORD LoadFile( IXMLArchive *ar, IHashString *fileName )
	{
		if( ar != NULL )
		{
			ar->SeekTo(0);

			StdString nodeType;
			float version;
			StdString rootName;
			ar->GetNode( nodeType );
			ar->Read( version, _T("version") );
			ar->Read( rootName, _T("name") );
			CHashString hszRootType( nodeType );
			CHashString hszRootName( rootName );
			static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
			FINDOBJECTPARAMS param;
			param.hszName = &hszRootName;
			DWORD findObjectRetVal = m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );

			if( (findObjectRetVal == MSG_HANDLED) && (param.bFound == false ) )
			{
				ar->SeekTo(0);
				XMLParentNodeTracker nodeTracker;

				// We don't want to limit the entire system with name restrictions. It would
				// cause problems if two different state machines shared a similar names such
				// items as events. We will use this map to keep track of parent nodes with their associated names.
				NODEIDTOHASHIDMAP nodeIDToHashIDMap;

				while( ar->GetNode(nodeType) )
				{
					nodeTracker.OnNodeMoved( ar );

					CHashString hszNodeType( nodeType );
					CHashString hszParentName(_T(""));
					CHashString hszObjectName( _T("") );

					if( nodeTracker.parentPosition > 0 )
					{
						DWORD parentHashID = nodeIDToHashIDMap[ nodeTracker.parentPosition ];
						hszParentName.Init( m_ToolBox->GetHashString(parentHashID) );
						GenerateObjectName( &hszObjectName, &hszNodeType, ar );
					}
					else
					{
						hszObjectName.Init( rootName );
					}

					CreateObject( ar, &hszParentName, &hszNodeType, &hszObjectName );

					nodeIDToHashIDMap[ nodeTracker.currentPosition ] = hszObjectName.GetUniqueID();
				}

				static DWORD msgHash_MapToFileName = CHashString(_T("MapToFileName")).GetUniqueID();
				m_ToolBox->SendMessage(msgHash_MapToFileName, sizeof(IHashString), fileName, &hszRootName, &hszRootType );
			}
		}

		return MSG_HANDLED_STOP;
	}

	///	create next object in the hierarchy
	///	\param	ar - pointer to IXMLArchive with object initialization data
	///	\param	parentName - name of the parent object
	///	\param	type - type of the creating object
	///	\param	objectName - unique generated name for object. Function may write 
	///						 own name to this parameter. This allows to adjust hierarchy
	virtual void CreateObject( IXMLArchive * ar, IHashString *parentName, IHashString *type, IHashString *objectName )
	{
		CREATEOBJECTPARAMS createObjectParams;
		createObjectParams.parentName = parentName;
		createObjectParams.typeName = type;
		createObjectParams.name = objectName;
		static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_CreateObject, sizeof(CREATEOBJECTPARAMS), &createObjectParams );

		SERIALIZEOBJECTPARAMS serializeObjectParams;
		serializeObjectParams.name = objectName;
		serializeObjectParams.archive = ar;
		static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_SerializeObject, sizeof(SERIALIZEOBJECTPARAMS), &serializeObjectParams );

		INITOBJECTPARAMS initObjectParams;
		initObjectParams.name = objectName;
		static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
		m_ToolBox->SendMessage( msgHash_InitObject, sizeof(INITOBJECTPARAMS), &initObjectParams );
	}

	/// Handles writing from the archive
	virtual DWORD SaveFile( IXMLArchive *ar, IHashString *fileName )
	{
		IComponent *managerComponent = m_ToolBox->CreateComponent( &m_ManagerName, 0 );
		CManager *manager = (CManager*)managerComponent;
		IDTOOBJECTMAP *rootObjectMap = manager->GetObjectMap( &m_StartNodeName );
		if( rootObjectMap != NULL )
		{
			IDTOOBJECTMAP::iterator rootObjectIterator = rootObjectMap->begin();

			for( ; rootObjectIterator != rootObjectMap->end(); rootObjectIterator++ )
			{
				IObject *iteratedObject = dynamic_cast<IObject*>(rootObjectIterator->second);
				ar->SetIsWriting( true );
				iteratedObject->Serialize( *ar );
			}
		}

		return MSG_HANDLED_STOP;
	}

	virtual const TCHAR *GetLoadDescription() = 0;

	virtual const TCHAR *GetSaveDescription() = 0;

	virtual bool GetPrecachableBool() = 0;
	
	virtual CACHABLEQUERYCALLBACK GetPrecachableFunc() = 0;

	/// Generates a unique name for a given object.
	virtual void GenerateObjectName( IHashString *objectName, IHashString *nodeType, IXMLArchive *archive )
	{
		GENERATEUNIQUEOBJECTNAMEPARAMS generateNameParams;
		generateNameParams.name = nodeType;
		generateNameParams.newname = objectName;

		static DWORD msgHash_GenerateUniqueObjectName = CHashString(_T("GenerateUniqueObjectName")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_GenerateUniqueObjectName, sizeof(generateNameParams), &generateNameParams );
	}

protected:
	IToolBox *m_ToolBox;

private:
	CHashString				m_HashName;
	CHashString				m_ManagerName;
	CHashString				m_StartNodeName;
	LSRC_MESSAGE			m_LoadSaveRegisterData;
};

#endif //#ifndef _LOADSAVERBASE_H_