///============================================================================
/// \file		SplineComponent.cpp
/// \brief		Header file for Spline Component
/// \date		11-14-2006
/// \author		Richard Nguyen
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

#ifndef _SPLINECOMPONENT_HPP_
#define _SPLINECOMPONENT_HPP_

class CSplineEditor;

/// class so we can hook into messaging system of Elemental Engine
class CSplineComponent : public IComponent
{
	SINGLETON(CSplineComponent);
private:
	CSplineEditor *m_Parent;
public:
	/// Set Parent Item for Editor
	/// \param parent - parent item
	/// \return void
	void SetParent(CSplineEditor *parent);
	/// Create Editor Component
	/// \param argptr - list or arguments for creation
	/// \return newly created IComponent or singleton instance
	static IComponent *Create(int nArgs, va_list argptr);
	/// Serialize object
	/// \param ar - archive to use for serialization
	/// \return void
	void Serialize(IArchive &ar);
	/// Get the component type
	/// \param void
	/// \return IHashString pointer to component type
	IHashString *GetComponentType();
	/// Tells us if the component is a type of the given component
	/// \param compType - component type to compare
	/// \return true if it is, false if not
	bool IsKindOf(IHashString *compType);
	
	// ----------- Messages Functions ---------------//
	DWORD OnItemDropped(DWORD size, void *param);
	DWORD OnLoadSplineEditorItem(DWORD size, void *param);
	DWORD OnCreate(DWORD size, void *param);
};

#endif // #ifndef _SPLINECOMPONENT_HPP_
