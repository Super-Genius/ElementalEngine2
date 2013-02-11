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
#include "Cal3dUtil.h"

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

bool BuildObject( IHashString *pObjectName, IHashString *pComponentType, IArchive *pArchive )
{
	CREATEOBJECTPARAMS cop;
	cop.name = pObjectName;
	cop.parentName = NULL;
	cop.typeName = pComponentType;
	static DWORD msgHash_CreateObject = CHashString(_T("CreateObject")).GetUniqueID();
	if (EngineGetToolBox()->SendMessage(msgHash_CreateObject, sizeof(cop), &cop) != MSG_HANDLED)
		return false;

	SERIALIZEOBJECTPARAMS sop;
	sop.name = pObjectName;
	sop.archive = pArchive;
	static DWORD msgHash_SerializeObject = CHashString(_T("SerializeObject")).GetUniqueID();
	if (EngineGetToolBox()->SendMessage(msgHash_SerializeObject, sizeof(sop), &sop) != MSG_HANDLED)
		return false;

	INITOBJECTPARAMS iop;
	iop.name = pObjectName;
	static DWORD msgHash_InitObject = CHashString(_T("InitObject")).GetUniqueID();
	if (EngineGetToolBox()->SendMessage(msgHash_InitObject, sizeof(iop), &iop) != MSG_HANDLED)
		return false;

	return true;
}