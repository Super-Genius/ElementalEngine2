///============================================================================
/// \file		NavMeshRenderObject.h
/// \brief		Declaration of CNavMeshRenderObject Class
/// \date		09-07-2005
/// \author		Brian Bazyk
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

#ifndef __NAVMESHOBJECTRENDEROBJECT_H__
#define __NAVMESHOBJECTRENDEROBJECT_H__

#include "CObjectTemplate.hpp"
#include "IRenderObject.h"

class CNavMeshRenderObject : public CSceneObject<IRenderObject>
{
protected:
	/// \brief Default Constructor
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CNavMeshRenderObject(IHashString *parentName, IHashString *name);

public:
	/// \briefDefault Destructor
	~CNavMeshRenderObject();

	/// \brief Create an instance
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	/// \brief Update Function
	/// \return	True on sucess, False on failure
	virtual bool Update();

	/// \brief DeInit Function
	virtual void DeInit();

	/// \brief Init Function
	virtual void Init();

	/// \brief serialize
	/// \param ar - The Archive
	virtual void Serialize(IArchive &ar);

	// IRenderObject
	virtual bool Render( UINT pass, IEffect * override );
	virtual bool IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass );
	virtual DWORD GetRenderPriority();
	virtual UINT GetNumPass() { return 1; };
	virtual void SetPosition( const Vec3 &vec );
	virtual Vec3 &GetPosition();
	virtual IHashString * GetBaseSortClass(){ return GetComponentType(); }
	virtual bool IsAlphaObject(){ return false; }
	IMeshObject * GetMeshObject();
	virtual UINT GetNumGeometry(){ return 0; }
	virtual IGeometry* GetGeometry( int index ){ return NULL; }
	virtual bool GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation );
	virtual bool IsKindOf( IHashString * compType );
	virtual void AttachLight( ILightObject * light ) { }
	virtual void DetachAllLights() { }
	
	virtual IMaterial * GetMaterial( int index ){ return NULL; }
	virtual UINT GetNumMaterial() { return 0; }
	virtual ISceneSettings * GetSceneSettings(){ return NULL; }

	// ** MESSAGES ** //
	DWORD OnMouseMove(DWORD size, void *param);

private:
	void MouseMove( MOUSECOORDS *pMouseMove );

	/// Class Type
	CHashString m_hszNameType;

	NAVINDEX m_iClosestVertexToMouseRay;
};


#endif //__NAVMESHRENDEROBJECT_H__