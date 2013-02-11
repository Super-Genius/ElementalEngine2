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
#include "LightmapEditor.h"
#include "LightMapEditorDialog.h"
#include ".\lightmapeditordialog.h"
#include "stdio.h"
#include <string>
using namespace std;

//name filtering hacks
vector< string > g_nameFilters;
void LoadNameFilters( TCHAR * file )
{
	CREATEARCHIVE ca;
	ca.mode = 0x00000001;
	static CHashString type = _T("File");
	ca.streamType = &type;
	ca.streamData = (void*) file;
	static DWORD msgHash_CreateArchive = CHashString(_T("CreateArchive")).GetUniqueID();
	DWORD retval = EngineGetToolBox()->SendMessage(msgHash_CreateArchive, sizeof(CREATEARCHIVE), &ca);
	if ((retval == MSG_NOT_HANDLED) || (retval == MSG_ERROR) || (retval == MSG_WARNING))
	{
		return;
	}
	
	IArchive *ar = ca.archive;
	StdString szTemp;
	unsigned int numBytes = ar->Read(szTemp);
	ar->Close();
	if (numBytes > 0)
	{
		string curname = "";
		string buffer = (const TCHAR *)szTemp;
		int i = 0;
		for(;i < (int)buffer.length(); i++ )
		{
			if( buffer[ i ] == ' ' ||
				buffer[ i ] == '\n' ||
				buffer[ i ] == '\r' ||
				buffer[ i ] == '\t')
			{
				if( curname.length() > 0 )
				{
					g_nameFilters.push_back( curname );
					curname = "";
				}
				continue;
			}else
			{
				curname += buffer[ i ];
			}
		}
		if( curname.length() > 0 )
		{
			g_nameFilters.push_back( curname );
			curname = "";
		}
	}
}

bool CheckNameIsInFilter( char * name )
{
	for( int i = 0; i < (int)g_nameFilters.size(); i++) 
	{
		if( strstr( name, g_nameFilters[ i ].c_str() ) != NULL )
		{
			return true;
		}
	}
	return false;
}
// CLightMapEditorDialog dialog


IMPLEMENT_DYNAMIC(CLightMapEditorDialog, CDialog)
CLightMapEditorDialog::CLightMapEditorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLightMapEditorDialog::IDD, pParent)
{
	
}



BOOL CLightMapEditorDialog::OnInitDialog()
{
	if( !CDialog::OnInitDialog() )
	{
		return FALSE;
	}
	//Hack to exclude objects using a list:
	g_nameFilters.clear();
	LoadNameFilters( _T("lightmap_filters.txt") );
	//g_nameFilters.push_back( "Grass_Clump" );
	//g_nameFilters.push_back( "PointObject" );
	//g_nameFilters.push_back( "RenderPoint" );
	//g_nameFilters.push_back( "TestPoint" );

	char buffer[1024];
	GENERATORPARAMSMSG gParams;
	static DWORD msgHash_GetGeneratorParams = CHashString(_T("GetGeneratorParams")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_GetGeneratorParams, sizeof( gParams),  &gParams );
	itoa( gParams.NUMPHOTONS, buffer, 10 );
	m_NumPhotons.SetWindowText( buffer );
	itoa( gParams.NUMBOUNCES, buffer, 10 );
	m_Bounces.SetWindowText( buffer );
	itoa( gParams.PHOTON_SAMPLES, buffer, 10 );
	m_PhotonSamples.SetWindowText( buffer );
	sprintf( buffer, "%f", gParams.PHOTON_INDIRECT_RANGE_MAX );
	m_PhotonSampleRange.SetWindowText( buffer );
	sprintf( buffer, "%f", gParams.SHADOW_EPSILON );
	m_ShadowEpsilon.SetWindowText( buffer );
	sprintf( buffer, "%f", gParams.ENERGY_CUTOFF );
	m_EnergyCutoff.SetWindowText( buffer );

	sprintf( buffer, "%f", gParams.TEXTURE_TO_WORLD_SPACE_RATIO );
	m_UnitsPerTexel.SetWindowText( buffer );
	sprintf( buffer, "%d", gParams.MIN_TEXTURE_SIZE );
	m_MinTextureSize.SetWindowText( buffer );
	sprintf( buffer, "%d", gParams.MAX_TEXTURE_SIZE );
	m_MaxTextureSize.SetWindowText( buffer );
	sprintf( buffer, "%f", 0.f );
	m_SparseDataRange.SetWindowText( buffer );
	sprintf( buffer, "%f", 3.f );
	m_BlurDirect.SetWindowText( buffer );
	sprintf( buffer, "%f", 3.f );
	m_BlurIndirect.SetWindowText( buffer );
	UpdateData(false);
	return TRUE;
}

CLightMapEditorDialog::~CLightMapEditorDialog()
{
}

void CLightMapEditorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USEINDIRECT, m_DirectCheck );
	DDX_Control(pDX, IDC_NUMPHOTONS, m_NumPhotons);
	DDX_Control(pDX, IDC_BOUNCETEST, m_Bounces);
	DDX_Control(pDX, IDC_PHOTONSAMPLES, m_PhotonSamples);
	DDX_Control(pDX, IDC_PHOTONSAMPLERANGE, m_PhotonSampleRange);
	DDX_Control(pDX, IDC_SHADOWEPSILON, m_ShadowEpsilon);
	DDX_Control(pDX, IDC_ENERGYCUTOFF, m_EnergyCutoff);
	DDX_Control(pDX, IDC_WORLDUNITSPERTEXEL, m_UnitsPerTexel);
	DDX_Control(pDX, IDC_MINTEXTURESIZE, m_MinTextureSize);
	DDX_Control(pDX, IDC_MAXTEXTURESIZE, m_MaxTextureSize);
	DDX_Control(pDX, IDC_NORMALIZERANGE, m_SparseDataRange);
	DDX_Control(pDX, IDC_BLURDIRECTRANGE, m_BlurDirect);
	DDX_Control(pDX, IDC_BLURINDIRECTRANGE, m_BlurIndirect);
	DDX_Control(pDX, IDC_OUTPUTDIRECTORY, m_OutputDirectory );
}


BEGIN_MESSAGE_MAP(CLightMapEditorDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTONGENERATE, OnBnClickedButtonGenerate)
	ON_BN_CLICKED(IDC_USEINDIRECT, OnBnClickedUseindirect)
END_MESSAGE_MAP()


// CLightMapEditorDialog message handlers

void CLightMapEditorDialog::OnBnClickedButtonGenerate()
{
	// TODO: Add your control notification handler code here
	GENERATORPARAMSMSG gParams;
	UpdateData(true);
	DWORD state = m_DirectCheck.GetCheck();
	if( state == BST_CHECKED )
	{
		gParams.DIRECT_LIGHT_ONLY = true;
	}

	// TODO: Add your control notification handler code here
	//grab all the controls
	CString buffer;
	//set generation variables:
	gParams.SPECULAR_CHANCE = 40;
	m_NumPhotons.GetWindowText( buffer );
	gParams.NUMPHOTONS = atoi( buffer );
	m_Bounces.GetWindowText( buffer );
	gParams.NUMBOUNCES = atoi( buffer );	
	m_PhotonSamples.GetWindowText( buffer );
	gParams.PHOTON_SAMPLES = atoi( buffer );
	m_PhotonSampleRange.GetWindowText( buffer );
	gParams.PHOTON_INDIRECT_RANGE_MAX = (float)atof( buffer);	
	m_ShadowEpsilon.GetWindowText( buffer );
	gParams.SHADOW_EPSILON = (float)atof( buffer );
	m_EnergyCutoff.GetWindowText( buffer );
	gParams.ENERGY_CUTOFF = (float)atof( buffer );
	m_UnitsPerTexel.GetWindowText( buffer );
	gParams.TEXTURE_TO_WORLD_SPACE_RATIO = (float)atof( buffer );
	m_MinTextureSize.GetWindowText( buffer );
	gParams.MIN_TEXTURE_SIZE = atoi( buffer );
	m_MaxTextureSize.GetWindowText( buffer );
	gParams.MAX_TEXTURE_SIZE = atoi( buffer );
	m_SparseDataRange.GetWindowText( buffer );
	m_BlurDirect.GetWindowText( buffer );
	m_BlurIndirect.SetWindowText( buffer );

	m_OutputDirectory.GetWindowText( buffer );
	CHashString OutputHash( (TCHAR*)buffer.GetBuffer() );
	
	static DWORD msgHash_SetGeneratorParams = CHashString(_T("SetGeneratorParams")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_SetGeneratorParams, sizeof( gParams),  &gParams );
	static DWORD msgHash_SetLightmapOutputDirectory = CHashString(_T("SetLightmapOutputDirectory")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_SetLightmapOutputDirectory, sizeof( IHashString),  &OutputHash );

	//set params, go through all things attributed as lightmaps
	LightMapObjectVisitor * visitor =new LightMapObjectVisitor();
	static DWORD msgHash_VisitQuadTree = CHashString(_T("VisitQuadTree")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_VisitQuadTree, sizeof( IVisitor*),  (IVisitor*) visitor );
	ADDLIGHTMAPPABLEOBJECT addLightmappableMsg;
	static CHashString s_Terrainhash( _T("CTerrainSectorRenderObject") );
	for( int i = 0; i < (int)visitor->m_Objects.size(); i++ )
	{
		IRenderObject * obj = visitor->m_Objects[ i ];
		if( obj )
		{
			addLightmappableMsg.name = obj->GetName(); 
			if( addLightmappableMsg.name  && 
				CheckNameIsInFilter( (char*)addLightmappableMsg.name->GetString() ) == false )
			{
				addLightmappableMsg.type = obj->GetComponentType(); 
				if( addLightmappableMsg.type &&
					addLightmappableMsg.type->GetUniqueID() == s_Terrainhash.GetUniqueID() )
				{
					addLightmappableMsg.bUseIncomingTexCoord = true; 
				}
				else
				{
					addLightmappableMsg.bUseIncomingTexCoord = false; 
				}
				addLightmappableMsg.useTextureCoordIndex = 0; //use mask's which is stretched over the sector
				static DWORD msgHash_AddLightmappableObject = CHashString("AddLightmappableObject").GetUniqueID();
				EngineGetToolBox()->SendMessage(msgHash_AddLightmappableObject, sizeof( ADDLIGHTMAPPABLEOBJECT ),&addLightmappableMsg );
			}
		}
	}
	//generate it
	static DWORD msgHash_BuildLightmapScene = CHashString(_T("BuildLightmapScene")).GetUniqueID();
	EngineGetToolBox()->SendMessage(msgHash_BuildLightmapScene, 0, 0 );
	
}

void CLightMapEditorDialog::OnBnClickedUseindirect()
{
	
	
}
