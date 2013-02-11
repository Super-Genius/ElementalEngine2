///============================================================================
/// \file		ReturnEdit.h
/// \brief		Edit control that loses focus when enter is pressed
/// \date		06/11/2007
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

#ifndef _C_RETURNEDIT_H_
#define _C_RETURNEDIT_H_

class CReturnEdit : public CEdit
{
public:
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
};

#endif //_C_RETURNEDIT_H_