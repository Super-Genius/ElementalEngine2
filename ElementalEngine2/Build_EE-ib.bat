echo *** SYNCING TO PERFORCE ***

"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/ActiveLock/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/Beyondata/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/bison/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/boost/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/Cal3d/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/COptionTreeDLL/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/DevIL/..."
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
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/NVIDIA/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/Novodex/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/OpenAl 1.1 SDK/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/OpenTree/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/physfs/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/RakNet/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/Scintilla/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/sizecbar-V2.44/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/Yasper/..."
"C:\Program Files\Perforce\p4" sync "//depot/Development/3rd Party/zlib/..."

"C:\Program Files\Perforce\p4" sync "//depot/Development/ElementalEngine2/..."

echo *** COMPILING EE2-Debug ***
"C:\Program Files\Xoreax\IncrediBuild\BuildConsole.exe" /build /cfg="debug" "C:\development\ElementalEngine2\ElementalEngine.sln" > BuildLog\EE-Debug-%date:~12,2%_%date:~4,2%_%date:~7,2%-%time:~0,2%_%time:~3,2%_%time:~6,2%.log

echo *** COMPILING EE2-Release ***
"C:\Program Files\Xoreax\IncrediBuild\BuildConsole.exe" /build /cfg="release" "C:\development\ElementalEngine2\ElementalEngine.sln" > BuildLog\EE-Release-%date:~12,2%_%date:~4,2%_%date:~7,2%-%time:~0,2%_%time:~3,2%_%time:~6,2%.log


