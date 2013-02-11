///============================================================================
/// \file		StateDiagramEntity.h
/// \brief		Header file for State Machine Editor state diagram entity
/// \date		03-16-2007
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

#ifndef __STATEDIAGRAMENTITY_H__
#define __STATEDIAGRAMENTITY_H__

class CStateMachineDiagramEditor;

/////////////////// CStateDiagramEntity ////////////////////////////////////////
class CStateDiagramEntity : public CSimpleDiagramEntity
{
public:
	enum StateType
	{
		Normal,
		Start,
		End
	};

	CStateDiagramEntity( CDiagramEntity *parentEntity, IHashString *pStateMachineName );
	virtual ~CStateDiagramEntity();

	virtual CDiagramEntity*	Clone();

	virtual void	Draw( CDC* dc, CRect rect );

	virtual CSize	GetMinimumSize() const;

	virtual void	ShowPopup( CPoint point, CWnd* parent );
	virtual BOOL	DoMessage( UINT msg, CDiagramEntity* sender, CWnd* from );

	///	\brief	change entity parent to passed name
	void SetParentName( IHashString *newParentName, CStateMachineDiagramEditor* pEditor );

	///	\brief	save current object state after changes in UI
	virtual void	SaveProperties();
	///	\brief	read current object state and refresh visualization properties
	virtual void	LoadProperties();

	void SetStateType( StateType type );

	BOOL AddEntryEvent( CStateMachineDiagramEditor *editor );
	BOOL AddUpdateEvent( CStateMachineDiagramEditor *editor );
	BOOL AddExitEvent( CStateMachineDiagramEditor *editor );

protected:
	virtual LPCTSTR *GetValueNames() const;

private:
	// process "Up in Hierarchy" context menu item command
	BOOL UpInHiearchy( CStateMachineDiagramEditor *editor );
	// process "Add ... Event" context menu item commands
	BOOL AddEvent( CStateMachineDiagramEditor *editor, LPCTSTR parameterTag, LPCTSTR eventSuffix );

	static CPen *GetStartStatePen();
	static CPen *GetEndStatePen();

private:
	CHashString	m_hszStateMachineName;
	StateType m_StateType;
};

/////////////////// CFlatStateModeDiagramEntity ////////////////////////////////
class CFlatStateModeDiagramEntity : public CStateDiagramEntity
{
public:
	CFlatStateModeDiagramEntity( CDiagramEntity *parentEntity, 
								 IHashString *pStateMachineName );

	virtual void SelectionEdit( CWnd* parent );

	virtual LPCTSTR *GetValueNames() const;
};

#endif // #ifndef __STATEDIAGRAMENTITY_H__