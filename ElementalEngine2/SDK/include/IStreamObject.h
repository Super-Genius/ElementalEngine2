///============================================================================
/// \file	IStreamObject.h
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

#ifndef ISTREAMOBJECT_H
#define ISTREAMOBJECT_H

class IStreamObject
{
public:
	virtual ~IStreamObject() {};
	virtual int GetStride() = 0;
	virtual int GetNumElements() = 0;
	virtual void Alloc( const int in_elements, const int in_stride )= 0;
	virtual void Free() = 0;
	virtual void * GetData() = 0;
};

#endif