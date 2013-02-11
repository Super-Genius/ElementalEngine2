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

#ifndef HIERARCHICALANIMATION_H
#define HIERARCHICALANIMATION_H

#include "stdafx.h"
#include <vector>

using namespace std;


class HierarchicalAnimation :
	public CObjectTemplate<IObject>
{
	/// \brief The Default Constructor for HierarchicalAnimation
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	HierarchicalAnimation(IHashString *parentName, IHashString *name, bool bAddToHier );

public:
	/// \brief	The Default Destructor for HierarchicalAnimation
	~HierarchicalAnimation();

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
	DWORD OnGetAnimationTime(DWORD size, void *params);
public:
	bool	ReadAnimations( IArchive &ar );
	HierarchicalNode * GetNode( UINT id );
	void CleanNodeMapList();
	void ReadKeyframeData( IArchive &ar, HierarchicalNode * node );
	UINT AddNode( StdString name, int id, int pid );

	//Keyframe data for animatoin goes here, how to link to nodes hrmm
	HNODEMAP				m_NodeMap;
	HNODEVECTOR	m_NodeMapList;
	HNODEVECTOR	m_RootNodes;
	float			m_fTime;

private: // hey marv... shouldn't those vars be... private?
	bool m_bAddToHierarchy;
};

#endif