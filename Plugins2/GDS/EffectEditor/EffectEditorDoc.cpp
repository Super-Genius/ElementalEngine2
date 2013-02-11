///============================================================================
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

#include "stdafx.h"
#include "EffectEditor.h"
#include "EffectEditorDoc.h"


// CEffectEditorDoc

IMPLEMENT_DYNCREATE(CEffectEditorDoc, CDocument)

CEffectEditorDoc::CEffectEditorDoc()
{
}

BOOL CEffectEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CEffectEditorDoc::~CEffectEditorDoc()
{
}


BEGIN_MESSAGE_MAP(CEffectEditorDoc, CDocument)
END_MESSAGE_MAP()


// CEffectEditorDoc diagnostics

#ifdef _DEBUG
void CEffectEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEffectEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CEffectEditorDoc serialization

void CEffectEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CEffectEditorDoc commands
