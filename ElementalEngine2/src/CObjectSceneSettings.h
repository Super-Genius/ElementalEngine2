///============================================================================
/// \file	CObjectSceneSettings.h
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

#ifndef CObjectSceneSettings_H
#define CObjectSceneSettings_H
#include <stack>
using namespace std;

class CObjectSceneSettings : public ISceneSettings
{
	/// \brief The Default Constructor for CObjectSceneSettings
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	CObjectSceneSettings(IHashString *parentName, IHashString *name);
public:
	CObjectSceneSettings(){DeInit();};
	virtual void SetShaderFlags( SHADER_RENDER_FLAG encoding );
	virtual void AddShaderFlag( SHADER_RENDER_FLAG encoding );
	virtual void RemoveShaderFlag( SHADER_RENDER_FLAG encoding );
	virtual bool IsEnabled( SHADER_RENDER_FLAG encoding );
	virtual SHADER_RENDER_FLAG GetShaderFlags();
	
	virtual void SetShadowCast( bool val );
	virtual void SetShadowReceive( bool val );
	virtual void SetLightmapCast( bool val );
	virtual void SetLightmapReceive( bool val );
	virtual void SetFogDistance( float val );
	virtual void SetLOD( int val );
	virtual void PushFlagState();//saves current state
	virtual void PopFlagState();
	/// Set the number of lights that affect this object
	virtual void SetActiveLightCount( int iLightCount );
	virtual int GetActiveLightCount() const;
	virtual void SetDetailLightCount( int iDetailLightCount );
	virtual int GetDetailLightCount() const;
	virtual void SetShadowMapCount( int iShadowMapCount );
	virtual int GetShadowMapCount() const;

	virtual void SetElapsedTime(float et) { m_fElapsedTime = et; }
	virtual float GetElapsedTime() { return m_fElapsedTime; }

public:
	/// IObject  functions
	/// \brief	Create the a instance of IObject
	/// \return	IComponent pointer
	static IComponent *Create(va_list argptr);
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
	SHADER_RENDER_FLAG	m_Flags;
	stack< SHADER_RENDER_FLAG > m_SavedStates;
	int m_iDetailLightCount;
	int m_iActiveLightCount;
	int m_iShadowMapCount;
	float m_fElapsedTime;
};

#endif