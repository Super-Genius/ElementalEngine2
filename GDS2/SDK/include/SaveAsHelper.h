///============================================================================
/// \file		SaveAsHelper.h
/// \brief		Header file for <insert brief description here>
/// \date		08-08-2005
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

#ifndef _SAVEASHELPER_H_
#define _SAVEASHELPER_H_

/// SaveFileAsByExtension - initiates save as dialog
/// \param extenstion information in format
///		_T("Database XML Files (*.dxl)\0" "*.dxl\0"	"\0\0"));
///	\return TCHAR *fileName or NULL for canceled
TCHAR *SaveFileAsByExtension(TCHAR *extensionInfo);

#endif // #ifndef _SAVEASHELPER_H_
