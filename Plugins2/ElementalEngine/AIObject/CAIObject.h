///=====================================================================
/// \file	CAIObject.h
/// \brief	Defines of CAIObject Class
///         This file defines a CAIObject which is an AI Object
///			for entities
///			Inherits from IObject
/// \date	02/15/2005
/// \author	Patrick Ghiocel
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

#ifndef _CAIOBJECT_H__
#define _CAIOBJECT_H__

typedef list<CAIObjectCommand*> AICOMMANDSEQUENCE;
typedef map<DWORD, AICOMMANDSEQUENCE::iterator> IDTOSEQUENCEITERATORMAP;

class CAIObject : public CObjectTemplate<IObject>
{
private:
	/// \brief The Default Constructor for CAIObject
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CAIObject( IHashString *parentName, IHashString *name );

public:
	/// \brief Create the a instance of IObject
	/// \param argptr - pointer to arguments for this type of object
	/// \returns IComponent*: Pointer to This
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief The Default Destructor for CAIObject
	~CAIObject();

	/// \brief The Update Function for the Object
	/// \return bool: Success
	virtual bool Update( void );

	/// \brief The DeInit Function for the Object
	/// \return void
	virtual void DeInit( void );

	/// override object template override
	virtual void Serialize(IArchive &ar);

	void AddAIObjectCommand( CAIObjectCommand *object );

	void RemoveAIObjectCommand( CAIObjectCommand *object );

	/// Executes the current command of the specified object
	void ExecuteCurrentCommand( IHashString *targetName );

	/// Cancels the command sequence of the specified object
	void CancelCommandSequence( IHashString *targetName );

	/// Message handler to create a list iterator for a target item.
	/// Executes the first command in the list
	DWORD OnStartCommandSequence( DWORD size, void *params );

	/// Message handler to iterate to the next command and execute it
	DWORD OnCompleteCurrentCommand( DWORD size, void *params );

	/// Removes the iterator for the target item, thereby canceling its
	/// command sequence
	DWORD OnClearCommandSequence( DWORD size, void *params );

private:
	CHashString					m_msgExecuteCommand;
	AICOMMANDSEQUENCE			m_AICommandSequence;
	IDTOSEQUENCEITERATORMAP		m_SequenceIteratorMap;
};

#endif