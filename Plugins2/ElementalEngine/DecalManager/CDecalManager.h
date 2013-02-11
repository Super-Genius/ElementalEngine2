///============================================================================
/// \file	CDecalManager.h
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

#ifndef DECALMANAGER_H
#define DECALMANAGER_H

#include "CManager.h"

#include <queue>
#define MAX_SPRITE_ELEMENTS 32768 //16384

using namespace std;
struct DECALQUEUEENTRY
{
	struct DECALVERTEX
	{
		float	pos[3];
		float	normal[3];
		float	uv[2];
		DWORD	color;
		float	time[2];
		DECALVERTEX()
		{
			color = 0xFFFFFFFF;
			time[0] = 0;
			time[1] = 0;
		}
	};
	DECALVERTEX		m_Verts[6];
	
};


typedef priority_queue< int, vector< int> , greater< int > >	DELETEDQUEUE;
typedef vector< pair< IBaseTextureObject *, vector< DECALQUEUEENTRY > > >	TEXTUREDECALLIST;
typedef pair< IBaseTextureObject *, vector< DECALQUEUEENTRY > > TEXTUREVECTORPAIR;
class CDecalManager : public CManager
{
public:

	/// \brief The Default Destructor for CParticleManager
	/// \return void
	~CDecalManager( );

	/// \brief	Function to get this component.  This is a singleton
	/// \param	void
	/// \return	IComponent*: This Manager
	static IComponent* Create( int nArgs, va_list argptr );

	/// \brief	Update function for the Particle Manager
	/// \return	void
	virtual void Update( DWORD tickCount );
	
	/// \brief	DeInitialization
	/// \return	bool: Success
	virtual bool DeInit( void );
	
	DWORD OnAddDecalSprite(DWORD size, void *params);
	DWORD OnAddDecalTemp( DWORD size, void * params );
private:
	void LoadDecalEffect();
	void ProcessDecalQueue();
	SINGLETONCONSTRUCTOROVERRIDE( CDecalManager );
	/// \brief The Default Constructor for CParticleManager
	/// \return void
	CDecalManager();
	TEXTUREDECALLIST m_DecalQueue;
	IEffect * m_DecalEffect;
	IBaseTextureObject * m_DecalTextureTest;
	static float m_PrecentageUpdateIteration;
	vector< DELETEDQUEUE >	m_EmptyIndices;
	vector< size_t > curIteration;
	lua_State* m_LuaState;

};

#endif //#ifndef DECALMANAGER_H