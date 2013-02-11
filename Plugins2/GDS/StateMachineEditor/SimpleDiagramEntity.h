///============================================================================
/// \file	SimpleDiagramEntity.h
/// \brief	Declaration for State Machine Editor Simple diagram entity
/// \date	12/05/2007
/// \author	Andrey Ivanov
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
#ifndef __SIMPLE_DIAGRAM_ENTITY_H__
#define __SIMPLE_DIAGRAM_ENTITY_H__

class CSimpleDiagramEntity : public CDiagramEntity
{
protected:
	enum
	{
		LIGHT_LIGHT_GRAY = 0xF0F0F0,
		LIGHT_GRAY = 0xD3D3D3,
		GRAY = 0x808080,
		BLACK = 0,
	};
public:
	CSimpleDiagramEntity( CDiagramEntity *parentEntity );
	virtual ~CSimpleDiagramEntity();

	virtual CDiagramEntity*	Clone();

	virtual void	Draw( CDC* dc, CRect rect );

	virtual void	SetName( CString name );
	virtual void	SetRect( double left, double top, double right, double bottom );

	virtual BOOL	DoMessage( UINT msg, CDiagramEntity* sender, CWnd* from );

	virtual void	ShowProperties();
	
	virtual IAttributeObject *GetAttributeObject();

	virtual void	SaveProperties();
	///	\brief	read current object state and refresh visualization properties
	virtual void LoadProperties();

	///	this function refreshes all entities with passed labels. If old label is
	///	equal to new label then this means that state references should now be 
	///	updated. Nevertheless object should update own state based on passed label
	/// because user may select to not update references
	///	\param	oldLabel - old state label
	///	\param	newLabel - new state label.
	virtual void OnStateUpdate( const StdString &oldLabel, const StdString &newLabel ) {};

	///	check if object refers to state with passed label
	///	\param	label - label of the state
	///	\return	true if object refers to the state
	virtual bool IsStateReferred( const StdString &label ) const
	{
		return false;
	}

	bool IsAbstract() const
	{
		return m_bAstract;
	}

protected:
	CPen *GetPen() const;
	CBrush *GetBrush() const;
	COLORREF GetColor() const;
	CFont *GetFont() const;

	static HFONT CreateBaseFont( bool italic );

protected:
	static IToolBox *m_ToolBox;
	bool m_bAstract;
};

#endif // #ifndef __SIMPLE_DIAGRAM_ENTITY_H__