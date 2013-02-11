///============================================================================
/// \file		CoordinateObject.h
/// \brief		Declaration of CBoundingBox Object Class
/// \date		09-18-2006
/// \author		Richard Nguyen
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

#ifndef __COORDINATEOBJECT_H__
#define __COORDINATEOBJECT_H__

class CCoordinateObject : public CObjectTemplate<IObject>
{
private:
	/// Default Constructor
	CCoordinateObject( IHashString *parentName, IHashString *name );
	/// Default Destructor
	~CCoordinateObject();

public:
	static IComponent *Create(int nArgs, va_list argptr);

	/// Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize( IArchive &ar );

	/// Initializes the CBoundingBox
	virtual void Init();

	/// \brief	The Update Function for the Object
	virtual bool Update();

	/// \brief	The DeInit Function for the Object
	virtual void DeInit();

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);

	/// MESSAGES
	DWORD OnGetGlobalPosition( DWORD size, void *params );
	DWORD OnSetGlobalPosition( DWORD size, void *params );

private:
	IToolBox				*m_ToolBox;
	CCoordinateRenderObject	*m_RenderObject;
	CHashString				m_hszNameType;
};

#endif //__COORDINATEOBJECT_H__

