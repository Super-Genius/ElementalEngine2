///============================================================================
/// \file		EventDiagramEntity.h
/// \brief		Header file for State Machine Editor event diagram entity
/// \date		06-20-2007
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

#ifndef __EVENTDIAGRAMENTITY_H__
#define __EVENTDIAGRAMENTITY_H__

class CEventDiagramEntity : public CSimpleDiagramEntity
{
public:
	CEventDiagramEntity( CDiagramEntity *parentEntity );
	virtual ~CEventDiagramEntity();

	virtual CDiagramEntity*	Clone();

	virtual void	Draw( CDC* dc, CRect rect );

	virtual CSize	GetMinimumSize() const;

	virtual void	ShowPopup( CPoint point, CWnd* parent );
	virtual BOOL	DoMessage( UINT msg, CDiagramEntity* sender, CWnd* from );

	virtual void	SelectionEdit( CWnd* parent );
	virtual void	ShowPropertiesDialog( CWnd* parent, BOOL show = TRUE );
	virtual void	SaveProperties();
	///	\brief	read current object state and refresh visualization properties
	///	\param	name - name of state for serialize
	virtual void	LoadProperties();
private:
	static CBrush *GetDefaultBrush();

	static StdString GetActionName(IHashString *pEEName);
	static StdString GetTransitionName(IHashString *pEEName);

	///	\brief	collect event labels for event's parent state
	///	\param	labels - output list with event labels
	void GetOverrideEventLabels( vector<StdString> &labels ) const;
	///	\brief	collect state labels for current event
	///	\param	labels - output list with state labels
	void GetStateLabels( vector<StdString> &labels ) const;

private:
	vector<StdString>	m_Actions;
};

#endif // #ifndef __EVENTDIAGRAMENTITY_H__