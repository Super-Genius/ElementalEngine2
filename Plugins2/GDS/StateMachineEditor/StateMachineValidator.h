///=====================================================================
/// \file	StateMachineValidator.h
/// \brief	Declaration of CStateMachineValidator
/// \date	12/28/2007
/// \author	Andrey Ivanov
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
#ifndef __STATE_MACHINE_VALIDATIOR_H__
#define __STATE_MACHINE_VALIDATIOR_H__

class CValidationError
{
public:
	CValidationError( IHashString *name, LPCTSTR message )
		: m_hszName( name )
		, m_szMessage( message )
	{
	}

	const IHashString *GetName() const
	{
		return &m_hszName;
	}

	LPCTSTR GetMessage() const
	{
		return m_szMessage;
	}

private:
	CHashString m_hszName;
	CString	m_szMessage;
};

class CStateMachineValidator
{
	enum
	{
		STATE_MACHINE,
		STATE,
		EVENT,
		ACTION,
		TRANSITION,
		OBJECTS_COUNT
	};

	typedef vector<StdString> Labels;

public:
	///	\brief	constructor
	///	\param	name - name of the validated state machine
	CStateMachineValidator( IHashString *name );

	///	\brief	check passed serialization data for error
	///	\param	sop - pointer to serialize object parameters
	///	\param	errors - list with found errors
	///	\return	true if passed data is valid for object
	bool ValidateObject( SERIALIZEOBJECTPARAMS *sop, list<CValidationError> &errors );

	///	\brief	check state machine with passed name for error based on whole 
	///			rule list
	///	\param	errors - list with found errors
	///	\return	true if state machine is valid
	bool Validate( list<CValidationError> &errors );

	///	\brief	reset internal caches
	void ResetCaches();

private:
	///	\brief	validate data for state machine's object
	///	\param	type - type of the object
	///	\param	objectName - pointer to IHashString with object name
	///	\param	ar - archive with checked data
	///	\param	checkName - set to true if name from archive data should be 
	///					    checked for uniqueness. This value may be set to 
	///						false if existing state machine data is checking
	///	\param	errors - list with found errors
	///	\return	true if passed data is valid for state machine object
	bool ValidateObject( IHashString *type, IHashString *objectName, IArchive *ar,
						 bool checkName, list<CValidationError> &errors );

	///	\brief	validate data for state machine object
	///	\param	objectName - pointer to IHashString with object name
	///	\param	ar - archive with checked data
	///	\param	checkName - set to true if name from archive data should be 
	///					    checked for uniqueness. This value may be set to 
	///						false if existing state machine data is checking
	///	\param	errors - list with found errors
	///	\return	true if passed data is valid for state machine object
	bool DoValidateStateMachine( IHashString *objectName, IArchive *ar,
								 bool checkName, list<CValidationError> &errors );

	///	\brief	validate data for state object
	///	\param	objectName - pointer to IHashString with object name
	///	\param	ar - archive with checked data
	///	\param	checkName - set to true if name from archive data should be 
	///					    checked for uniqueness. This value may be set to 
	///						false if existing state machine data is checking
	///	\param	errors - list with found errors
	///	\return	true if passed data is valid for state object
	bool DoValidateState( IHashString *objectName, IArchive *ar,
						  bool checkName, list<CValidationError> &errors );

	///	\brief	validate data for event object
	///	\param	objectName - pointer to IHashString with object name
	///	\param	ar - archive with checked data
	///	\param	checkName - set to true if name from archive data should be 
	///					    checked for uniqueness. This value may be set to 
	///						false if existing state machine data is checking
	///	\param	errors - list with found errors
	///	\return	true if passed data is valid for event object
	bool DoValidateEvent( IHashString *objectName, IArchive *ar,
						  bool checkName, list<CValidationError> &errors );

	///	\brief	validate data for action object
	///	\param	objectName - pointer to IHashString with object name
	///	\param	ar - archive with checked data
	///	\param	errors - list with found errors
	///	\return	true if passed data is valid for action object
	bool DoValidateAction( IHashString *objectName, IArchive *ar,
						   list<CValidationError> &errors );

	///	\brief	validate data for transition object
	///	\param	objectName - pointer to IHashString with object name
	///	\param	ar - archive with checked data
	///	\param	errors - list with found errors
	///	\return	true if passed data is valid for transition object
	bool DoValidateTransition( IHashString *objectName, IArchive *ar,
							   list<CValidationError> &errors );

	void FillStateLabels();

	bool IsStateLabel( const StdString &label );

	void FillStateNames( DWORD msg, IHashString *name, Labels &labels ) const;
	void FillStateEvents( IHashString *name, Labels &events ) const;
	void FillStateAvailableEvents( IHashString *name, Labels &events ) const;

	bool IsEventLabel( const Labels &events, const StdString &event ) const
	{
		return binary_search( events.begin(), events.end(), event );
	}

private:
	IToolBox *m_pToolBox;
	CHashString m_hszStateMachineName;
	Labels m_StateLabels;
};

#endif //__STATE_MACHINE_VALIDATIOR_H__
