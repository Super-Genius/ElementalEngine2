///============================================================================
/// \file	TransitionDiagramEntity.h
/// \brief	Declaration for State Machine Editor Transition diagram entity
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
#ifndef __TRANSITION_DIAGRAM_ENTITY_H__
#define __TRANSITION_DIAGRAM_ENTITY_H__

/////////////////// CBaseTransitionDiagramEntity ///////////////////////////////
class CBaseTransitionDiagramEntity : public CSimpleDiagramEntity
{
	// helper class with description of lines' endpoints
	struct CLine
	{
		CLine() : srcX( 0 ), srcY( 0 ), destX( 0 ), destY( 0 )
		{
		}

		CLine operator *( double zoom ) const
		{
			CLine res( *this );
			res.srcX *= zoom;
			res.srcY *= zoom;
			res.destX *= zoom;
			res.destY *= zoom;
			return res;
		}

		double srcX;
		double srcY;
		double destX;
		double destY;
	};

public:
	CBaseTransitionDiagramEntity( CDiagramEntity *parentEntity );
	virtual ~CBaseTransitionDiagramEntity();

	virtual CDiagramEntity*	Clone();

	virtual CGdiObject *SetPen( CDC* dc ) const;
	virtual CGdiObject *SetBrush( CDC* dc, bool source ) const;
	virtual void	DrawObject( CDC* dc, double zoom );

	virtual CRect	GetRect() const;
	virtual int		GetHitCode( CPoint point ) const;
	virtual int		GetHitCode( const CPoint& point, const CRect& rect ) const;
	virtual BOOL	BodyInRect( CRect rect ) const;
	virtual HCURSOR	GetCursor( int hit ) const;

	virtual void	ShowPopup( CPoint point, CWnd* parent );
	virtual BOOL	DoMessage( UINT msg, CDiagramEntity* sender, CWnd* from );

	virtual void	SelectionEdit( CWnd* parent );

	///	\brief	get pointer to source entity for transition line drawing
	virtual CDiagramEntity *GetSource() const;
	///	\brief	get pointer to target entity for transition line drawing
	virtual CDiagramEntity *GetTarget() const;
	///	\breif	return virtual screen height
	virtual int GetVirtualHeight() const;

private:
	CLine CalcLineRect() const;
	static double CalculateLength( const CLine &line );

protected:
	static CHashString m_hszTransitionType;
	int	m_iWeight;
};

/////////////////// CDrivenTransitionDiagramEntity /////////////////////////////
class CDrivenTransitionDiagramEntity : public CBaseTransitionDiagramEntity
{
public:
	CDrivenTransitionDiagramEntity( CDiagramEntity *parentEntity,
									CStateMachineDiagramEditor *pEditor,
									CDiagramEntity *source, bool sourceInView, 
									CDiagramEntity *target, bool targetInView );

	virtual ~CDrivenTransitionDiagramEntity();

	virtual CGdiObject *SetBrush( CDC* dc, bool source ) const;
	///	\brief	get pointer to source entity for transition line drawing
	virtual CDiagramEntity *GetSource() const;
	///	\brief	get pointer to target entity for transition line drawing
	virtual CDiagramEntity *GetTarget() const;
	///	\breif	return virtual screen height
	virtual int GetVirtualHeight() const;

private:
	CStateMachineDiagramEditor *m_pEditor;
	///	pointers to source event and target state
	///	one of these pointer may be NULL (but not both)
	CDiagramEntity *m_pSource;
	CDiagramEntity *m_pTarget;
	bool	m_bSourceInView;
	bool	m_bTargetInView;
};

/////////////////// CTransitionDiagramEntity ///////////////////////////////
///	This class matches the real transition object in EE hierarchy. It manages 
///	fake drawing entities, but never draws itself because we do not expand event
///	content
class CTransitionDiagramEntity : public CBaseTransitionDiagramEntity
{
public:
	CTransitionDiagramEntity( CDiagramEntity *parentEntity, CStateMachineDiagramEditor *pEditor );
	virtual ~CTransitionDiagramEntity();

	///	\brief	read current object state and refresh visualization properties
	virtual void	LoadProperties();

	void RemoveDerived( CDrivenTransitionDiagramEntity *pEntity );

	///	this function refreshes all entities with passed labels. If old label is
	///	equal to new label then this means that state references should now be 
	///	updated. Nevertheless object should update own state based on passed label
	/// because user may select to not update references
	///	\param	oldLabel - old state label
	///	\param	newLabel - new state label.
	virtual void OnStateUpdate( const StdString &oldLabel, const StdString &newLabel );

	///	check if object refers to state with passed label
	///	\param	label - label of the state
	///	\return	true if object refers to the state
	virtual bool IsStateReferred( const StdString &label ) const;

private:
	void ReleaseDrivenEntities();
	void UpdateDrivenEntities();

	///	\brief	look up hierarchy for parent state which matches passed parent
	///	\param	pParent	- pointer to parent of the desired view		
	///	\param	pEntity - pointer to entity to look up
	///	\return	pointer to entity which has pParent as parent and pEntity as child
	///			NULL if such entity does not exist
	static CDiagramEntity *GetEntitiyInView( CDiagramEntity *pParent, CDiagramEntity *pEntity );

private:
	CStateMachineDiagramEditor *m_pEditor;
	StdString m_targetState;
	CBaseTransitionDiagramEntity *m_pDrivenEntity1;
	CBaseTransitionDiagramEntity *m_pDrivenEntity2;
	CBaseTransitionDiagramEntity *m_pDrivenEntity3;
};

/////////////////// CDraggingTransitionDiagramEntity ///////////////////////////
class CDraggingTransitionDiagramEntity : public CBaseTransitionDiagramEntity
{
public:
	CDraggingTransitionDiagramEntity( CDiagramEntity *parentEntity, 
									  CDiagramEntity *source, CDiagramEntity *target);

	virtual CGdiObject *SetPen( CDC* dc ) const;
	virtual CGdiObject *SetBrush( CDC* dc, bool bInView ) const;
	///	\brief	get pointer to source entity for transition line drawing
	virtual CDiagramEntity *GetSource() const;
	///	\brief	get pointer to target entity for transition line drawing
	virtual CDiagramEntity *GetTarget() const;
private:
	///	pointers to source event and target state
	///	one of these pointer may be NULL (but not both)
	CDiagramEntity *m_pSource;
	CDiagramEntity *m_pTarget;
};

#endif // #ifndef __TRANSITION_DIAGRAM_ENTITY_H__