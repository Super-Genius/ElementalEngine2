///============================================================================
/// \file	CShaderPath.h
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

#ifndef CSHADERPATH_H
#define CSHADERPATH_H

#include "CShaderDefines.h"

#define SHADERTYPEMAX 2
#define VERTEXSHADERTYPE 0
#define PIXELSHADERTYPE 1

class CShaderPath : public CObjectTemplate<IShaderPath >
{	
	/// \brief The Default Constructor for CShaderPath
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CShaderPath(IHashString *parentName, IHashString *name);
	static map< DWORD, DWORD > gs_FlagHash;
	static void HashInitialize();
public:

	/// IShaderPath functions
	virtual bool Reload();
	virtual SHADERMATRIX* GetVertexShaderMatrix() { return &m_ShaderMatrix[VERTEXSHADERTYPE]; }
	virtual SHADERMATRIX* GetPixelShaderMatrix() { return &m_ShaderMatrix[PIXELSHADERTYPE]; }
	virtual SHADERENTRYMAP* GetVertexShaderEntries() { return &m_ShaderEntries[VERTEXSHADERTYPE]; }
	virtual SHADERENTRYMAP* GetPixelShaderEntries() { return &m_ShaderEntries[PIXELSHADERTYPE]; }
	virtual SHADERENTRY* GetVertexShaderMatch( SHADER_RENDER_FLAG flags );
	virtual SHADERENTRY* GetPixelShaderMatch( SHADER_RENDER_FLAG flags );
	/// end IShaderPath functions

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
	SHADERENTRY* GetShaderMatch( int type, SHADER_RENDER_FLAG flags );
	SHADER_RENDER_FLAG GetBestMatch( SHADER_RENDER_FLAG flags, int shadertype );
	void RecursiveShaderLoad( SHADERENTRY *pShaderEntry, DWORD iDefineCount );

	SHADERENTRYMAP m_ShaderEntries[ SHADERTYPEMAX ];

	typedef map<DWORD, CShaderDefines*> SHADERDEFINESMAP;
	SHADERDEFINESMAP m_ShaderDefinesMap;

	vector< SHADER_RENDER_FLAG > m_OriginalPaths[ SHADERTYPEMAX ]; 
	SHADERMATRIX m_ShaderMatrix[ SHADERTYPEMAX ];
	multimap< StdString, DWORD > g_SerializeData[ SHADERTYPEMAX ];
	vector< StdString > m_Effects;
	StdString	m_API;
	StdString	m_ShaderVersion;
	StdString	m_Version;
};

#endif