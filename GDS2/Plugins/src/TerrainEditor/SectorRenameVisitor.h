///============================================================================
/// \file		SectorRenameVisitor.h
/// \brief		Header file for Visitor patter to rename sector data files
/// \date		07-06-2005
/// \author		Kenneth Hurley
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

#ifndef _SECTORRENAMEVISITOR_H_
#define _SECTORRENAMEVISITOR_H_

class CSectorRenameVisitor : public IVisitor
{
public:
	CSectorRenameVisitor();
	/// tells the callee that we want to visit depth first
	bool IsHierarchicalVisitor();
	/// set the base name to rename the sectors with
	void SetBaseName(const TCHAR *baseName);
	/// the actual function to rename a layers files
	void ChangeSectorLayerNames(IHashString *sectorName);
	/// Visitor function callback
	bool CSectorRenameVisitor::Visit( IComponent * component, bool bVisitEnter);

private:
	void SetNewLayerMaskName(IHashString *name, int sectorX, int sectorY, int Layer);

private:
	/// the sector class name (hashed)
	CHashString m_SectorClassHashName;
	/// the new path we want to set
	TCHAR newPath[_MAX_PATH];
	/// the new filename portion we want to set
	TCHAR newName[_MAX_FNAME];
	/// quick pointer ot toolbox.
	IToolBox *m_ToolBox;
};


#endif // #ifndef _SECTORRENAMEVISITOR_H_
