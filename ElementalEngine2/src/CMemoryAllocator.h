///=====================================================================
/// \file	CMemoryAllocator.h
/// \brief	HEader for CMemoryAllocator Class
/// \date	10/15/2007
/// \author	Brian Bazyk
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
///=====================================================================

#ifndef _CMEMORYALLOCATOR_H__
#define _CMEMORYALLOCATOR_H__

#include "EEMemory.h"

namespace ElementalEngine 
{

class CMemoryAllocator : public IMemoryAllocator
{
	SINGLETONCONSTRUCTOROVERRIDE(CMemoryAllocator);
public:
	static IComponent *Create(int nArgs, va_list argptr);

	CMemoryAllocator();
	~CMemoryAllocator();
	virtual void Serialize(IArchive &ar);
	virtual IHashString *GetComponentType();
	virtual bool IsKindOf(IHashString *compType);

	virtual void* Malloc( size_t bytes );
	virtual void Free( void *p );

protected:
	// cached toolbox access
	IToolBox *m_ToolBox;
	// component name
	CHashString m_ComponentType;
};

} //namespace ElementalEngine 
#endif