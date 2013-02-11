///============================================================================
/// \file		ViewObject.h
/// \brief		Stores the structure data for a different Items
/// \date		06-13-2005
/// \author		Halbert Nakagawa
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

#ifndef VIEWOBJECT_H
#define VIEWOBJECT_H

class ViewFormatObject
{
public:

	virtual ~ViewFormatObject() {}

	virtual StdString& GetName() { return m_szName; }
	virtual StdString& GetBasicType() { return m_szBasicType; }
	virtual StdString& GetViewType() { return m_szViewType; }
	virtual bool IsKey() { return m_bIsKey; }
	virtual void SetName(StdString &name) { m_szName = name; }
	virtual void SetType(StdString &type) { m_szBasicType = type; }
	virtual void SetKey(bool val) { m_bIsKey = val; }

protected:
	StdString	m_szName;
	StdString	m_szBasicType;
	StdString	m_szViewType;
	bool m_bIsKey;

};

typedef list<ViewFormatObject *> VIEWOBJECTLIST;
typedef map<StdString, VIEWOBJECTLIST *> COMPTYPETOVOLISTMAP;

#endif