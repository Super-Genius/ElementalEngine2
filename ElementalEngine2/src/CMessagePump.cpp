///==========================================================================
/// \file	CMessagePump.cpp
/// \brief	Generic message pump for in game  object communication
/// \date	2-23-2005
/// \author	Kyle Swaim
///==========================================================================

#include "StdAfx.h"
#include "CMessagePump.hpp"

CMessagePump::CMessagePump()
{
	// should add a null message on init tp prevent
	// unintended flipping
	m_ReadQue.clear();
	m_WriteQue.clear();
}

CMessagePump::~CMessagePump()
{
	m_ReadQue.clear();
	m_WriteQue.clear();
}

void CMessagePump::WriteMessage( CMessage message)
{
	m_WriteQue.push_back( message );
}

CMessage CMessagePump::ReadMessage()
{
	// If m_ReadQue is empty then it is time to
	// flip m_ReadQue and m_WriteQue
	if( m_ReadQue.empty() == true )
	{
		Flip();
	}

	CMessage message = m_ReadQue.front();
	m_ReadQue.pop_front();

	return message;
}

void CMessagePump::Update()
{
	Flip();
}

void CMessagePump::Flip()
{
	m_ReadQue.swap( m_WriteQue );
	m_WriteQue.clear();
	// add a NULL message to the write to prevent unintended flipping
}