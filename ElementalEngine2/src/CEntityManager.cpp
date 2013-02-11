///==========================================================================
/// \file	CEntityManager.cpp
/// \brief	Manager for CEntity objects
///			This file defines CEntityManager which manages the creation
///			and manipulation of CEntity objects. 
///			Inherits from CManager
/// \date	2-22-2005
/// \author	Kyle Swaim
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
///==========================================================================

#include "StdAfx.h"

//#include "IToolBox.h"

#include "CEntityManager.h"
#include "CEntity.h"

REGISTER_COMPONENT_SINGLETON(CEntityManager);

REGISTER_MESSAGE_HANDLER(VisitEntities, OnVisitEntities, CEntityManager);
REGISTER_MESSAGE_HANDLER(Start, OnStart, CEntityManager);
REGISTER_MESSAGE_HANDLER(Stop, OnStop, CEntityManager);
//REGISTER_MESSAGE_HANDLER(GetAffectedObjects, OnGetAffectedObjects, CEntityManager);

///
/// Constructor / Destructor
///

CEntityManager::CEntityManager() : 
	CManager(_T("CEntityManager"))
{
}

IComponent *CEntityManager::Create(int nArgs, va_list argptr)
{
	// Return the singleton instance of this class
	return SINGLETONINSTANCE( CEntityManager );
}

CEntityManager::~CEntityManager()
{
	DeInit();
}

///
/// Inherited methods
///

bool CEntityManager::Init()
{
	// Set to run by default
	m_IsRunning = true;

	return CManager::Init();
}

bool CEntityManager::CompareNames(StdString searchString, StdString compString)
{
	static StdString token(_T("\\"));
	size_t subLen;
	// if searchString is null, positive match w/ everything
	if(_tcscmp(searchString, _T("")) == 0)
	{
		return true;
	}
	// if compString is null, no match
	if(_tcscmp(compString, _T("")) == 0)
	{
		return false;
	}
	const TCHAR* searchStringArray = searchString.c_str();
	const TCHAR* compStringArray = compString.c_str();

	while( _tcscmp(searchStringArray, _T("")) != 0 )
	{
		subLen = _tcscspn(compStringArray, token); //length of substring to token

		if (subLen > 0)
		{
			// compare substrings up to token 
			if( _tcsncmp(searchStringArray, compStringArray, subLen) == 0 )
			{
				return true;
			}

			// now check if compString's substring's next char is == token 
			if( compStringArray[subLen] != token.c_str()[0] )
			{
				// did not return above, therefore does not match
				return false;			
			}		

			// continue...advance ptr
			compStringArray = _tcsninc(compStringArray, subLen+1);
		}
		else
		{
			// string out
			return false;
		}
	}
	// if exited while loop..then true
	return true;
}

bool CEntityManager::VisitNode(IHashString *searchString, IVisitor *visitorPattern)
{
	static CHashString compType(_T("CEntity"));
	IDTOOBJECTMAP *entityMap = GetObjectMap(&compType);
	if(entityMap == NULL)
	{
		return true;
	}
	
	if (!visitorPattern)
	{
		return false;
	}

	IDTOOBJECTMAP::iterator osIter = entityMap->begin();	
	CEntity *entity;
	if (searchString != NULL)
	{
		for(; osIter != entityMap->end(); ++osIter)
		{
			entity = dynamic_cast<CEntity *>(osIter->second);
			if (entity == NULL)
			{
				m_ToolBox->SetErrorValue(ERR_NULL_POINTER);
				m_ToolBox->Log(LOGERROR, _T("Unable to cast IObject* to CEntity* \n"));
				return false;
			}
			if (CompareNames(searchString->GetString(), entity->GetEntityTypes()) )
			{
				IComponent *visitComp = dynamic_cast<IComponent *>(osIter->second);
				if (visitComp != NULL)
				{
					visitorPattern->Visit(visitComp, true);
				}
				else
				{
					IHashString *typeName = visitComp->GetComponentType();
					EngineGetToolBox()->Log(LOGWARNING, 
						_T("component of type %s was used as IComponent and is not one.\n"),
						typeName);
				}
			}
		}
	}
	else
	{
		for(; osIter != entityMap->end(); ++osIter)
		{
			IComponent *visitComp = dynamic_cast<IComponent *>(osIter->second);
			if (visitComp != NULL)
			{
				visitorPattern->Visit(visitComp, true);
			}
			else
			{
				IHashString *typeName = visitComp->GetComponentType();
				EngineGetToolBox()->Log(LOGWARNING, 
					_T("component of type %s was used as IComponent and is not one.\n"),
					typeName);
			}
		}
	}
	return true;
}

DWORD CEntityManager::OnVisitEntities(DWORD size, void *params)
{
	VISITENTITYDATA *ved;
	
	VERIFY_MESSAGE_SIZE(sizeof(VISITENTITYDATA), size);

	ved = (VISITENTITYDATA *)params;
	
	if (!VisitNode(ved->searchString, ved->visitor))
	{
		return MSG_ERROR;
	}
	else
	{
		return MSG_HANDLED_PROCEED;
	}
}

/// \brief	Start running the physics simulation
DWORD CEntityManager::OnStart(DWORD size, void *params)
{
	m_IsRunning = true;

	return MSG_HANDLED_PROCEED;
}

/// \brief	Stop running the physics simulation
DWORD CEntityManager::OnStop(DWORD size, void *params)
{
	m_IsRunning = false;

	return MSG_HANDLED_PROCEED;
}