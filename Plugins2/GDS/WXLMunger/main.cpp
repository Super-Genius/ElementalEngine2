///============================================================================
/// \file		main.cpp
/// \brief		wxl munger application
/// \date		08-09-2005
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
#include "Main.h"
#include "WorldVisitor.h" // Used to visit hierarchy
#include "WorldLoader.h"

char g_MapList[] = {"C:\\RedPhoenix\\Maps\\MapList.txt"};
char g_BaseDir[] = {"C:\\RedPhoenix\\Maps\\"};
char g_Report[] = {"C:\\RedPhoenix\\Models\\ModelReport.xml"};


void main( void )
{
	printf("START WXLMUNGER\n\n");
	
	IToolBox* m_ToolBox = EngineGetToolBox();
	HINSTANCE m_LoadSaveManager = LoadLibrary(_T("LoadSaveManager.dll"));
	HINSTANCE m_TimeOfDayObjectDLL = LoadLibrary(_T("plugins/timeofday.dlo"));
	std::vector<StdString> vMapList;
	

	// Load Map List File
	printf("LOADING MAPLIST: %s\n", g_MapList);
	FILE *fpMapList = NULL;
	fpMapList = fopen( g_MapList, "ra" );
	while( feof( fpMapList ) == 0 )
	{
		char tmp1[255] = "\0";
		char tmp2[255] = "\0";

		strcpy( tmp1, g_BaseDir );
		fgets( tmp2, 255, fpMapList );
		int read = (int)strlen(tmp2);
		tmp2[read-1] = '\0';
		strcat( tmp1, tmp2 );
		StdString tmp( tmp1 );
		vMapList.push_back(tmp);
	}
	fclose( fpMapList );

	// Create World Visitor
	CWorldVisitor WorldVisitor;
	WorldVisitor.CreateReport( g_Report );
	WorldVisitor.SetObjectExclusion( true );
	//WorldVisitor.AddObject( _T("CRenderObject") );
	WorldVisitor.AddObject( _T("CV3ORenderObject") );
	WorldVisitor.AddObject( _T("CPhysicsObject") );
	printf("\n");

	// DEBUG STUFF
	//vMapList.clear();
	//vMapList.push_back(StdString("C:\\RedPhoenix\\maps\\Maps_munger.wxl"));
	// END DEBUG

	for( unsigned int i = 0; i < vMapList.size(); i++ )
	{
		printf("LOADING WORLD: %s\n", (const char*)vMapList[i]);
		//Load WXL File
		CWorldLoader WorldLoader;
		if( WorldLoader.LoadWorld( vMapList[i] ) == false )
		{	
			printf("*ERROR LOADING WORLD: %s\n", (const char*)vMapList[i]);
			
			static DWORD msgHash_ClearHierarchy = CHashString(_T("ClearHierarchy")).GetUniqueID();
			m_ToolBox->SendMessage(msgHash_ClearHierarchy, 0, NULL, NULL, NULL);
			FreeLibrary(m_LoadSaveManager);

			return;
		}

		printf("PARSING WORLD: %s\n", (const char*)vMapList[i]);
		// Visit Hierarchy	
		VISITHIERARCHYDATA vhd;
		vhd.name = NULL;
		vhd.visitor = &WorldVisitor;
		static DWORD msgHash_VisitHierarchy = CHashString(_T("VisitHierarchy")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_VisitHierarchy, sizeof(VISITHIERARCHYDATA), &vhd, NULL, NULL);
	
		// Save WXL File
		
		CWorldLoader WorldSaver;
		printf("SAVING WORLD: %s\n", (const char*)vMapList[i]);
		if( WorldSaver.SaveWorld( vMapList[i] ) == false )
		{
			printf("World Savinging Error\n");
		}
		
		static DWORD msgHash_ClearHierarchy = CHashString(_T("ClearHierarchy")).GetUniqueID();
		m_ToolBox->SendMessage(msgHash_ClearHierarchy, 0, NULL, NULL, NULL);
		printf("\n");
	}

	printf("OUTPUTING REPORT: %s\n", g_Report);
	WorldVisitor.OutputReport();
	FreeLibrary(m_LoadSaveManager);
	system("PAUSE");
	return;
}