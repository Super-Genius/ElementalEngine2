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

#include "StdAfx.h"
#include "AmbientOcclusionVisitor.h"

void CAmbientOcclusionVisitor::Init()
{
	m_fWorldExtent = 1.0f;
	static DWORD msgHash_GetWorldExtents = CHashString(_T("GetWorldExtents")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetWorldExtents, sizeof(float), &m_fWorldExtent );
}

bool CAmbientOcclusionVisitor::Visit( IComponent * component, bool bVisitEnter )
{
	if (!m_pList)
		return false;

	IQuadNode *qnode = (IQuadNode*)component;

//	Vec3 qnodeAABBMin, qnodeAABBMax;
//	qnode->GetBoundingAABB( qnodeAABBMin, qnodeAABBMax, m_WorldExtent );

	Vec3 qnodePos;
	float qnodeRadius;
	qnode->GetBoundingSphere( qnodePos, qnodeRadius );

	INTERSECTION_RESULT intersectionResult = math.IntersectSphereSphere( qnodePos, qnodeRadius, m_SphereCenter, m_fSphereRadius );
	if (intersectionResult == INTERSECTION_INSIDE)
	{
		// qnode is entirely inside the occluder sphere, add all objects without further testing
		IDTOOBJECTMAP *ObjSet = qnode->GetObjSet( RENDEROBJECT3D );
		if (ObjSet)
		{
			IDTOOBJECTMAP::iterator itrSet;
			for (itrSet = ObjSet->begin(); itrSet != ObjSet->end(); itrSet++)
			{
				m_pList->push_back( itrSet->second );
			}
		}
		return true;
	}
	else if (intersectionResult == INTERSECTION_PARTIAL)
	{
		// qnode is partially inside the occluder sphere, do per-object testing
		IDTOOBJECTMAP *ObjSet = qnode->GetObjSet( RENDEROBJECT3D );
		if (ObjSet)
		{
			IDTOOBJECTMAP::iterator itrSet;
			for (itrSet = ObjSet->begin(); itrSet != ObjSet->end(); itrSet++)
			{
				IObject *curObj = (itrSet->second);	
				IRenderObject *curRObject = dynamic_cast<IRenderObject*>(curObj); 
				//check if renderobject type
				if( curRObject != NULL)
				{
					// get object's bounding sphere
					Vec3 objPos;
					float objRadius;
					curRObject->GetBoundingSphere( objPos, objRadius );

					// test object's bounding sphere
					if (math.IntersectSphereSphere( objPos, objRadius, m_SphereCenter, m_fSphereRadius ) != INTERSECTION_OUTSIDE)
					{
						m_pList->push_back( curObj );
					}
				}
			}
		}
		return true;
	}
	return false;
}
