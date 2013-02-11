//---------------------------------------------------------------------
// File:		Singleton.h
// Description:	template for Singleton objects
// Created:		11/30/03
// Author:		Kenneth L. Hurley
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
//---------------------------------------------------------------------

#ifndef SINGLETON_H
#define SINGLETON_H

//
// this code is so that object can register themselves with
// the system.
//
template <class T> 
class CSingleton 
{ 

public: 
	static T *Instance() 
	{ 
		static T _instance; 
		return &_instance; 
	}; 

private: 
	CSingleton() {}; 
	~CSingleton() {}; 
}; 

#define SINGLETONINSTANCE(T)						\
	CSingleton< T >::Instance()

#define SINGLETON(T)								\
	protected:										\
		friend class CSingleton< T >;				\
		T(){};

#define SINGLETONCONSTRUCTOROVERRIDE(T)				\
	protected:										\
		friend class CSingleton< T >;

#endif	// #ifndef SINGLETON_H
