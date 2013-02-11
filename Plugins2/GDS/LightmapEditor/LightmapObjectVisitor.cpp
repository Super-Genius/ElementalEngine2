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

#include "stdafx.h"

bool LightMapObjectVisitor::Visit( IComponent * component, bool bVisitEnter )
{
	if( !component )
	{
		return false;
	}
	IObject * curObj;
	IRenderObject * curRObject;
	IDTOOBJECTMAP * ObjSet;
	IQuadNode * qnode = (IQuadNode *)component;		
	ObjSet = qnode->GetObjSet( RENDEROBJECT3D );
	if( ObjSet )
	{
		//iterate through objects of this node:
		IDTOOBJECTMAP::iterator iter;
		for( iter = ObjSet->begin(); iter != ObjSet->end();
			iter++ )
		{
			curObj = (iter->second);	
			curRObject = dynamic_cast<IRenderObject*>(curObj); 
			//check if renderobject type
			if( curRObject != NULL)
			{
				//check if it can use lightmaps?
				m_Objects.push_back( curRObject );
			}
		}
	}
	return true;
}