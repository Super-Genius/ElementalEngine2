#pragma once

// the Crypto Layer is just a basic encryptor/decryptor
// All MCP communications (incoming and outgoing) can be encrypted
// if outgoing encrypted, the crypto layer will be fed the data in order to encrypt it
// if incoming encrypted, the crypto layer will be fed the data in order to decrypt it
// in all cases, the structure of the data is the same - its a binary blob of a given length
// [blob]
// which consists of a rijndael key and a rijndael-encrypted block
// [rkey][rblock]
// The rijndael key is 16 bytes long (128bit) and is encrypted using RSA encryption.
// When this crypto layer encrypts or decrypts it will use a given private key to do so.
// the rijndael encrypted block [rblock], when decrypted, consists of
// [PHAT][decrypted length][actual data][padding] ... where [PHAT][decrypted length] header is
// 8 bytes and the length does not include the padding or the header, just the actual data.

#include "osrng.h"
#include "rsa.h"
#include "randpool.h"

static const char ENCRYPTED_CHECKSUM[]  = "PHAT";

#pragma pack(push,1)

typedef struct 
{
	char m_CheckSum[4]; //"PHAT"
	unsigned long m_DecryptedDataLength; // must be convertable to signed without dataloss so limit is 2^31 not 2^32
} s_EncryptedPHATHeader;

#pragma pack(pop)

class CPYDLLEngine;

class CCryptoLayer
{
protected:
	unsigned char m_ucRSAEncryptionKey[1024]; // key used to encrypt data - the public key
	unsigned long m_ulEncryptionKeyLength;
#ifndef _XBOX
	CryptoPP::AutoSeededRandomPool m_RandomPool;
#else
	CryptoPP::RandomPool m_RandomPool;
#endif
	CryptoPP::RSAES_PKCS1v15_Encryptor *m_pEncryptor;
	bool m_bKeysOK;

	CPYDLLEngine *m_pParent;
public:
	bool IsReady() const { return m_bKeysOK; }

	// Given an unencrypted data length, get how long the final encrypted data will take
	// this will be the header plus the 16 byte key plus padding plus the original size.
	unsigned long GetEncryptedSize(const unsigned long datalen) const;

	// Given an encrypted data length, find out how big a buffer you need for the final decrypted data.
	// Note:  This is not the final decrypted size, as this is just how much you'll need
	// it will be padding-aligned
	unsigned long GetDecryptedSize(const unsigned long datalen) const;

	// loads the crypto up with the given key.
	// will destroy any key currently loaded and reset the decryptor.
	void LoadKeys(const unsigned char * const encryptionkey,const unsigned long encryptionkeyLength);

	// encrypts data into buffer.
	// returns false if the destbuffer is too small etc
	bool Encrypt(const unsigned char * const data,const unsigned long datalen,unsigned char * const destbuffer,unsigned long destbufferlen,unsigned char * const outputkeys);

	// Decrypts data
	// returns FALSE if the checksum (PHAT) did not correctly disassemble
	// returns FALSE if the size of the data in the header is not smaller or equal to the remaining bytes.
	bool Decrypt(const unsigned char * const data,const unsigned long datalen,unsigned char * const destbuffer,unsigned long &destbufferlen,const unsigned char * const decryptionkeys);

	unsigned char GetCryptographicallyStrongRandomByte();

	CCryptoLayer(CPYDLLEngine *pParent);
	~CCryptoLayer(void);
};
