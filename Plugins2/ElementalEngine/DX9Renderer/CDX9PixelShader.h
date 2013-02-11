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

#ifndef CDX9PIXELSHADER_H
#define CDX9PIXELSHADER_H


class CPixelShader : public CObjectTemplate<IShader >
{
	CPixelShader(IHashString *parentName, IHashString *name);
public:
	~CPixelShader();
	virtual bool Apply( IShaderDefines *pDefines );
	virtual bool Reload( IShaderDefines *pDefines );
	virtual bool LoadVariant( IShaderDefines *pDefines, bool bCompile );
	virtual bool SetConstant( DWORD hash, float * data, int num );
	virtual bool SetConstant( DWORD hash, float x, float y, float z, float w );

public:
	/// IObject  functions
	/// \brief	Create the a instance of IObject
	/// \return	IComponent pointer
	static IComponent *Create(int nArgs, va_list argptr);
	/// \brief	The Initialization for the Object
	/// \param	IManager* pManager: Parent Manager
	virtual void Init();
	/// \brief	The Update Function for the Object
	/// \return	True on sucess, False on faulure
	virtual bool Update();
	/// \brief	The DeInit Function for the Object
	virtual void DeInit();
	/// \brief either reads or writes the entity information to a file
	virtual void Serialize(IArchive &ar);

private:
	void ReleaseShader();

	StdString						m_Code;
	
	LPDIRECT3DPIXELSHADER9			m_PixelShader;
	LPD3DXCONSTANTTABLE				m_PixelShaderConstTable; //release this after
	IRenderer	* m_Renderer;
	typedef list<unsigned long> SHADERVARIANTSLIST;
	typedef map<unsigned long, LPDIRECT3DPIXELSHADER9> SHADERVARIANTSMAP;
	SHADERVARIANTSMAP m_ShaderVariants;
	bool m_bVariantMissingOneTimeWarning;
};
#endif