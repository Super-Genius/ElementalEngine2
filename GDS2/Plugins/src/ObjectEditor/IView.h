///============================================================================
/// \file		IView.h
/// \brief		Interface for the View
///				Class that displays data for an object
/// \date		06-08-2005
/// \author		Halbert Nakagawa
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

#ifndef IVIEW_H
#define IVIEW_H

/// \Interface	IView
/// \brief		Interface for the View

class IView
{

public:
	//IView() = 0;
	virtual ~IView() {}

	/// Initializes the view
	/// \param	*fileName - the xml file to setup the view
	virtual void Init(const StdString &fileName) = 0;

	/// Deinitializes the view
	virtual void DeInit() = 0;

	/// Updates the view with a different object type
	/// \param  *viewName - IHashString ptr to the name of the view
	/// \param	*objectType - IHashString ptr to the type of object
	virtual bool Update(IHashString *viewName, IHashString *objectType) = 0;

	/// Serialize the view data
	/// \param	ar - The archive to write the data to
	/// \param	bRead - if true, then read, else write
	virtual void Serialize(IArchive &ar, bool bRead = false) = 0;

	/// Returns the number of Entries for the View
	/// \return int - number of entries
	virtual int GetNumEntries( IHashString *compType ) = 0;

protected:

	/// the name of this view
	StdString	m_viewName;

};

#endif