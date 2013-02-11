///==========================================================================
/// \file	IScene.h
/// \brief	Scene Interface
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
///==========================================================================

#ifndef	ISCENE_H
#define	ISCENE_H

namespace ElementalEngine
{

class IScene : public IObject
{
public:
	virtual ~IScene() { }

	virtual void UpdateObject( ISceneObject *pObject ) = 0;
	virtual void RemoveGlobalObject( ISceneObject* object ) = 0;
};

} //namespace ElementalEngine

#endif	// ISCENE_H