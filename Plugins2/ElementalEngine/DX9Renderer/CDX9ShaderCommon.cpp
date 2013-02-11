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
#include <fcntl.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef WIN32
HRESULT CDX9IncludeManager::Open(D3DXINCLUDE_TYPE IncludeType,
			LPCSTR pName, 
			LPCVOID pParentData, 
			LPCVOID *ppData,
			UINT *pBytes
			)
{
	StdString szPath = pName;

	struct _stat filestat;
	int fd;
	fd = _open( szPath, _O_RDONLY  | _O_BINARY );
	if( fd == -1 )//error
	{
		return E_FAIL;
	}
	_fstat( fd, &filestat );
	UINT size = filestat.st_size;
	BYTE* pData = new BYTE[size + 1];
	if( pData == NULL )
		return E_OUTOFMEMORY;
	_read( fd, (void*)pData, size );
	pData[ size ] = NULL;
	*ppData = pData;
	*pBytes = size;
	_close( fd );
	return S_OK;
}

HRESULT CDX9IncludeManager::Close(LPCVOID pData)
{
	BYTE* pData2 = (BYTE*)pData;
	SAFE_DELETE_ARRAY( pData2 );
	return S_OK;
}
#endif//WIN32
