///============================================================================
/// \file		DetailObjectShaderCallback.h
/// \brief		Header file for the DetailObject shader callback
/// \date		05-27-2007
/// \author		Kenneth Hurley
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

#ifndef _DETAILOBJECTSHADERCALLBACK_H_
#define _DETAILOBJECTSHADERCALLBACK_H_

class CDetailObjectShaderCallback : public IShaderCallback
{
public:
	/// Construction
	CDetailObjectShaderCallback();
	/// Destruction
	~CDetailObjectShaderCallback();

	/// Shader callback function
	/// \param shader - pointer to shader interface
	/// \param time - ?
	virtual void ShaderCallback(IShader * shader, float time);

	/// Get the name of the shader callback
	/// \param void
	/// \return pointer to an IHashString which is the name of the shader callback
	virtual IHashString *GetCallbackName();

	/// Setup shader constant (Time)
	/// \param time = time of day
	/// \return void
	void SetTimeConstant(float time) { m_fTime = time; }

private:
	/// Tool box
	IToolBox *m_ToolBox;
	/// name of the shader callback
	IHashString *m_ShaderCallbackName;
	/// renderer interface
	IRenderer *m_Renderer;
	/// time of day (used in shader)
	float m_fTime;
};

#endif // _DETAILOBJECTSHADERCALLBACK_H_
