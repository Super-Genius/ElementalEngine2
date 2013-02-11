///============================================================================
/// \file		HierarchicalModelsDefine.h
/// \brief		Header file for HierarchicalModel plugin defines
/// \date		10-12-2006
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

#ifndef _HIERARCHICALMODELSDEFINE_H_
#define _HIERARCHICALMODELSDEFINE_H_

// tolua_begin
struct HIERARCHICALOFFSETPARAM
{
	Vec3 *	m_Offset;
	EulerAngle * m_RotationOffset;
	int		m_Child;
	HIERARCHICALOFFSETPARAM()
	{
		m_Offset = NULL;
		m_RotationOffset;
		m_Child = 0;
	}
};


struct HIERARCHICALPOSITIONMATRIX
{
	Matrix3x3 *	m_OutMatrix;
	int	m_Child;
	HIERARCHICALPOSITIONMATRIX()
	{
		m_OutMatrix = NULL;
		m_Child = 0;
	}
};

struct HIERARCHICALANIMATION
{
	IHashString * animtemplate;
	float	speed;
	bool	loop;
	bool	freezeOnLastFrame;
	HIERARCHICALANIMATION()
	{
		animtemplate = NULL;
		speed = 1.f;
		loop = false;
		freezeOnLastFrame = false;
	}
};


struct HIERARCHICALMODELGETCHILDID
{
	IHashString * m_Childname;
	int			  m_RetID;
};

struct HIERARCHICALMODELGETANIMLENGTH
{
	float length;
};

struct HRCMATERIALOVERRIDEPARAMS
{
	IHashString *hsNodeName;
	MATERIALOVERRIDEPARAMS overrideParams;

	HRCMATERIALOVERRIDEPARAMS()
	{
		hsNodeName = NULL;
	}
};

// tolua_end

struct GETHIERARCHICALTYPEPARAMS
{
	IHashString *name;
	IObject *type;
	GETHIERARCHICALTYPEPARAMS()
	{
		name = NULL;
		type = NULL;
	}
};

#endif // #ifndef _HIERARCHICALMODELSDEFINE_H_
