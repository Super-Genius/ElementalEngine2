///============================================================================
/// \file	CShaderDefines.h
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

#ifndef CSHADERDEFINES_H__
#define CSHADERDEFINES_H__

#define INVALID_SHADERDEFINE_INDEX (DWORD)0xFFFFFFFF

class CShaderDefines : public IShaderDefines
{
public:
	CShaderDefines();

	virtual void SetValuesFromEncoding( const SHADERDEFINESENCODING &encoding );
	virtual SHADERDEFINESENCODING GetValuesEncoding() const;
	virtual DWORD GetDefineCount() const;
	virtual DWORD GetDefineIndex( const TCHAR *szDefineName ) const;
	virtual const char* GetDefineName( DWORD iDefineIndex ) const;
	virtual void GetDefineRange( DWORD iDefineIndex, int &out_iValueMin, int &out_iValueMax ) const
	{
		assert( iDefineIndex != INVALID_SHADERDEFINE_INDEX && iDefineIndex < m_ShaderDefines.size() );
		if (iDefineIndex != INVALID_SHADERDEFINE_INDEX)
		{
			out_iValueMin = m_ShaderDefines[iDefineIndex].iValueMin;
			out_iValueMax = m_ShaderDefines[iDefineIndex].iValueMax;
		}
	}
	virtual int GetDefineValue( DWORD iDefineIndex ) const
	{
		assert( iDefineIndex != INVALID_SHADERDEFINE_INDEX && iDefineIndex < m_ShaderDefines.size() );
		if (iDefineIndex != INVALID_SHADERDEFINE_INDEX)
			return m_ShaderDefines[iDefineIndex].iValue;
		else
			return 0;
	}

	inline void SetDefineValue( SHADERDEFINEENUM eShaderDefine, int iValue )
	{
		if (m_vCachedIndices[eShaderDefine] != INVALID_SHADERDEFINE_INDEX)
			SetDefineValue( m_vCachedIndices[eShaderDefine], iValue );
	}
	virtual void SetDefineValue( DWORD iDefineIndex, int iValue )
	{
		assert( iDefineIndex != INVALID_SHADERDEFINE_INDEX && iDefineIndex < m_ShaderDefines.size() );
		if (iDefineIndex != INVALID_SHADERDEFINE_INDEX)
		{
			assert( iValue <= m_ShaderDefines[iDefineIndex].iValueMax );
			m_ShaderDefines[iDefineIndex].iValue = iValue;
		}
	}

	void LoadShaderDefines( const TCHAR *szShaderDefinesFile );
	void ZeroValues();

private:
	IToolBox *m_ToolBox;
	struct SHADERDEFINE
	{
		StdString szDefineName;
		int iValueMin; // min define value
		int iValueMax; // max define value
		int iValue; // current value of this define
		int iBitStart; // internally used to find encoding
		int iValueBitMask; // internally used to find encoding
	};
	vector<SHADERDEFINE> m_ShaderDefines;

	DWORD m_vCachedIndices[SHADERDEFINE_COUNT];
};

#endif