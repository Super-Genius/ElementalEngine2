///==========================================================================
/// \file	CTransform.h
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

#ifndef	CTRANSFORM_H
#define	CTRANSFORM_H

///==========================================================================
/// \class	CTransform
/// \brief	Transform object for spacial positioning
///==========================================================================
class CTransform 
{
public:
	/// \brief	Default Constructor
	CTransform();
	//CTransform( IHashString *parentName, IHashString *name );
	/// \brief	Destructor
	~CTransform();

	/// \brief	Get the translation vector
	/// \return	The translation vector
	Vec3& GetTranslation();
	/// \brief	Get the translation vector
	/// \return	The translation vector
	EulerAngle& GetEuler();
	/// \brief	Get the translation vector
	/// \return	The translation vector
	Quaternion& GetQuaternion();
	/// \brief	Get the translation vector
	/// \return	The translation vector
	Vec3& GetScale();
	/// \brief	Get the translation vector
	/// \return	The translation vector
	Matrix4x4& GetTransform();

	/// \brief	Set the translation vector
	///			This will update the other transform variables as well
	///			to keep everything consistant.
	/// \paran translation - The new translation vector
	void SetTranslation( Vec3& translation );
	/// \brief	Set the rotation euler vector
	///			This will update the other transform variables as well
	///			to keep everything consistant.
	/// \paran translation - The new rotation euler vector
	void SetEuler( EulerAngle& euler );
	/// \brief	Set the rotation quaternion
	///			This will update the other transform variables as well
	///			to keep everything consistant.
	/// \paran translation - The new rotation quaternion
	void SetQuaternion( Quaternion& quaternion );
	/// \brief	Set the scale vector
	///			This will update the other transform variables as well
	///			to keep everything consistant.
	/// \paran translation - The new scale vector
	void SetScale( Vec3& scale );
	/// \brief	Set the transform matrix
	///			This will update the other transform variables as well
	///			to keep everything consistant.
	/// \paran translation - The new translation matrix
	void SetTransform( Matrix4x4& transform );

private:
	Vec3		m_Translation;
	EulerAngle	m_Euler;
	Quaternion	m_Quaternion;
	Vec3		m_Scale;
	Matrix4x4	m_Transform;

	bool m_DirtyQuaternion;
	bool m_DirtyMatrix;
};

#endif // CTRANSFORM_H