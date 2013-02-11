///==========================================================================
/// \file	CMessage.hpp
/// \brief	Message class
/// \date	2-23-2005
/// \author	Kyle Swaim
///==========================================================================

#ifndef	CMESSAGE_HPP
#define	CMESSAGE_HPP

///==========================================================================
/// \class	CMessage
/// \brief	Message class
///==========================================================================
class CMessage
{
public:
	/// \brief	Default constuctor
	CMessage();
	/// \brief	Destructor
	~CMessage();

	/// \brief	Set the message id
	/// \param	id - The id portion of the message
	void SetId( DWORD id );
	/// \brief	Set the message portion of the message
	/// \param	message - The message portion of the message
	void SetMessage( DWORD message );

	/// \brief	Return the full message id
	/// \return	The full message id
	DWORD GetId();
	/// \brief	Return the object id portion of the id
	/// \return	The object id portion of the id
	WORD GetObjectId();
	/// \brief	Return the message id portion of the id
	/// \return	The message id portion of the id
	WORD GetMessageId();
	/// \brief	Return the message
	/// \return	The message
	DWORD GetMessage();

private:
	DWORD m_Id;
	DWORD m_Message;
};

#endif	// CMESSAGE_HPP