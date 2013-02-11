///============================================================================
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
///============================================================================

#include "stdafx.h"

void ObjectCubeGenerator::Generate( TCHAR * filename, ObjectCubeMap * map, IRenderObject * pEnt )
{
	MathUtil Math;
	EulerAngle rotation;
	Vec3 position(0.0f,0.0f,0.0f);
	Vec3 scaling(0.025f, 0.025f, 0.025f);
	Vec3 right;
	Vec3 eye(0.0f,0.0f,0.0f);
	Vec3 up(0.0f,0.0f,1.0f);
	Matrix3x3	matRot;
	Matrix3x3 preRotMat;
	float s, t, sc, tc;
	Vec3 vecOut;
	int face, x, y, xTile, yTile;
	DWORD pos = 0;
	const int imageBufferSize = (DESTSIZE*DESTSIZE*4);
	BYTE *imageBuffer;
	Vec3 entityPosition;
	float entityBoundingRadius;
	char temp[1024];
	IToolBox * m_ToolBox = EngineGetToolBox();

	std::vector< Vec3 > m_RotationVectors;

	CHashString RTName( _T("CUBEMAPGEN_RENDERTEXTURE") );
	CHashString CameraName( _T("CUBEMAPGEN_CAMERA") );

	CREATEOBJECTPARAMS cop;
	cop.name = &CameraName;
	cop.parentName = NULL;
	static DWORD msgHash_CreateCamera = CHashString(_T("CreateCamera")).GetUniqueID();
	DWORD res = m_ToolBox->SendMessage(msgHash_CreateCamera, sizeof(cop), &cop, NULL, NULL );
	if( res != MSG_HANDLED )
	{
		return;
	}
	//Get the model filename
	CHashString modelFileName;
	static DWORD msgHash_GetModelFileName = CHashString(_T("GetModelFileName")).GetUniqueID();
	static CHashString hsCal3dType(_T("Cal3DRenderObject"));
	DWORD retVal = m_ToolBox->SendMessage(msgHash_GetModelFileName, sizeof(CHashString), &modelFileName, pEnt->GetName(), &hsCal3dType);
	if( retVal == MSG_HANDLED && map )
	{
		map->SetTypeData( &modelFileName );
	}

	//create the render target
	ITextureObject * m_renderTarget;
	CREATETEXTUREPARAMS tobj;
	tobj.bitDepth = BPP*8;
	tobj.Name = &RTName;
	tobj.sizeX = VISION_INIT_SIZE;
	tobj.sizeY = VISION_INIT_SIZE;
	tobj.TextureObjectInterface = NULL;
	tobj.bRenderTargetTexture = RENDER_TARGET_COLOR_BUFFER;
	static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();
	 res = EngineGetToolBox()->SendMessage(msgHash_CreateTexture, sizeof(tobj), &tobj );
	if( res == MSG_HANDLED )
	{
		// store the texture id in the user data of the map
		m_renderTarget = dynamic_cast< ITextureObject * >( tobj.TextureObjectInterface );
	}

	// grab the render interface
	GETRENDERERINTERFACEPARAMS renderInterface;

	static DWORD msgHash_GetRendererInterface = CHashString(_T("GetRendererInterface")).GetUniqueID();
	m_ToolBox->SendMessage(msgHash_GetRendererInterface, 
						   sizeof(GETRENDERERINTERFACEPARAMS), 
						   &renderInterface);

	IRenderer * Renderer = renderInterface.m_RendererInterface;

	if( pEnt && m_renderTarget )
	{
		// allocate image buffer
		imageBuffer = (BYTE *)malloc( imageBufferSize );
		// initialize image buffer
		memset( imageBuffer, (char)255, imageBufferSize );
		// get the bounding box for the entity
		pEnt->GetBoundingSphere( entityPosition, entityBoundingRadius );
		//create and setup camera orientation
		Vec3 campos(0.0f,0.0f,3.f*(entityBoundingRadius));
		Vec3 vLookAt = Vec3(0.0f,0.0f,0.0f);
		//change active camera here to ortho view!
		static DWORD msgHash_SetGlobalPosition = CHashString(_T("SetGlobalPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetGlobalPosition, sizeof(Vec3), &campos, &CameraName );		
		static DWORD msgHash_SetLookAtPosition = CHashString(_T("SetLookAtPosition")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_SetLookAtPosition, sizeof(Vec3), &vLookAt, &CameraName );			
		static DWORD msgHash_SetUseRenderTargetAspect = CHashString(_T("SetCameraUseBackBufferAspectRatio")).GetUniqueID();
		bool useRTAspect = true;
		m_ToolBox->SendMessage(msgHash_SetUseRenderTargetAspect, sizeof(bool), &useRTAspect, &CameraName );			
		
		m_ToolBox->SendMessage(msgHash_SetGlobalPosition,sizeof( Vec3 ), &vLookAt, pEnt->GetParentName() );

		pEnt->DetachAllLights();
		CHashString ambientLightName(_T("CubeMapGenLight"));
		CHashString CLOName( _T("CLightObject") );
		ILightObject * ambientLight = (ILightObject*)m_ToolBox->CreateComponent( &CLOName, 2, &ambientLightName, NULL );
		ambientLight->SetColor( 1,1,1 );
		ambientLight->SetIntensity( 1 );
		ambientLight->SetAttenuationDistance( 100000 );
		ambientLight->SetLightType( AMBIENT_LIGHT );
		pEnt->AttachLight( ambientLight );
		// compute rotation vectors
		for (face = 0; face < 6; ++face)
		{
			for (y = (TILESIZE/2); y < (CUBEMAPSIZE + TILESIZE / 2); y+=TILESIZE)
			{
				for (x = (TILESIZE/2); x < (CUBEMAPSIZE + TILESIZE / 2); x+=TILESIZE)
				{
					// move pixels to center
					s = ((float)x) / (float)CUBEMAPSIZE;
					t = ((float)y) / (float)CUBEMAPSIZE;
					sc = s*2.0f - 1.0f;
					tc = t*2.0f - 1.0f;

					// create rotation vector
					vecOut.x = -sc;
					//vecOut.y = -1.0f;
					//vecOut.z = -tc;
					vecOut.z = 1.0f;
					vecOut.y = -tc;

					// normalize
					vecOut.Normalize();
					// store in vector list
					m_RotationVectors.push_back(vecOut);
				}
			}
		}
		// create face textures
		for (face = 0; face < 6; ++face)
		//for (face = 3; face < 4; ++face)
		{
			for (yTile = 0; yTile < NUMTILES; ++yTile)
			{
				for (xTile = 0; xTile < NUMTILES; ++xTile)
				{
					sprintf(temp, _T("%f, %f, %f\n"), m_RotationVectors[pos].x, m_RotationVectors[pos].y, m_RotationVectors[pos].z);
					OutputDebugString(temp);
					
					// compute pre rotation matrix based of the face
					switch (face)
					{
						case CUBEMAP_POS_X:
							Math.EulerToMatrix(0.0f, 0.0f, 0.0f, preRotMat);
								//Renderer->SetBackgroundColor( 255, 255, 0, 0 );
							break;
						case CUBEMAP_NEG_X:
							Math.EulerToMatrix(0.0f, 0.0f, 180.0f, preRotMat);
								//Renderer->SetBackgroundColor( 0, 255, 255, 0 );
							break;
						case CUBEMAP_POS_Y:
							Math.EulerToMatrix(0.0f, 0.0f, 90.0f, preRotMat);
								//Renderer->SetBackgroundColor( 0, 255, 0, 0 );
							break;
						case CUBEMAP_NEG_Y:
							Math.EulerToMatrix(0.0f, 0.0f, 270.0f, preRotMat);
								//Renderer->SetBackgroundColor( 255, 0, 0, 0 );
							break;
						case CUBEMAP_POS_Z:
							Math.EulerToMatrix(90.f, 0.0f, 0.0f, preRotMat);	
								//Renderer->SetBackgroundColor( 0, 0, 255, 0 );
							break;
						case CUBEMAP_NEG_Z: 
							Math.EulerToMatrix(270.0f, 0.0f, 0.0f, preRotMat);
								//Renderer->SetBackgroundColor( 255, 150, 0, 0 );
							break;
					}

					// establish a right vector based on the largest component of the rotation vector
					if( fabs(m_RotationVectors[pos].x) > fabs( m_RotationVectors[pos].y) 
						&& fabs(m_RotationVectors[pos].x) > fabs( m_RotationVectors[pos].z))
					{
						right = Vec3(0, 1, 0 );
					}
					else 
					{
						right = Vec3(1, 0, 0 );
					}

					// compute up vector based on the rotation vec (view vec) and the right vector
					up = -right.Cross(m_RotationVectors[pos]);

					sprintf(temp, _T("UP: %f, %f, %f\n"), up.x, up.y, up.z);
					OutputDebugString(temp);
					
					// use vision's lookat function to creat the rotation matrix
					Math.MatrixLookAt( Vec3(0.0f,0.0f,0.0f), m_RotationVectors[pos], up, matRot);
					// multiply the lookat matrix by the "pre" rotation matrix created above
					//Math.MultMatrixRight(preRotMat, matRot);
					//preRotMat = preRotMat.Times( matRot ); //Check this is the correct way
					preRotMat = matRot.Times( preRotMat ); //Check this is the correct way
					// extract euler angles from the matrix
					Math.MatrixToEuler(preRotMat, rotation);
					// rotate the object
					static DWORD msgHash_SetGlobalEuler = CHashString(_T("SetGlobalEuler")).GetUniqueID();
					m_ToolBox->SendMessage(msgHash_SetGlobalEuler,sizeof( Vec3 ), &rotation, pEnt->GetParentName() );
					pEnt->Update();

					OBJECTLIST renderList;
					renderList.push_back( (IObject*)pEnt );

					//send an immediate render to texture	
					RENDERLISTTOTEXTUREPARAMS params;
					params.dwSortMethod = 0;
					params.objList = &renderList;
					params.RenderTexture = m_renderTarget;
					params.Camera = &CameraName;
					static DWORD msgHash_RenderListToTexture = CHashString(_T("RenderListToTexture")).GetUniqueID();
					if( Renderer )
					{
						Renderer->SetBackgroundColor( 150, 150, 150, 0 );
					}
					EngineGetToolBox()->SendMessage(msgHash_RenderListToTexture, sizeof(params), &params );
					// create the texture
					WriteBits(imageBuffer, xTile, yTile, m_renderTarget);
					// increment position in rotation vector list
					++pos;
				}
			}
         
			// create face texture
			CreateTexture(filename, face, imageBuffer, map);
		}
		pEnt->DetachAllLights();
		// free buffer
		free(imageBuffer);
		//delete render target
		DELETEOBJECTPARAMS dop;
		dop.name = &RTName;
		static DWORD msgHash_DeleteObject = CHashString(_T("DeleteObject")).GetUniqueID();
		EngineGetToolBox()->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop, NULL, NULL);
		//delete camera
		dop.name = &CameraName;
		EngineGetToolBox()->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop, NULL, NULL);
		//delete light
		dop.name = &ambientLightName;
		EngineGetToolBox()->SendMessage(msgHash_DeleteObject, sizeof(DELETEOBJECTPARAMS), &dop, NULL, NULL);
	
		m_ToolBox->SendMessage(msgHash_SetGlobalPosition,sizeof( Vec3 ), &entityPosition, pEnt->GetParentName() );

	}
}


bool ObjectCubeGenerator::CreateTexture(TCHAR * szFilename, int counter, BYTE *buffer, ObjectCubeMap * map )
{
	StdString name;
	static char tempName[2048];
	static char faceName[10];
	static char saveFileName[ 2048];
	ITextureObject * pTexture = NULL;

	switch (counter)
	{
		case CUBEMAP_POS_X:
			sprintf(faceName, _T("POS_X"));
			break;
		case CUBEMAP_NEG_X:
			sprintf(faceName, _T("NEG_X"));
			break;
		case CUBEMAP_POS_Y:
			sprintf(faceName, _T("POS_Y"));
			break;
		case CUBEMAP_NEG_Y:
			sprintf(faceName, _T("NEG_Y"));
			break;
		case CUBEMAP_POS_Z:
			sprintf(faceName, _T("POS_Z"));
			break;
		case CUBEMAP_NEG_Z: 
			sprintf(faceName, _T("NEG_Z"));
			break;
	}

	// get the pathname without the extension
	//sprintf(tempName, _T("%s_%03d"), name, counter);
	sprintf(tempName, _T("%s_%s"), szFilename, faceName);
	name = tempName;
	// append extension
	name += ".tga"; 

	CHashString texname( (const TCHAR *)name );

	// convert the raw data into an HBitmap
	CREATETEXTUREPARAMS tobj;
	tobj.bitDepth = BPP*8;
	tobj.Name = &texname;
	tobj.sizeX = DESTSIZE;
	tobj.sizeY = DESTSIZE;
	tobj.TextureObjectInterface = NULL;
	tobj.numMips = 0;
	static DWORD msgHash_CreateTexture = CHashString(_T("CreateTexture")).GetUniqueID();
	DWORD res = EngineGetToolBox()->SendMessage(msgHash_CreateTexture, sizeof(tobj), &tobj );
	if( res == MSG_HANDLED )
	{
		// store the texture id in the user data of the map
		pTexture = dynamic_cast< ITextureObject * >( tobj.TextureObjectInterface );
	}
	if( !pTexture )return false;

	//get the interface
	if( !pTexture->Write( buffer ) )
	{
		EngineGetToolBox()->Log( LOGERROR, _T("Could not write to cubemap object texture\n")  );
	}
	_tcscpy( saveFileName, (const TCHAR *)name );
	//save it
	static DWORD msgHash_SaveFileByExtension = CHashString(_T("SaveFileByExtension")).GetUniqueID();
	DWORD retVal =  EngineGetToolBox()->SendMessage(msgHash_SaveFileByExtension, sizeof(LPTSTR), saveFileName );
	if( retVal == MSG_HANDLED )
	{
		//save to map data
		if( map )
		{
			map->SetFaceData( counter, texname.GetUniqueID() );
		}
		return true;
	}
	else
	{
		EngineGetToolBox()->Log( LOGERROR, _T("Error saving cubemap object texture\n")  );
		return false;
	}
}


bool ObjectCubeGenerator::WriteBits(BYTE *buffer, int xtile, int ytile, ITextureObject * renderTarget )
{
	if( !buffer ||
		!renderTarget )
	{
		return false;
	}
	const int sizeFramebuffer = VISION_INIT_SIZE*VISION_INIT_SIZE*4;
	const int sizeImageBuffer = SRCSIZE*SRCSIZE*4;
	BYTE *frameGrab;
	BYTE *image;
	int i, j;
	int sourceX;
	int sourceY;
	int y;
	BYTE *srcPtr;
	BYTE *destPtr;

	// allocate memory for color image
	image = (BYTE *)malloc( sizeImageBuffer );
	// initialize image to white
	memset( image, (BYTE)255, sizeImageBuffer );

	// grab frame from rendertarget, force a read and not a read from the saved buffer
	// reading from mip 0
	frameGrab = (BYTE*)renderTarget->Read( 0, true ); 

	// swap the colors
	if( frameGrab )
	{
		for (i = 0; i < SRCSIZE; ++i)
		{
			for (j = 0; j < SRCSIZE; ++j)
			{
				sourceX = (int)(((float)i/(float)SRCSIZE)*(float)VISION_INIT_SIZE);
				sourceY = (int)(((float)((SRCSIZE-1)-j)/(float)SRCSIZE)*(float)VISION_INIT_SIZE);
				sourceX = ( sourceX > VISION_INIT_SIZE ) ? VISION_INIT_SIZE : sourceX ;
				sourceY = ( sourceY > VISION_INIT_SIZE ) ? VISION_INIT_SIZE : sourceY ;
				char b1 = *(frameGrab + sourceY*4*VISION_INIT_SIZE + sourceX*4);
				char b2 = *(frameGrab + sourceY*4*VISION_INIT_SIZE + sourceX*4 + 1);
				char b3 = *(frameGrab + sourceY*4*VISION_INIT_SIZE + sourceX*4 + 2);
				char b4 = *(frameGrab + sourceY*4*VISION_INIT_SIZE + sourceX*4 + 3);
				*(image + j*SRCSIZE*4+  i*4) = b1;
				*(image + j*SRCSIZE*4+  i*4 + 1) = b2;
				*(image + j*SRCSIZE*4+  i*4 + 2) = b3;
				*(image + j*SRCSIZE*4+  i*4 + 3) = b4;
			}
		}
	}

	// init source pointer to the start of the image
	srcPtr = image;
	// init dest pointer
	destPtr = buffer + (((ytile * DESTSIZEBYTES) * SRCSIZE) + (xtile * SRCSIZEBYTES));

	// go through rows of the image
	for (y = 0; y < SRCSIZE; ++y)
	{
		// copy line into dest buffer
		memcpy(destPtr, srcPtr, SRCSIZEBYTES);
		// increment pointers
		srcPtr += SRCSIZEBYTES;
		destPtr += DESTSIZEBYTES;
	}

	// free buffers
	//free(frameGrab);
	free( image );
	// success!
	return true;
}