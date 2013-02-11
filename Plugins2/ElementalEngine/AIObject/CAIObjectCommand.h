///=====================================================================
/// \file	CAIObjectCommand.h
/// \brief	Defines of CAIObjectCommand Class
///			This file defines a CAIObjectCommand which is a single
///			command of a sequence belonging to a parent CAIObject
///			Inherits from IObject
/// \date	05/23/2007
/// \author	Richard Nguyen
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

#ifndef _CAIOBJECTCOMMAND_H__
#define _CAIOBJECTCOMMAND_H__

class CAIObjectCommand : public CObjectTemplate<IObject>
{
public:
	/// \brief The Default Constructor for CAIObjectCommand
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CAIObjectCommand( IHashString *parentName, IHashString *name );

	/// Accepts *params as an IHashString and populates it as a copy of m_hszCommandString
	DWORD OnGetCommandString( DWORD size, void *params );

	/// Notifies the parent that this command is complete and is ready for the next
	DWORD OnCompleteCommand( DWORD size, void *params );

	/// Notifies the parent that the current command has been aborted
	DWORD OnCancelCommand( DWORD size, void *params );

	/// Creates a properties schema for the editor view
	DWORD OnGetSchema( DWORD size, void *params );

	/// \brief Create the a instance of IObject
	/// \param argptr - pointer to arguments for this type of object
	/// \returns IComponent*: Pointer to This
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief The Default Destructor for CAIObjectCommand
	~CAIObjectCommand();

	/// \brief The Update Function for the Object
	/// \return bool: Success
	virtual bool Update( void );

	/// \brief The DeInit Function for the Object
	/// \return void
	virtual void DeInit( void );

	/// override object template override
	virtual void Serialize(IArchive &ar);

private:
	CHashString m_hszCommandString;

};

#endif //#ifndef _CAIOBJECTCOMMAND_H__