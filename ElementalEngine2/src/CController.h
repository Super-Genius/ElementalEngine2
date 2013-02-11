///============================================================================
/// \file	CController.h
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

#ifndef CCONTROLLER_H
#define CCONTROLLER_H

#include "IController.h"

namespace ElementalEngine
{

class CController: public IController
{
public:
	CController()
	{
		m_PrecalculatedMatrix.SetIdentity();
		m_Parent = NULL;
		m_Rotation.Set( 0,0,0 );
		m_Translation.Set( 0,0,0 );
		m_Scale.Set( 1.f, 1.f, 1.f );
		m_Data = NULL;
	}
	virtual IController * GetParent()
	{
		return m_Parent;
	}
	virtual list< IController * > * GetChildren( )
	{
		return &m_Children;
	}
	/// creates a double link between parent and child or nothing if a link already exists.
	virtual bool  SetParent( IController * surf )
	{
		if( surf )
		{
            m_Parent = surf;
			surf->SetChild( this );
		}else
		{
			if( m_Parent )
			{
				//unlink
				surf->RemoveChild( surf );
				m_Parent = NULL;
			}
		}
		return true;
	}
	/// creates a double link between parent and child or nothing if one already exists.
	virtual bool SetChild( IController * surf )
	{
		if( surf )
		{
			//find the child first
			bool found = false;
			if( m_Children.size() > 0 )
			{
				list< IController*>::iterator iter = m_Children.begin();
				for( ; iter != m_Children.end(); ++iter)
				{
					if( surf ==  (*iter) )
					{
						return true;
					}
				}
			}
			m_Children.push_back( surf );
			surf->SetParent( this );
		}
		else
		{
			//NULL child, do nothing
		}
		return true;
	}
	virtual bool RemoveChild( IController * surf )
	{
		if( surf )
		{
			if( m_Children.size() > 0 )
			{
				list< IController*>::iterator iter = m_Children.begin();
				for( ; iter != m_Children.end(); ++iter )
				{
					if( surf ==  (*iter) )
					{
						m_Children.erase( iter );
						surf->RemoveParent();
						return true;
					}
				}
			}
		}
		return false;		
	}
	virtual bool RemoveParent()
	{
		if( m_Parent )
		{
			m_Parent->RemoveChild( this );
			m_Parent = NULL;
		}
		return true;
	}
	/// Sets the scaling from the parent
	virtual void SetScaling( Vec3 &scale )
	{
		m_Scale.Set( scale );
	}
	/// Gets the scaling from the parent
	virtual Vec3 GetScaling()
	{
		return m_Scale;
	}
	/// Sets the rotation offset from the parent
	virtual void SetRotationOffset( EulerAngle &RotationOffset )
	{
		static MathUtil math;
		//convert to radians
		m_Rotation = RotationOffset;
	}
	/// Gets the rotation offset from the parent
	virtual EulerAngle GetRotationOffset()
	{
		return m_Rotation;
	}
	/// Sets the translation offset from the parent
	virtual void SetTranslationOffset( Vec3 &TranslationOffset )
	{
		m_Translation.Set( TranslationOffset ); 
	}
	/// Gets the rotation offset from the parent
	virtual Vec3 GetTranslationOffset()
	{
		return m_Translation;
	}

	/// recursively builds the rotation based on precalculated parent. Builds rotation for all its children
    virtual bool BuildRotation()
	{
		//m_PrecalculatedMatrix.SetIdentity();		
		m_PrecalculatedMatrix.SetRotation( m_Rotation );
		m_PrecalculatedMatrix.SetTranslation( m_Translation );

		
		m_PrecalculatedMatrix.m[0] *= m_Scale.x;
		m_PrecalculatedMatrix.m[1] *= m_Scale.x;
		m_PrecalculatedMatrix.m[2] *= m_Scale.x;
		m_PrecalculatedMatrix.m[3] *= m_Scale.x;
			
		m_PrecalculatedMatrix.m[4] *= m_Scale.y;
		m_PrecalculatedMatrix.m[5] *= m_Scale.y;
		m_PrecalculatedMatrix.m[6] *= m_Scale.y;
		m_PrecalculatedMatrix.m[7] *= m_Scale.y;
			
		m_PrecalculatedMatrix.m[8] *= m_Scale.z;
		m_PrecalculatedMatrix.m[9] *= m_Scale.z;
		m_PrecalculatedMatrix.m[10] *= m_Scale.z;
		m_PrecalculatedMatrix.m[11] *= m_Scale.z;

		//get parent matrix if any and multiply
		if( m_Parent &&
			m_Parent->GetMatrix() )
		{
			Matrix4x4 * ParentMat = m_Parent->GetMatrix();
			m_PrecalculatedMatrix *= (*ParentMat);
		}
		return true;
	}

	virtual void RecurseBuild( IController * c )
	{
		if( !c ) c = this;

		//build the parent rotation first
		c->BuildRotation();

		list< IController * >::iterator iter = m_Children.begin();
		for( ; iter != m_Children.end(); ++iter )
		{
            IController * child = (*iter);
			if( child )
			{
				RecurseBuild( child );
			}
		}
	}

	/// Get precalculated rotation matrix
	virtual Matrix4x4 * GetMatrix()
	{
		return &m_PrecalculatedMatrix;
	}
	/// Set precalculated rotation matrix
	virtual void SetMatrix(Matrix4x4 * m)
	{
		memcpy( m_PrecalculatedMatrix.m, m->m, sizeof( m->m ) );
		//recalc pos, scale and rot
		MathUtil math;
		Matrix3x3 rotMat;
		rotMat.SetFrom4x4( m->GetMatrix() );
		m_Rotation.SetFromMatrix( rotMat );
		m_Translation.x= m_PrecalculatedMatrix.m[12];
		m_Translation.y = m_PrecalculatedMatrix.m[13];
		m_Translation.z = m_PrecalculatedMatrix.m[14];
	}

	virtual UINT GetNumChildren()
	{
		return (UINT)m_Children.size();
	}

	virtual void Release()
	{
		//is this ok..? For releasing in the correct DLL space since these are allocated
		//through the renderer, should check for ref counts
		delete this;
	}

private:
	void * m_Data;
	IController * m_Parent;
	list< IController * > m_Children;
	EulerAngle		m_Rotation;
	Vec3		m_Translation;
	Vec3		m_Scale;
	Matrix4x4	m_PrecalculatedMatrix; //use build to calculate
};

}//namespace ElementalEngine

#endif