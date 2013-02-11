///============================================================================
/// \file		Cal3dModelsDefine.h
/// \brief		Header file for Cal3DModels plugin defines
/// \date		12-12-2006
/// \author		Marvin Gouw
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

#ifndef _CAL3DMODELSSDEFINE_H_
#define _CAL3DMODELSSDEFINE_H_

#define ANIMATIONLIST list<StdString>

/// struct for getting the model interface to add to renderlist
typedef struct GETCALMODELINTERFACE
{
	IHashString *modelName; /// name of model in
	IRenderObject *modelRO; /// model render object to return
}GETCALMODELINTERFACE;

// tolua_begin
struct GETCALPARENT
{
	IHashString * parent;
	GETCALPARENT()
	{
		parent = NULL;
	}
};
struct ANIMATIONID
{
	StdString AnimationName;
	int		  AnimationId;
	ANIMATIONID()
	{
		AnimationId = -1;
	}
};

struct GETANIMATIONLISTPARAMS
{
	int numAnimations;
	ANIMATIONLIST * AnimationList;
	GETANIMATIONLISTPARAMS()
	{
		numAnimations = 0;
		AnimationList = NULL;
	}
};

struct RENDERBUFFERALLOCATION
{
	BUFFERALLOCATIONSTRUCT	m_IndexAllocation;
	BUFFERALLOCATIONSTRUCT	m_VertexAllocation;
	IVertexBufferObject *	m_VertexBufferInterface;
	IIndexBuffer *			m_IndexBufferInterface;
	IMaterial *	m_Material;
	RENDERBUFFERALLOCATION()
	{
		m_VertexBufferInterface = NULL;
		m_IndexBufferInterface = NULL;
		m_Material = NULL;
	}
};

struct GETRENDERBUFFERALLOCATIONMSG
{
	vector< RENDERBUFFERALLOCATION > m_GeometryAllocationsOut;
};

struct GETRENDERBUFFERALLOCATIONBYNAMEMSG
{
	vector< RENDERBUFFERALLOCATION > m_GeometryAllocationsOut;
	IHashString* m_hszName;

	GETRENDERBUFFERALLOCATIONBYNAMEMSG()
	{
		m_GeometryAllocationsOut.clear();
		m_hszName = NULL;
	}
};


struct REGISTERCAL3DANIMATIONCALLBACK
{
	int				AnimationId;
	float			AnimationTriggerTime;
	bool			bTriggerOnComplete;
	bool			bTriggerOnStop;
	IHashString		*AnimationName;
	IHashString		*StateObjectName;
	IHashString		*EventName;
	IHashString		*CustomMessageName; // Default is in Cal3D of TriggerEvent
	IArchive		*EventParamsArchive;
	REGISTERCAL3DANIMATIONCALLBACK()
	{
		AnimationId = -1;
		AnimationTriggerTime = -1;
		AnimationName = NULL;
		bTriggerOnComplete = false;
		bTriggerOnStop = false;
		StateObjectName = NULL;
		EventName = NULL;
		CustomMessageName = NULL;
		EventParamsArchive = NULL;
		StateObjectName = NULL;
	}
};


struct RENDEROBJECTANIMATIONPARAMS
{
	int			LayerNumber;//For vision, this has ot be 0-3, so 4 layers total
	IHashString *	AnimationName;
	int				AnimationID;
	float			BlendIn;
	float			BlendOut;
	bool			bLoop;
	bool			LockLastFrame;
	RENDEROBJECTANIMATIONPARAMS()
	{
		AnimationID = 0;
		AnimationName = NULL;
		bLoop = false;
		LayerNumber = 0;
		BlendIn = 0;
		BlendOut = 0;
		LockLastFrame = false;
	}
};

struct GETCLOTHPHYSICSGEOMETRYPARAMS
{
	IGeometry *pGeometry;
	IHashString *pNodeName;
};

// tolua_end

#endif // #ifndef REGISTERCAL3DANIMATIONCALLBACK
