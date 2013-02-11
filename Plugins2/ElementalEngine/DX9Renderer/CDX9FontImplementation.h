///============================================================================
/// \file	CDX9FontImplementation.h
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

#ifndef DX9_FONTIMPLEMENTATION_H
#define DX9_FONTIMPLEMENTATION_H

class CDX9FontImplementation : public IFontImplementation
{
	static IFontImplementation * s_instance;
public:
	virtual IFontObject * CreateFont();
	virtual void DestroyFont( IFontObject * );
	virtual int GetFontPixWidth(IHashString *fontFace, UINT fontsize, const TCHAR* text);
	static IFontImplementation * Instance();
	static void Destroy();
};
#endif