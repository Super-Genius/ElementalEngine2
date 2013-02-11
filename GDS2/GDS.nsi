; GDS
; 10/26/2007

;--------------------------------

; Set to not use compression
SetCompress off

; The name of the installer
Name "GDS"

; The file to write
OutFile "GDS Installer.exe"

; The default installation directory

InstallDir "$PROGRAMFILES\Graffiti Entertainment\GDS"

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "SOFTWARE\GRAFFITI ENTERTAINMENT\GDS\GDS" "Install_Dir"

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "GDS (required)"

  SectionIn RO
  
  ; uninstalls previous installation, if there
  Call removeOldInstallFunc
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File /r "bin\release\*.*"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM "SOFTWARE\GRAFFITI ENTERTAINMENT\GDS" "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\GDS" "DisplayName" "NSIS GDS"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\GDS" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\GDS" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\GDS" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

;--------------------------------

Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Graffiti Entertainment\GDS"
  CreateShortCut "$SMPROGRAMS\Graffiti Entertainment\GDS\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Graffiti Entertainment\GDS\GDS.lnk" "$INSTDIR\GDS.exe" "" "$INSTDIR\GDS.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\GDS"
  DeleteRegKey HKLM "SOFTWARE\GRAFFITI ENTERTAINMENT\GDS"
  
  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Graffiti Entertainment\GDS\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\Graffiti Entertainment\GDS"
  RMDir /r "$INSTDIR"

  ; Remove files and uninstaller  
  Delete $INSTDIR\*.*  

SectionEnd

;-------------------------------
Function removeOldInstallFunc

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\GDS"
  DeleteRegKey HKLM "SOFTWARE\GRAFFITI ENTERTAINMENT\GDS"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Graffiti Entertainment\GDS\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\Graffiti Entertainment\GDS"
  RMDir /r "$INSTDIR"

FunctionEnd