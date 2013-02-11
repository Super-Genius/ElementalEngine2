///============================================================================
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

#ifndef HIERARCHICALNODE_H
#define HIERARCHICALNODE_H

class HierarchicalNode;

typedef map< DWORD, HierarchicalNode *> HNODEMAP;
typedef vector< HierarchicalNode *> HNODEVECTOR;

#define ROTATE	0
#define SCALE	1
#define POS		2
#define MAX_CHANNELS	3

// Node Types
#define NodeType_Cal3D "Cal3D"
#define NodeType_Cloth "Cloth"
#define NodeType_Dummy "DUMMY"


class HierarchicalAnimation;
class HierarchicalModelType;

class HierarchicalNode
{
public:
	static const UINT INVALID_NODE;

	HierarchicalNode();
	~HierarchicalNode();

	/// Is this node a root object? 
	bool IsRoot();

	/// Set the 
	void SetObject( IGeometry * obj );

	/// Set the cal object reference
	//void SetCalObject( const TCHAR * classname );
	/// does this node reference a cal object?
	//bool IsCalObject();
	//TCHAR * GetClassName(){ return (TCHAR*)m_Cal3dClassName.GetString(); }

	void SetObjectType( StdString szType );
	StdString GetObjectType( void );

	void SetFilePath( const TCHAR* szFilePath );
	TCHAR* GetFilePath( void );

	/// Set the child of this node
	void SetParent( HierarchicalNode * parent, UINT pid );
	HierarchicalNode* GetParent( void );
	/// Get Parent Index;
	UINT GetParentIndex();

	///Sets the name
	void SetName( StdString name );
	///Grabs the name
	StdString GetName();
	
	/// Get the quaternion rotation
	/// use of const allows compiler to better optimize if it can
	/// time = current time
	/// currentkeyframe = to optimize searches
	int GetRotation( Quaternion &q, const float time = 0.f, const int currentkeyframe = 0 );

	/// Get the euler rotation
	/// use of const allows compiler to better optimize if it can
	/// time = current time
	/// currentkeyframe = to optimize searches
	int GetRotation( EulerAngle &euler, const float time = 0.f,const  int currentkeyframe = 0 );

	/// Get the translation
	/// use of const allows compiler to better optimize if it can
	/// time = current time
	/// currentkeyframe = to optimize searches
	int GetTranslation( Vec3 &translation, const float time = 0.f,const  int currentkeyframe = 0 );

	/// Get the scaling
	/// use of const allows compiler to better optimize if it can
	/// time = current time
	/// currentkeyframe = to optimize searches
	void SetScaling( Vec3 &scale, int frame );
	int GetScaling( Vec3 &scaling, const float time = 0.f, const int currentkeyframe = 0 );
	
	void SetInitialPos( Quaternion rot, Vec3 trans, Vec3 scale);
	int GetNumFrames( DWORD channel ); //see #define above for channels
	void ResetKeyFrames( UINT sizerot, UINT sizescale, UINT sizetranslation );
	void CleanKeys();
	
	void SetOffsetScale( Vec3 &scale )
	{
		if( scale.x == 1 &&	scale.y == 1 &&	scale.z == 1 )ZeroOffsetScale = true;
		else ZeroOffsetScale = false;
		m_OffsetScale.Set( scale );
	}
	bool GetOffsetScale( Vec3 &outscale )
	{
		if( ZeroOffsetScale )return false;
		outscale.Set( m_OffsetScale );
		return true;
	}
	void SetOffsetRotation( EulerAngle &Rotation )
	{
		if( Rotation.roll == 0 &&	Rotation.pitch == 0 &&	Rotation.yaw == 0 )ZeroOffsetRotation = true;
		else ZeroOffsetRotation = false;
		m_OffsetRotation.Set( Rotation );
	}
	bool GetOffsetRotation( EulerAngle &outRotation )
	{
		if( ZeroOffsetRotation )return false;
		outRotation.Set( m_OffsetRotation );
		return true;
	}
	void SetOffsetTranslation( Vec3 &Translation )
	{
		if( Translation.x == 0 &&	Translation.y == 0 &&	Translation.z == 0 )ZeroOffsetTranslation = true;
		else ZeroOffsetTranslation = false;
		m_OffsetTranslation.Set( Translation );
	}
	bool GetOffsetTranslation( Vec3 &outTranslation )
	{
		if( ZeroOffsetTranslation )return false;
		outTranslation.Set( m_OffsetTranslation );
		return true;
	}

	UINT				m_Index;
	set< HierarchicalNode * > m_Children;
private:
	//these two classes should inherit off a base hierarchical node manipulator
	friend class HierarchicalAnimation;
	friend class HierarchicalModelType;
	StdString			m_Name;
	HierarchicalNode *	m_Parent;
	IGeometry *			m_Geometry; //not being used right now, everything is in cal format

	Quaternion		*	m_Rotation;
	Vec3			*	m_Scaling;
	Vec3			*	m_Translation;

	bool				ZeroOffsetRotation;
	EulerAngle			m_OffsetRotation;
	bool				ZeroOffsetScale;
	Vec3				m_OffsetScale;
	bool				ZeroOffsetTranslation;
	Vec3				m_OffsetTranslation;

	float			*	m_KeyFrames[ MAX_CHANNELS ];
	int					m_LastKey[ MAX_CHANNELS ];
	//CHashString		m_Cal3dClassName;
	//bool				m_Cal3DObject;
	StdString			m_szObjectType;
	CHashString			m_hszFilePath;
	UINT				m_ParentId;
private:
	///some private inline helper functions built for speed. Const to help with compiler optimizations
	__forceinline bool ValidateKeyframe( const int key, const UINT channel )
	{
		return ( key >= 0 && key <= m_LastKey[ channel ] );
	}

	/// iterates backward to find the correct key
	__forceinline int GetSmallerKey( int key, const float time, const UINT channel )
	{		
		//quick checks
		if( key == 0 ){ return 0; }
		else if( m_KeyFrames[ channel ][ --key ] < time ){ return key; }
		
		//quick check failed, iterate, don't have too be too optimal here
		while(	key > 0 &&
				m_KeyFrames[ channel ][ key ] > time )
		{
			--key;
		}
		return key;
	}
	/// iterates forward to find the correct key
	__forceinline int GetBiggerKey( int key, const float time, const UINT channel )
	{		
		//quick checks
		if( key == m_LastKey[ channel ] ){ return m_LastKey[ channel ]; }
		else if( m_KeyFrames[ channel ][ ++key ] > time ){ return key; }
		
		//quick check failed, iterate, don't have too be too optimal here
		while(	key < m_LastKey[ channel ] &&
				m_KeyFrames[ channel ][ key ] < time )
		{
			++key;
		}
		return key;
	}
	__forceinline void GetKeyFrames( const int currentkeyframe, const float time,
									int &prev, int&next, float &relativetime, const UINT channel )
	{
		if( ValidateKeyframe ( currentkeyframe, channel ) )
		{
			//change direction to backward if the new time is smaller than the last time
			if( time <  m_KeyFrames[ channel ][ currentkeyframe ] )//reverse traversal
			{ 
				//time is smaller, we're going backward
				next = GetSmallerKey( currentkeyframe, time, channel );
				prev = next == m_LastKey[ channel ] ? m_LastKey[ channel ] : next + 1;
				if( prev != next )
				{
					relativetime = m_KeyFrames[ channel ][ prev ] - time;//0 == closer to prev				
					float divisor = m_KeyFrames[ channel ][ prev ] - m_KeyFrames[ channel ][ next ];
					relativetime = divisor == 0 ? 0 : relativetime / divisor;
				}

			}else //forward traversal
			{
				//time is bigger, we're going forward
				next = GetBiggerKey( currentkeyframe, time, channel );
				if( time >= m_KeyFrames[ channel ][ m_LastKey[ channel ] ] )
				{
					relativetime = 0;
					prev = next = m_LastKey[ channel ];
				}
				else
				{
					prev = next == 0 ? 0 : next - 1;
					if( prev != next )
					{
						relativetime = time - m_KeyFrames[ channel ][ prev ];			
						float divisor = m_KeyFrames[ channel ][ next ] - m_KeyFrames[ channel ][ prev ];
						relativetime = divisor == 0 ? 0 : relativetime / divisor;
					}
					else
					{
						relativetime = 0;
					}
				}
			}
		}else
		{
			next = 0;
			prev = 0;
		}
	}
	__forceinline void VecSlerp( const Vec3 &va, const Vec3 &vb, const float time, Vec3& vout )
	{
		float oneminustime = 1.f - time;
		vout.x = oneminustime*va.x + time*vb.x;
		vout.y = oneminustime*va.y + time*vb.y;
		vout.z = oneminustime*va.z + time*vb.z;
	}
};

#endif