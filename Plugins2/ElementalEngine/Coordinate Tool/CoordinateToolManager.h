///============================================================================
/// \file		CoordinateToolManager.h
/// \brief		Declaration of CCoordinateToolManager Class
/// \date		08-03-2007
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

#ifndef __COORDINATETOOLMANAGER_H__
#define __COORDINATETOOLMANAGER_H__

class CCoordinateToolManager : public CManager
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CCoordinateToolManager );

public:
	// Default Constructor
	CCoordinateToolManager();

	// Default Destructor
	virtual ~CCoordinateToolManager( void );

	/// \brief	Function to get this component.
	///			Returns the singleton instance in this case
	static IComponent *Create(int nArgs, va_list argptr);

	/// Initializes the component and returns true on success,
	/// otherwise false
	virtual bool Init();

	/// \fn virtual void Update()
	/// \brief Function call back when system is being updated
	virtual void Update(DWORD tickCount);

	/// \brief Serialize this component to the archive.
	///	\param ar = reference to the archive class
	virtual void Serialize(IArchive &ar);

	/// \brief return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// \brief Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType) { return false; }

private:
	IToolBox *m_ToolBox;
};

#endif // #ifndef __COORDINATETOOLMANAGER_H__
