///============================================================================
/// \file		C2DLine.h
/// \brief		Header file for Vision 2d Line Adapter
/// \date		04-04-2005
/// \author		Josh Lulewicz
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

#ifndef C2DLINE_H
#define C2DLINE_H

#include "IRenderObject.h"
#include "Vec3.h"

/// \class CVis2DLine
/// \brief Adapter for Vision's 2D Line functionality
class C2DLine : public CSceneObject<IRenderObject>
{
private:
	/// \brief The Default Constructor for CVis2DLine
	/// \param parentName = name of parent in hierarchy
	/// \param name = IHashString * name of this object
	C2DLine(IHashString *parentName, IHashString *name);

public:
	/// \brief	The Default Destructor for CVis2DLine
	~C2DLine();

	/// \brief	Create an instance of CVis2DLine
	/// \return	Pointer to this
	static IComponent *Create(int nArgs, va_list argptr);

	virtual void Serialize(IArchive &ar);

	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( UINT pass, IEffect * override );

	/// \brief Compare for sorting
	/// \param obj = object to compare to
	/// \param sortMethod = a sort method specifying how to compare(set in RENDEROBJECTCOMPARE)
	/// \param curRenderPass = the current render pass to sort to
	/// \return bool = must return true if (this pointer) < (obj)
	virtual bool IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass ) { return true; }

	/// \brief This is for render priorities between classes
	/// will change as soon as a new architecture can be designed to better incorporate
	/// different renderobject types being pushed on the same sorted list
	/// \return DWORD = the priority key this returns, the higher the number,
	/// the higher its priority
	virtual DWORD GetRenderPriority(){ return 0; }

	/// \brief Number of passes this render object has to go through
	/// \return UINT = the number of passes that this object has to go through
	virtual UINT GetNumPass() { return 1; }

	/// \brief Sets the position of the render object
	/// \param position Vec3 class reference to fill in
	virtual void SetPosition( const Vec3 &position ) { }

	/// \brief Gets the position of the render object
	/// \return Vec3 = position f the object
	virtual Vec3 &GetPosition() 
	{ 
		static Vec3 tempVec(0,0,0); 
		return tempVec;
	}

	/// \brief Gets bounding sphere of object. This is a 3D interface call that should be implemented for only
	///			3D objects. Might want to seperate more of this later
	/// \param position = vector that will be filled the center of the sphere
	/// \param radius = float that will be filled with the radius of the sphere
	virtual void GetBoundingSphere( Vec3 &position, float &radius )
	{ 
		position.Set( 0,0,0 );
		radius = 0;
	}

	/// Construct the 2D line
	/// \param startX - x value at the line's origin
	/// \param startY - y value at the line's origin
	///	\param endX - x value at the line's end
	/// \param endY - y value at the line's end
	/// \param r, g, b - red, green, blue of the line
	/// \param width - width of the line
	/// \return true on build success, false on failure
	virtual bool Build2DLine(float startX, float startY,
		                     float endX, float endY, 
							 int r, int g, int b, float width);

	virtual IHashString * GetBaseSortClass(){ return GetComponentType(); }

	/// \brief returns if this renderobject needs to be sorted in the transparency list
	/// \return bool = true if this object uses transparency
	virtual bool IsAlphaObject(){ return false; }
		
	/// returns the mesh associated with this renderobject(should be a quad, but
	/// not implemented right now
	virtual IMeshObject * GetMeshObject(){ return NULL; };

	/// Gets the number of Geometry elements this renderobject possesses or if it uses geometry at all	
	virtual UINT GetNumGeometry(){ return 0; }

	/// Grabs the geometry pieces of the render object if there is any uses it for render instead
	/// of the render object render(might need to be changed)
	virtual IGeometry* GetGeometry( int index ){ return NULL; }

	/// \brief GetBoundingBox default implementation
	virtual bool	GetBoundingBox( Vec3 &pos, Vec3 &dim, EulerAngle &rotation )
	{	
		return false;
	}
	virtual void AttachLight( ILightObject * light ){}
	virtual ISceneSettings * GetSceneSettings(){ return NULL; }
	/// Signals a detach of all light objects
	virtual void DetachAllLights(){}
	virtual IMaterial * GetMaterial( int index ){return NULL;}
	virtual UINT GetNumMaterial( ) { return 0; }
private:
	float m_fStartX;	// x value of start
	float m_fStartY;	// y value of start
	float m_fEndX;		// x value of end
	float m_fEndY;		// y value of end
	float m_fWidth;		// width of the line
	int	m_iRed;			// red component of the line color
	int m_iGreen;		// green component of the line color
	int m_iBlue;		// blue component of the line color

	IRenderer *m_Renderer;	// renderer interface
};


#endif  // CVIS2DLINE_H