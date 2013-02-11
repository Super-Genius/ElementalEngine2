///============================================================================
/// \file		LuaScriptVar.h
/// \brief		Header file for Lua Script Variable
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

#ifndef LUASCRIPTVAR_H
#define LUASCRIPTVAR_H

class CLuaScriptVar
{
public:
	CLuaScriptVar();

    ~CLuaScriptVar(void);

	StdString GetName();
	void SetName( StdString Name );

	StdString GetType();
	void SetType( StdString Type );

	bool GetIsDefault();
	void SetIsDefault( bool VariableIsDefault );

	StdString GetValueAsString( void );
	void SetValue( StdString Value );

	bool GetValueAsBool( void );
	void SetValue( bool Value );

	int GetValueAsInt( void );
	void SetValue( int Value );
	
	float GetValueAsFloat( void );
	void SetValue( float Value );

private:
	StdString m_VariableName;
	StdString m_VariableValue;
	// Type Of Variable
	// String
	// Bool
	// Int
	// Float
	StdString m_VariableType;
	bool m_VariableIsDefault;

};

#endif //#ifndef LUASCRIPTVAR_H
