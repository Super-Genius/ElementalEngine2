///==========================================================================
/// \file	CTransform.cpp
/// \brief	Transform object for spacial positioning
/// \date	12-18-2006
/// \author Kyle Swaim
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
///==========================================================================

#include "StdAfx.h"

#include "CTransform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTransform::CTransform()
{
	m_Scale.Set(1.0f, 1.0f, 1.0f);

	m_DirtyMatrix = false;
	m_DirtyQuaternion = false;
}

CTransform::~CTransform()
{
}

Vec3& CTransform::GetTranslation()
{
	return m_Translation;
}

EulerAngle& CTransform::GetEuler()
{
	return m_Euler;
}

Quaternion& CTransform::GetQuaternion()
{
	// If the quat is dirty then recalculate it
	if( m_DirtyQuaternion )
	{
		m_Quaternion.FromEuler( m_Euler );
		m_DirtyQuaternion = false;
	}

	return m_Quaternion;
}

Vec3& CTransform::GetScale()
{
	return m_Scale;
}

Matrix4x4& CTransform::GetTransform()
{
	// if the matrix is dirty then recaclulate it
	if( m_DirtyMatrix )
	{
		m_Transform.SetTranslation( m_Translation );
		m_Transform.SetRotation( m_Euler );
		m_Transform.SetScale( m_Scale );
	}

	m_DirtyMatrix = false;

	return m_Transform;
}

void CTransform::SetTranslation( Vec3& translation )
{
	m_Translation.Set( translation );

	m_DirtyMatrix = true;
}

void CTransform::SetEuler( EulerAngle& euler )
{
	m_Euler.Set( euler );

	m_DirtyMatrix = true;
	m_DirtyQuaternion = true;
}

void CTransform::SetQuaternion( Quaternion& quaternion )
{
	// Set all applicable transforms
	m_Quaternion.Set( quaternion.x, quaternion.y, quaternion.z, quaternion.w );
	quaternion.ToEuler( m_Euler );

	m_DirtyQuaternion = false;
	m_DirtyMatrix = true;
}

void CTransform::SetScale( Vec3& scale )
{
	m_Scale.Set( scale );

	m_DirtyMatrix = true;
}

void CTransform::SetTransform( Matrix4x4& transform )
{
	m_Translation.Set( transform.GetTranslation() );
	m_Euler.SetFromMatrix( transform );
	m_Scale.Set( transform.GetScale() );
	m_Transform.SetFrom4x4( transform.m );

	m_DirtyMatrix = false;
	m_DirtyQuaternion = true;
}
