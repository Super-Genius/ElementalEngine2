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

#ifndef CDX9SHADERCOMMON_H
#define CDX9SHADERCOMMON_H

struct D3D9SamplerStates
{
	DWORD	m_Filter;
	DWORD	m_UMode;
	DWORD	m_VMode;
	D3D9SamplerStates()
	{
		m_Filter = D3DTEXF_POINT;
		m_UMode = D3DTADDRESS_WRAP;
		m_VMode = D3DTADDRESS_WRAP;
	}
};

#ifdef WIN32
class CDX9IncludeManager : public ID3DXInclude 
{
public:
	STDMETHOD(Open)(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
	STDMETHOD(Close)(LPCVOID pData);
};
#endif

bool GetStringFromFile( StdString &filename, StdString &outstring );
BOOL LoadFileIntoBuffer( const TCHAR* pFileName, LPVOID& pFileData, UINT& iFileDataSize, bool bBinary );

#endif