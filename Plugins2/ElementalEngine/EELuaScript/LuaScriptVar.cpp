///============================================================================
/// \file		LuaScriptVar.cpp
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

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLuaScriptVar::CLuaScriptVar( )
{
	m_VariableName = "";
	m_VariableValue = "";
	m_VariableType = "String";
	m_VariableIsDefault = true;
}

CLuaScriptVar::~CLuaScriptVar( )
{

}

StdString CLuaScriptVar::GetName( void )
{
	return m_VariableName;
}

void CLuaScriptVar::SetName( StdString Name )
{
	m_VariableName = Name;
}

StdString CLuaScriptVar::GetType( void )
{
	return m_VariableType;
}

void CLuaScriptVar::SetType( StdString Type )
{
	m_VariableType = Type;
}

bool CLuaScriptVar::GetIsDefault( void )
{
	return m_VariableIsDefault;
}

void CLuaScriptVar::SetIsDefault( bool VariableIsDefault )
{
	m_VariableIsDefault = VariableIsDefault;
}

StdString CLuaScriptVar::GetValueAsString( void )
{
	return m_VariableValue;
}

void CLuaScriptVar::SetValue( StdString Value )
{
	m_VariableValue = Value;
}

bool CLuaScriptVar::GetValueAsBool( void )
{
	if( m_VariableValue == "true" )
	{
		return true;
	}
	return false;
}

void CLuaScriptVar::SetValue( bool Value )
{
	if( Value == true )
	{
		m_VariableValue = "true";
	}
	else
	{
		m_VariableValue = "false";
	}
}

int CLuaScriptVar::GetValueAsInt( void )
{
	return (int)atoi(m_VariableValue.c_str());
}

void CLuaScriptVar::SetValue( int Value )
{
	char szValue[32];
	sprintf( szValue, "%d", Value );
	m_VariableValue = szValue;
}

float CLuaScriptVar::GetValueAsFloat( void )
{
	return (float)atof(m_VariableValue.c_str());
}

void CLuaScriptVar::SetValue( float Value )
{
	char szValue[32];
	sprintf( szValue, "%f", Value );
	m_VariableValue = szValue;
}