///============================================================================
/// \file		main.cpp
/// \brief		model munger application
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
#include "Main.h"
#include <conio.h>

//MUNGER BOY!
int main( int argc, int argv[] )
{
	RecurseThroughFiles( "c:\\redphoenix\\models\\" );
	printf( "\nModels Munged: %i\n", g_nModelFiles );
	getch();
	return 0;
}