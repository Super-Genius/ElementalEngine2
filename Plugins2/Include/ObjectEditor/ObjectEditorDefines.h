///============================================================================
/// \file		ObjectEditorDefines.h
/// \brief		Header file for a Object Editor Defines
/// \date		02-13-2007
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

#ifndef	OBJECTEDITORDEFINES_H
#define	OBJECTEDITORDEFINES_H

struct EDITORMOVEOBJECTPARAMS
{
	IHashString *pObjectName;
	Vec3 vecMove;
	bool bUseMoveAccum;
};

enum MANIPULATE_STATE
{
	MANIPULATE_SELECT,
	MANIPULATE_MOVE,
	MANIPULATE_ROTATE,
	MANIPULATE_SCALE,
};

enum LOCK_STATE
{
	LOCK_NONE,
	LOCK_XY,
	LOCK_XZ,
	LOCK_YZ,
	LOCK_X,
	LOCK_Y,
	LOCK_Z
};

/// Parameters for EditorGetObjectToolConfig message.
struct EDITORGETOBJECTTOOLCONFIGPARAMS
{
	/// True if object tool is active.
	bool bActive;
	/// Object manipulate state.
	MANIPULATE_STATE objectManipulateState;
	/// Object manipulation lock state.
	LOCK_STATE objectLockState;
};

/// Flag specifies which object tool properties should be modified.
enum OBJECT_TOOL_CONFIG_MODIFY_FLAGS
{
//	OTCONFIG_MODIFY_ACTIVE = (1 << 1),   // not implemented yet
	OTCONFIG_MODIFY_MANIPULATE_STATE = (1 << 2),
	OTCONFIG_MODIFY_LOCK_STATE = (1 << 3)
};

/// Parameters for EditorSetObjectToolConfig message.
struct EDITORSETOBJECTTOOLCONFIGPARAMS
{
	/// Sum of flags specifies which object tool configuration properties
	/// should be modified. (see OBJECT_TOOL_CONFIG_MODIFY_FLAGS)
	int modifyFlags;
//	/// True if object tool is active.
//	bool bActive;
	/// Object manipulate state.
	MANIPULATE_STATE objectManipulateState;
	/// Object manipulation lock state.
	LOCK_STATE objectLockState;
};

#endif
