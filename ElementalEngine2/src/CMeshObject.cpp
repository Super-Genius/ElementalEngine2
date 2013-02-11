///==========================================================================
/// \file	CMeshObject.hpp
/// \brief	Implementation of CMeshObject interface
/// \date	3/09/2005
/// \author Marvin Gouw
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
///==========================================================================

#include "StdAfx.h"
#include "CMeshObject.h"
#include "IStreamObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

REGISTER_COMPONENT(CMeshObject);


///
/// Constructor / Destructor
///

CMeshObject::CMeshObject(IHashString *parentName, IHashString *name) : 
	OBJECTTEMPLATE( CRenderManager, CMeshObject, IMeshObject, parentName, name )
{
	//Not sure if we want to add to hierarchy
	//AddToHierarchy();
	DeInit();
}

CMeshObject::~CMeshObject()
{
	DeInit();
	//Not sure if we want to add to hierarchy
	//RemoveFromHierarchy();
}

/// \brief The Initialization for the Object
/// \return pointer to an IComponent
IComponent *CMeshObject::Create(int nArgs, va_list argptr)
{
	IHashString *name;
	IHashString *parentName;
	IObject *self;

	name = va_arg(argptr, IHashString *);
	parentName = va_arg(argptr, IHashString *);
	// create the object however you must create it
	try
	{
		// try and create an instance
		self = new CMeshObject(parentName, name);
	}
	catch(...)
	{
		return NULL;
	}

	return self;
}
///
/// Init / Update / DeInit
///
void CMeshObject::Init()
{
	DeInit();
}

/// \brief Update the object
/// \return true or false
bool CMeshObject::Update()
{
	return true;
}


void CMeshObject::DeInit()
{
	//frees internal data
	if( !m_StreamData.empty() )
	{
		CStreamObject * curStream = NULL;
		STREAMOBJECTMAP::iterator iter =	m_StreamData.begin();
		for( ;iter != m_StreamData.end(); iter++ )
		{
			curStream = &((*iter).second);
			curStream->Free();
		}
	}
	
}

void  CMeshObject::SetStream( DWORD streamname, void * pIn, int numElements, int nStride, int nOffset, int nTotalStride )
{
	if( pIn )
	{
		//find the map
		STREAMOBJECTMAP::iterator iter = m_StreamData.find( streamname );
		if( iter == m_StreamData.end() )
		{
			//add new entry
			CStreamObject newstream;
			iter = m_StreamData.insert( m_StreamData.begin(), pair< DWORD, CStreamObject >( streamname, newstream ) );
		}
		//now edit the stream
		CStreamObject &curStream = (*iter).second;
		curStream.Alloc( numElements, nStride );
		unsigned char * bytepointer = (unsigned char * )curStream.GetData();
		unsigned char * sourcepointer = (unsigned char * )pIn;
		sourcepointer += nOffset;
		//unleave data
		for( int i = 0; i < numElements; i++ )
		{
			memcpy( bytepointer, sourcepointer, nStride );
			bytepointer += nStride;
			sourcepointer += nTotalStride;
		}
		int a = 0;//debug var
	}
}

bool CMeshObject::SetStreamData( void *pIn, int numElements,  CHANNELDESCRIPTORLIST &flagArray )
{
	//for each stream channel, set the data
	int totalstride =0;
	list< int > CurOffsets;

	for( int i = 0; i < (int)flagArray.size(); i++ )
	{
		ChannelDesc * curStream = &flagArray[ i ];		
		CurOffsets.push_back( totalstride );
		totalstride += curStream->Stride;
	}
	list< int >::iterator iter = CurOffsets.begin();
	for( int i = 0; i < (int)flagArray.size(); i++, iter++ )
	{
		ChannelDesc * curStream = &flagArray[ i ];
		SetStream( curStream->ChannelHash,  pIn, numElements, curStream->Stride, (*iter), totalstride  );
	}
	return false;
}

int CMeshObject::CheckValidGetFlags( int numElementsRequested, CHANNELDESCRIPTORLIST &flagArray )
{
	//checks to see if all the flags are in the mesh and if the strides are matching
	CStreamObject * pStream;
	STREAMOBJECTMAP::iterator iter;
	bool CheckResult = true;
	int streamsize = 0;
	int streamsizecount = 0;

	if( m_StreamData.empty() )
	{
		CheckResult = false;
	}
	else
	{
		for( int k = 0; k < (int)flagArray.size(); k++ )
		{
			ChannelDesc * curStream = &flagArray[ k ];
			//find the stream
			iter = m_StreamData.find( curStream->ChannelHash );
			if( iter != m_StreamData.end() )
			{
				//found, check if valid stride
				pStream = &((*iter).second);
				if( pStream->GetStride() != curStream->Stride
					|| pStream->GetNumElements() < numElementsRequested )
				{
					CheckResult = false;
					break;
				}
				//check if stream sizes are the same
				if( streamsize != pStream->GetNumElements() )
				{
					streamsizecount++;
					streamsize = pStream->GetNumElements();
				}
			}
			else
			{
				CheckResult = false;
				break;
			}
		}
	}
	if( streamsizecount > 1 || !CheckResult )
	{
		//two streams have different sizes or requested size or stride differs
		return 0;
	}
	if( numElementsRequested == 0 )
		return streamsize;
	else
		return numElementsRequested;
}

MESHSTREAMDESC CMeshObject::GetStreamData( void * pOut, CHANNELDESCRIPTORLIST &flagArray, int numElements )
{
	CStreamObject * pStream;
	STREAMOBJECTMAP::iterator iter;
	MESHSTREAMDESC outdesc;

	//Calculate requested stride:
	int totalstride =0;
	for( int i = 0; i < (int)flagArray.size(); i++ )
	{
		ChannelDesc * curStream = &flagArray[ i ];	
		totalstride += curStream->Stride;
	}
	
	int streamElements = CheckValidGetFlags( numElements, flagArray); 
	outdesc.NumElements = streamElements;
	outdesc.Stride = totalstride;
	//means we just want to get the size
	if( !pOut )
	{
		return outdesc;
	}
	//check if flags are valid
	if( streamElements )
	{
		//allocate buffer if necessary:
		unsigned char * pBufPointer = (unsigned char *)pOut;
		unsigned char * pCurSource = NULL;

		if( flagArray.size() > 1 )//interleave, we want multiple channels in the same buffer
		{
			//for each stream channel, set the data in interleaved format
			//second phase
			for( int i = 0; i < streamElements; i++ )
			{
				for( int k = 0; k < (int)flagArray.size(); k++ )
				{
					//find the match, extremely slow temp right now
					ChannelDesc * curStream = &flagArray[ k ];
					if( curStream )
					{
						//find the stream(should be valid since we did a previous check with 
						//CheckValidGetFlags
						iter = m_StreamData.find( curStream->ChannelHash );
						pStream = &((*iter).second);
						pCurSource = (unsigned char *)pStream->GetData();
						pCurSource += i*pStream->GetStride();
						memcpy( pBufPointer, pCurSource, pStream->GetStride() );
						pBufPointer += pStream->GetStride();
					}
				}
			}
			return outdesc;
		}
		else//single channel, if sizearray == 1
		{
			ChannelDesc * curStream = &flagArray[ 0 ];
			if( curStream )
			{
				//find the stream
				iter = m_StreamData.find( curStream->ChannelHash );
				//find the stream(should be valid since we did a previous check with 
				//CheckValidGetFlags
				pStream = &((*iter).second);
				memcpy( pBufPointer, pStream->GetData(), pStream->GetStride()*streamElements );		
			}
			return outdesc;
		}
	}
	return outdesc;
}

void CMeshObject::GetMeshDescription( CHANNELDESCRIPTORLIST &ChannelDescsOut )
{
	ChannelDescsOut.clear();
	//Fills the array with available channels
	if( !m_StreamData.empty() )
	{
		STREAMOBJECTMAP::iterator iter;
		iter = m_StreamData.end();
		for( ;iter != m_StreamData.begin(); iter-- )
		{
			ChannelDesc newstream;
			CStreamObject &ref = (*iter).second;
			newstream.ChannelHash = (*iter).first;
			newstream.Stride = ref.GetStride();
			ChannelDescsOut.push_back( newstream );
		}
	}
}
