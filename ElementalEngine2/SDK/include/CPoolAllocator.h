///============================================================================
/// \file	CPoolAllocator
/// \brief  Pool allocator
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

#ifndef	_CPOOLALLOCATOR_H_
#define _CPOOLALLOCATOR_H_

// dlmalloc.h
#ifdef __cplusplus
extern "C" {
#endif
void* dlmalloc(size_t);
void  dlfree(void*);
#ifdef __cplusplus
};  /* end of extern "C" */
#endif

namespace ElementalEngine 
{

template <typename T> class CPoolAllocator;

// Specialized template for type void.
template <>
class CPoolAllocator<void>
{
public:
	typedef void* pointer;
	typedef const void* const_pointer;
	// reference to void members are impossible.
	typedef void value_type;
	template <class U> 
	struct rebind { typedef CPoolAllocator<U> other; };
};    

// The member function 'destroy()' needs to know whether to call the destructor or not.
// These helper functions uses the way C++ does function lookups in order to call the 
// destructor appropriately.
namespace pool_alloc {
	inline void destruct(char*) {}
	inline void destruct(wchar_t*) {}
	template <typename T>
	inline void destruct(T* t) { t->~T(); }
}

template <typename T>
class CPoolAllocator
{
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;

	template <class U> 
	struct rebind { typedef CPoolAllocator<U> other; };

	CPoolAllocator(){}
	pointer address(reference x) const {retuen &x;}
	const_pointer address(const_reference x) const {return &x;}
	pointer allocate(size_type size, CPoolAllocator<void>::const_pointer hint = 0)
	{
		return static_cast<pointer>(dlmalloc(size*sizeof(T)));
	}
	//for Dinkumware:
	char *_Charalloc(size_type n){return static_cast<char*>(dlmalloc(n));}
	// end Dinkumware

	template <class U> CPoolAllocator(const CPoolAllocator<U>&){}
	void deallocate(pointer p, size_type n)
	{
		dlfree(p);
	}
	void deallocate(void *p, size_type n)
	{
		dlfree(p);
	}
	size_type max_size() const throw() {return size_t(-1) / sizeof(value_type);}
	void construct(pointer p, const T& val)
	{
		new(static_cast<void*>(p)) T(val);
	}
	void construct(pointer p)
	{
		new(static_cast<void*>(p)) T();
	}
	void destroy(pointer p){pool_alloc::destruct(p);}
//	static void dump(){mem_.dump();};

private:
};

} //namespace ElementalEngine 

#endif // _CPOOLALLOCATOR_H_