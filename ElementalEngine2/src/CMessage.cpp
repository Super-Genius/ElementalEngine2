///==========================================================================
/// \file	CMessage.cpp
/// \brief	Message class
/// \date	2-23-2005
/// \author	Kyle Swaim
///==========================================================================

#include "StdAfx.h"
#include "CMessage.hpp"

CMessage::CMessage()
{
	m_Id		= 0;
	m_Message	= 0;
}

CMessage::~CMessage()
{
}

void CMessage::SetId( DWORD id )
{
	m_Id = id;
}

void CMessage::SetMessage( DWORD message )
{
	m_Message = message;
}

DWORD CMessage::GetId()
{
	return m_Id;
}

WORD CMessage::GetObjectId()
{
	return LOWORD( m_Id );
}

WORD CMessage::GetMessageId()
{
	return HIWORD( m_Id );
}

DWORD CMessage::GetMessage()
{
	return m_Message;
}