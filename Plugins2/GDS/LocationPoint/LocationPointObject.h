///============================================================================
/// \file		LocationPointObject.h
/// \brief		Declaration of CLocationPointObject Object Class
/// \date		12/13/2006
/// \author		Nick Tourte
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
/*



DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED



#ifndef __LOCATIONPOINTOBJECT_H__
#define __LOCATIONPOINTOBJECT_H__

class CLocationPointObject : public CObjectTemplate<IObject>
{
public:
	// Default constructor
	CLocationPointObject( IHashString *parentName, IHashString *name, bool bAddToHier );

	// Default destructor
	~CLocationPointObject();

	/// \fn		virtual void Init()
	/// \brief	Initialize the object
	void Init();
 
	/// \fn		virtual void DeInit()
	/// \brief	Deinitialize the object
	void DeInit();

	static IComponent *Create(int nArgs, va_list argptr);	

	/// Gets the render object assigned to this location point
	/// \return IRenderObject that was created
	IRenderObject* GetRenderObject()
	{
		return dynamic_cast<IRenderObject*>(m_RendObj);
	}

	void Serialize(IArchive &ar);

	/// Receives this message when the entity parent gets updated
	DWORD OnSetDirtyTransformFlag(DWORD size, void *param);

	/// These two are received from the render object, need to be passed up to entity
	DWORD OnParentUpdateBoundingBox(DWORD size, void *param);
	DWORD OnParentRemoveBoundingBox(DWORD size, void *param);

	/// These are received from the render object
	DWORD OnGetGlobalPosition(DWORD size, void *param);
	DWORD OnGetGlobalEuler(DWORD size, void *param);
	DWORD OnGetGlobalScale(DWORD size, void *param);
	DWORD OnGetBoundingBox(DWORD size, void *param);
	DWORD OnGetGlobalTransform(DWORD size, void *param);

private:

	/// Creates the render object to use in editor (deprecated)
	void CreateRenderObject();

	/// Creates an editor entity that gets placed in the hierarchy instead of this guy
	void CreateEditorEntityMunge();

	/// Destroys render object
	void DestroyRenderObject();

	bool m_bAddToHierarchy;
	
	/// Internal render object
	IObject *m_RendObj;

	/// Filename of render object to create
	StdString m_szROFileName;

	/// Position and orientation of entity above
	Vec3 m_Position;
	Vec3 m_Orientation;
	Vec3 m_Scale;	
};

#endif //#ifndef __LocationPointObject_H__
*/
