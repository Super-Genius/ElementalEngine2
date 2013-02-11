///============================================================================
/// \file		ObjectCubeMapShaderCallback.h
/// \brief		Header file for CObjectCubeMapShaderCallback
/// \date		10-04-2005
/// \author		Josh Lulewicz
/// \note		Copyright (c) 2005 Signature Devices, Inc.
///============================================================================

#ifndef _OBJECTCUBEMAPSHADERCALLBACK_H_
#define _OBJECTCUBEMAPSHADERCALLBACK_H_

class CObjectCubeMapShaderCallback : public IShaderCallback
{
public:
	/// Construction
	CObjectCubeMapShaderCallback();
	/// Destruction
	~CObjectCubeMapShaderCallback();

	/// Shader callback function
	/// \param shader - pointer to shader interface
	/// \param time - ?
	/// \return <insert return description here>
	virtual void ShaderCallback(IShader * shader, float time);
	/// Get the name of the shader callback
	/// \param void
	/// \return pointer to an IHashString which is the name of the shader callback
	virtual IHashString *GetCallbackName();
	/// Set the fade distance for the shader callback
	/// \param distance - distance in which to start fading
	/// \return void
	void SetFadeDistance(float distance) { m_fFadeDistance = distance; }


private:
	/// Tool box
	IToolBox *m_ToolBox;
	/// name of the shader callback
	IHashString *m_ShaderCallbackName;
	/// renderer interface
	IRenderer *m_Renderer;	
	/// fade distance
	float m_fFadeDistance;
};

#endif // _OBJECTCUBEMAPSHADERCALLBACK_H_
