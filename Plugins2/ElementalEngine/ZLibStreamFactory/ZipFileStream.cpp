///============================================================================
/// \file		ZipFileStream.cpp
/// \brief		Implementation of CZipFileStream. 
/// \date		10/28/2007
/// \author		Andrey Ivanov
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
#include "StdAfx.h"
#include "ZipFileStream.h"

namespace ElementalEngine
{
/*
class CPHYSFSJanitor
{
public:
	CPHYSFSJanitor()
		: m_bInitialized(true)
	{
		IToolBox *pToolBox = EngineGetToolBox();
		if (!PHYSFS_init(NULL))
		{
			m_bInitialized = false;
			StdString sMessage(PHYSFS_getLastError());
			pToolBox->Log(LOGERROR, _T("Failed to intialize PhysFS: %s\n"), sMessage.c_str());
		}
		else
		{
			StdString sResourcePath;
			pToolBox->GetDirectories(NULL, &sResourcePath);
			if (!sResourcePath.empty())
			{
				if (!PHYSFS_addToSearchPath(sResourcePath.c_str(), 1))
				{
					StdString sMessage(PHYSFS_getLastError());
					pToolBox->Log(LOGERROR, _T("Failed to set search dir for PhysFS: %s\n"), sMessage.c_str());
				}
				if (!PHYSFS_setWriteDir(sResourcePath.c_str()))
				{
					StdString sMessage(PHYSFS_getLastError());
					pToolBox->Log(LOGERROR, _T("Failed to set write dir for PhysFS: %s\n"), sMessage.c_str());
				}
			}
			else
			{
				pToolBox->Log(LOGWARNING, _T("Directories for PhysFS were not initalized\n"));
			}
		}
	}

	~CPHYSFSJanitor()
	{
		if (!PHYSFS_deinit())
		{
			StdString sMessage(PHYSFS_getLastError());
			EngineGetToolBox()->Log(LOGERROR, _T("Failed to deintialize PhysFS: %s\n"), sMessage.c_str());
		}
	}

	bool IsInitalized() const
	{
		return m_bInitialized;
	}

private:
	bool m_bInitialized;
};
*/

CZipFileStream::CZipFileStream()
	: m_dwMode(0)
	, m_iReadLength(0)
	, m_iOffset(0)
	, m_file(NULL)
{
}

CZipFileStream::~CZipFileStream()
{
	Close();
}

/// \brief	Initialize memory input stream
/// \param	path - path in physfs
/// \param mode = STREAM_MODE_READING or STREAM_MODE_WRITING
/// \return	true if stream is initialized
bool CZipFileStream::Init(const char *filename, DWORD mode)
{
//	static CPHYSFSJanitor janitor;
//	if (!janitor.IsInitalized())
//	{
//		return false;
//	}

	m_filename = filename;
	// checking stream mode
	mode &= (STREAM_MODE_READ | STREAM_MODE_WRITE);
	if (0 == mode)
	{
		assert(!"CZipFileStream can be created with STREAM_MODE_READ or STREAM_MODE_WRITE mode");
		StdString sPath = filename;
		EngineGetToolBox()->Log(LOGERROR, _T("Failed to create PhysFS stream: %s\n"), sPath.c_str());
		return false;
	}

	if ((STREAM_MODE_READ | STREAM_MODE_WRITE) == mode)
	{
		assert(!"CZipFileStream cannot be created for both reading and writing");
		StdString sPath = filename;
		EngineGetToolBox()->Log(LOGERROR, _T("Failed to create PhysFS stream: %s\n"), sPath.c_str());
		return false;
	}

	StdString szFileName = SetPathDrive( filename, _T("") );
	szFileName.MakeSafeFileName();
	szFileName.ConformSlashesForward();
		
	if (mode & STREAM_MODE_READ)
	{
		m_file = PHYSFS_openRead(szFileName.c_str());
	}
	else
	{
		m_file = PHYSFS_openWrite(szFileName.c_str());
	}

	if (NULL == m_file)
	{
		ReportPhysFSError();
		return false;
	}

	m_dwMode = mode;

	return true;
}

DWORD CZipFileStream::SeekTo(DWORD offset, int aSeek /*= SEEK_SET*/)
{
	assert(NULL != m_file);
	PHYSFS_uint64 newOffset = 0;
	// determine new absolute offset in the stream
	switch (aSeek)
	{
		case SEEK_SET:
			newOffset = offset;
			break;
		case SEEK_CUR:
			{
				PHYSFS_sint64 pos = PHYSFS_tell(m_file);
				if (-1 == pos)
				{
					ReportPhysFSError();
					return (DWORD)pos;
				}
				newOffset = pos + offset;
			}
			break;
		case SEEK_END:
			{
				PHYSFS_sint64 pos = PHYSFS_fileLength(m_file);
				if (-1 == pos)
				{
					ReportPhysFSError();
					return (DWORD)pos;
				}
				newOffset = pos + offset;
			}
			break;
		default:
			assert(!"Unsupported direction");
	}

	if (!PHYSFS_seek(m_file, newOffset))
	{
		ReportPhysFSError();
	}

	return (DWORD)newOffset;
}

bool CZipFileStream::IsReading()
{
	return ((m_dwMode & STREAM_MODE_READ) == STREAM_MODE_READ);
}

bool CZipFileStream::IsWriting()
{
	return ((m_dwMode & STREAM_MODE_WRITE) == STREAM_MODE_WRITE);
}

size_t CZipFileStream::Read(void* buffer, size_t len)
{
	assert(IsReading());
	assert(NULL != m_file);
	PHYSFS_sint64 count = PHYSFS_read(m_file, buffer, 1, (PHYSFS_uint32)len);
	if (-1 == count)
	{
		ReportPhysFSError();
		return 0;
	}
	return (size_t)count;
}

size_t CZipFileStream::Write(const void* buffer, size_t len)
{
	assert(IsWriting());
	assert(NULL != m_file);
	PHYSFS_sint64 count = PHYSFS_write(m_file, buffer, 1, (PHYSFS_uint32)len);
	if (-1 == count)
	{
		ReportPhysFSError();
		return 0;
	}
	return (size_t)len;
}

void CZipFileStream::Close()
{
	m_dwMode = 0;
	if (m_file != NULL)
	{
		PHYSFS_close(m_file);
		m_file = NULL;
	}
	m_filename.clear();
}

bool CZipFileStream::IsEndOfFile()
{
	assert(NULL != m_file);
	return 0 != PHYSFS_eof(m_file);
}

long CZipFileStream::Tell()
{
	assert(NULL != m_file);
	PHYSFS_sint64 pos = PHYSFS_tell(m_file);
	if (-1 == pos)
	{
		ReportPhysFSError();
	}
	return (long)pos;
}


///	\brief	add search path to physfs. This affects streams opened for reading
///	\param	path - path to file system object. It may refer to folder, zip file
///				   or other types of archives (grp, hog, mvl, qpak, wad)
///	\return	true if path was added successfully
bool CZipFileStream::AddSearchPath(const char *path)
{
	if (!PHYSFS_addToSearchPath(path, 1))
	{
		StdString sPath(path);
		StdString sMessage(PHYSFS_getLastError());
		EngineGetToolBox()->Log(LOGERROR, _T("Failed to register search path '%s' in PhysFS error: %s\n"), sPath.c_str(), sMessage.c_str());
		return false;
	}
	return true;
}

/// checks if a file exists
/// \param path - path to file
/// \return bool true if found, else returns false
bool CZipFileStream::CheckFileExist(const char *path)
{
	StdString szFilePath = SetPathDrive( path , _T("") );
	szFilePath.MakeSafeFileName();
	szFilePath.ConformSlashesForward();

	if( PHYSFS_exists( szFilePath ) )
	{
		return true;
	}
	return false;
}

///	\breif	report PhysFS error to EE
void CZipFileStream::ReportPhysFSError()
{
	StdString sMessage(PHYSFS_getLastError());
	EngineGetToolBox()->Log(LOGINFORMATION, _T("PhysFS stream '%s' error: %s\n"), m_filename.c_str(), sMessage.c_str());
}

} //namespace ElementalEngine
