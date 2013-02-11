///============================================================================
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

#include "stdafx.h"
//#include <LibXML++/libxml++.h>
#include "LightmapCacheLoader.h"
//#include "PerformanceProfiler.h"

REGISTER_COMPONENT_SINGLETON(LightmapCacheLoader);
REGISTER_MESSAGE_HANDLER(LoadFile, OnLoadLMPFile, LightmapCacheLoader);
REGISTER_MESSAGE_HANDLER(SaveFile, OnSaveLMPFile, LightmapCacheLoader);

CHashString LightmapCacheLoader::m_HashName(_T("LightmapCacheLoader"));

LSRC_MESSAGE LightmapCacheLoader::m_LoadSaveRegisterData = {
		&LightmapCacheLoader::m_HashName,
		_T("Lightmap Cache Files (*.lmp)\0" "*.lmp\0"
		"\0\0"),
		_T("Lightmap Cache Files (*.lmp)\0" "*.lmp\0"
		"\0\0"), 
		false, NULL,
};

LightmapCacheLoader::LightmapCacheLoader()
{
	m_ToolBox = EngineGetToolBox();

	DWORD result;
	static DWORD msgHash_RegisterLoadSaveComponent = CHashString(_T("RegisterLoadSaveComponent")).GetUniqueID();
	result = m_ToolBox->SendMessage(msgHash_RegisterLoadSaveComponent, sizeof(LSRC_MESSAGE),
		&m_LoadSaveRegisterData, NULL, NULL);
	if (result != MSG_HANDLED)
		m_ToolBox->Log(LOGERROR, _T("%s(%d): Failed to register Load Save Extension"));

}

// create function for this Particle loader
IComponent *LightmapCacheLoader::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE( LightmapCacheLoader );
}

IHashString *LightmapCacheLoader::GetComponentType()
{
	return &m_HashName;
}

bool LightmapCacheLoader::IsKindOf(IHashString *compType)
{
	return (compType->GetUniqueID() == GetComponentType()->GetUniqueID());
}

//Check if the file is UNICODE
//for debugging purposes only.
#if 0
int IsUnicodeFile(char* szFileName)
{
    FILE *fpUnicode;
    char l_szCharBuffer[80];

    //Open the file
    if((fpUnicode= fopen(szFileName,"r")) == NULL)
    return 0; //Unable to open file

    if(!feof(fpUnicode))
    {
        fread(l_szCharBuffer,80,1,fpUnicode);
        fclose(fpUnicode);
        if(IsTextUnicode(l_szCharBuffer,80,NULL))
        {
            return 2; //Text is Unicode
        }
       else
       {
           return 1; //Text is ASCII
       }
    }
    return 0; // Some error happened
}
#endif

/////////////////////////////////////////////////////////////
//	LOAD the cache
/////////////////////////////////////////////////////////////
DWORD LightmapCacheLoader::OnLoadLMPFile(DWORD size, void *params)
{
//	PERFORMANCE_PROFILER_AUTO(Cal3dLoader_loadfile);

	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFilepath = (TCHAR *)params;

	StdString szFilename = ExtractFileName(pFilepath);
	CHashString hszFilename(szFilename);

	StdString szFilepath(pFilepath);
	CHashString hszFilepath(pFilepath);

	// create a new Memory Archive
	IArchive *MemArchive;
	CHashString streamType(_T("File"));
	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_READ | STREAM_MODE_BINARY;
	ca.streamData = pFilepath;
	ca.streamType = &streamType;
	DWORD retVal;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	if (retVal = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
	{
		return MSG_ERROR;
	}
	MemArchive = (ca.archive);


	TCHAR buf[_MAX_PATH];
	_tcscpy( buf, (TCHAR*)pFilepath );
	TCHAR * lwr = buf ;
	CHashString modelname( lwr );
	StdString szModelname( lwr );

	// only load an effect file once
	static DWORD msgHash_FindObject = CHashString(_T("FindObject")).GetUniqueID();
	FINDOBJECTPARAMS param;
	param.hszName = &modelname;
	m_ToolBox->SendMessage(msgHash_FindObject, sizeof(FINDOBJECTPARAMS), &param );
	if ( param.bFound == true )
	{
		MemArchive->Close();
		// effect already loaded
		return MSG_HANDLED_STOP;
	}
	
	MemArchive->SetIsWriting( false );
	MemArchive->SeekTo(0);

	LightmapCache cache;
	MemArchive->Read( cache.m_NumVertices, _T("NumVertices"));
	MemArchive->Read( cache.m_NumIndices, _T("NumIndices"));
	int bRemapped = 0;
	MemArchive->Read( bRemapped, _T("IsRemapped"));
	cache.m_bRemapped = bRemapped == 0 ? false : true;

	ResizeCacheMessage ResizeMsg;
	ResizeMsg.cache = &cache;
	ResizeMsg.numUVData = cache.m_NumVertices*2;
	if( cache.m_bRemapped )
	{
		ResizeMsg.numRemappedData =  cache.m_NumVertices;
		ResizeMsg.numRemappedIndices = cache.m_NumIndices;
	} 
	static DWORD resizeCacheMsg = CHashString(_T("ResizeLightmapCache")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(resizeCacheMsg, sizeof( ResizeCacheMessage ), &ResizeMsg );
	if( retval != MSG_HANDLED )
	{
		m_ToolBox->Log( LOGERROR, _T("Error, could not resize lightmap cache, manager not available" ) );
		return MSG_ERROR;
	}

	cache.m_Hash = modelname.GetUniqueID();
	unsigned int iVal;
	for( int i = 0; i < cache.m_NumVertices*2; i++ )
	{
		MemArchive->Read( cache.m_UVData[ i ] );
	}
	if( cache.m_bRemapped )
	{
		for( int i = 0; i < cache.m_NumIndices; i++ )
		{
			MemArchive->Read( iVal );
			cache.m_RemappedIndices[ i ] = iVal;
		}
		for( int i = 0; i < cache.m_NumVertices; i++ )
		{
			MemArchive->Read( iVal );
			cache.m_RemappedData[ i ] = iVal;
		}
	}
	static DWORD msgHash_RegisterLightmapCache = CHashString(_T("RegisterLightmapCache")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_RegisterLightmapCache, sizeof( cache ), &cache );
	MemArchive->Close();
	return MSG_HANDLED_STOP;
}

/////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////
DWORD LightmapCacheLoader::OnSaveLMPFile(DWORD size, void *params)
{
	VERIFY_MESSAGE_SIZE(size, sizeof(TCHAR *));
	TCHAR *pFilepath = (TCHAR *)params;

	StdString szFilename = ExtractFileName(pFilepath);
	CHashString hszFilename(szFilename);

	StdString szFilepath(pFilepath);
	CHashString hszFilepath(pFilepath);
	
	LightmapCache cache;
	cache.m_Hash = hszFilepath.GetUniqueID();
	static DWORD msgHash_GetLightmapCache = CHashString(_T("GetLightmapCache")).GetUniqueID();
	DWORD retval = m_ToolBox->SendMessage(msgHash_GetLightmapCache, sizeof( cache ), &cache );
	if( retval == MSG_HANDLED )
	{
		//save it off
		// create a new Memory Archive
		IArchive *MemArchive;
		CHashString streamType(_T("File"));
		CREATEARCHIVE ca;
		ca.mode = STREAM_MODE_WRITE | STREAM_MODE_BINARY;
		ca.streamData = pFilepath;
		ca.streamType = &streamType;
		DWORD retVal;
		static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
		if (retVal = m_ToolBox->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca) != MSG_HANDLED)
		{
			return MSG_ERROR;
		}
		MemArchive = (ca.archive);
		MemArchive->SetIsWriting( true );
		MemArchive->SeekTo(0);

		MemArchive->Write( cache.m_NumVertices );
		MemArchive->Write( cache.m_NumIndices );
		int bRemapped = cache.m_bRemapped == false ? 0 : 1;
		MemArchive->Write( bRemapped );
		unsigned int iVal;
		for( int i = 0; i < cache.m_NumVertices*2; i++ )
		{
			MemArchive->Write( cache.m_UVData[ i ] );
		}
		if( bRemapped )
		{
			for( int i = 0; i < cache.m_NumIndices; i++ )
			{
				iVal = cache.m_RemappedIndices[ i ];
				MemArchive->Write( iVal  );
			}
			for( int i = 0; i < cache.m_NumVertices; i++ )
			{
				iVal = cache.m_RemappedData[ i ];
				MemArchive->Write( iVal  );
			}
		}
		MemArchive->Close();
		return MSG_HANDLED_STOP;
	}
	return MSG_ERROR;
}