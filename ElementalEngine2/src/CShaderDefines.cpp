///============================================================================
/// \file	CShaderDefines.cpp
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
#include "CShaderDefines.h"

static MathUtil math;

CShaderDefines::CShaderDefines()
{
	m_ToolBox = EngineGetToolBox();
}

void CShaderDefines::SetValuesFromEncoding( const SHADERDEFINESENCODING &encoding )
{
	for (DWORD i=0; i < m_ShaderDefines.size(); i++)
	{
		m_ShaderDefines[i].iValue = ((encoding.iEncoding >> m_ShaderDefines[i].iBitStart) & m_ShaderDefines[i].iValueBitMask);
	}
}

SHADERDEFINESENCODING CShaderDefines::GetValuesEncoding() const
{
	SHADERDEFINESENCODING encoding;
	encoding.iEncoding = 0;
	for (DWORD i=0; i < m_ShaderDefines.size(); i++)
	{
		encoding.iEncoding |= m_ShaderDefines[i].iValue << m_ShaderDefines[i].iBitStart;
	}
	return encoding;
}

DWORD CShaderDefines::GetDefineCount() const
{
	return static_cast<DWORD>(m_ShaderDefines.size());
}

DWORD CShaderDefines::GetDefineIndex( const TCHAR *szDefineName ) const
{
	for (DWORD i=0; i < m_ShaderDefines.size(); i++)
	{
		if (m_ShaderDefines[i].szDefineName == szDefineName)
		{
			return i;
		}
	}
	return INVALID_SHADERDEFINE_INDEX;
}

const char* CShaderDefines::GetDefineName( DWORD iDefineIndex ) const
{
	assert( iDefineIndex != INVALID_SHADERDEFINE_INDEX && iDefineIndex < m_ShaderDefines.size() );
	if (iDefineIndex != INVALID_SHADERDEFINE_INDEX)
		return m_ShaderDefines[iDefineIndex].szDefineName;
	else
		return "";
}

void CShaderDefines::LoadShaderDefines( const TCHAR *szShaderDefinesFile )
{
	if (StdString(szShaderDefinesFile).empty())
		return;

	// load shader define file
	CHashString streamType(_T("File"));
	CREATEARCHIVE ca;
	ca.mode = STREAM_MODE_READ;
	ca.streamData = const_cast<TCHAR*>(szShaderDefinesFile);
	ca.streamType = &streamType;
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	DWORD hr = m_ToolBox->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &ca);
	if( hr != MSG_HANDLED )
	{
		m_ToolBox->Log(LOGERROR, _T("Failed to load shader define file: %s\n"), szShaderDefinesFile );
		return;
	}
	IArchive *pArchive = ca.archive;

	// check file version
	int iVersion;
	pArchive->Read(iVersion);
	if (iVersion != 1)
	{
		m_ToolBox->Log(LOGERROR, _T("Invalid version in shader define file: %s\n"), szShaderDefinesFile );
		pArchive->Close();
		return;
	}

	// read shader defines
	int iDefineCount;
	pArchive->Read(iDefineCount);
	for (int i=0; i < iDefineCount; i++)
	{
		SHADERDEFINE shaderDefine;
		pArchive->Read( shaderDefine.szDefineName );
		pArchive->Read( shaderDefine.iValueMin );
		pArchive->Read( shaderDefine.iValueMax );

		m_ShaderDefines.push_back( shaderDefine );
	}

	// calculate internal encoding values
	int iBitIndex = 0;
	for (DWORD i=0; i < m_ShaderDefines.size(); i++)
	{
		int iValueMin = m_ShaderDefines[i].iValueMin;
		int iValueMax = m_ShaderDefines[i].iValueMax;
		int iMaxBitValue = math.UpperPowerOfTwo(iValueMax - iValueMin + 1);
		int iBitCount = (int)math.Log2( (float)iMaxBitValue );

		// calculate which bits are used to encode the value
		m_ShaderDefines[i].iValueBitMask = iMaxBitValue - 1;
		m_ShaderDefines[i].iBitStart = iBitIndex;
		iBitIndex += iBitCount;
	}

	// get cached define indices
	m_vCachedIndices[SHADERDEFINE_COMPLEXLIGHTS] = GetDefineIndex( _T("ACTIVELIGHTS") );
	m_vCachedIndices[SHADERDEFINE_SIMPLELIGHTS] = GetDefineIndex( _T("DETAILLIGHTS") );
	m_vCachedIndices[SHADERDEFINE_SHADOWMAPS] = GetDefineIndex( _T("SHADOWMAPS") );
	pArchive->Close();
}

void CShaderDefines::ZeroValues()
{
	for (DWORD i=0; i < m_ShaderDefines.size(); i++)
	{
		m_ShaderDefines[i].iValue = 0;
	}
}
