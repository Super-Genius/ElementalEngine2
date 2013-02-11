///============================================================================
/// \file		AmbientOcclusionRenderObject.h
/// \brief		Declaration of AmbientOcclusion Editor Render Object Component
/// \date		04-09-2007
/// \author		Brian Bazyk
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

#ifndef _AMBIENTOCCLUSIONRENDEROBJECT_H_
#define _AMBIENTOCCLUSIONRENDEROBJECT_H_

#include "CRenderObject.h"

namespace ElementalEngine {

class CAmbientOcclusionEditorRenderObject : public CRenderObject
{
public:
	static IComponent *Create(int nArgs, va_list argptr);

	CAmbientOcclusionEditorRenderObject(IHashString *parentName, IHashString *name);
	~CAmbientOcclusionEditorRenderObject();

	void Init();
	void DeInit();

	/// 
	void GenerateAmbientOcclusionMap( Vec3 targetPoint, OBJECTLIST &targetList, OBJECTLIST &occluderList, LIGHTSAMPLELIST &lightList, ITextureObject *pOutTexture );
	/// render light sampling positions
	void RenderLightSampleList( LIGHTSAMPLELIST &lightList );

	void GenerateAmbientOcclusion();

	virtual OBJECTSORTTYPES GetSceneSortID() const { return RENDEROBJECT3D; }

	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( UINT pass, IEffect * override );
	/// \brief Compare for sorting
	/// \param obj = object to compare to
	/// \param sortMethod = a sort method specifying how to compare(set in RENDEROBJECTCOMPARE)
	/// \param curRenderPass = the current render pass to sort to
	/// \return bool = must return true if (this pointer) < (obj)
	virtual bool IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass );
	/// \brief This is for render priorities between classes
	/// will change as soon as a new architecture can be designed to better incorporate
	/// different renderobject types being pushed on the same sorted list
	/// \return DWORD = the priority key this returns, the higher the number,
	/// the higher its priority
	virtual DWORD GetRenderPriority();
	/// \brief Number of passes this render object has to go through
	/// \return UINT = returns one pass for this implementation
	virtual UINT GetNumPass();
	/// \brief Gets bounding sphere of object. This is a 3D interface call that should be implemented for only
	///			3D objects. Might want to seperate more of this later
	/// \param position = vector that will be filled the center of the sphere
	/// \param radius = float that will be filled with the radius of the sphere
	virtual void GetBoundingSphere( Vec3 &position, float &radius );
	/// \brief Gets the base sort class which implements IRenderObject::IsLessThan()
	/// child classes must not re-implement IsLessThan() if they return the BaseSortClass hash!!
	/// typically, each render object has a IsLessThan() and returns a unique BaseSortClass
	virtual IHashString * GetBaseSortClass();
	/// \brief returns if this renderobject needs to be sorted in the transparency list
	/// \return bool = true if this object uses transparency
	virtual bool IsAlphaObject();
	/// returns the mesh associated with this renderobject(should be a quad, but
	/// not implemented right now
	virtual IMeshObject * GetMeshObject();

private:
	IRenderer *m_pRenderer;

	CHashString m_hszLightCameraName;

	CHashString m_hszAccumTextureName;
	ITextureObject *m_pAccumTexture;
	CHashString m_hszDepthTextureName;
	ITextureObject *m_pDepthTexture;

	int m_iAccumTextureResolution;
	int m_iDepthTextureResolution;

	IEffect *m_pAmbientOcclusionEffect;
	IEffect *m_pAmbientOcclusionShadowEffect;
};

}
#endif // _AMBIENTOCCLUSIONRENDEROBJECT_H_
