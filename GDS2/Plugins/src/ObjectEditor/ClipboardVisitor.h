///============================================================================
/// \file		ClipboardVisitor.h
/// \brief		Header file for Visitor patter to manipulate hierarchical object in a clipboard
/// \date		04-02-2007
/// \author		Brian Bazyk
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

#ifndef _CLIPBOARDVISITOR_H_
#define _CLIPBOARDVISITOR_H_

struct CLIPBOARDOBJECT
{
	CHashString szName;
	CHashString szParent;
	CHashString szComponentType;
	IArchive *pArchive;
};

typedef list<CLIPBOARDOBJECT> CLIPBOARDOBJECTLIST;

class CClipboardVisitor : public IVisitor
{
public:
	CClipboardVisitor();
	~CClipboardVisitor();
	void Reset();
	inline CLIPBOARDOBJECTLIST& GetObjectList() { return m_ObjectList; }
	/// tells the callee that we want to visit depth first
	bool IsHierarchicalVisitor();
	bool Visit( IComponent * component, bool bVisitEnter);

private:
	IToolBox *m_ToolBox;
	CLIPBOARDOBJECTLIST m_ObjectList;
};


#endif // #ifndef _SECTORRENAMEVISITOR_H_
