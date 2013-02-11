///============================================================================
/// \file		StateLoader.h
/// \brief		Header file for Defined Database Loader
/// \date		04-18-2007
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

#ifndef _STATELOADER_H_
#define _STATELOADER_H_

class CStateLoader : public LoadSaverBase
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CStateLoader);
	CStateLoader();

public:
	// Implementation of GetLoadDescription from LoadSaverBase
	const TCHAR *GetLoadDescription();

	// Implementation of GetSaveDescription from LoadSaverBase
	const TCHAR *GetSaveDescription();

	// Implementation of GetPrecachableBool from LoadSaverBase
	bool GetPrecachableBool();

	// Implementation of GetPrecachableFunc from LoadSaverBase
	CACHABLEQUERYCALLBACK GetPrecachableFunc();

	/// standard static component create function
	static IComponent *Create(int nArgs, va_list argptr);

protected:
	virtual DWORD SaveFile( IXMLArchive *ar, IHashString *fileName );

	virtual void GenerateObjectName( IHashString *objectName, IHashString *nodeType, IXMLArchive *archive );

	virtual void CreateObject( IXMLArchive * ar, IHashString *parentName, 
							   IHashString *type, IHashString *objectName );
private:

private:
	CHashString	m_hszStateMachineName;
	Hash2Hash m_Label2State;
};

#endif	// #ifndef _STATELOADER_H_