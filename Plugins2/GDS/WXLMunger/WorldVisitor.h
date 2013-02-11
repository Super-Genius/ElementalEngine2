///============================================================================
/// \file		CWorldVisitor.h
/// \brief		Header file for visitor pattern to manipulate models in heirarchy
/// \date		05-25-2005
/// \author		Patrick Ghiocel
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
#ifndef _CWORLDVISTISTOR_H_
#define _CWORLDVISTISTOR_H_

#include "Main.h"
#include <assert.h>
#include "Vec3.h"

struct MODELINFO
{
	MODELINFO( )
	{
		m_wszClassification = _T("MODEL");
		m_wszNewFileName = _T("");
	}

	StdString m_wszClassification;
	StdString m_wszNewFileName;
};
typedef std::map<StdString, MODELINFO> MODELMAP;

struct CPhysObjectStruct
{
	StdString szName;
	Vec3 vPosition;
	Vec3 vRotation;
	Vec3 vScale;
	float fMass;
	StdString szDynamic;
	int iNumShapes;
};
	

class CWorldVisitor : public IVisitor
{
public:
	CWorldVisitor( );

	~CWorldVisitor( );
	
	bool Visit( IComponent * component, bool bVisitEnter );
	
	bool IsHierarchicalVisitor( void );
	
	void SetArchiver( IArchive * pArchiver );
	
	void CreateReport( StdString wszFileName );
	void OutputReport();
	
	void SetObjectExclusion( bool fFlag );
	void AddObject( StdString wszNameType );
	bool CheckObject( StdString wszNameType );

private:
	IToolBox* m_ToolBox;

	IArchive* m_pArchiver;
	//IArchive* m_pReporter;
	IXMLArchive *m_pReporter;
	bool m_bObjectExclusion;
	std::vector<StdString> m_vObjectTypes;
	int m_iMemSize;
	char* m_pMemChunk;
	MODELMAP m_vModelReference;
	int m_iNewEntries;
	int m_iV3ONameEdits;
	StdString m_szLastV3OFileName;	// This is used for the CPhysicsObject refactor
};

#endif // _CWORLDVISTISTOR_H_
