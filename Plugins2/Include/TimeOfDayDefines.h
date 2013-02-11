///============================================================================
/// \file		TimeOfDayDefines.h
/// \brief		Header file for Time of Day Defines
/// \date		09-21-2005
/// \author		Josh Lulewicz
/// \note		Copyright (c) 2005 Signature Devices, Inc.
///============================================================================

#ifndef _TIMEOFDAYDEFINES_H
#define _TIMEOFDAYDEFINES_H

struct SUNLIGHTKEYFRAME
{
	StdString m_wszName;
	Vec3 m_AmbientColor;
	Vec3 m_FullbrightColor;
	float m_fDawnWeight;
	float m_fNightWeight;
};
typedef std::vector<SUNLIGHTKEYFRAME> SUNLIGHTKEYFRAMELIST;

struct FOGKEYFRAME
{
	StdString m_wszName;
	Vec3 m_FogColor;
	float m_fFogStart;
	float m_fFogEnd;
};
typedef std::vector<FOGKEYFRAME> FOGKEYFRAMELIST;

typedef struct TODFRAMESSTRUCT
{
	SUNLIGHTKEYFRAMELIST *sunList;
	FOGKEYFRAMELIST *fogList;
} TODFRAMESSTRUCT;



#endif // #ifndef _TIMEOFDAYDEFINES_H