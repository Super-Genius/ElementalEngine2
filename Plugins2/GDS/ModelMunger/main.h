///============================================================================
/// \file		main.h
/// \brief		header file for model munger
/// \date		09-06-2005
/// \author		Patrick Ghiocel
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
#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "ElementalEngine.hpp"
#include "EEDefines.h"
#include "Singleton.h"
#include "CHashString.hpp"
#include "Register.h"
#include "..\..\LoadSaveManager\LoadSaveMessages.hpp"	// for certain structures from LoadSaveManager
#include "BaseMessages.h"
#include "Vec3.h"

using namespace std;

const char* g_wszBaseDirector = _T("c:\\redphoenix\\");
static int g_nModelFiles = 0;

// DEFINES FOR MUNGE OPERATIONS

// outputs to stdout model info that can be piped to a file
//#define GENERATE_MODELINFO			// 

// Generates a v3s file for each v3o file, assigning the correct shader given the properties in the model.
//#define GENERATE_V3S			// 

// Creates a "PSL" or Physics Shapes XML File from the current "SXL" or Spawn XML File
// Also modifies the SXL to no longer contain the PhysicsShape Data
//#define GENERATE_PSL						// 09/06/2005 - void GeneratePSL( char* filename )

//
#define CHECK_PSL						// 09/10/2005 - void CheckPSL( char* filename )

struct VisionMat
{
	string m_Name;
	int		m_Transparency;
	string m_Texture;
	string m_Normalmap;
};

/*
char * GetNextToken( char ** parsestring, char * delim )
{
	char * endstr;
	char * beginstr = strstr( (*parsestring), delim );
	if( beginstr )
	{
		endstr = beginstr;
		endstr++;
		if( !endstr 
}
*/

int GetRelativeVSFDirectory( char * filename )
{
	//C:RedPhoenix/
	char * nexttoken = &filename[12];
	//count number of '/'
	int size = (int)strlen( nexttoken );
	int countdirectories = 0;
	for( int i = 0; i < size; i++ )
	{
		if( nexttoken[ i ] == '\\' || nexttoken[ i ] == '/' )
		{
			countdirectories++;
		}
	}
	return countdirectories;
}

// MUNGE FUNCTION OPERATIONS

void GenerateModelInfo( char* filename )
{
	char buffer[1024];
	std::vector< string > shaderList;
	std::vector< string > matList;
	fstream fileOpen;
	fileOpen.open( filename, ios::in );
	int numsurfaces = 0;
	int surfacesvisited = 0;
	if( fileOpen.is_open() )
	{
		bool bNormalMapPresent = false;

		while( !fileOpen.eof()  )
		{
			fileOpen.getline( buffer, sizeof(buffer), '\n' );
			if( strstr( buffer, "NUMSURFACES" ) )
			{
				sscanf( &buffer[13], "%d", &numsurfaces );
//				cout << numsurfaces << endl;				
			}
			else if( strstr( buffer, "SRF," ) )
			{
				VisionMat newmat;
				//find transparency info
				char *start, *end;
				end = strstr( buffer, "," );
				for( int i = 0; i < 8; i++ ) //normal map is at comma 9
				{
					start = end;
					start++;
					end = strstr( start, "," );
				}

				*end = '\0';

				char *normalmap = start;
				bNormalMapPresent = strcmp( normalmap, " 0" ) != 0;

				char *start_surface_name = &buffer[5];
				char *end_surface_name = strstr( start_surface_name, "," );
				*end_surface_name = '\0';

				surfacesvisited++;
				if( surfacesvisited ==  numsurfaces )
				{
					break;
				}
			}			
		}

		printf( "%4i ", g_nModelFiles );
		if (bNormalMapPresent)
		{
			// normal map
			printf( "N " );
		}
		else
		{
			// no normal map
			printf( "  " );
		}
		printf( "%s\n", filename );
	}
	fileOpen.close();
}

void GenerateV3S( char* filename )
{
	char buffer[1024];
	std::vector< string > shaderList;
	std::vector< string > matList;
	fstream fileOpen;
	fileOpen.open( filename, ios::in );
	int numsurfaces = 0;
	int surfacesvisited = 0;
	if( fileOpen.is_open() )
	{
		cout << "."; // show some sign of life

		while( !fileOpen.eof()  )
		{
			fileOpen.getline( buffer, sizeof(buffer), '\n' );
			if( strstr( buffer, "NUMSURFACES" ) )
			{
				sscanf( &buffer[13], "%d", &numsurfaces );
//				cout << numsurfaces << endl;				
			}
			else if( strstr( buffer, "SRF," ) )
			{
				VisionMat newmat;
				//find transparency info
				char *start, *end;
				end = strstr( buffer, "," );
				for( int i = 0; i < 8; i++ ) //normal map is at comma 9
				{
					start = end;
					start++;
					end = strstr( start, "," );
				}

				*end = '\0';

				char *normalmap = start;

				if (strcmp( normalmap, " 0" ) == 0)
					shaderList.push_back( "Model" );
				else
					shaderList.push_back( "Model_normalmap" );

				char *start_surface_name = &buffer[5];
				char *end_surface_name = strstr( start_surface_name, "," );
				*end_surface_name = '\0';
//				cout << start_surface_name << endl;
				matList.push_back( start_surface_name );

				surfacesvisited++;
				if( surfacesvisited ==  numsurfaces )
				{
					break;
				}
			}			
		}		
	}
	fileOpen.close();
	fstream fileWrite;
	strcpy( buffer, filename );
	int size = (int)strlen( buffer );
	buffer[ size - 1 ] = 'S';
	fileWrite.open( buffer, ios::out | ios::trunc );
	fileWrite << "[VNUM=1.01]" << endl << "FXLIB, ";
	int loopdirs = GetRelativeVSFDirectory( filename ) - 1;
	for( int i = 0; i < loopdirs; i++ )
	{
		fileWrite << "..\\";
	}

	fileWrite << "Shader\\Terrain.vsf" << endl;

	//loop materials
	for( int i = 0; i < numsurfaces; i++ )
	{
		fileWrite << "SRFFX, ";
		fileWrite << matList[i];
		fileWrite << ", 1, ";
		fileWrite << shaderList[i];
		fileWrite << ":" << endl;
	}
	fileWrite.close();
}

// Creates a "PSL" or Physics Shapes XML File from the current "SXL" or Spawn XML File
// Also modifies the SXL to no longer contain the PhysicsShape Data
//#define GENERATE_PSL						// 09/06/2005 - void CreatePSL( char* filename )

//<CSpawner Name="World">
struct CSpawnerStruct
{
	StdString szName;
};

//<CEntity Name="SpawnEntity" EntityTypes="models\objects\trees\">
struct CEntityStruct
{
	StdString szName;
	StdString szType;
};

//<CV3ORenderObject Name="Spawn_RenderObj" FileName="models\objects\trees\veg_tree_maple_spring" 
// PositionX="0" PositionY="0" PositionZ="0" RotationX="0" RotationY="0" RotationZ="0" ScalingX="0.025000" 
// ScalingY="0.025000" ScalingZ="0.025000">
struct CV3ORenderObjectStruct
{
	StdString szName;
	StdString szFileName;
	Vec3 vPosition;
	Vec3 vRotation;
	Vec3 vScale;
};

//<CPhysicsObject Name="Spawn_PhysicsObject" posX="0.000000" posY="0.000000" posZ="0.000000" rotX="0.000000" 
// rotY="0.000000" rotZ="0.000000" scaleX="0.025000" scaleY="0.025000" scaleZ="0.025000" mass="1.000000" 
// dynamics="dynamic" numshapes="1">
struct CPhysObjectStruct
{
	StdString szName;
	Vec3 vPosition;
	Vec3 vRotation;
	Vec3 vScale;
	float fMass;
	StdString szDynamic;
	int iNumShapes;
};

//<CPhysShape shape="box" posX="1.55624" posY="2.29899" posZ="-77.5794" rotX="0" rotY="0" rotZ="0" 
// boxX="24.5657" boxY="19.0305" boxZ="214.128" group="0" Link="tree_Point">
struct CPhysShapeBoxStruct
{
	StdString szShape;
	Vec3 vPosition;
	Vec3 vRotation;
	Vec3 vBBox;
	int iGroup;
	StdString szLink;
};

//<CPhysShape shape="sphere" posX="1.55624" posY="2.29899" posZ="-77.5794" rotX="0" rotY="0" rotZ="0" 
// radius="15.5" group="0" Link="tree_Point">
struct CPhysShapeSphereStruct
{
	StdString szShape;
	Vec3 vPosition;
	Vec3 vRotation;
	float fRadius;
	int iGroup;
	StdString szLink;
};

void GeneratePSL( char* filename )
{
	// Format:
	/*
	<?xml version="1.0"?>
	<CSpawner Name="World">
	<CEntity Name="SpawnEntity" EntityTypes="models\objects\trees\">
		<CV3ORenderObject Name="Spawn_RenderObj" FileName="models\objects\trees\veg_tree_maple_spring" PositionX="0" PositionY="0" PositionZ="0" RotationX="0" RotationY="0" RotationZ="0" ScalingX="0.025000" ScalingY="0.025000" ScalingZ="0.025000">
		</CV3ORenderObject>
		<CPhysicsObject Name="Spawn_PhysicsObject" posX="0.000000" posY="0.000000" posZ="0.000000" rotX="0.000000" rotY="0.000000" rotZ="0.000000" scaleX="0.025000" scaleY="0.025000" scaleZ="0.025000" mass="1.000000" group="0" dynamics="dynamic" numshapes="1">
			<CPhysShape shape="box" posX="1.55624" posY="2.29899" posZ="-77.5794" rotX="0" rotY="0" rotZ="0" boxX="24.5657" boxY="19.0305" boxZ="214.128" Link="tree_Point">
			</CPhysShape>
		</CPhysicsObject>
	</CEntity>
	</CSpawner>
	*/
	CSpawnerStruct tmpCSpawner;
	CEntityStruct tmpCEntity;
	CV3ORenderObjectStruct tmpCV3ORenderObject;
	CPhysObjectStruct tmpCPhysObject;
	std::vector<CPhysShapeBoxStruct> tmpBoxShapes;
	std::vector<CPhysShapeSphereStruct> tmpSphereShapes;

	// Setup File Paths for PSL File
	char shapeFilePath[260];
	strcpy(shapeFilePath, filename );
	char* tmpptr = strstr( shapeFilePath, "sxl" );
	tmpptr[0] = 'p';
	tmpptr[1] = 's';
	tmpptr[2] = 'l';
	int pathlen = (int)strlen( shapeFilePath );
	int basepathlen = (int)strlen( g_wszBaseDirector );

	char shapeFile[260] = "";
	tmpptr = strstr( shapeFilePath, g_wszBaseDirector );
	strncpy( shapeFile, tmpptr + basepathlen, pathlen - basepathlen );

	// Quick Check to see if the PSL already exists. if so, exit out early
	FILE* testOpen;
	testOpen = fopen( shapeFilePath, "rt" );

	if( testOpen != NULL )
	{
		fclose(testOpen);
		return;
	}
	
	// Parse SXL
	DWORD retVal;
	IXMLArchive *Archive = NULL;
	CHashString streamType(_T("File"));
	CREATEARCHIVE caRead;
	caRead.streamData = (void *)filename;
	caRead.mode = STREAM_MODE_READ;
	caRead.streamType = &streamType;
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (retVal = EngineGetToolBox()->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &caRead) != MSG_HANDLED)
	{
		return;
	}
	Archive = dynamic_cast<IXMLArchive *>(caRead.archive);

	// CSpawner
	Archive->Read( tmpCSpawner.szName, _T("Name") );
	// CEntity
	Archive->Read( tmpCEntity.szName, _T("Name")  );
	Archive->Read( tmpCEntity.szType, _T("EntityTypes")  );
	// CV3ORenderObject
	Archive->Read( tmpCV3ORenderObject.szName, _T("Name") );
	Archive->Read( tmpCV3ORenderObject.szFileName, _T("FileName") );
	Archive->Read( tmpCV3ORenderObject.vPosition, _T("Position") );
	Archive->Read( tmpCV3ORenderObject.vRotation, _T("Rotation") );
	Archive->Read( tmpCV3ORenderObject.vScale, _T("Scaling") );
	// CPhysicsObject
	Archive->Read( tmpCPhysObject.szName, _T("Name") );
	Archive->Read( tmpCPhysObject.vPosition, _T("pos") );
	Archive->Read( tmpCPhysObject.vRotation, _T("rot") );
	Archive->Read( tmpCPhysObject.vScale, _T("scale") );
	Archive->Read( tmpCPhysObject.fMass, _T("mass") );
	Archive->Read( tmpCPhysObject.szDynamic, _T("dynamics") );
	Archive->Read( tmpCPhysObject.iNumShapes, _T("numshapes") );

	for( int i = 0; i < tmpCPhysObject.iNumShapes; i++ )
	{
		StdString tmpType;
		Archive->Read( tmpType, _T("shape") );

		if( strcmp( tmpType, "box" ) == 0 )
		{
			CPhysShapeBoxStruct tmpBoxShape;
			tmpBoxShape.szShape = tmpType;

			Archive->Read( tmpBoxShape.vPosition, _T("pos") );
			Archive->Read( tmpBoxShape.vRotation, _T("rot") );
			Archive->Read( tmpBoxShape.vBBox, _T("box") );
			Archive->Read( tmpBoxShape.iGroup, _T("group") );
			Archive->Read( tmpBoxShape.szLink, _T("Link") );
			
			tmpBoxShapes.push_back( tmpBoxShape );
		}
		else if( strcmp( tmpType, "sphere" ) )
		{
			CPhysShapeSphereStruct tmpSphereShape;
			tmpSphereShape.szShape = tmpType;

			Archive->Read( tmpSphereShape.vPosition, _T("pos") );
			Archive->Read( tmpSphereShape.vRotation, _T("rot") );
			Archive->Read( tmpSphereShape.fRadius, _T("radius") );
			Archive->Read( tmpSphereShape.iGroup, _T("group")  );
			Archive->Read( tmpSphereShape.szLink, _T("Link")  );

			tmpSphereShapes.push_back( tmpSphereShape );
		}
	}
	Archive->Close();

	// Save SXL
	CREATEARCHIVE caSXLOut;
	caSXLOut.streamData = filename;
	caSXLOut.mode = STREAM_MODE_WRITE;
	if (EngineGetToolBox()->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &caSXLOut) != MSG_HANDLED)
	{
		return;
	}
	Archive = dynamic_cast<IXMLArchive *>(caSXLOut.archive);

	// CSpawner
	Archive->StartClass( _T("CSpawner") );
	Archive->Write( tmpCSpawner.szName, _T("Name") );
	// CEntity
	Archive->StartClass( _T("CEntity") );
	Archive->Write( tmpCEntity.szName, _T("Name")  );
	Archive->Write( tmpCEntity.szType, _T("EntityTypes")  );
	// CV3ORenderObject
	Archive->StartClass( _T("CV3ORenderObject") );
	Archive->Write( tmpCV3ORenderObject.szName, _T("Name") );
	Archive->Write( tmpCV3ORenderObject.szFileName, _T("FileName") );
	Archive->Write( tmpCV3ORenderObject.vPosition, _T("Position") );
	Archive->Write( tmpCV3ORenderObject.vRotation, _T("Rotation") );
	Archive->Write( tmpCV3ORenderObject.vScale, _T("Scaling") );
	Archive->EndClass();
	// CPhysicsObject
	Archive->StartClass( _T("CPhysicsObject") );
	Archive->Write( tmpCPhysObject.szName, _T("Name") );
	Archive->Write( tmpCPhysObject.vPosition, _T("pos") );
	Archive->Write( tmpCPhysObject.vRotation, _T("rot") );
	Archive->Write( tmpCPhysObject.vScale, _T("scale") );
	Archive->Write( tmpCPhysObject.fMass, _T("mass") );
	Archive->Write( tmpCPhysObject.szDynamic, _T("dynamics") );
	Archive->Write( shapeFile, _T("shapeFile") );
	Archive->EndClass();
	// CEntity
	Archive->EndClass();
	// CSpawner
	Archive->EndClass();

	Archive->Close();

	// Create PSL
	CREATEARCHIVE caPSLOut;
	caPSLOut.streamData = shapeFilePath;
	caPSLOut.mode = STREAM_MODE_WRITE;
	if (EngineGetToolBox()->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &caPSLOut) != MSG_HANDLED)
	{
		return;
	}
	Archive = dynamic_cast<IXMLArchive *>(caPSLOut.archive);

	// CPhysShapeDescriptor
	Archive->StartClass( _T("CPhysShapeDescriptor") );
	Archive->Write( (int)(tmpBoxShapes.size() + tmpSphereShapes.size()) , _T("numshapes") );
	for( unsigned int i = 0; i < tmpBoxShapes.size(); i++ )
	{
		Archive->StartClass( _T("CPhysShape") );

		Archive->Write( tmpBoxShapes[i].szShape, _T("shape") );
		Archive->Write( tmpBoxShapes[i].vPosition, _T("pos") );
		Archive->Write( tmpBoxShapes[i].vRotation, _T("rot") );
		Archive->Write( tmpBoxShapes[i].vBBox, _T("box") );
		Archive->Write( tmpBoxShapes[i].iGroup, _T("group") );
		Archive->Write( tmpBoxShapes[i].szLink, _T("Link") );

		Archive->EndClass();
	}
	for( unsigned int i = 0; i < tmpSphereShapes.size(); i++ )
	{
		Archive->StartClass( _T("CPhysShape") );

		Archive->Write( tmpSphereShapes[i].szShape, _T("shape") );
		Archive->Write( tmpSphereShapes[i].vPosition, _T("pos") );
		Archive->Write( tmpSphereShapes[i].vRotation, _T("rot") );
		Archive->Write( tmpSphereShapes[i].fRadius, _T("radius") );
		Archive->Write( tmpSphereShapes[i].iGroup, _T("group")  );
		Archive->Write( tmpSphereShapes[i].szLink, _T("Link")  );

		Archive->EndClass();
	}
	// CPhysShapeDescriptor
	Archive->EndClass();

	Archive->Close();

	printf(".");
}

void CheckPSL( char* filename )
{
	DWORD retVal;
	IXMLArchive *Archive = NULL;
	CHashString streamType(_T("File"));
	CREATEARCHIVE caRead;
	caRead.streamData = (void *)filename;
	caRead.mode = STREAM_MODE_READ;
	caRead.streamType = &streamType;
	static DWORD msgHash_CreateXMLArchive = CHashString(_T("CreateXMLArchive")).GetUniqueID();
	if (retVal = EngineGetToolBox()->SendMessage(msgHash_CreateXMLArchive, sizeof(CREATEARCHIVE), &caRead) != MSG_HANDLED)
	{
		return;
	}
	Archive = dynamic_cast<IXMLArchive *>(caRead.archive);


	StdString szCPhysShapeDescriptor;
	StdString szCPhysShape;
	StdString szShape;
	int	iNumShapes;

	Archive->GetNode( szCPhysShapeDescriptor );

	if( szCPhysShapeDescriptor != _T("CPhysShapeDescriptor") )
	{
		return;
	}

	Archive->Read( iNumShapes, _T("numshapes") );

	for( int i = 0; i < iNumShapes; i++ )
	{
		Archive->GetNode( szCPhysShape );
		Archive->Read( szShape, _T("shape") );

		if( (strcmp( szShape, "box" ) != 0) && (strcmp( szShape, "sphere" ) != 0) )
		{
			printf("%s\n", filename);
			Archive->Close();
			return;
		}
	}

	Archive->Close();
}

void RecurseThroughFiles( char * startdir )
{
	WIN32_FIND_DATA FindFileData;
	bool bKeepSearching = true;
	string cur_search;
	string cur_path;
	string curdir;

	//build a search out of the path:
	curdir = startdir;
	cur_search = startdir;
	cur_search += "*";	

	HANDLE search = FindFirstFile( (char*)cur_search.c_str(),  &FindFileData );				
	if (search == INVALID_HANDLE_VALUE) 
	{
		//BAD MUNGER
		return;
	} 
	else
	{
		while( bKeepSearching )
		{
			if (!FindNextFile(search, &FindFileData)) 
			{
				bKeepSearching = false;
				break;
			}
			//add to groups
			if(  FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				if( !strstr( FindFileData.cFileName, "." ) )
				{		
					cur_path = curdir;
					cur_path += FindFileData.cFileName;
					cur_path += "\\";
					RecurseThroughFiles( (char*)cur_path.c_str() );
				}
			}
			
			else //FILE
			{
                //we do whatever to file
				//cout << FindFileData.cFileName << endl;
				if( strstr( FindFileData.cFileName, ".V3O" ) )
				{		
					//apply munger to file
					cur_path = curdir;
					cur_path += FindFileData.cFileName;

					// PLACE MUNGE APPLICATION HERE

#ifdef GENERATE_MODELINFO
					GenerateModelInfo( (char*)cur_path.c_str() );
#endif

#ifdef GENERATE_V3S
					GenerateV3S( (char*)cur_path.c_str() );
					g_nModelFiles++;
#endif
				}

#ifdef GENERATE_PSL						
				if( strstr(FindFileData.cFileName, ".SXL") || strstr(FindFileData.cFileName, ".sxl") )
				{
					cur_path = curdir;
					cur_path += FindFileData.cFileName;

					GeneratePSL( (char*)cur_path.c_str() );
				}
#endif

#ifdef CHECK_PSL
				if( strstr(FindFileData.cFileName, ".PSL") || strstr(FindFileData.cFileName, ".psl") )
				{
					cur_path = curdir;
					cur_path += FindFileData.cFileName;

					CheckPSL( (char*)cur_path.c_str() );
				}
#endif

			}
		}				
		FindClose(search);
	}
}
