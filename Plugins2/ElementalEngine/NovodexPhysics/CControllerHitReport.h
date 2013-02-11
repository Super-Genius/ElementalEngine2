///============================================================================
/// \file	CControllerHitReport.h
/// \brief	Callback report for collisions
/// \date	10-30-2006
/// \author	Kyle Swaim
/// \note	Copyright (c) 2005-2006 Signature Devices, Inc.
///============================================================================

#ifndef CCONTROLLERHITREPORT_H
#define CCONTROLLERHITREPORT_H

///============================================================================
/// \class	CControllerHitReport
/// \brief	Callback report for collisions
///============================================================================
class CControllerHitReport : public NxUserControllerHitReport
{
public:
	CControllerHitReport();

	virtual ~CControllerHitReport();

public:
	virtual NxControllerAction onShapeHit( const NxControllerShapeHit& hit );
	virtual NxControllerAction onControllerHit( const NxControllersHit& hit );

private:
	IToolBox* m_ToolBox;
};

#endif	// CCONTROLLERHITREPORT_H