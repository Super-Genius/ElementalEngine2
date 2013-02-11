///============================================================================
/// \file		EditorEntity.h
/// \brief		Declaration of editor entity class
/// \date		03/20/2007
/// \author		Nick Tourte
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

#ifndef _C_EDITORENTITY_H_
#define _C_EDITORENTITY_H_

class CEditorEntity : public CObjectTemplate<CProxyObject>
{
protected:
	
	/// ctor
	CEditorEntity(IHashString *parentName, IHashString *name, bool bAddToHier);
public:

	/// dtor
	~CEditorEntity();

	/// Create func
	static IComponent *Create(int nArgs, va_list argptr);

	/// Overridden Functions piped to proxy object
	virtual void Serialize(IArchive &ar) { CProxyObject::Serialize(ar); }
	virtual void Init() { CProxyObject::Init(); }
	virtual void DeInit() { CProxyObject::DeInit(); }
	virtual void SetParentName(IHashString *inName) { CProxyObject::SetParentName(inName); }

	/// Sets the dirty transform flag (when an entity above calls this guy)
	/// \param size = 0
	/// \param param = NULL
	/// \return MSG_HANDLED_PROCEED
//	DWORD OnSetDirtyTransformFlag(DWORD size, void *param);

private:
	CHashString m_hsEntityType;
	bool m_bInHierarchy;
};

#endif // _C_EDITORENTITY_H_