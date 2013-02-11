#pragma once

class CPYDLLEngine;

// comment this out to prevent pre-allocation
#define PREALLOCATE_DATA_SPACE

// Set this to just above the general size of your most frequently heavy requests
// probably stats or login.
#define DATA_PREALLOCATION_AMOUNT 128

// its actually MAX_UINT16 minus the header sizes and stuff
// but we'll give a ~500 byte breathing room for the encryption stuff.
#define MAXIMUM_MESSAGE_SIZE 65000

#define MAGIC_HEADER "PHAT"

#pragma pack(push,1)

typedef struct
{
	char			m_szMAGIC[4]; // "PHAT"
	unsigned long	m_ulType;
	unsigned long	m_ulSizeOfDataBlob;
	unsigned char	m_ucCrypto;
} s_PYPacketHeader;

#pragma pack( pop)

// This class encapsulates one request item
// Its either currently living in the queue, or it is the active request
// or its in the "done list"
class CRequestItem
{
private:
	CRequestItem(void) { ; } // disallow allow blank construction from the outside (ie, by the STL)
	CRequestItem(const CRequestItem &target) {; } // disallow copy construction (ie, by the STL)

	// converts milliseconds to TIMEVAL stucture which is seconds and microseconds.
	static timeval UTIL_MSToTimeVal(const unsigned long milliseconds);
	// safely send data on a given socket.  Sends it all until there's nothing left to send or an error occured.
	bool UTIL_SafeSend(const unsigned char * const data,const unsigned long datalength);
	bool UTIL_SafeRecv(unsigned char * const data,const unsigned long datalength);
	bool UTIL_CreateFreshSocket(SOCKET &outputsocket,const SOCKADDR_IN &localSocketAddr,const unsigned long connectionTimeoutMS);
protected:
	volatile eRequestStatus m_status;
	volatile bool m_bGarbage;
	volatile SOCKET m_ConnectionSocket;

	unsigned char m_ucEncryptionKeys[32]; // 16 bytes (128 bits) for key, 16 bytes for IV;
	unsigned long m_ulRequestID;
	bool m_bUsingEncryption;
	unsigned long m_ulRequestType;
	CPYDLLEngine *m_pParent;
	unsigned char *m_pUserData;
	unsigned long m_ulUserDataLength;
	unsigned long m_iRefCount;
	time_t m_LastTouchTime;
	std::vector<unsigned char> m_Data;
	std::vector<unsigned char> m_ResponseData;
	std::vector<unsigned char>::iterator m_ResponseReader;

	
	char m_MagicHeader[4];

public:
	int GetRefCount() { return m_iRefCount; }
	int IncRef() { return ++m_iRefCount; }
	int DecRef() { return --m_iRefCount; }

	void Cancel();
	void Process(const SOCKADDR_IN &localSocketAddr,const SOCKADDR_IN &serverSocketAddr,const unsigned long connectionTimeoutMS);

	void setGarbage() { m_bGarbage = true; }
	bool getGarbage() const { return m_bGarbage;}

	bool isExpired(unsigned long maxSeconds);
	
	CRequestItem(const bool encryption,const unsigned long requestType,const unsigned long requestID,CPYDLLEngine *pParent);
	~CRequestItem(void);

	bool getUsingEncryption() const { return m_bUsingEncryption; }
	
	unsigned long getRequestID() const { return m_ulRequestID; }
	
	eRequestStatus getStatus() const { return m_status; }
	void setStatus(const eRequestStatus stat) { m_status = stat; }

	unsigned long getRequestType() const { return m_ulRequestType; }
	
	unsigned long getUserDataLength() const { return m_ulUserDataLength; }

	void copyUserData(const unsigned long bytes,void * const destination);
	void setUserData(const unsigned long bytes,const void * const source);

	bool addData(const unsigned long bytes,const void * const source);

	bool readData(const unsigned long requestBytes,void * const destination, unsigned long * const actualBytes);

	inline bool operator<(const CRequestItem& other) const 
	{
      return m_ulRequestID < other.getRequestID();
    }
	inline bool operator==(const CRequestItem& other) const
	{
		return m_ulRequestID == other.getRequestID();
	}
};

// Because the STL doesn't like pointers as its container members we have to use
// a smart pointer of some kind.  Sure, we could use a lot of 
// copying (slow), or an even smarter pointer, but this one is fast.
template <typename T> class SmartContainerPTR
{
protected:
	T *m_pPtr;
public:
	explicit SmartContainerPTR() { m_pPtr = NULL; }
	
	explicit SmartContainerPTR(const SmartContainerPTR &target) 
	{ 
		m_pPtr = target.m_pPtr; 
		if (m_pPtr) m_pPtr->IncRef();
	}
	explicit SmartContainerPTR(T * target) 
	{ 
		m_pPtr = target; 
		if (m_pPtr) m_pPtr->IncRef();
	}
	~SmartContainerPTR() 
	{
		if (m_pPtr)
		{
			if (m_pPtr->DecRef()==0) delete m_pPtr;
		}
	}
	T *GetElement() const { return m_pPtr; }
	T& operator*()  const {return *m_pPtr;}
    T* operator->() const {return m_pPtr;}

	bool operator<(const SmartContainerPTR& other) const 
	{
      return (*m_pPtr) < (*other.m_pPtr);
    }
	
	bool operator==(const SmartContainerPTR& other) const
	{
		return (*m_pPtr)==(*other.m_pPtr);
	}

	// copy this object
	SmartContainerPTR & operator=(SmartContainerPTR & r)
    {
        if (this != &r) 
		{	
			T *oldptr = m_pPtr; // we go to great lengths to only delete once the operation completes
			m_pPtr = r.m_pPtr; // becasue this could be a hand-off.
			if (m_pPtr) m_pPtr->IncRef();
			if (oldptr)
			{
				if (oldptr->DecRef()==0)
				{
					delete oldptr;
				}

			}
        }
        return *this;
    }

	// copy pointer directly
	SmartContainerPTR & operator=(T * r)
    {
        if (m_pPtr != r)
		{	
			T *oldptr = m_pPtr;
			m_pPtr = r;
			if (m_pPtr) m_pPtr->IncRef();
			if (oldptr)
			{
				if (oldptr->DecRef()==0)
				{
					delete oldptr;
				}

			}
        }
        return *this;
    }
};