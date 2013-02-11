///=====================================================================
/// \file	DependentProcessors.h
/// \brief	Declaration of functions for processing
/// \date	11/16/2007
/// \author	Andrey Ivanov
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
///=====================================================================
#ifndef _GDS_UNDOREDO_DEPENDENT_PROCESSORS_H_
#define _GDS_UNDOREDO_DEPENDENT_PROCESSORS_H_

class CDependentProcessors
{
	typedef DWORD (*FProcessor)(IHashString *, bool);
	typedef map<DWORD, FProcessor> ProcessorMap;

public:
	CDependentProcessors();

	DWORD ProcessObject(IHashString *name, bool undo);

private:
	static DWORD ProcessCTerrainSector(IHashString *name, bool undo);
	static DWORD ProcessCTerrainSectorRenderObject(IHashString *name, bool undo);

private:
	ProcessorMap m_Processors;
	static IToolBox *m_pToolBox;
};

#endif // _GDS_UNDOREDO_DEPENDENT_PROCESSORS_H_