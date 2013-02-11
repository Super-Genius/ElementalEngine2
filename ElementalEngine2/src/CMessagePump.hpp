///==========================================================================
/// \file	CMessagePump.hpp
/// \brief	Generic message pump for in game object comminucation
/// \date	2-23-2005
/// \author	Kyle Swaim
///==========================================================================

#ifndef	CMESSAGEPUMP_HPP
#define	CMESSAGEPUMP_HPP

#include <deque>
#include "CMessage.hpp"

using namespace std;

///==========================================================================
/// \class	CMessagePump
/// \brief	Generic message pump for in game object communication.
///			The pump is FIFO and reading a message also removes the message.
///==========================================================================
class CMessagePump
{
public:
	CMessagePump();
	~CMessagePump();

	/// \brief	Write a message to the message pump
	/// \param	message - The message being written to the message pump
	void WriteMessage( CMessage message );
	/// \brief	Read and remove the oldest message from the message pump
	/// \return	The oldest message in the message pump
	CMessage ReadMessage();

	/// \brief	Updates the message pump state
	///			call once per frame
	void Update();


private:
	/// \brief	Flip the read and write buffers
	void Flip();

	deque< CMessage > m_ReadQue;
	deque< CMessage > m_WriteQue;
};

#endif	// CMESSAGEPUMP_HPP