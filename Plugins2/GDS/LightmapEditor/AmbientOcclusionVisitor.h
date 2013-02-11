///============================================================================
/// \file		AmbientOcclusionVisitor.h
/// \brief		Declaration of AmbientOcclusion Vistor Component
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

#ifndef _AMBIENTOCCLUSIONVISITOR_H_
#define _AMBIENTOCCLUSIONVISITOR_H_

namespace ElementalEngine {

class CAmbientOcclusionVisitor : public IVisitor
{
public:
	CAmbientOcclusionVisitor()
	{
		m_pList = NULL;
		m_fWorldExtent = 1.0f;
	}
	virtual bool Visit( IComponent * component, bool bVisitEnter );
	virtual bool IsHierarchicalVisitor( void ) { return false; }
	void Init();
	inline void SetList( OBJECTLIST *pList )
	{
		m_pList = pList;
	}
	inline void SetSphere( Vec3 center, float radius )
	{
		m_SphereCenter = center;
		m_fSphereRadius = radius;
	}

private:
	MathUtil math;
	float m_fWorldExtent;
	Vec3 m_SphereCenter;
	float m_fSphereRadius;
	OBJECTLIST *m_pList;
};

}
#endif // _AMBIENTOCCLUSIONVISITOR_H_
