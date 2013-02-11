///============================================================================
/// \file		LuaScriptVarList.h
/// \brief		Header file for Lua Script Variable List
/// \date		01-29-2007
/// \author		D. Patrick Ghiocel
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

#ifndef LUASCRIPTVARLIST_H
#define LUASCRIPTVARLIST_H

#ifndef	_COBJECTTEMPLATE_HPP
#include "CObjectTemplate.hpp"
#endif

typedef vector<CLuaScriptVar> LUAVARVEC;

class CLuaScriptVarList : public CObjectTemplate<IObject> 
{
protected:
	/// \brief Default Constructor
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CLuaScriptVarList(IHashString *parentName, IHashString *name);

public:
	~CLuaScriptVarList();

	static IComponent *Create(int nArgs, va_list argptr);

	virtual void Serialize(IArchive &ar);
	virtual void Init();
	virtual void DeInit();


	/* MESSAGES */
    DWORD OnGetSchema(DWORD size, void *in_params);

	DWORD OnReloadSchemas(DWORD size, void *in_params);

private:
	void CheckVarList( void );
	void LoadSchema( void );
	void UnloadSchema();

	// EE Toolbox
	IToolBox* m_ToolBox;

	// Filename of CVar Schema
	CHashString m_SchemaFileName;

	// Database File
	IDatabase* m_pSchemaDB;
	
	// Vector of CLuaScriptVars	
	LUAVARVEC m_Variables;

};

#endif //#ifndef LUASCRIPTVARLIST_H
