///============================================================================
/// \file	ISceneSettings.h
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

#ifndef SCENESETTINGS_H
#define SCENESETTINGS_H

namespace ElementalEngine
{
class ISceneSettings //: public IObject
{
public:
	/// Destructor for ISceneSettings
	virtual ~ISceneSettings() {};

	/// Sets the render flags
	/// \param encoding - SHADER_RENDER_FLAG (dword) to be set
	virtual void SetShaderFlags( SHADER_RENDER_FLAG encoding ) = 0;

	/// Adds a render flags
	/// \param encoding - SHADER_RENDER_FLAG (dword) to be added
	virtual void AddShaderFlag( SHADER_RENDER_FLAG encoding ) = 0;

	/// Removes the render flags
	/// \param encoding - SHADER_RENDER_FLAG (dword) to be removed
	virtual void RemoveShaderFlag( SHADER_RENDER_FLAG encoding ) = 0;

	/// Returns if the specified render flag is set to true
	/// \param encoding - SHADER_RENDER_FLAG (dword) to check
	/// \return - true if enabled, else false
	virtual bool IsEnabled( SHADER_RENDER_FLAG encoding ) = 0;

	/// Gets the render flags for this scene
	/// \return - SHADER_RENDER_FLAG (dword)
	virtual SHADER_RENDER_FLAG GetShaderFlags() = 0;
	
	/// Sets Shadow Cast for this scene (or object)
	/// \param val - bool indicating true or false for shadow casting
	virtual void SetShadowCast( bool val ) = 0;
	
	/// Sets Shadow Recieve for this scene (or object)
	/// \param val - bool indicating true or false for shadow recieving
	virtual void SetShadowReceive( bool val )= 0;
	
	/// Sets Lightmap Cast for this scene (or object)
	/// \param val - bool indicating true or false for lightmap casting
	virtual void SetLightmapCast( bool val )= 0;
	
	/// Sets Lightmap Recieve for this scene (or object)
	/// \param val - bool indicating true or false for lightmap recieving
	virtual void SetLightmapReceive( bool val ) =0;
	
	/// Sets Fog Distance for this scene (or object)
	/// \param val - float giving the fog distance value
	virtual void SetFogDistance( float val ) = 0;
	
	/// Sets Level-Of-Detail for this scene (or object)
	/// \param val - int indicating level of detail
	virtual void SetLOD( int val ) = 0;
	 
	/// Sets the number of active lights for this scene (or object)
	/// \param iLightCount - int indicating number of active lights
	virtual void SetActiveLightCount( int iLightCount ) = 0;

	/// Gets the number of active lights for this scene (or object)
	/// \return - int indicating number of active lights
	virtual int GetActiveLightCount( void ) const = 0;

	/// Sets the number of detail lights for this scene (or object)
	/// \param iDetailLightCount - int indicating number of detail lights
	virtual void SetDetailLightCount( int iDetailLightCount ) = 0;
	
	/// Gets the number of detail lights for this scene (or object)
	/// \return - int indicating number of detail lights
	virtual int GetDetailLightCount( void ) const = 0;

	/// Sets the number of shadow maps for this scene (or object)
	/// \param iShadowMapCount - int indicating number of shadow maps
	virtual void SetShadowMapCount( int iShadowMapCount ) = 0;

	/// Gets the number of shadow maps for this scene (or object)
	/// \return - int indicating number of shadow maps
	virtual int GetShadowMapCount( void ) const = 0;

	/// Pushes the current SHADER_RENDER_FLAG onto a stack
	virtual void PushFlagState( void ) = 0;

	/// Pops the current SHADER_RENDER_FLAG on the stack off
	virtual void PopFlagState( void ) = 0;

	/// Sets the amount of elapsed time (for effect calculations)
	/// \param et - float indicating amount of elapsed time
	virtual void SetElapsedTime(float et) = 0;

	/// Gets the amount of elapsed time (for effect calculations)
	/// \return - float indicating amount of elapsed time
    virtual float GetElapsedTime() = 0;
};
}

#endif