///============================================================================
/// \file		iSpinnerViewObject.h
/// \brief		Header file for spinner object in a view interface
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

#ifndef _ISPINNERVIEWOBJECT_H_
#define _ISPINNERVIEWOBJECT_H_

class ISpinnerViewObject : public ViewFormatObject
{
public:
	virtual ~ISpinnerViewObject() {}

	/// Set the min, max range, and increment value for the spinner
	/// \param fMin - the minimum range
	/// \param fMax - the maximum range
	/// \param fIncVal - the increment value
	virtual void SetSpinnerFormat(float fMin, float fMax, float fIncVal) = 0;

	/// Get the minimum range
	virtual float GetMin() = 0;

	/// Get the maximum range
	virtual float GetMax() = 0;

	/// Get the increment value
	virtual float GetIncVal() = 0;

	/// Sets the default value
	virtual void SetDefaultValue(float value) = 0;

	/// Gets the default value
	virtual float GetDefaultValue() = 0;

};

#pragma comment(linker, "/include:_SpinnerRegistered")
#endif // #ifndef _ISPINNERVIEWOBJECT_H_
