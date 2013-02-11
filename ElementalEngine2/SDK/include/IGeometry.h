///============================================================================
/// \file	IGeometry
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

#ifndef IGEOMETRY_H
#define IGEOMETRY_H
//#include "BaseMessages.h"
#include "IRenderer.h"

namespace ElementalEngine
{
class ISceneSettings;
class IController;
class IBaseTextureObject;
class IHashString;
struct BUFFERALLOCATIONSTRUCT;

/// IGeometry
/// Interface for Render Geometry
class IGeometry
{
public:
	/// Destructor
	virtual ~IGeometry() {};

	/// Sets the material for this piece of geometry
	/// \param mat - pointer to IMaterial interface
	virtual void SetMaterial( IMaterial* mat ) = 0;
	
	/// Gets the material for this piece of geometry
	/// \return - pointer to IMaterial interface; NULL if surface does not have a listing of the stage name
	virtual IMaterial* GetMaterial( void ) = 0;

	/// responsible for loading the mesh into internal buffers and then allocating vertex buffer space for the mesh
	/// \param mesh - pointer to IMeshObject interface
	/// \return - true on success
	virtual bool SetMesh( IMeshObject * mesh ) = 0;
	
	/// locks the vertex buffer
	/// \param offset - unsigned int of the offset into the buffer
	/// \param size - unsigned int of the size of the buffer
	/// \param pData - pointer to a void pointer of the locked vertex data
	/// \return - true on success
	virtual bool LockVertices( UINT offset, UINT size, void ** pData ) = 0;

	/// unlocks the vertex buffer
	/// \return - true on success
	virtual bool UnlockVertices( void ) = 0;

	/// get number of vertices
	/// \return - unsigned int of the number of vertices
	virtual UINT GetNumVertices( void ) = 0;

	/// allows for resizing of the vertex buffer
	/// \param size - unsigned int indicating the size
	/// \param stridebytes - unsigned int indicating the stride
	/// \param VertexFormat - pointer to a CHANNELDESCRIPTORLIST describing the vertex format
	/// \return - true on success
	virtual bool ResizeVertices( UINT size, UINT stridebytes, CHANNELDESCRIPTORLIST * VertexFormat ) = 0;

	/// locks the index buffer
	/// \param offset - unsigned int of the offset into the buffer
	/// \param size - unsigned int of the size of the buffer
	/// \param pData - pointer to a void pointer of the locked index data
	/// \return - true on success
	virtual bool LockIndices( UINT offset, UINT size, void ** pData ) = 0;	

	/// unlocks the index buffer
	/// \return - true on success
	virtual bool UnlockIndices( void ) = 0;
	
	/// get number of indices
	/// \return - unsigned int of the number of indices
	virtual UINT GetNumIndices( void ) = 0;

	/// allows for resizing of the index buffer
	/// \param size - unsigned int indicating the size
	/// \param stridebytes - unsigned int indicating the stride
	/// \return - true on success
	virtual bool ResizeIndices( UINT size, UINT stridebytes ) = 0;

	/// get number of primitives
	/// \return - unsigned int of the number of primitives
	virtual UINT GetNumPrimitives( void ) = 0;

	/// sets the primitive type
	/// \param primtype - enum of primtype to set this geometry as ( DP_TRIANGLELIST, DP_TRIANGLEFAN, DP_TRIANGLESTRIP, DP_POINTLIST, DP_LINELIST )
	virtual void SetPrimitiveType( DRAWPRIMITIVETYPE primtype ) = 0;

	/// Renders this surface with a shader
	/// \param override - pointer to an IEffect interface to override the geometries assigned IEffect
	/// \return - true on success
	virtual bool Render( IEffect * override = NULL ) = 0;
	
	/// Render given some scene settings, called when the geometry is used internally in an RO
	/// \param settings - pointer to an ISceneSettings interface
	/// \param override - pointer to an IEffect interface to override the geometries assigned IEffect
	/// \return - true on success
	virtual bool Render( ISceneSettings * settings, IEffect * override = NULL ) = 0;
	 
	/// render just the index buffer without setting textures( draw primitive );
	/// \return - true on success
	virtual bool DrawGeometry( void ) = 0;
	
	/// Gets the controller associated with this packet(for transformations)
	/// \return - pointer to an IController interface
	virtual IController * GetController( void ) = 0;

	/// Sets the controller associated with this packet(for transformations)
	/// \param controller - pointer to an IController interface
	virtual void SetController( IController * controller ) = 0;
	
	/// Gets the index buffer for pre allocated buffers, for example, we want to share a vertex pool
	/// \param alloc - a generic buffer allocation struct to be filled in
	virtual bool GetIndexBufferAllocation( BUFFERALLOCATIONSTRUCT &alloc ) = 0;

	/// Gets the vertex buffer for pre allocated buffers, for example, we want to share a vertex pool
	/// \param alloc - a generic buffer allocation struct to be filled in
	virtual bool GetVertexBufferAllocation( BUFFERALLOCATIONSTRUCT &alloc ) = 0;

	/// Sets the index buffer for pre allocated buffers, for example, we want to share a vertex pool
	/// \param alloc - a generic buffer allocation struct to be used
	virtual bool SetIndexBufferAllocation( BUFFERALLOCATIONSTRUCT &alloc ) = 0;
	
	/// Sets the vertex buffer for pre allocated buffers, for example, we want to share a vertex pool	
	/// \param alloc - a generic buffer allocation struct to be used
	virtual bool SetVertexBufferAllocation( BUFFERALLOCATIONSTRUCT &alloc ) = 0;

	/// get direct access to vertex buffer interface
	/// \return - pointer to an IVertextBufferObject interface
	virtual IVertexBufferObject* GetVertexBufferInterface( void ) = 0;

	/// get direct access to index buffer interface
	/// \return - pointer to an IVertextBufferObject interface
	virtual IIndexBuffer* GetIndexBufferInterface( void ) = 0;
	
	/// Attaches a light object to this Render object
	/// \param light - pointer to ILightObject interface
	/// \param index - index that this light goes in (default = -1)
	virtual void AttachLight( ILightObject * light, UINT index = -1 ) = 0;

	/// Signals a detach of all light objects
	virtual void DetachAllLights( void ) = 0;

	/// Resets the light count
	virtual void ResetLightCount( void ) = 0;

	/// Sets the scene settings for this geometry
	/// \param settings - pointer to ISceneSettings interface
	virtual void SetSceneSettings( ISceneSettings * settings ) = 0;

	/// Gets the scene settings for this geometry
	/// \return - pointer to ISceneSettings interface
	virtual ISceneSettings * GetSceneSettings( void ) = 0;
	
	/// Set the lightmap texture for this geometry
	/// \param texture - pointer to IBaseTextureObject interface
	virtual void SetLightmapTexture( IBaseTextureObject* texture ) = 0;

	/// Sets the lightmap uv coord stream
	/// \param uvstream - pointer to a float array 
	/// \param numelements - int number of floats in the array
	virtual void SetLightmapUVStream( float * uvstream, int numelements ) = 0;

	/// Releases this geometry's render items (buffers, etc)
	virtual void Release( void ) = 0;

	/// Sets a vec4 override for the material(typically for vertex / pixel shader constants)
	/// \param hsType = type of var to set (pix const, vert const, etc)
	/// \param hsName = name of var to set
	/// \param vecValue = value to set
	virtual void SetMaterialOverride(DWORD hsType, DWORD hsName, Vec4 vecValue) = 0;

	/// Sets a string override for the material(textures, renderstates)
	/// \param hsType = type of var to set (tex, render state, etc)
	/// \param hsName = name of var to set
	/// \param szValue = text value to set
	virtual void SetMaterialOverride(DWORD hsType, DWORD hsName, StdString szValue) = 0;

	/// Resets the material override, so the geometry renders with its original material
	virtual void ClearMaterialOverride( void ) = 0;

	/// Checks to see if the geometry has alpha or not
	/// \return true if alphablend enabled
	virtual bool HasAlpha( void ) = 0;
};

} //namespace ElementalEngine
#endif