///============================================================================
/// \file		ShapeManager.h
/// \brief		Declaration of CShapeManager Class
/// \date		07-26-2005
/// \author		D. Patrick Ghiocel
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

#ifndef __SHAPEMANAGER_H__
#define __SHAPEMANAGER_H__

#include "CManager.h"

class CAxisAlignedBoundingBox
{
public:
	CHashString m_hszObjectName;
	Vec3 min;
	Vec3 max;
	int red;
	int green;
	int blue;
	int alpha;
};

class CObjectOrientedBoundingBox
{
public:
	CHashString m_hszObjectName;
	Matrix4x4 localTransform;
	Vec3 min;
	Vec3 max;
	int red;
	int green;
	int blue;
};

class CSphere
{
public:
	CHashString m_hszObjectName;
	float radius;
	int red;
	int green;
	int blue;
	int resolution;
};

class CLine
{
public:
	CHashString m_hszObjectName;
	Vec3 start;
	Vec3 end;
	int red;
	int blue;
	int green;
	float width;
};

class CPoint
{
public:
	CHashString m_hszObjectName;
	int red;
	int blue;
	int green;
	float size;
};

class CShapeManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CShapeManager );

	/// \brief	Default Destructor
	///			Since CShapeManager is implemented as a singleton the
	///			constructor is private.
	CShapeManager();

public:

	/// Default Destructor
	virtual ~CShapeManager( );

	/// Function to get this component.
	///			Returns the singleton instance in this case
	static IComponent *Create( int nArgs, va_list argptr );

	/// Initializes the CShapeManager 
	/// \return	True on sucess, False on failure
	virtual bool Init();

	/// Updates the CShapeManager
	virtual void Update(DWORD tickCount);

	///	DeInitializes the CShapeManager
	/// \return	True on sucess, False on failure
	virtual bool DeInit();

	// Add a render object to the list of shape render objects
	void AddShapeRenderObject( IRenderObject* shapeRenderObject );

	std::vector<CAxisAlignedBoundingBox>& GetAxisAlignedBoundingBoxes() { return m_AxisAlignedBoundingBoxes; }
	std::vector<CObjectOrientedBoundingBox>& GetObjectOrientedBoundingBoxes() { return m_ObjectOrientedBoundingBoxes; }
	std::vector<CSphere>& GetSpheres() { return m_Spheres; }
	std::vector<CLine>& GetLines() { return m_Lines; }
	std::vector<CPoint>& GetPoints() { return m_Points; }

	// **** MESSAGES **** //
	DWORD OnAddAxisAlignedBox( DWORD size, void *params );
	DWORD OnRemoveAxisAlignedBox( DWORD size, void *params );
	DWORD OnAddObjectOrientedBox( DWORD size, void *params );
	DWORD OnRemoveObjectOrientedBox( DWORD size, void *params );
	DWORD OnAddSphere( DWORD size, void *params );
	DWORD OnRemoveSphere( DWORD size, void *params );
	DWORD OnAddLine( DWORD size, void *params );
	DWORD OnRemoveLine( DWORD size, void *params );
	DWORD OnAddPoint( DWORD size, void *params );
	DWORD OnRemovePoint( DWORD size, void *params );

private:
	/// Helper function to create the axis aligned box render object
	void CreateAxisAlignedBoxRenderObject();
	/// Helper function to delete the axis aligned box render object
	void DeleteAxisAlignedBoxRenderObject();

	/// Helper function to create the object oriented box render object
	void CreateObjectOrientedBoxRenderObject();
	/// Helper function to delete the object oriented box render object
	void DeleteObjectOrientedBoxRenderObject();

	/// Helper function to create the sphere render object
	void CreateSphereRenderObject();
	/// Helper function to delete the sphere render object
	void DeleteSphereRenderObject();

	/// Helper function to create the line render object
	void CreateLineRenderObject();
	/// Helper function to delete the line render object
	void DeleteLineRenderObject();

	/// Helper function to create the point render object
	void CreatePointRenderObject();
	/// Helper function to delete the point render object
	void DeletePointRenderObject();

	/// Hashstring for the AxisAlignedBox Class this Manager handles
	static CHashString m_hszAxisAlignedBoxClass;
	/// Hashstring for the object name of the AxisAlignedBox render object
	static CHashString m_hszAxisAlignedBoxRenderObjectName;
	
	/// Hashstring for the ObjectOrientedBox Class this Manager handles
	static CHashString m_hszObjectOrientedBoxClass;
	/// Hashstring for the object name of the ObjectOrientedBox render object
	static CHashString m_hszObjectOrientedBoxRenderObjectName;
	
	/// Hashstring for the Sphere Class this Manager handles
	static CHashString m_hszSphereClass;
	/// Hashstring for the object name of the Sphere render object
	static CHashString m_hszSphereRenderObjectName;
	
	/// Hashstring for the Line Class this Manager handles
	static CHashString m_hszLineClass;
	/// Hashstring for the object name of the Line render object
	static CHashString m_hszLineRenderObjectName;
	
	/// Hashstring for the Point Class this Manager handles
	static CHashString m_hszPointClass;
	/// Hashstring for the object name of the Point render object
	static CHashString m_hszPointRenderObjectName;

	/// List of all shape render objects
	/// Is there a better way to do this with the stored names?
	OBJECTLIST m_ShapeRenderObjects;

	/// List of axis aligned bounding boxes
	std::vector<CAxisAlignedBoundingBox> m_AxisAlignedBoundingBoxes;
	/// List of object oriented bounding boxes
	std::vector<CObjectOrientedBoundingBox> m_ObjectOrientedBoundingBoxes;
	/// List of spheres
	std::vector<CSphere> m_Spheres;
	/// List of lines
	std::vector<CLine> m_Lines;
	/// List of points
	std::vector<CPoint> m_Points;
};

#endif // __SHAPEMANAGER_H__