///============================================================================
/// \file		CoordinateToolManager.h
/// \brief		Declaration of CCoordinateToolManager Class
/// \date		07-26-2005
/// \author		D. Patrick Ghiocel, Dmitriy S. Sergeyev
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

#ifndef __COORDINATETOOLMANAGER_H__
#define __COORDINATETOOLMANAGER_H__

#include "CManager.h"

class CCoordinateToolManager : public CManager
{
public:
	/// Picking object type for axes.
	static CHashString m_pickingObjectType;
	/// X-axis box picking physics object name.
	static StdString m_xAxisBoxPostfix;
	/// Y-axis box picking physics object name.
	static StdString m_yAxisBoxPostfix;
	/// Z-axis box picking physics object name.
	static StdString m_zAxisBoxPostfix;

	/// Axis type.
	enum AXIS
	{
		AXIS_NONE, AXIS_X, AXIS_Y, AXIS_Z
	};

private:
	SINGLETONCONSTRUCTOROVERRIDE(CCoordinateToolManager);

	/// \brief	Default Destructor
	///			Since CCoordinateToolManager is implemented as a singleton the
	///			constructor is private.
	CCoordinateToolManager();

public:

	/// Default Destructor
	virtual ~CCoordinateToolManager();

	/// Function to get this component.
	///			Returns the singleton instance in this case
	static IComponent *Create(int nArgs, va_list argptr);

	/// Initializes the CCoordinateToolManager 
	/// \return	True on sucess, False on failure
	virtual bool Init();

	/// Updates the CCoordinateToolManager
	virtual void Update(DWORD tickCount);

	///	DeInitializes the CCoordinateToolManager
	/// \return	True on sucess, False on failure
	virtual bool DeInit();

	/// Add a render object to the list of render objects
	void AddRenderObject(IRenderObject* renderObject);

	/// Remove a render object from the list of reneder objects
	void RemoveRenderObject(IRenderObject* renderObject);

	/// Find coordinate tool by parent.
	IObject* FindByParent(IHashString* parent);

	/// Helper function retrieves axis by physics object name.
	static AXIS GetAxisByObjectName(IHashString* name);

	// **** MESSAGES **** //
	DWORD OnAddAxis(DWORD size, void *params);
	DWORD OnRemoveAxis(DWORD size, void *params);
	DWORD OnLeftClickTool(DWORD size, void *param);
	DWORD OnMouseMove(DWORD size, void *param);
	DWORD OnDeleteObject(DWORD size, void *param);

private:

	/// Helper function to build the axis
	void BuildAxis(IHashString* parentName);
	/// Helper function to clean up the axis
	void CleanAxis();

	/// Hashstring for the Coordinate Tool Class this Manager handles.
	static CHashString m_hszCoordinateToolClass;

	/// Postfix for Coordinate Tool object name.
	static StdString m_wszCoordinateToolPostfix;

	/// List of all render objects
	/// Is there a better way to do this with the stored names?
	OBJECTLIST m_renderObjects;

	/// Object list we're responsible to handle
	PICKABLELIST m_PickableObjects;

	/// True when coordinate is active.
	bool m_active;

	/// Object tool config which should be restored on tool deactivation.
	EDITORSETOBJECTTOOLCONFIGPARAMS m_objectToolConfig;

	/// Name of the active coordinate tool
	IHashString* m_CoordinateToolName;
};

#endif // __COORDINATETOOLMANAGER_H__