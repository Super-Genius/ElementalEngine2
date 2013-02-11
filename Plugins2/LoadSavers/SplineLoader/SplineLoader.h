///============================================================================
/// \file		SplineLoader.h
/// \brief		Loader for spline data files
/// \date		11-14-2006
/// \author		Richard Nguyen
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

#ifndef SPLINELOADER_H
#define SPLINELOADER_H

class CSplineLoader : public LoadSaverBase
{
private:
	SINGLETONCONSTRUCTOROVERRIDE( CSplineLoader );
	CSplineLoader();

public:
	/// standard static component create function
	static IComponent *Create( int nArgs, va_list argptr );

protected:
	// Implementation of LoadFile from LoadSaverBase
	DWORD LoadFile( const TCHAR *fileName, IArchive *archive );

	// Implementation of SaveFile from LoadSaverBase
	DWORD SaveFile( const TCHAR *fileName, IArchive *archive );

	// Implementation of GetLoadDescription from LoadSaverBase
	const TCHAR *GetLoadDescription();

	// Implementation of GetSaveDescription from LoadSaverBase
	const TCHAR *GetSaveDescription();

	// Implementation of GetPrecachableBool from LoadSaverBase
	bool GetPrecachableBool();

	// Implementation of GetPrecachableFunc from LoadSaverBase
	CACHABLEQUERYCALLBACK GetPrecachableFunc();

private:
	DWORD LoadXMLFile( const TCHAR *fileName, IXMLArchive *archive );
	DWORD ReadSplineNode( const TCHAR *fileName, IXMLArchive *archive );
	DWORD ReadSplinePointNode( const TCHAR *fileName, IXMLArchive *archive, IHashString *splineObjectName );

	// Creates, serializes, and initializes the spline into the system
	bool CreateSpline( TCHAR *builtName );

	void BuildName( TCHAR *buffer, StdString szName );
};

#endif //#ifndef SPLINELOADER_H