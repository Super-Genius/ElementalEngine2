#ifndef IRENDERPACKET_H
#define IRENDERPACKET_H

#include "IController.h"

namespace ElementalEngine
{

class IRenderPacket
{
public:
	/// \brief Compare for sorting
	/// \param obj = object to compare to
	/// \param sortMethod = a sort method specifying how to compare(set in RENDEROBJECTCOMPARE)
	/// \param curRenderPass = the current render pass to sort to
	/// \return bool = must return true if (this pointer) < (obj)
	virtual bool IsLessThan( const IRenderObject * obj, DWORD sortMethod, int curRenderPass ) = 0;
	
	/// \brief This is for render priorities between classes
	/// will change as soon as a new architecture can be designed to better incorporate
	/// different renderobject types being pushed on the same sorted list
	/// \return DWORD = the priority key this returns, the higher the number,
	/// the higher its priority
	virtual DWORD GetRenderPriority() = 0;

	/// \brief Gets the base sort class which implements IRenderPacket::IsLessThan()
	/// child classes must not re-implement IsLessThan() if they return the BaseSortClass hash!!
	/// typically, each render object has a IsLessThan() and returns a unique BaseSortClass
	virtual IHashString * GetBaseSortClass() = 0;

	/// \brief returns if this renderobject needs to be sorted in the transparency list
	/// \return bool = true if this object uses transparency
	virtual bool IsAlphaObject() = 0;

	/// \brief Render the renderobject
	/// \return true if object was rendered, false otherwise
	virtual bool Render( ) = 0;
	

};

} //namespace ElementalEngine

#endif