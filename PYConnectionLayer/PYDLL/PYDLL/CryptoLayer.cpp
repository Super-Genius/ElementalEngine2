#include "StdAfx.h"
#include "CryptoLayer.h"
#include "rijndael.h"
#include "filters.h"
#include "modes.h"
#include "pydll.h"
#include "PYDLLEngine.h"

using namespace CryptoPP;
// how big the key is once decrypted
#define DECRYPTED_RIJNDAEL_KEY_LENGTH 16
// how big the key inflates when encrypted
#define ENCRYPTED_RIJNDAEL_KEY_LENGTH 128

CCryptoLayer::CCryptoLayer(CPYDLLEngine *pParent) : m_bKeysOK(false), m_pParent(pParent)
{
#ifdef _XBOX
	srand((unsigned int)(GetTickCount() & 0xFFFF));
	unsigned char inputbyte;
	for (int a=0;a<512;a++)
	{
		// I wish we had a better source of entropy :(
		inputbyte = (unsigned char)(rand() % 255);
		m_RandomPool.IncorporateEntropy(&inputbyte,1);
	}
#endif

	memset(m_ucRSAEncryptionKey,0,1024);

	m_pEncryptor = NULL;

	m_ulEncryptionKeyLength = 0;

	// Comment out the rest of this constructor for actual deployment
	// ----------------------------------------------------------------------------------------------

	// a note on public key cryptography
	// You can in fact encrypt things using a private key
	// but that would be a terrible cryptographic concept and very weak.
	// you should instead ALWAYS encrypt stuff with a public key, and decrypt it with a private key
	// therefore in this DLL we will always consider the public key as the one that is doing the encryption
	// (the encryption key)
	// and the private key as the decryption key.

	/*// generate a PRIVATE key that can be used to decrypt stuff
	RSAES_PKCS1v15_Decryptor Decryptor(m_RandomPool, 1024);
	ArraySink sink(m_ucRSADecryptionKey,1024);
	Decryptor.DEREncode(sink);
	m_ulDecryptionKeyLength = (unsigned long)sink.TotalPutLength();

	//to generate a public key from that private key youd do the following

	RSAES_PKCS1v15_Encryptor Encryptor(Decryptor);
	ArraySink sink2(m_ucRSAEncryptionKey,1024);
	Encryptor.DEREncode(sink2);
	m_ulEncryptionKeyLength = (unsigned long)sink.TotalPutLength();

	RSAES_OAEP_SHA_Encryptor pub(pubFile); 

	// now pre-build the encryptors!
	m_pEncryptor = new RSAES_PKCS1v15_Encryptor(StringSource(m_ucRSAEncryptionKey,m_ulEncryptionKeyLength,true,NULL));
	m_pDecryptor = new RSAES_PKCS1v15_Decryptor(StringSource(m_ucRSADecryptionKey,m_ulDecryptionKeyLength,true,NULL));*/
	// ----------------------------------------------------------------------------------------------
	
}

CCryptoLayer::~CCryptoLayer(void)
{
	if (m_pEncryptor) delete m_pEncryptor;
}

// Given an unencrypted data length, get how long the final encrypted data will take
// this will be the header plus the 16 byte key plus padding plus the original size.
unsigned long CCryptoLayer::GetEncryptedSize(const unsigned long datalen) const
{
	// if the original data blob is datalen
	unsigned long finallen = datalen;
	// Final format is (16 byte keyblock) + ([PHAT][decrypted length][actual data][padding]) ...

	// first, the PHAT + decrypted length adds 8 bytes to the Rijndael encrypted block
	finallen += 8;
	// then the rest is padded up to the rijndael blocksize of 16
	while (finallen	% Rijndael::BLOCKSIZE != 0)
		finallen++;

	// finally, add the key block which does NOT have to be the rijndael block length since its not 
	// encrypted using rijndael
	finallen += ENCRYPTED_RIJNDAEL_KEY_LENGTH; // the key block.

	return finallen;
}

// Given an encrypted data length, find out how big a buffer you need for the final decrypted data.
// Note:  This is not the final decrypted size, as this is just how much you'll need
// it will be padding-aligned
unsigned long CCryptoLayer::GetDecryptedSize(const unsigned long datalen) const
{
	// the size of the buffer will be at least the size of the PHAT-header plus the size of the encrypted rijndael key
	// since the phat-header is smaller than a block, its really blocksize...
	if (datalen < Rijndael::BLOCKSIZE) return 0;

	// the largest the decrypted data could possibly be, is the size of the encrypted data
	// minus the header size (8 bytes)
	return (datalen - 8);
}

// loads the crypto up with the given key.
// will destroy any key currently loaded and reset the decryptor.

// There are two sets of keys, from the point of view of the MCP:
// OUTGOING (public+private) set of keys
// INCOMING (public+private) set of keys.
// ONLY a PRIVATE key can be used to DECRYPT and only PUBLIC keys can be used to ENCRYPT
// This means that the MCP sends two keys to the client
// one meant to decrypt the data (the MCP's private outgoing key)
// one meant to encrypt the data (the MCP's public incoming key)
// the MCP keeps its public outgoing key secret, and uses it to encrypt outgoing communications
// the MCP keeps its private incoming key secret, and uses it to decrypt incoming communications
// ideally when PhatYaffle establishes a business relationship with a new partner
// part of the package they give them will be the two keys they need to get, as well as list 
// of messageIDs and such.
void CCryptoLayer::LoadKeys(const unsigned char * const encryptionkey,const unsigned long encryptionkeyLength)
{
	if (!encryptionkey) throw std::exception("Null encryption key");

	if (encryptionkeyLength < 1) throw std::exception("Zero length encryption key");
	
	if (encryptionkeyLength > 1024) throw std::exception("Encryption key bytes expected is too large");

	m_bKeysOK = false;

	memcpy(m_ucRSAEncryptionKey,encryptionkey,encryptionkeyLength);
	
	m_ulEncryptionKeyLength = encryptionkeyLength;

	if (m_pEncryptor) delete m_pEncryptor;

	m_pEncryptor = new RSAES_PKCS1v15_Encryptor(StringSource(m_ucRSAEncryptionKey,m_ulEncryptionKeyLength,true));
	
	m_bKeysOK = true;
}

// encrypts data into buffer.
// returns false if the destbuffer is too small etc
bool CCryptoLayer::Encrypt(const unsigned char * const data,const unsigned long datalen,unsigned char * const destbuffer,unsigned long destbufferlen,unsigned char * const outputkeys)
{
	if (!data) return false;
	if (!destbuffer) return false;
	if (datalen > destbufferlen) return false;

	unsigned long finalsize = GetEncryptedSize(datalen);
	if (destbufferlen < finalsize) return false;

	s_EncryptedPHATHeader head;
	memcpy(head.m_CheckSum,ENCRYPTED_CHECKSUM,4);
	head.m_DecryptedDataLength = datalen;

	// generate a rijndael key
	// we actually generate PHAT, key (16 bytes) and IV (blocksize)
	unsigned char block[DECRYPTED_RIJNDAEL_KEY_LENGTH + Rijndael::BLOCKSIZE + 4UL]; 
	unsigned char outputblock[ENCRYPTED_RIJNDAEL_KEY_LENGTH];
	
	block[0] = 'P';
	block[1] = 'H';
	block[2] = 'A';
	block[3] = 'T';
	for (int a=0;a<DECRYPTED_RIJNDAEL_KEY_LENGTH + Rijndael::BLOCKSIZE;++a)
	{
		block[a + 4] = GetCryptographicallyStrongRandomByte();
		outputkeys[a] = block[a + 4];
	}

	// heres the chain we'll construct
	// StringSource(block) --> Encryptor(key) ---> ArraySink(outputblock)
	// This is all possible in a single line using cryptopp:
	// dont worry aout the NEW's in the following statement
	// once a source, filter, or sink is given an object pointer, it 'owns' that object pointer
	// and will destroy it once it destructs.
	StringSource(block,4UL + DECRYPTED_RIJNDAEL_KEY_LENGTH + Rijndael::BLOCKSIZE, true, new PK_EncryptorFilter(m_RandomPool,*m_pEncryptor, new ArraySink(outputblock,ENCRYPTED_RIJNDAEL_KEY_LENGTH)));

	unsigned char *pos = &destbuffer[0];
	memcpy(pos,outputblock,ENCRYPTED_RIJNDAEL_KEY_LENGTH);
	pos += ENCRYPTED_RIJNDAEL_KEY_LENGTH;
	// so now the rijndael key has been generated and written to the buffer
	// now we need to encrypt the rest of the data to it.
	// init the encryption with the key (block) and the length of the key, and the IV (which starts at block[16])
	CFB_Mode<Rijndael >::Encryption cfbEncryption(&block[4],DECRYPTED_RIJNDAEL_KEY_LENGTH,&block[4 + DECRYPTED_RIJNDAEL_KEY_LENGTH]);
	// use a stream transform to automatically pack it with high quality padding.
	StreamTransformationFilter cfbEncryptor(cfbEncryption, new ArraySink(pos,destbufferlen-ENCRYPTED_RIJNDAEL_KEY_LENGTH));

	// final step - endian conversion
	m_pParent->UTIL_EndianConvert(&head.m_DecryptedDataLength,sizeof(head.m_DecryptedDataLength));
	cfbEncryptor.Put((byte *)&head,sizeof(head));
	cfbEncryptor.Put(data,datalen);
	cfbEncryptor.MessageEnd();

	return true;
}

// Decrypts data
// returns FALSE if the checksum (PHAT) did not correctly disassemble
// returns FALSE if the size of the data in the header is not smaller or equal to the remaining bytes.
// warning: puts the true final length inside the destbuffer len!
bool CCryptoLayer::Decrypt(const unsigned char * const data,const unsigned long datalen,unsigned char * const destbuffer,unsigned long &destbufferlen,const unsigned char * const decryptionkeys)
{
	if (!data) return false;
	if (!destbuffer) return false;
	if (datalen < Rijndael::BLOCKSIZE) return false; // must contain at least one block

	unsigned long finalsize = GetDecryptedSize(datalen);
	if (destbufferlen < finalsize) return false; // won't fit!
	
	// format is
	// [PHAT]
	// LENGTH
	// DATA
	// Rijndael key is input.

	const unsigned char * const pos = data;
	// we're going to decrypt a block at a time so that we dont have to worry about huge allocations
	unsigned char tempbuffer[Rijndael::BLOCKSIZE]; 
	s_EncryptedPHATHeader head;
	CFB_Mode<Rijndael >::Decryption ecbDecryption(decryptionkeys,DECRYPTED_RIJNDAEL_KEY_LENGTH,&decryptionkeys[DECRYPTED_RIJNDAEL_KEY_LENGTH]);

	// optimization : decrypt the first block to peek at the header first.
	unsigned long remaining_encrypted_bytes = datalen;
	unsigned long remaining_decrypted_bytes = 0; // unknown at this time
	unsigned long encrypted_position = 0; // start reading just after the key
	unsigned long decrypted_position = 0; // we start writing at zero.
	
	ecbDecryption.ProcessData(tempbuffer,&data[encrypted_position],Rijndael::BLOCKSIZE);

	remaining_encrypted_bytes -= Rijndael::BLOCKSIZE;
	encrypted_position += Rijndael::BLOCKSIZE;
	
	memcpy(&head,tempbuffer,sizeof(head));

	if (strncmp(head.m_CheckSum,ENCRYPTED_CHECKSUM,4)!=0)
	{
		return false;
	}

	m_pParent->UTIL_EndianConvert(&head.m_DecryptedDataLength,sizeof(head.m_DecryptedDataLength));

	unsigned long actualsize = head.m_DecryptedDataLength;

	if (actualsize > destbufferlen)
	{
		return false;
	}
	
	remaining_decrypted_bytes = actualsize;
	int bytes_besides_header = min(remaining_decrypted_bytes,Rijndael::BLOCKSIZE - sizeof(head));

	memcpy(&destbuffer[decrypted_position],&tempbuffer[sizeof(head)],bytes_besides_header);
	remaining_decrypted_bytes -= bytes_besides_header;
	decrypted_position += bytes_besides_header;

	// now that we know the header is O.K, we decrypt the rest of the data.
	while (remaining_encrypted_bytes > 0)
	{
		// process a block
		ecbDecryption.ProcessData(tempbuffer,&data[encrypted_position],Rijndael::BLOCKSIZE);

		// copy the block entirely, or the remaining bytes if less than a block, into the tempbuffer.
		memcpy(&destbuffer[decrypted_position],tempbuffer,min(remaining_decrypted_bytes,Rijndael::BLOCKSIZE));

		remaining_encrypted_bytes -= Rijndael::BLOCKSIZE;
		encrypted_position += Rijndael::BLOCKSIZE;
		decrypted_position += min(remaining_decrypted_bytes,Rijndael::BLOCKSIZE);
		remaining_decrypted_bytes -= min(remaining_decrypted_bytes,Rijndael::BLOCKSIZE);
	}

	destbufferlen = actualsize;
	
	return true;
}


unsigned char CCryptoLayer::GetCryptographicallyStrongRandomByte()
{
	return m_RandomPool.GenerateByte();
}