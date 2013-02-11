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

#ifndef HIERARCHICALMODELTYPE_H
#define HIERARCHICALMODELTYPE_H
#include "stdafx.h"


struct VERTEX_NOSKIN
{
    float    pos[3];
    float	normal[3];
    float	 tu,tv;
	float	tangent[4];
};

struct GETHIERARCHYTEMPLATEPARAM
{
	HNODEVECTOR * list;
	GETHIERARCHYTEMPLATEPARAM()
	{
		list = NULL;
	}
};

class HierarchicalModelType : public CObjectTemplate<IObject>
{
	/// \brief The Default Constructor for HierarchicalModelType
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	HierarchicalModelType(IHashString *parentName, IHashString *name, bool bAddToHier );

public:
	/// \brief	The Default Destructor for HierarchicalModelType
	~HierarchicalModelType();

	/// \brief	Create an instance of IObject
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief Init Function
	virtual void Init( void );

	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();

	/// \brief	The DeInit Function for the Object
	virtual void DeInit();

	/// Serialize
	virtual void Serialize(IArchive &ar);

	DWORD OnGetHierarchyTemplate(DWORD size, void *params);
	DWORD OnGetChildID(DWORD size, void *params);
public:
	bool RenderSubGeometry( UINT id, UINT pass );
	HierarchicalNode * AddNode( StdString name, int id, int pid );
	bool LoadGeometry( IArchive &ar, IGeometry *geom );
	bool LoadCalModel( IArchive &ar, HierarchicalNode * node, StdString filename );
	bool LoadClothModel( IArchive &ar, HierarchicalNode * node, StdString filename );
	bool LoadControl(  IArchive &ar, HierarchicalNode * node );
	bool SetupTextures( IArchive &ar, IGeometry *geom );

	int GetChildID(IHashString *name);
	HNODEVECTOR* GetHierarchyTemplate();

	HierarchicalNode * GetNode( UINT id );
	void CleanNodeMapList();

	HNODEMAP		m_NodeMap;
	HNODEMAP		m_NodeNameIDMap;
	HNODEVECTOR	m_NodeMapList;
	//geometry to render
	//vector< IGeometry * >
private:
	static void CheckInitializedStaticVariables();
	static bool m_bClassInitialized;
	static CHANNELDESCRIPTORLIST m_HGeomFormat;
	static IToolBox * m_ToolBox;
	bool m_bAddToHierarchy;
};

#endif