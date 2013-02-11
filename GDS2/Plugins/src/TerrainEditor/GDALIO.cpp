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

#include "StdAfx.h"

#include "gdal_priv.h"
#include "ogrsf_frmts.h"

// the geospatial formats we recognize
TCHAR *RecognizedImportFormats = {
		_T("All Files (*.*)\0" "*.*\0")
		_T("Arc/Info Ascii Grid Files (*.grd)\0" "*.grd\0")
		_T("Arc/Info Binary Grid Files (*.adf)\0" "*.adf\0")
		_T("BSB Nautical Chart Format (*.kap)\0" "*.kap\0")
		_T("Microsoft Windows Device Independent Bitmap (*.bmp) \0" "*.bmp\0")
		_T("VTP Binary Terrain Format (*.bt)\0" "*.bt\0")
		_T("First Generation USGS DOQ (*.doq)\0" "*.doq\0")
		_T("Military Elevation Data (*.dt?)\0" "*.dt?\0")
		_T("ERMapper Compressed Wavelets (*.ecw)\0" "*.ecw\0")
		_T("ENVI Labelled Raster (*.hdr)\0" "*.hdr\0")
		_T("Envisat Image Product (*.n1)\0" "*.n1\0")
		_T("FITS (*.fits)\0" "*.fits\0")
		_T("Graphics Interchange Format (*.gif)\0" "*.gif\0") 
		_T("TIFF / GeoTIFF (*.tif)\0" "*.tif\0")
		_T("JPEG JFIF (*.jpg)\0" "*.jpg;\0")
		_T("JPEG2000 (*.jp2, *.j2k)\0" "*.jpg; *.j2k\0")
		_T("USGS ASCII DEM (*.dem)\0" "*.dem\0")
		_T("Erdas Imagine (*.img)\0" "*.img\0")
		_T("Raster Matrix Format (*.rsw, *.mtw)\0" "*.rsw;*.mtw\0")
		_T("USGS SDTS DEM (*catd.ddf)\0" "*catd.ddf\0")
		_T("X11 Pixmap (.xpm)\0" "*.xpm\0")
		_T("\0\0")
};

TCHAR *RecognizedExportFormats = {
		_T("Arc/Info Ascii Grid Files (*.grd)\0" "*.grd\0")
		_T("Arc/Info Binary Grid Files (*.adf)\0" "*.adf\0")
		_T("Microsoft Windows Device Independent Bitmap (*.bmp) \0" "*.bmp\0")
		_T("VTP Binary Terrain Format (*.bt)\0" "*.bt\0")
		_T("ERMapper Compressed Wavelets (*.ecw)\0" "*.ecw\0")
		_T("ENVI Labelled Raster (*.hdr)\0" "*.hdr\0")
		_T("FITS (*.fits)\0" "*.fits\0")
		_T("Graphics Interchange Format (*.gif)\0" "*.gif\0") 
		_T("TIFF / GeoTIFF (*.tif)\0" "*.tif\0")
		_T("JPEG JFIF (*.jpg)\0" "*.jpg;\0")
		_T("JPEG2000 (*.jp2, *.j2k)\0" "*.jpg; *.j2k\0") 
		_T("Erdas Imagine (*.img)\0" "*.img\0")
		_T("Raster Matrix Format (*.rsw, *.mtw)\0" "*.rsw;*.mtw\0")
		_T("X11 Pixmap (.xpm)\0" "*.xpm\0")
		_T("\0\0")
};

// constructor
CGDALIO::CGDALIO()
{
    GDALAllRegister();
}

CGDALIO::~CGDALIO()
{
	delete OGRSFDriverRegistrar::GetRegistrar();
    GDALDestroyDriverManager();
}

//
//    adfGeoTransform[0] /* top left x */
//    adfGeoTransform[1] /* w-e pixel resolution */
//    adfGeoTransform[2] /* rotation, 0 if image is "north up" */
//    adfGeoTransform[3] /* top left y */
//    adfGeoTransform[4] /* rotation, 0 if image is "north up" */
//    adfGeoTransform[5] /* n-s pixel resolution */

bool CGDALIO::Import(TERRAINCREATIONPARAMS *tcParms)
{
	TCHAR OpenFileName[_MAX_PATH];
	OPENFILENAME Ofn;
	double adfGeoTransform[6];
	GDALDataset  *poDataset;
	CString errMess;
	IToolBox *pToolBox;

	pToolBox = EngineGetToolBox();
	if (!pToolBox)
	{
		return false;
	}

	// let's open all files up, because some GeoSpatial files
	// don't have extensions
	memset(&Ofn, 0, sizeof(OPENFILENAME));
	Ofn.lStructSize = sizeof(OPENFILENAME);
	Ofn.lpstrFilter = RecognizedImportFormats;
	Ofn.nFilterIndex = 1;
	sprintf(OpenFileName, "*.*");
	Ofn.lpstrFile = OpenFileName;
	Ofn.nMaxFile = _MAX_PATH;
	Ofn.lpstrTitle = _T("Open GeoSpatial File");
	Ofn.hwndOwner = AfxGetApp()->GetMainWnd()->m_hWnd;
	Ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

	if (!GetOpenFileName(&Ofn))
	{
		// cancel selected.
		return false;
	}

	poDataset = (GDALDataset *) GDALOpen( OpenFileName, GA_ReadOnly );
	if( poDataset == NULL )
    {
		errMess.Format(_T("Unable to open File\n%s\nError: %s"),
			OpenFileName, CPLGetLastErrorMsg());
		MessageBox(NULL, _T("File Open Error!"), (const TCHAR *)errMess, MB_OK);
		return false;
    }

    if (poDataset->GetGeoTransform( adfGeoTransform ) != CE_None )
    {
		errMess.Format(_T("Unable to retrieve dimensions of File\n%s\nError: %s"),
			OpenFileName, CPLGetLastErrorMsg());
		MessageBox(NULL, _T("File Open Error!"), (const TCHAR *)errMess, MB_OK);
		GDALClose(poDataset);
		return false;
	}

	CHashString filePath;
	static DWORD msgHash_GetSceneSavePath = CHashString(_T("GetSceneSavePath")).GetUniqueID();
	pToolBox->SendMessage(msgHash_GetSceneSavePath, sizeof(IHashString*), &filePath );
	TCHAR relPathName[_MAX_PATH];
	_tcscpy( relPathName, filePath.GetString() );
	// split up the path
	TCHAR newPath[_MAX_PATH];
	TCHAR newName[_MAX_FNAME];
	_tsplitpath(pToolBox->GetRelativePath(relPathName), NULL, newPath, newName, NULL);
	// make sure lower case for IHashString Compares
	_tcslwr(newPath);
	_tcslwr(newName);

	// clear the sector modified map.
	SINGLETONINSTANCE(CTerrainEditor)->OnWorldModified(false);

    errMess.Format( _T("Size is %dx%dx%d\n"), 
            poDataset->GetRasterXSize(), poDataset->GetRasterYSize(),
            poDataset->GetRasterCount() );

	errMess.Format(_T( "Origin = (%.6f,%.6f)\n"),
			adfGeoTransform[0], adfGeoTransform[3] );

	errMess.Format(_T("Pixel Size = (%.6f,%.6f)\n"),
			adfGeoTransform[1], adfGeoTransform[5] );

	// for now assume data is in meters
	tcParms->TerrainWidth = (DWORD)ceil(((float)poDataset->GetRasterXSize() * 
							fabs(adfGeoTransform[1])) / tcParms->SectorSize);
	tcParms->TerrainHeight = (DWORD)ceil(((float)poDataset->GetRasterYSize() * 
							fabs(adfGeoTransform[5])) / tcParms->SectorSize);

	//Temp: Pause thumbnail thread:
	bool pause = true;
	static DWORD msgHash_PauseThumbnailGenerationThread = CHashString(_T("PauseThumbnailGenerationThread")).GetUniqueID();
	pToolBox->SendMessage(msgHash_PauseThumbnailGenerationThread, sizeof( bool * ),&pause, NULL, NULL );
	//End Temp
	
	static DWORD msgHash_TerrainManagerCreateTerrain = CHashString(_T("TerrainManagerCreateTerrain")).GetUniqueID();
	pToolBox->SendMessage(msgHash_TerrainManagerCreateTerrain, sizeof( TERRAINCREATIONPARAMS ),
		tcParms, NULL, NULL );

	//Temp: Unpause thumbnail thread
	pause = false;
	pToolBox->SendMessage(msgHash_PauseThumbnailGenerationThread, sizeof( bool * ),&pause, NULL, NULL );
	//End Temp

	static DWORD msgHash_WorldModified = CHashString(_T("WorldModified")).GetUniqueID();
	pToolBox->SendMessage(msgHash_WorldModified, sizeof(BOOL), (void *)true);

	SINGLETONINSTANCE(CTerrainEditor)->SetAllSectorsModified();

	GDALClose(poDataset);
	return true;

}