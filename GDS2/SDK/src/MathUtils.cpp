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
#include "BaseMessages.h"

bool PlaneMouseRayIntersection( const Vec3 &planePos, const Vec3 &planeNorm, float mouseX, float mouseY, Vec3 &hitPos )
{
	RAYFROMMOUSEPARAMS rayfrommouse;
	rayfrommouse.mouseX = mouseX;
	rayfrommouse.mouseY = mouseY;
	Vec3 rayPos, rayDir;
	rayfrommouse.rayPos = &rayPos;
	rayfrommouse.rayDir = &rayDir;
	static DWORD msgHash_GetRayFromMouseCoords = CHashString(_T("GetRayFromMouseCoords")).GetUniqueID();
	if (EngineGetToolBox()->SendMessage(msgHash_GetRayFromMouseCoords, sizeof(rayfrommouse), &rayfrommouse ) == MSG_HANDLED)
	{
		// collide with plane
		float planeD = -planePos.Dot(planeNorm);
		float Vd = planeNorm.Dot(rayDir);
		if (Vd == 0.0f)
			return false; // ray parallel to plane
		float V0 = -(planeNorm.Dot(rayPos) + planeD);
		float t = V0 / Vd;
		hitPos = rayPos + rayDir * t;
		return (t > 0.0f);
	}
	return false;
}
