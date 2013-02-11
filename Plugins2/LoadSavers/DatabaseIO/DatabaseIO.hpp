///============================================================================
/// \file		DatabaseIO.hpp
/// \brief		Header file for Lua Script Loader
/// \date		05-31-2005
/// \author		Josh Lulewicz
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

#ifndef _DATABASEIO_H_
#define _DATABASEIO_H_

class CDatabaseIO;

typedef void (CDatabaseIO::*DATARWFUNC)(IArchive *ar, DATABASEDATA *data, TCHAR *tag);
typedef map<DWORD, DATARWFUNC> IDTODATARWMAP;

class CDatabaseIO : public IComponent
{
private:
	SINGLETONCONSTRUCTOROVERRIDE(CDatabaseIO);
	CDatabaseIO();
	/// functions for reading database items
	void DBReadChar(IArchive *ar, DATABASEDATA *data, TCHAR *tag);
	void DBReadUChar(IArchive *ar, DATABASEDATA *data, TCHAR *tag);
	void DBReadShort(IArchive *ar, DATABASEDATA *data, TCHAR *tag);
	void DBReadUShort(IArchive *ar, DATABASEDATA *data, TCHAR *tag);
	void DBReadLong(IArchive *ar, DATABASEDATA *data, TCHAR *tag);
	void DBReadULong(IArchive *ar, DATABASEDATA *data, TCHAR *tag);
	void DBReadFloat(IArchive *ar, DATABASEDATA *data, TCHAR *tag);
	void DBReadDouble(IArchive *ar, DATABASEDATA *data, TCHAR *tag);
	void DBReadBool(IArchive *ar, DATABASEDATA *data, TCHAR *tag);
	void DBReadString(IArchive *ar, DATABASEDATA *data, TCHAR *tag);
	// functions to write database items
	void DBWriteChar(IArchive *ar, DATABASEDATA *data, TCHAR *tag);
	void DBWriteShort(IArchive *ar, DATABASEDATA *data, TCHAR *tag);
	void DBWriteLong(IArchive *ar, DATABASEDATA *data, TCHAR *tag);
	void DBWriteFloat(IArchive *ar, DATABASEDATA *data, TCHAR *tag);
	void DBWriteDouble(IArchive *ar, DATABASEDATA *data, TCHAR *tag);
	void DBWriteBool(IArchive *ar, DATABASEDATA *data, TCHAR *tag);
	void DBWriteString(IArchive *ar, DATABASEDATA *data, TCHAR *tag);

public:

	/// We are the serializer, so we don't do anything with this.
	virtual void Serialize(IArchive &ar) {};

	/// return a string of the component type
	/// \return IHashString *reference to textual classname
	virtual IHashString *GetComponentType();

	/// Checks if this is similar to it's base classes component
	/// \return true if it is simlar, else false
	virtual bool IsKindOf(IHashString *compType);


	/// standard static component create function
	static IComponent *Create(int nArgs, va_list argptr);

	// ------------------ Messages ------------------ //
	/// message to load a lua file
	DWORD OnLoadDatabaseFile(DWORD size, void *params);
	/// message to save a lua file
	DWORD OnSaveDatabaseFile(DWORD size, void *params);

private:
	static CHashString m_HashName;
	IToolBox *m_ToolBox;
	static LSRC_MESSAGE m_LoadSaveRegisterData;
	IDTODATARWMAP m_DataReadFunctions;
	IDTODATARWMAP m_DataWriteFunctions;
};

#endif	// #ifndef _XMLWORLDLOADER_H_
