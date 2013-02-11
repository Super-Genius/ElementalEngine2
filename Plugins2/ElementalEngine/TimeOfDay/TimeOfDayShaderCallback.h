///============================================================================
/// \file		TimeOfDayShaderCallback.h
/// \brief		Header file for the Time of Day shader callback
/// \date		10-31-2005
/// \author		Josh Lulewicz
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

#ifndef _TIMEOFDAYSHADERCALLBACK_H_
#define _TIMEOFDAYSHADERCALLBACK_H_

class CTimeOfDayShaderCallback : public IShaderCallback
{
public:
	/// Construction
	CTimeOfDayShaderCallback();
	/// Destruction
	~CTimeOfDayShaderCallback();

	/// Shader callback function
	/// \param shader - pointer to shader interface
	/// \param time - ?
	/// \return <insert return description here>
	virtual void ShaderCallback(IShader * shader, float time);
	/// Get the name of the shader callback
	/// \param void
	/// \return pointer to an IHashString which is the name of the shader callback
	virtual IHashString *GetCallbackName();

private:
	/// Tool box
	IToolBox *m_ToolBox;
	/// name of the shader callback
	IHashString *m_ShaderCallbackName;
	/// renderer interface
	IRenderer *m_Renderer;	
};

#endif // _TIMEOFDAYSHADERCALLBACK_H_
