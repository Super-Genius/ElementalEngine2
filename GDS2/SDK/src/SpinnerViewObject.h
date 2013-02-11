///============================================================================
/// \file		SpinnerViewObject.h
/// \brief		Header file for spinner object in a view
/// \date		08-03-2005
/// \author		Kenneth Hurley
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

#ifndef _SPINNERVIEWOBJECT_H_
#define _SPINNERVIEWOBJECT_H_

class CSpinnerViewObject : public ISpinnerViewObject
{
public:
	CSpinnerViewObject();
	CSpinnerViewObject(StdString &name, StdString &basicType);
	virtual ~CSpinnerViewObject();

	/// \brief	Creates a spinner item and adds it to the tree
	///			Will also create and return ViewFormatObject containing 
	///			formatting data for the created option tree item.
	/// \param	ar - the xml archive to read data from
	/// \return	ViewFormatObject* - returns a new ViewFormatObject
	static ViewFormatObject* CreateFormatObject(IXMLArchive &ar);

	/// Set the min, max range, and increment value for the spinner
	/// \param fMin - the minimum range
	/// \param fMax - the maximum range
	/// \param fIncVal - the increment value
	void SetSpinnerFormat(float fMin, float fMax, float fIncVal);

	/// Get the minimum range
	float GetMin();

	/// Get the maximum range
	float GetMax();

	/// Get the increment value
	float GetIncVal();

	/// Sets the default value
	void SetDefaultValue(float value) { m_fDefaultVal = value; }

	/// Gets the default value
	float GetDefaultValue() { return m_fDefaultVal; }

private:
	float	m_fMin;
	float	m_fMax;
	float	m_fIncVal;
	float	m_fDefaultVal;

};

#endif // #ifndef _SPINNERVIEWOBJECT_H_
