///============================================================================
/// \file		BrushObject.h
/// \brief		Declaration of the CBrushObject Class
/// \date		07-07-2005
/// \author		Patrick Ghiocel
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

#ifndef _CBRUSHOBJECT_H_
#define _CBRUSHOBJECT_H_

/// CBrushObject: Brushes for Terrain Editing
class CBrushObject 
{
public:
	CBrushObject( StdString wszName, StdString wszFileName, StdString wszOperator ) ;
	~CBrushObject();

	void Destroy();
	
	/// Bitmap Icon
	CBitmap* m_pBitmap;

	/// Brush Raw DATA
	float *m_pData;
	/// Brush Size
	int m_iSize;

	/// Brush Name
	StdString m_wszName;

	/// Filename of Bitmap to base other things off of
	StdString m_wszFileName;

	/// Operator
	StdString m_wszOperator;
private:
	static ITextureObject* LoadTexture(LPCTSTR path);

};

#endif	// #ifndef _CBRUSHOBJECT_H_
