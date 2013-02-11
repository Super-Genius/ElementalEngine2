///============================================================================
/// \file	IAnimatedTexture.h
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

#ifndef IANIMATEDTEXTUREOBJECT_H
#define IANIMATEDTEXTUREOBJECT_H

namespace ElementalEngine
{
/// IAnimatedTextureObject interface (inherits from IBaseTextureObject)
class IAnimatedTextureObject: public IBaseTextureObject
{
public:
	/// Destructor
	virtual ~IAnimatedTextureObject() {};

	/// Read a particular frame from an animated texture
	/// \param frame - unsigned int indicating the frame to read
	/// \return - void pointer to the texture data
	virtual void* Read( UINT frame) = 0;

	/// Write a particular frame to an animated texture
	/// \param frame - unsigned int indicating the frame to write
	/// \param p - void pointer to the texture data
	/// \return - true if success
	virtual bool Write( UINT frame, void *p ) = 0;

	/// Locks the texture and returns the data
	/// \param frame - unsigned int indicating the frame to read
	/// \param pitch - pointer to int 
	/// \return - void pointer to the texture data
    virtual void * Lock( UINT frame, int * pitch ) = 0;

	/// Unlocks the texture
	virtual void Unlock( void ) = 0;

	/// Gets the number of frames for the texture
	/// \return - unsigned int of the number of frames for the texture
	virtual UINT GetNumFrames( void ) = 0;

	/// Gets a particular portion of a frame from the texture
	/// \param frame - unsigned int of the frame to read
	/// \param u1 - U portion of starting coordinate
	/// \param v1 - V portion of starting coordinate
	/// \param u2 - U portion of ending coordinate
	/// \param v2 - V portion of ending coordinate
	/// \param out - pointer to a pointer to an ITextureObject interface of the resulting texture
	virtual void GetFrame( UINT frame, float &u1, float &v1, float &u2, float &v2, ITextureObject **out  ) = 0;

	/// Gets the frame rate of this texture
	/// \return - float indicating the frame rate
	virtual float GetFrameRate( void ) = 0;
};

}	//namespace ElementalEngine

#endif