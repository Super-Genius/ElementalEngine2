echo *** SYNCING TO PERFORCE ***

"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/ActiveLock/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/Beyondata/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/bison/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/boost/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/Cal3d/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/COptionTreeDLL/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/DevIL/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/DevIL-docs/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/DiagramEditor/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/Flex/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/freetype-2.2.1/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/GDAL/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/GlobalVR/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/LibXML++/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/lua/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/Max6.0SDK/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/Max7.0SDK/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/Maya5.0SDK/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/MFCGridCtrl/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/mlogger/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/Novodex/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/Novodex v2.1/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/NullSoft-Installer/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/NVIDIA/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/OpenAl 1.1 SDK/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/OpenTree/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/physfs/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/RakNet/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/scintilla/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/sizecbar-V2.44/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/Yasper/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/Zlib/..."

"C:\Program Files\Perforce\p4" sync "//depot/Development/TAS/..."
"C:\Program Files\Perforce\p4" sync "//depot/TAS/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/Heroes/..."
"C:\Program Files\Perforce\p4" sync "//depot/HerosDemo/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/Marvel/..."

"C:\Program Files\Perforce\p4" sync "//depot/Development/ElementalEngine2/..."

"C:\Program Files\Perforce\p4" sync "//depot/Development/Plugins2/..."

"C:\Program Files\Perforce\p4" sync "//depot/Development/GDS2/..."

echo %date

echo *** COMPILING GDS2-Debug ***
"C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\IDE\devenv.com" /build debug "C:\development\GDS2\GDSBase.sln" > BuildLog\GDS2-Debug-%date:~12,2%_%date:~4,2%_%date:~7,2%-%time:~0,2%_%time:~3,2%_%time:~6,2%.log

echo *** COMPILING GDS2-Release ***
"C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\IDE\devenv.com" /build release "C:\development\GDS2\GDSBase.sln" > BuildLog\GDS2-Release-%date:~12,2%_%date:~4,2%_%date:~7,2%-%time:~0,2%_%time:~3,2%_%time:~6,2%.log


