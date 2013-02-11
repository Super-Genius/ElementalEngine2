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

#include "StdAfx.h"

CSectorRenameVisitor::CSectorRenameVisitor() :
	  m_SectorClassHashName(_T("CTerrainSector"))
{
	m_ToolBox = EngineGetToolBox();
}

void CSectorRenameVisitor::SetBaseName( const TCHAR *baseName)
{
	// make sure relative path
	const TCHAR *relPathName = m_ToolBox->GetRelativePath(baseName);
	// split up the path
	_tsplitpath(relPathName, NULL, newPath, newName, NULL);
	// make sure lower case for IHashString Compares
	_tcslwr(newPath);
	_tcslwr(newName);
}


bool CSectorRenameVisitor::IsHierarchicalVisitor() 
{
	return false; 
}

void CSectorRenameVisitor::ChangeSectorLayerNames(IHashString *sectorName)
{
	CHashString hszNewPath(newPath);

	static DWORD msgHash_TerrainSetPath = CHashString(_T("TerrainSetPath")).GetUniqueID();
	DWORD retVal = m_ToolBox->SendMessage(msgHash_TerrainSetPath, sizeof(IHashString), &hszNewPath, sectorName);
	if (retVal != MSG_HANDLED)
	{
		m_ToolBox->Log( LOGERROR, _T("%s(%i): Failed to set terrain path."), __FILE__, __LINE__ );
		return;
	}

	// we changed the name, so better save it out
	SINGLETONINSTANCE(CTerrainEditor)->SetSectorModified(sectorName);
}

bool CSectorRenameVisitor::Visit( IComponent * component, bool bVisitEnter)
{
	if (component->GetComponentType()->GetUniqueID() == m_SectorClassHashName.GetUniqueID())
	{
		IObject *object;
		object = dynamic_cast<IObject *>(component);
		ChangeSectorLayerNames(object->GetName());
	}
	return true;
}
