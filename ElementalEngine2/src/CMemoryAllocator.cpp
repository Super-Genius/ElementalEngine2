///=====================================================================
/// \file	CMemoryAllocator.cpp
/// \brief	Implementation of CMemoryAllocator Class
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

#include "StdAfx.h"
#include "CMemoryAllocator.h"

REGISTER_COMPONENT_SINGLETON(CMemoryAllocator);

IComponent *CMemoryAllocator::Create(int nArgs, va_list argptr)
{
	return SINGLETONINSTANCE(CMemoryAllocator);
}

CMemoryAllocator::CMemoryAllocator() : m_ComponentType(CHashString(_T("CMemoryAllocator")))
{
	m_ToolBox = EngineGetToolBox();
}

CMemoryAllocator::~CMemoryAllocator()
{
	m_ToolBox->UnRegisterComponent( &m_ComponentType );
}

void CMemoryAllocator::Serialize(IArchive &ar)
{
}

IHashString *CMemoryAllocator::GetComponentType()
{
	return &m_ComponentType;
}

bool CMemoryAllocator::IsKindOf(IHashString *compType)
{
  return (compType->GetUniqueID() == m_ComponentType.GetUniqueID());
}

void* CMemoryAllocator::Malloc( size_t bytes )
{
	bytes += 2;
	void *p = dlmalloc(bytes);
	char *c = (char*)p;
	c[0] = 'h';
	c[1] = 'i';
	p = c + 2;
	return p;
}

void CMemoryAllocator::Free( void *p )
{
	char *c = (char*)p - 2;
	assert( c[0] == 'h' );
	assert( c[1] == 'i' );
	dlfree(c);
}
