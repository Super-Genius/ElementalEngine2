///============================================================================
/// \file		ObjectCubeMapManager.cpp
/// \brief		Implementation of the CObjectCubeMapManager class
/// \date		10-04-2005
/// \author		Josh Lulewicz
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
///============================================================================


#include "stdafx.h"

// temp
#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FLOOR(x) ((int)(x) - ((x) < 0 && (x) != (int)(x)))
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))

#define IMAGE_SIZE 512
#define TILE_SIZE 128

REGISTER_COMPONENT_SINGLETON(CObjectCubeMapManager);

REGISTER_MESSAGE_HANDLER(LoadObjectCubeMap, OnLoadObjectCubeMap, CObjectCubeMapManager);
REGISTER_MESSAGE_HANDLER(AddVisibleEntitiesToScene, OnCheckLODs, CObjectCubeMapManager);
REGISTER_MESSAGE_HANDLER(SetFadeDistance, OnSetFadeDistance, CObjectCubeMapManager);
REGISTER_MESSAGE_HANDLER(GenerateCubeMap, OnGenerateCubeMap, CObjectCubeMapManager);
REGISTER_MESSAGE_HANDLER(AddCubeMap, OnAddCubeMap, CObjectCubeMapManager);


const static StdString cubemapTypeFile = _T("ObjectProperties\\CubemapTypes.xct");
	
CObjectCubeMapManager::CObjectCubeMapManager() : 
	CManager(_T("CObjectCubeMapManager"), OBJECTCUBEMAPPRIORITY, OBJECTCUBEMAPPRIORITY),
	m_ccamTypeName(_T("CCamera"))
{
	m_ToolBox = EngineGetToolBox();	
	//memset(m_Texture2D, 0, sizeof(m_Texture2D));
	m_iTextureSize = IMAGE_SIZE;
	m_Texture2DName = NULL;
	m_CubeMapShader = NULL;
	m_fFadeDistance = 200.0f;

	// we have no objects currently
	m_Objects.clear();

	// create 2d texture name
	m_Texture2DName = new CHashString(_T("TempCubeFaceTexture"));

	// init
	Init();
}

CObjectCubeMapManager::~CObjectCubeMapManager( void )
{
	DeInit();
	
	if (m_Texture2DName)
	{
		delete m_Texture2DName;
		m_Texture2DName = NULL;
	}
}


IComponent* CObjectCubeMapManager::Create( int nArgs, va_list argptr)
{
	// Return the singleton instance of this class
	return SINGLETONINSTANCE( CObjectCubeMapManager );
}

bool CObjectCubeMapManager::Init()
{
	CREATETEXTUREPARAMS createtex;
	TCHAR name[1024];
	CHashString tempName;
	int i;
	DWORD retVal;

	// create 6 2D textures for loading cube map faces
	for (i = 0; i < CUBEMAP_TOTAL_DIR; ++i)
	{
		// generate unique name
		sprintf(name, _T("%s_%03d"), m_Texture2DName->GetString(), i);
		tempName.Init(name);
		// create message
		createtex.Name = &tempName;
		createtex.sizeX = m_iTextureSize;
		createtex.sizeY = m_iTextureSize;
		createtex.bitDepth = 32;
		// create the texture
		static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_CreateTexture, sizeof(createtex), &createtex );
		if( createtex.TextureObjectInterface )
		{
			// grab the interface
			m_Texture2D[i] = dynamic_cast< ITextureObject* >( createtex.TextureObjectInterface );
		}
	}
	
	// get the shader
	if( !m_CubeMapShader )
	{
		CREATEEFFECTINTERFACE effectMsg;
		CHashString CubeMapEffect(_T("shaders\\objectcubemap.efx"));

		effectMsg.m_Name = &CubeMapEffect;
		effectMsg.m_EffectInterface = NULL;
		static DWORD msgHash_GetEffectInterface = CHashString(_T("GetEffectInterface")).GetUniqueID();
		retVal = m_ToolBox->SendMessage(msgHash_GetEffectInterface, sizeof(CREATEEFFECTINTERFACE), &effectMsg);
		// I really wish the programmers would use defensive programming.
		if (retVal != MSG_HANDLED)
		{
					return false;
		}
		if (effectMsg.m_EffectInterface != NULL)
		{
			m_CubeMapShader = 	effectMsg.m_EffectInterface;
			m_CubeMapShader->SetTextureStageFlags( 0, 0, SAMPLERSTATE_U, TEXTURESTAGE_TADDRESS_WRAP );
			m_CubeMapShader->SetTextureStageFlags( 0, 0, SAMPLERSTATE_V, TEXTURESTAGE_TADDRESS_WRAP );
		}
	}
	return true;
}


bool CObjectCubeMapManager::AddObject( IObject* Object, IHashString *group )
{
	return false;
}

bool CObjectCubeMapManager::DeleteObject( IObject* Object )
{
	return false;
}

void CObjectCubeMapManager::Update(DWORD tickCount)
{

}

bool CObjectCubeMapManager::DeInit()
{
	TEXTUREOBJECTPARAMS tobj;
	CUBEMAPMAP::iterator cmIter;
	CHashString name;
	TCHAR builtname[1024];
	CHashString deleteName;
	int i;

	// free 2D Texture
	if (m_Texture2D[0])
	{
		for (i = 0; i < CUBEMAP_TOTAL_DIR; ++i)
		{
			sprintf(builtname, _T("%s_%03d"), m_Texture2DName->GetString(), i);
			deleteName.Init(builtname);

			tobj.bLoad = false;
			tobj.Name = &deleteName;
			tobj.TextureObjectInterface = NULL;
			static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
			EngineGetToolBox()->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
			m_Texture2D[i] = NULL;
		}
	}
	// free cubemaps
	for (cmIter = m_CubeMaps.begin(); cmIter != m_CubeMaps.end(); ++cmIter)
	{
		name.Init(m_ToolBox->GetHashString((*cmIter).first));
		tobj.bLoad = false;
		tobj.Name = &name;
		tobj.TextureObjectInterface = NULL;
		static DWORD msgHash_RemoveTexture = CHashString(_T("RemoveTexture")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_RemoveTexture, sizeof(tobj), &tobj );
		(*cmIter).second = NULL;
	}

	// clear the map
	m_CubeMaps.clear();
	m_CubeMapShader = NULL;

	CManager::DeInit();
	return true;
}

DWORD CObjectCubeMapManager::OnAddCubeMap( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(ADDCUBEMAPMESSAGE));
	ADDCUBEMAPMESSAGE * parms = (ADDCUBEMAPMESSAGE*)params;
	if( parms && parms->CubeMapObject )
	{
		// add to texture names to textures map indexed by model name
		ObjectCubeMap * OCM = dynamic_cast< ObjectCubeMap* >( parms->CubeMapObject );
		if( OCM )
		{
			IHashString * name =  OCM->GetModelFileHash();
			if( name )
			{
				m_ModelCubeTexturesMap[ name->GetUniqueID() ] = (ObjectCubeMap*)parms->CubeMapObject;
				// add model name to filter set
				m_ModelsWithCubeMapsSet.insert( name->GetUniqueID() );
			}
		}
	}	
	return MSG_HANDLED_STOP;
}

DWORD CObjectCubeMapManager::OnLoadObjectCubeMap(DWORD size, void *params)
{
	VISITENTITYDATA ved;
	OBJECTLIST::iterator objIter;
	OBJECTLIST *objList = NULL;
	IHashString *objName;
	CHashString modelFileName;
	CHashString hashName;
	CUBEMAPMAP::iterator cmIter;
	CREATECUBETEXTUREPARAMS cctp;
	CHashString cubeTextureName;
	ICubeTextureObject *cubeMap;
	MODELTOCUBETEXTUREMAP::iterator mtctmIter;
	int i;
	StdString face;
	void * data[6];
	VERIFY_MESSAGE_SIZE( size, sizeof( IHashString ) );
	objName = (IHashString * )params;
	if( !objName )
	{
		return MSG_ERROR;
	}

	// add name to our set - NOT SURE IF THIS IS NEEDED YET OR NOT
	m_ObjectNamesWithCubeMaps.insert(objName->GetUniqueID());
	// get model file name from the object
	static DWORD msgHash_GetModelFileName = CHashString(_T("GetModelFileName")).GetUniqueID();
	static CHashString hsCal3dType(_T("Cal3DRenderObject"));
	m_ToolBox->SendMessage(msgHash_GetModelFileName, sizeof(CHashString), &modelFileName, objName, &hsCal3dType);
	// see if the name is in the map
	cmIter = m_CubeMaps.find(modelFileName.GetUniqueID());

	// if cubemap isn't in the map add it
	if (cmIter == m_CubeMaps.end())
	{
		// make sure we have created our temp texture
		if (m_Texture2D)
		{
			// create cubemap texture object
			cctp.Name = &modelFileName;
			cctp.size = m_iTextureSize;
			cctp.bitDepth = 32;
			static DWORD msgHash_CreateCubeTexture = CHashString(_T("CreateCubeTexture")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_CreateCubeTexture, sizeof(cctp), &cctp);
			if (cctp.CubeTextureObjectInterface)
			{
				cubeMap = cctp.CubeTextureObjectInterface;			
			}

			// now that we have the name of the model file, see if it is in our map of cubetexture names
			mtctmIter = m_ModelCubeTexturesMap.find( modelFileName.GetUniqueID() );

			if (mtctmIter != m_ModelCubeTexturesMap.end())
			{
				// it is so, load image files for the cubemap
				for (i = 0; i < CUBEMAP_TOTAL_DIR; ++i)
				{	
					// grab the cube texture array
					ObjectCubeMap * obj = mtctmIter->second;
					if( obj )
					{
						DWORD textureHash = obj->GetHashImage( i );
						// convert the dword to a hashstring
						hashName = m_ToolBox->GetHashString( textureHash );
						// convert the hashstring to a stdstring
						face = hashName.GetString();
						// load file
						m_Texture2D[i]->LoadFromFile(face);
						// read the file data
						data[i] = m_Texture2D[i]->Read();
					}
				}

				// set texture name
				cubeMap->SetTextureName(cctp.Name);
				// write data to cubemap face
				LoadCubeMapFromMemory( cubeMap, m_iTextureSize, 32, data);
				
				// add it to our map
				m_CubeMaps[modelFileName.GetUniqueID()] = cubeMap;
			}
		}
	}
	return MSG_HANDLED_PROCEED;
}


void CObjectCubeMapManager::LoadCubeMapFromMemory( ICubeTextureObject *cubeMap,
							int TextureSize, int BitDepth, void * srcBits[] )
{
	if( cubeMap && 
		TextureSize > 0 &&
		BitDepth > 0 &&
		srcBits )
	{
		UINT numBytesPerFace = (TextureSize*TextureSize*BitDepth)/8;
		BYTE * srcBytes = (BYTE*)srcBits[0];
		cubeMap->Write( CUBEMAP_FACE_X, srcBytes);
		srcBytes = (BYTE*)srcBits[1];
		cubeMap->Write( CUBEMAP_FACE_NEGATIVE_X, srcBytes );
		srcBytes = (BYTE*)srcBits[2];
		cubeMap->Write( CUBEMAP_FACE_Y,srcBytes );
		srcBytes = (BYTE*)srcBits[3];
		cubeMap->Write( CUBEMAP_FACE_NEGATIVE_Y, srcBytes );
		srcBytes = (BYTE*)srcBits[4];
		cubeMap->Write( CUBEMAP_FACE_Z, srcBytes );
		srcBytes = (BYTE*)srcBits[5];
		cubeMap->Write( CUBEMAP_FACE_NEGATIVE_Z, srcBytes );
	}
}

//=====================================================================
// Function:	GetSTCoordinatesAndFace
// Description:	returns a 2 vector given a from an x,y,z back to a
//				2d texture for the face of a cube
// Parameters:	DWORD face = cube face to map to
//				vecIn = 3 vector for input
//				vecOut[0..1] = 2 vector ST pointer for output
//				vecOut[2] = face				
// Returns:		.
//=====================================================================
void GetSTCoordinates(Vec3 *vecIn, Vec3 *vecOut)
{
	Vec3 normalVec;
	float largest;

	DWORD face;
	
	//TCHAR temp[1024];
	//sprintf(temp, "Vec: %f, %f, %f\n", vecIn->x, vecIn->y, vecIn->z);
	//OutputDebugString(temp);

	if (fabs(vecIn->x) > fabs(vecIn->y))
	{
		largest = vecIn->x;
		face = CUBEMAP_FACE_X;
	}
	else
	{
		largest = vecIn->y;
		face = CUBEMAP_FACE_Y;
	}
	
	if (fabs(largest) < fabs(vecIn->z))
	{
		largest = vecIn->z;
		face = CUBEMAP_FACE_Z;
	}

	if (largest < 0)
	{
		face++;
	}

	vecOut->z = (float)face;

	switch (face) 
	{
		case CUBEMAP_FACE_X:
			normalVec.x = abs(vecIn->x);
			normalVec.y = ((-vecIn->y / normalVec.x) + 1.0f) / 2.0f;
			normalVec.z = ((vecIn->z / normalVec.x) + 1.0f) / 2.0f;
			vecOut->x = normalVec.z;
			vecOut->y = normalVec.y;
			break;
		case CUBEMAP_FACE_NEGATIVE_X:
			normalVec.x = abs(vecIn->x);
			normalVec.y = ((-vecIn->y / normalVec.x) + 1.0f) / 2.0f;
			normalVec.z = ((-vecIn->z / normalVec.x) + 1.0f) / 2.0f;
			vecOut->x = normalVec.z;
			vecOut->y = normalVec.y;
			break;
		case CUBEMAP_FACE_Y:
			normalVec.y = abs(vecIn->y);
			normalVec.x = ((vecIn->x / normalVec.y) + 1.0f) / 2.0f;
			normalVec.z = ((-vecIn->z / normalVec.y) + 1.0f) / 2.0f;
			vecOut->x = normalVec.x;
			vecOut->y = normalVec.z;
			break;
		case CUBEMAP_FACE_NEGATIVE_Y:
			normalVec.y = abs(vecIn->y);
			normalVec.x = ((vecIn->x / normalVec.y) + 1.0f) / 2.0f;
			normalVec.z = ((vecIn->z / normalVec.y) + 1.0f) / 2.0f;
			vecOut->x = normalVec.x;
			vecOut->y = normalVec.z;
			break;
		case CUBEMAP_FACE_Z:
			normalVec.z = abs(vecIn->z);
			normalVec.x = ((-vecIn->x / normalVec.z) + 1.0f) / 2.0f;
			normalVec.y = ((-vecIn->y / normalVec.z) + 1.0f) / 2.0f;
			vecOut->x = normalVec.x;
			vecOut->y = normalVec.y;
			break;
		case CUBEMAP_FACE_NEGATIVE_Z:
			normalVec.z = abs(vecIn->z);
			normalVec.x = ((vecIn->x / normalVec.z) + 1.0f) / 2.0f;
			normalVec.y = ((-vecIn->y / normalVec.z) + 1.0f) / 2.0f;
			vecOut->x = normalVec.x;
			vecOut->y = normalVec.y;
			break;
	}
}



void GetCubeVector(DWORD face, float x, float y, float cx, float cy, Vec3 * vecOut)
{
	float s, t, sc, tc;

	// move pixels to center
	//s = ((float) x + 0.5f) / (float) cx;
	//t = ((float) y + 0.5f) / (float) cy;
	s = ((float) x) / (float) cx;
	t = ((float) y) / (float) cy;
	sc = s*2.0f - 1.0f;
	tc = t*2.0f - 1.0f;

	switch (face) 
	{
		case CUBEMAP_FACE_X:
			vecOut->x = 1.0f;
			vecOut->y = -tc;
			vecOut->z = -sc;
			break;
		case CUBEMAP_FACE_NEGATIVE_X:
			vecOut->x = -1.0f;
			vecOut->y = -tc;
			vecOut->z = sc;
			break;
		case CUBEMAP_FACE_Y:
			vecOut->x = sc;
			vecOut->y = 1.0f;
			vecOut->z = tc;
			break;
		case CUBEMAP_FACE_NEGATIVE_Y:
			vecOut->x = sc;
			vecOut->y = -1.0f;
			vecOut->z = -tc;
			break;
		case CUBEMAP_FACE_Z:
			vecOut->x = sc;
			vecOut->y = -tc;
			vecOut->z = 1.0f;
			break;
		case CUBEMAP_FACE_NEGATIVE_Z: 
			vecOut->x = -sc;
			vecOut->y = -tc;
			vecOut->z = -1.0f;
			break;
	}

	//vecOut->Normalize();
}

//This message is triggered by the cscenevisibility manager.
//It hijacks the visibility list
DWORD CObjectCubeMapManager::OnCheckLODs(DWORD size, void *params)
{
	
	PERFORMANCE_PROFILER_START( CObjectCubeMapManager_OnCheckLODS )
		
	VERIFY_MESSAGE_SIZE(size, sizeof(ADDRENDERLISTPARAMS));

	// extract params
	ADDRENDERLISTPARAMS *arlp;
	arlp = (ADDRENDERLISTPARAMS *)params;
	static DWORD msgHash_AddRenderList = CHashString(_T("AddRenderList")).GetUniqueID();
	if( m_CubeMaps.size() == 0 )
	{
		m_ToolBox->SendMessage(msgHash_AddRenderList, sizeof(*arlp), arlp);
		PERFORMANCE_PROFILER_STOP( CObjectCubeMapManager_OnCheckLODS );
		return MSG_HANDLED_PROCEED;
	}

	OBJECTLIST::iterator objIter;
	OBJECTLIST *visibleObjList = NULL;
	OBJECTLIST spriteObjList;
	OBJECTLIST *spriteObjListPtr = NULL;
	IHashString *objName;
	DWORDSET::iterator setIter;
	CHashString modelFileName;
	CUBEMAPMAP::iterator cmIter;
	MODELMAP modelMap;
	MODELMAP::iterator mmIter;
	std::vector<OBJECTLIST::iterator> iteratorsToDelete;
	DWORD index;
	int numModels;
	ADDSPRITELISTMESSAGE aslm;
	Vec3 currentPosition;
	float pos[3];
	int i = 0;
	ChannelDesc channelDesc[2];
	LODSpriteVertex *data = NULL;
	IHashString *pCameraName = NULL;
	Vec3 camPos, vecCamView, vecCamUp, vecCamRight;
	EulerAngle objRotation;
	StdString error;
	Vec3 viewVec, st, uvw, right, up, camView;
	Vec3 upperRight, lowerRight, upperLeft, lowerLeft;
	Vec3 worldUp;
	Vec3 camRight, camUp;	
	CAMERAVECTORSPARAMS camVectors;
	// params for getting the bounding box
	GETBOUNDINGSPHEREPARAMS bSphere;
	Vec3 objPos;
	bSphere.position = &objPos;

	
	// grab the visible object list
	visibleObjList = arlp->objList;

	// go through object list and filter out objects that are sprites
	for (objIter = visibleObjList->begin(); objIter != visibleObjList->end(); ++objIter)
	{
		// get the name of the object
		objName = (*objIter)->GetName();

		// get model file name from the object
		static DWORD msgHash_GetModelFileName = CHashString(_T("GetModelFileName")).GetUniqueID();
		static CHashString hsCal3dType(_T("Cal3DRenderObject"));
		if (m_ToolBox->SendMessage(msgHash_GetModelFileName, sizeof(CHashString), &modelFileName, objName, &hsCal3dType) == MSG_HANDLED)
		{	// see if the name is in the map
			cmIter = m_CubeMaps.find(modelFileName.GetUniqueID());

			// if object is a sprite (in the map)
			if (cmIter != m_CubeMaps.end())
			{
				// see if this model type is in the map
				mmIter = modelMap.find(modelFileName.GetUniqueID());

				// if it is...
				if (mmIter != modelMap.end())
				{
					// grab the object list
					spriteObjListPtr = &mmIter->second;
					// add the sprite to the objectlist in the map
					spriteObjListPtr->push_back((*objIter));
				}
				else
				{
					// otherwise add sprite to new object list
					spriteObjList.push_back((*objIter));
					// add new object list to the map
					modelMap[modelFileName.GetUniqueID()] = spriteObjList;
					// clear the old object list
					spriteObjList.clear();
				}

				// add object iterator to the delete list
				iteratorsToDelete.push_back(objIter);
			}
		}
	}

	// go through delete list and DELETE!
	for (index = 0; index < iteratorsToDelete.size(); ++index)
	{
		// remove from regular visible list
		visibleObjList->erase(iteratorsToDelete[index]);
	}
	// clear the delete list since we are done with it
	iteratorsToDelete.clear();

	
	// get active camera name
	CHashString hszCameraName;
	CONTEXTCAMPARAMS ccp;
	ccp.pRC = NULL;
	ccp.szCameraName = &hszCameraName;
	static DWORD msgHash_GetActiveCamera = CHashString(_T("GetActiveCamera")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetActiveCamera, sizeof(CONTEXTCAMPARAMS), &ccp ) != MSG_HANDLED)
	{
		// log error
		error = _T("Camera not found.\n");
		m_ToolBox->SetErrorValue(WARN_OBJECT_NOT_FOUND);
		m_ToolBox->Log(LOGWARNING, error);
		PERFORMANCE_PROFILER_STOP( CObjectCubeMapManager_OnCheckLODS );
		return false; // no camera
	}
	pCameraName = ccp.szCameraName;

	// get the camera's position
	static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &camPos, pCameraName,
		&m_ccamTypeName) != MSG_HANDLED)
	{
		// log error
		error = _T("Could not retrieve origin of the active camera.\n");
		m_ToolBox->SetErrorValue(WARN_OBJECT_NOT_FOUND);
		m_ToolBox->Log(LOGWARNING, error);
		PERFORMANCE_PROFILER_STOP( CObjectCubeMapManager_OnCheckLODS );
		return false;
	}

	// get camera basis vectors
	camVectors.vViewVector = &vecCamView;
	camVectors.vUpVector = &vecCamUp;
	camVectors.vRightVector = &vecCamRight;
	static DWORD msgHash_GetCameraVectors = CHashString(_T("GetCameraVectors")).GetUniqueID();
	if (m_ToolBox->SendMessage(msgHash_GetCameraVectors, sizeof(camVectors), &camVectors, 
		pCameraName, &m_ccamTypeName ) != MSG_HANDLED)
	{
		// log error
		StdString error;
		error = _T("Error retrieving camera vectors.\n");
		m_ToolBox->SetErrorValue(WARN_OBJECT_NOT_FOUND);
		m_ToolBox->Log(LOGWARNING, error);
		PERFORMANCE_PROFILER_STOP( CObjectCubeMapManager_OnCheckLODS );
		return false;
	}

	camUp.Set(vecCamUp.x, vecCamUp.y, vecCamUp.z);
	camUp.Normalize();

	/*
	// get camera view vector
	viewVec.Set(vecCamView.x,vecCamView.y, vecCamView.z);

	// invert it
	viewVec.x = -viewVec.x;
	viewVec.y = -viewVec.y;
	viewVec.z = -viewVec.z;
	*/

	camView.Set(vecCamView.x,vecCamView.y, vecCamView.z);

	// invert it
	camView.x = -camView.x;
	camView.y = -camView.y;
	camView.z = -camView.z;


	/*
	// determine the major axis by finding the largest component of the camera look vector
	if (fabs(camView.x) > fabs(camView.y))
	{
		largest = camView.x;
		ma = CUBEMAP_FACE_X;
	}
	else
	{
		largest = camView.y;
		ma = CUBEMAP_FACE_Y;
	}
	
	if (fabs(largest) < fabs(camView.z))
	{
		largest = camView.z;
		ma = CUBEMAP_FACE_Z;
	}

	if (largest < 0)
	{
		ma++;
	}
	
	// compute the right vector  right = up x look
	right.x	= camUp.y*camView.z-camUp.z*camView.y;
	right.y	= camUp.z*camView.x-camUp.x*camView.z;
	right.z	= camUp.x*camView.y-camUp.y*camView.x;

	// normalize right vector
	right.Normalize();

	// compute the up vector up = look x right
	up.x = camView.y*right.z-camView.z*right.y;
	up.y = camView.z*right.x-camView.x*right.z;
	up.z = camView.x*right.y-camView.y*right.x;
	*/

	// go through the sprites
	for (mmIter = modelMap.begin(); mmIter != modelMap.end(); ++mmIter)
	{
		// grab sprite object list
		spriteObjListPtr = &mmIter->second;
		// find out how many models of this type we have
		numModels = spriteObjListPtr->size();
		// calculate the number of vertices needs
		aslm.m_numVertices = numModels * 6;
		// allocate vertices
		data = new LODSpriteVertex[aslm.m_numVertices];

		// reset i
		i = 0;

		// go through models in the list
		for(objIter = spriteObjListPtr->begin(); objIter != spriteObjListPtr->end(); ++objIter)
		{ 
			// get the name of the object
			objName = (*objIter)->GetParentName();

			// Get the current position of the object
			static DWORD msgHash_GetGlobalPosition = CHashString(_T("GetGlobalPosition")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_GetGlobalPosition, sizeof(Vec3), &currentPosition, objName );

			float distance = sqrt((currentPosition.x - camPos.x)*
				                  (currentPosition.x - camPos.x) +
								  (currentPosition.y - camPos.y)*
								  (currentPosition.y - camPos.y) +
								  (currentPosition.z - camPos.z)*
								  (currentPosition.z - camPos.z));

			if (distance < m_fFadeDistance)
			{
				continue;
			}

			static CHashString hsRenderObjectType(_T("Cal3DRenderObject"));

			// get the bounding box for the object
			static DWORD msgHash_GetBoundingSphere = CHashString(_T("GetBoundingSphere")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_GetBoundingSphere, sizeof(bSphere), &bSphere, objName, &hsRenderObjectType ) != MSG_HANDLED)
			{
				// log error
				error = _T("Cannot retrieve boundingsphere for ");
				error += objName->GetString();
				error += _T("\n");
				m_ToolBox->SetErrorValue(WARN_OBJECT_NOT_FOUND);
				m_ToolBox->Log(LOGWARNING, error);
				PERFORMANCE_PROFILER_STOP( CObjectCubeMapManager_OnCheckLODS );
				return MSG_ERROR;
			}

			// get the rotation for the object
			static DWORD msgHash_GetGlobalEuler = CHashString(_T("GetGlobalEuler")).GetUniqueID();
			if (m_ToolBox->SendMessage(msgHash_GetGlobalEuler, sizeof(EulerAngle), &objRotation, objName) != MSG_HANDLED)
			{
				PERFORMANCE_PROFILER_STOP( CObjectCubeMapManager_OnCheckLODS );
				return MSG_ERROR;
			}

			// get position values
			pos[0] = currentPosition.x;
			pos[1] = currentPosition.y;
			pos[2] = currentPosition.z;

			// compute vector from the camera to the center of the object
			// swapt z,y for Vision->Cube space
#if 1
			viewVec.x = camPos.x - pos[0];
			viewVec.y = camPos.y - pos[1];
			viewVec.z = camPos.z - pos[2];

			/// TESTING ONLY - NO ONE PANIC!
			Vec3 view(viewVec.x, viewVec.z, viewVec.y);
			Matrix3x3 rotMat;
			MathUtil math;
			math.EulerToMatrix(objRotation, rotMat);

			view *= rotMat;

			Vec3 newViewVec;
			newViewVec.x = view.x;
			newViewVec.y = view.z;
			newViewVec.z = view.y;
			newViewVec.Normalize();
			////////////////////////////////
#else
			//viewVec.x = pos[0] - camPos.x;
			//viewVec.z = pos[1] - camPos.y;
			//viewVec.y = pos[2] - camPos.z;

			GetCubeVector(CUBEMAP_FACE_NEGATIVE_Z, 64, 64, IMAGE_SIZE, IMAGE_SIZE, &viewVec);
#endif
			viewVec.Normalize();

			GetSTCoordinates(&newViewVec, &st);
			st.x *= IMAGE_SIZE;
			st.y *= IMAGE_SIZE;
			st.x -= .5f;
			st.y -= .5f;

			// clamp coordinates
			st.x = MIN((float)FLOOR(st.x+0.5f), IMAGE_SIZE);
			st.y = MIN((float)FLOOR(st.y+0.5f), IMAGE_SIZE);
			st.x = MAX(st.x, 0);
			st.x = MAX(st.x, 0);

			// construct 6 verts with different uvs, all with center position. 
			Vec3 center(pos[0], pos[1], pos[2]);

			switch ((DWORD)st.z) 
			{
				case CUBEMAP_FACE_Z:
					worldUp.Set(0.0f, 0, -1.0f);
					break;
				case CUBEMAP_FACE_NEGATIVE_Z:
					worldUp.Set(0.0f, 0, 1.0f);
					break;
				default:
					worldUp.Set(0.0f,-1.0f, 0);
					break;
			}

			// compute camera right vector using our world up vector and the camera look vector
			camRight.x	= worldUp.y*viewVec.y-worldUp.z*viewVec.z;
			camRight.y	= worldUp.z*viewVec.x-worldUp.x*viewVec.y;
			camRight.z	= worldUp.x*viewVec.z-worldUp.y*viewVec.x;

			// compute new camera up vector using the camera right vector and the camera look vector
			camUp.x	= camRight.y*viewVec.y-camRight.z*viewVec.z;
			camUp.y	= camRight.z*viewVec.x-camRight.x*viewVec.y;
			camUp.z	= camRight.x*viewVec.z-camRight.y*viewVec.x;

			// compute the right vector  right = up x look
			right.x	= camUp.y*viewVec.y-camUp.z*viewVec.z;
			right.y	= camUp.z*viewVec.x-camUp.x*viewVec.y;
			right.z	= camUp.x*viewVec.z-camUp.y*viewVec.x;

			// normalize right vector
			right.Normalize();

			// compute the up vector up = look x right
			up.x = viewVec.z*right.z-viewVec.y*right.y;
			up.y = viewVec.y*right.x-viewVec.x*right.z;
			up.z = viewVec.x*right.y-viewVec.z*right.x;
			
			// compute the size of the sprite (height of the object + a percentage of the height)
			float size = bSphere.radius * 4.0f; //+ (bbox.dimZ * 0.25f);

			// corners of the sprite
			upperRight = center + right*size + up*size;
			upperLeft = center - right*size + up*size;
			lowerRight = center + right*size - up*size;
			lowerLeft = center - right*size - up*size;

			// compute verticies
			data[i*6].pos[0] = lowerLeft.x; //pos[0]-5;
			data[i*6].pos[1] = lowerLeft.y; //pos[1];
			data[i*6].pos[2] = lowerLeft.z; //pos[2]-5;
			uvw.x = (float)((int)(st.x / TILE_SIZE) * TILE_SIZE);
			uvw.y = (float)((int)((st.y + TILE_SIZE) / TILE_SIZE) * TILE_SIZE);
			GetCubeVector( (DWORD)st.z, uvw.x, uvw.y, IMAGE_SIZE, IMAGE_SIZE, &uvw );
			data[i*6].u = uvw.x;
			data[i*6].v = uvw.y;
			data[i*6].w = uvw.z;
			
			data[i*6 + 1].pos[0] = lowerRight.x; //pos[0]+5;
			data[i*6 + 1].pos[1] = lowerRight.y; //pos[1];
			data[i*6 + 1].pos[2] = lowerRight.z; //pos[2]-5;
			uvw.x = (float)((int)((st.x + TILE_SIZE) / TILE_SIZE) * TILE_SIZE);
			uvw.y = (float)((int)((st.y + TILE_SIZE) / TILE_SIZE) * TILE_SIZE);
			GetCubeVector( (DWORD)st.z, uvw.x, uvw.y, IMAGE_SIZE, IMAGE_SIZE, &uvw );
			data[i*6 + 1].u = uvw.x;
			data[i*6 + 1].v = uvw.y;
			data[i*6 + 1].w = uvw.z;

			data[i*6 + 2].pos[0] =  upperLeft.x; //pos[0]-5;
			data[i*6 + 2].pos[1] =  upperLeft.y; //pos[1];
			data[i*6 + 2].pos[2] =  upperLeft.z; //pos[2]+5;
			uvw.x = (float)((int)(st.x / TILE_SIZE) * TILE_SIZE);
			uvw.y = (float)((int)(st.y / TILE_SIZE) * TILE_SIZE);
			GetCubeVector( (DWORD)st.z, uvw.x, uvw.y, IMAGE_SIZE, IMAGE_SIZE, &uvw );
			data[i*6 + 2].u = uvw.x;
			data[i*6 + 2].v = uvw.y;
			data[i*6 + 2].w = uvw.z;
			
			data[i*6 + 3].pos[0] =  upperLeft.x; //pos[0]-5;
			data[i*6 + 3].pos[1] =  upperLeft.y; //pos[1];
			data[i*6 + 3].pos[2] =  upperLeft.z; //pos[2]+5;
			uvw.x = (float)((int)(st.x / TILE_SIZE) * TILE_SIZE);
			uvw.y = (float)((int)(st.y / TILE_SIZE) * TILE_SIZE);
			GetCubeVector( (DWORD)st.z, uvw.x, uvw.y, IMAGE_SIZE, IMAGE_SIZE, &uvw );
			data[i*6 + 3].u = uvw.x;
			data[i*6 + 3].v = uvw.y;
			data[i*6 + 3].w = uvw.z;
			
			data[i*6 + 4].pos[0] =  lowerRight.x; //pos[0]+5;
			data[i*6 + 4].pos[1] =  lowerRight.y; //pos[1];
			data[i*6 + 4].pos[2] =  lowerRight.z; //pos[2]-5;
			uvw.x = (float)((int)((st.x + TILE_SIZE) / TILE_SIZE) * TILE_SIZE);
			uvw.y = (float)((int)((st.y + TILE_SIZE) / TILE_SIZE) * TILE_SIZE);
			GetCubeVector( (DWORD)st.z, uvw.x, uvw.y, IMAGE_SIZE, IMAGE_SIZE, &uvw );
			data[i*6 + 4].u = uvw.x;
			data[i*6 + 4].v = uvw.y;
			data[i*6 + 4].w = uvw.z;
			
			data[i*6 + 5].pos[0] =  upperRight.x; //pos[0]+5;
			data[i*6 + 5].pos[1] =  upperRight.y; //pos[1];
			data[i*6 + 5].pos[2] =  upperRight.z; //pos[2]+5;
			uvw.x = (float)((int)((st.x + TILE_SIZE) / TILE_SIZE) * TILE_SIZE);
			uvw.y = (float)((int)(st.y / TILE_SIZE) * TILE_SIZE);
			GetCubeVector( (DWORD)st.z, uvw.x, uvw.y, IMAGE_SIZE, IMAGE_SIZE, &uvw );
			data[i*6 + 5].u = uvw.x;
			data[i*6 + 5].v = uvw.y;
			data[i*6 + 5].w = uvw.z;
			
			// increment i
			++i;
		}

		// setup descriptor
		channelDesc[0] = MESH_STREAM_VERTEX_POSITION3;
		channelDesc[1] = MESH_STREAM_VERTEX_CUBECOORD1;

		// find the model name in the map of cubemaps
		cmIter = m_CubeMaps.find(mmIter->first);

		if (cmIter == m_CubeMaps.end())
		{
			// something's wrong here
			//TODO: Free up the memory initialized!
			PERFORMANCE_PROFILER_STOP( CObjectCubeMapManager_OnCheckLODS );
			return MSG_ERROR;
		}

		// setup message params
		aslm.m_Data = data;
		aslm.m_ChannelDescriptor = channelDesc;
		aslm.m_NumChannels = 2;
		aslm.m_Texture = cmIter->second;
		aslm.m_Effect = m_CubeMapShader;
		
		// make sure our texture and shader are valid
		if (cmIter->second 
			&& m_CubeMapShader)
		{
			// Send data to sprite manager
			static DWORD msgHash_AddSprite = CHashString(_T("AddSprite")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_AddSprite, sizeof(aslm), &aslm);
		}

		// delete data
		if (data)
		{
			delete [] data;
			data = NULL;
		}
	}

	PERFORMANCE_PROFILER_STOP( CObjectCubeMapManager_OnCheckLODS );

	return MSG_HANDLED_PROCEED;
}

DWORD CObjectCubeMapManager::OnSetFadeDistance(DWORD size, void *params)
{

	return MSG_HANDLED_STOP;
}

IHashString* CObjectCubeMapManager::GetComponentType()
{
	static CHashString ocmmCT = _T("CObjectCubeMapManager");
	return &ocmmCT; 
}

DWORD CObjectCubeMapManager::OnGenerateCubeMap( DWORD size, void * params )
{
	VERIFY_MESSAGE_SIZE(size, sizeof(GENERATECUBEMAPMESSAGE));
	GENERATECUBEMAPMESSAGE * parms = (GENERATECUBEMAPMESSAGE*)params;
	ObjectCubeGenerator cGen;
	if( parms->filename &&
		parms->RenderObject )
	{
		cGen.Generate( (TCHAR*)parms->filename->GetString(), (ObjectCubeMap*)parms->CubeMapObject, parms->RenderObject );
		return MSG_HANDLED_STOP;
	}else
	{
		return MSG_ERROR;
	}
}


DWORD CObjectCubeMapManager::PumpHandler(DWORD size, void *data, IHashString *name, 
									 IHashString *compType, MSGFUNCTION msgFunction)
{
	MODELTOCUBETEXTUREMAP::iterator startIter;
	MODELTOCUBETEXTUREMAP::iterator endIter;

	DWORD retval = MSG_NOT_HANDLED;
		
	// specific object
	if (name != NULL)
	{
		startIter = m_ModelCubeTexturesMap.lower_bound(name->GetUniqueID());
		endIter = m_ModelCubeTexturesMap.upper_bound(name->GetUniqueID());
	}
	
	// pump through everything if no name is specified
	else
	{
		startIter = m_ModelCubeTexturesMap.begin();
		endIter = m_ModelCubeTexturesMap.end();
	}

	for (; startIter != endIter; startIter++)
	{
		IObject *object = startIter->second;

		if ((compType == NULL) || (object->IsKindOf(compType)))
		{
			retval = (object->*msgFunction)(size, data);

			if (retval == MSG_HANDLED_STOP)
			{
				return retval;
			}
		}
	}
	return retval;
}
/*
/////////////////////////////////////////////////
// scale the points
float oldX, oldY, oldZ;
float m[16] = {0,0,0,0,
				0,0,0,0,
				0,0,0,0,
				0,0,0,0};

m[0] = scale.x;
m[5] = scale.y;
m[10] = scale.z;
m[15] = 1.0f;

oldX=upperRight.x;
oldY=upperRight.y;
oldZ=upperRight.z;
upperRight.x=oldX*m[0] + oldY*m[1] + oldZ*m[2] + m[3];
upperRight.y=oldX*m[4] + oldY*m[5] + oldZ*m[6] + m[7];
upperRight.z=oldX*m[8] + oldY*m[9] + oldZ*m[10] + m[11];

oldX=upperLeft.x;
oldY=upperLeft.y;
oldZ=upperLeft.z;
upperLeft.x=oldX*m[0] + oldY*m[1] + oldZ*m[2] + m[3];
upperLeft.y=oldX*m[4] + oldY*m[5] + oldZ*m[6] + m[7];
upperLeft.z=oldX*m[8] + oldY*m[9] + oldZ*m[10] + m[11];

oldX=lowerRight.x;
oldY=lowerRight.y;
oldZ=lowerRight.z;
lowerRight.x=oldX*m[0] + oldY*m[1] + oldZ*m[2] + m[3];
lowerRight.y=oldX*m[4] + oldY*m[5] + oldZ*m[6] + m[7];
lowerRight.z=oldX*m[8] + oldY*m[9] + oldZ*m[10] + m[11];

oldX=lowerLeft.x;
oldY=lowerLeft.y;
oldZ=lowerLeft.z;
lowerLeft.x=oldX*m[0] + oldY*m[1] + oldZ*m[2] + m[3];
lowerLeft.y=oldX*m[4] + oldY*m[5] + oldZ*m[6] + m[7];
lowerLeft.z=oldX*m[8] + oldY*m[9] + oldZ*m[10] + m[11];
/////////////////////////////////////////////////////////////////
*/