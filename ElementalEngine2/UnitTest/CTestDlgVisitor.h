///============================================================================
/// \file		CTestDlgVisitor.h
/// \brief		Header file for visitor pattern to heirarchy
/// \date		03-27-2005
/// \author		Kenneth Hurley
/// \note		Copyright (c) 2005 Signature Devices, Inc.
///============================================================================
#ifndef _CTESTDLGVISTISTOR_H_
#define _CTESTDLGVISTISTOR_H_

#include <assert.h>
typedef map<DWORD, HTREEITEM> IDTOTREEMAP;

class CTestDlgVisitor : public IVisitor
{
public:
	CTestDlgVisitor(CTreeCtrl *treeCtrl, CRichEditCtrl *output)
	{
		m_TreeControl = treeCtrl;
		m_REditBox = output;
	}

	void Reset()
	{
		m_NameToTreeMap.clear();
	}

	bool IsHierarchicalVisitor( void )
	{
		return false;
	}

	bool Visit( IComponent * component, bool bVisitEnter )
	{
		IObject *theObject;
		IHashString *name;
		IHashString *parentName;
		HTREEITEM parentHTI;
		std::string str;

		theObject = dynamic_cast<IObject *>(component);
		assert(theObject);
		name = theObject->GetName();
		parentName = theObject->GetParentName();
		static CHashString nullHash(_T("NULL"));
		if (name == NULL)
		{
			name = &nullHash;
		}

		// default to root
		parentHTI = TVI_ROOT;

		// find base object to add to
		if (parentName != NULL)
		{
			IDTOTREEMAP::iterator nttmIter;
			nttmIter = m_NameToTreeMap.find(parentName->GetUniqueID());
			if (nttmIter == m_NameToTreeMap.end())
			{
				str = _T("Couldn't find ");
				str +=  parentName->GetString();
				str += _T(" in tree map\n");
				m_REditBox->ReplaceSel(str.c_str());
				return false;
			}

			parentHTI = nttmIter->second;
		}

		parentHTI = m_TreeControl->InsertItem(name->GetString(), parentHTI, TVI_LAST);
		m_NameToTreeMap[name->GetUniqueID()] = parentHTI;

		return true;
	}

private:
	CTreeCtrl *m_TreeControl;
	CRichEditCtrl *m_REditBox;
	IDTOTREEMAP m_NameToTreeMap;
};

#endif		// #ifndef _CTESTDLGVISTISTOR_H_
