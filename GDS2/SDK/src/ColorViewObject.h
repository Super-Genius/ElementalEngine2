///============================================================================
/// \file		ColorViewObject.h
/// \brief		Header file for color object in option tree view
/// \date		08/07/2006
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

#ifndef _COLORVIEWOBJECT_H_
#define _COLORVIEWOBJECT_H_

class CColorViewObject : public IColorViewObject
{
public:
	CColorViewObject();
	CColorViewObject(StdString &name, StdString &basicType);
	virtual ~CColorViewObject();

	///	Creates a color item and adds it to the tree
	/// \param	ar - the xml archive to read data from
	/// \return	ViewFormatObject* - returns a new ViewFormatObject
	static ViewFormatObject* CreateFormatObject(IXMLArchive &ar);

	/// Sets the default color upon first loading
	/// \param iDefColor - color to set to, packed rgb format
	void SetDefaultColor(int iDefColor)
	{
		m_iDefColor = iDefColor;
	}

	/// Gets the defualt color
	/// \return int color, packed rgb format
	int GetDefaultColor()
	{
		return m_iDefColor;	
	}

private:
	int m_iDefColor;

};

#endif // #ifndef _SPINNERVIEWOBJECT_H_
