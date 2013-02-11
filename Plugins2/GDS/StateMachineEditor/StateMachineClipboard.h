///=====================================================================
/// \file	StateMachineClipboard.h
/// \brief	Declaration of State Machine editor clipboard
/// \date	12/16/2007
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
#ifndef _STATE_MACHINE_CLIPBOARD_H_
#define _STATE_MACHINE_CLIPBOARD_H_

struct ObjectInfo
{
	CHashString hszType;
	CHashString hszName;
	CHashString hszParent;
};

class CStateMachineClipboard
{
public:
	CStateMachineClipboard( HWND hOwner );
	virtual ~CStateMachineClipboard();

	///	\brief	copy object hierarchies to clipboard
	///	\param	names - list with state machine's object names
	///	\return	true if objects were copied to clipboard
	bool Copy( const list<CString> &names ) const;
	
	///	\brief	copy objects to clipboard. State relations are not saved
	///	\param	names - list with state machine's object names
	///	\return	true if value was copied to clipboard
	//bool CopyFlat( const list<IHashString *> &names );

	///	\brief	paste objects from clipboard
	///	\param	pParentName - name of the state machine or single state to paste
	///	\param	object - list with created objects
	///	\return	true if objects were pasted from the clipboard
	bool Paste( IHashString *pParentName, vector<ObjectInfo> &objects ) const;

	///	report clipboard content availability. 
	///	This function checks clipboard format only
	///	\return	true if clipboard contains content for state machine
	bool CanPaste() const;

	///	report clipboard content availability. This function checks whole clipboard 
	///	content. It may return false if some objects cannot be pasted. i.e. events
	///	cannot be pasted as children of state machine object
	///	\param	pParentName - name of object to paste
	///	\return	true if whole clipboard content cannot be pasted for passed parent
	bool CanPastePartially( IHashString *pParentName ) const;

	///	\brief	check passed clipboard archive for correctness
	///	\param	pArchive - clipboard data archive
	///	\return	true if archive is valid
	bool IsValidArchive( IXMLArchive *pArchive ) const;

	///	\brief	create EE object from passed archive
	///	\param	pParentName - name of object to paste
	///	\param	pArchive - archive with object data
	///	\param	object - list with created objects
	///	\return	true if objects were created successfully
	bool CreateObjects( IHashString *pParentName, IXMLArchive *pArchive, vector<ObjectInfo> &objects ) const;

private:

	IEEStream *CreateMemoryStream() const;

	bool SetClipboardDataFromStream( IEEStream *pStream ) const;
	IXMLArchive *GetClipboardDataArchive() const;

	void DumpStream( IXMLArchive *pArchive, LPCTSTR szFilePath ) const;

private:
	IToolBox *m_pToolBox;
	const UINT m_ClipboardFormat;
	// clipboard owner window handle
	HWND m_hOwner;
};

#endif // _STATE_MACHINE_CLIPBOARD_H_