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

#ifndef USERALLOCATOR_H
#define USERALLOCATOR_H

class UserAllocator : public NxUserAllocator
{
public:
						UserAllocator();
	virtual				~UserAllocator();

			void		reset();

			void*		malloc(NxU32 size);
			void*		malloc(NxU32 size, NxMemoryType type);
			void*		mallocDEBUG(NxU32 size, const char* file, int line);
			void*		mallocDEBUG(NxU32 size, const char* file, int line, const char* className, NxMemoryType type);
			void*		realloc(void* memory, NxU32 size);
			void		free(void* memory);

			NxU32*		mMemBlockList;
			NxU32		mMemBlockListSize;
			NxU32		mMemBlockFirstFree;
			NxU32		mMemBlockUsed;

			NxI32		mNbAllocatedBytes;
			NxI32		mHighWaterMark;
			NxI32		mTotalNbAllocs;
			NxI32		mNbAllocs;
			NxI32		mNbReallocs;
};

#endif  // USERALLOCATOR_H
