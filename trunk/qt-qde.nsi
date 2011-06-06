; qt-qde.nsi
;
; Installer of qt4 runtime
; reg key: HKLM\Trolltech\Common\4.7.3\OpenSource\Key:RegSz

;--------------------------------

; The name of the installer
Name "QtDesktop RTL"

; The file to write
OutFile "qtdesktop-rtl.exe"

; The default installation directory
InstallDir $PROGRAMFILES\QtDesktop\RTL

; Registry key to check for directory (so if you install again, it will overwrite the old one automatically)
InstallDirRegKey HKLM "Software\QtDesktop\RTL" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "Core (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "setup.nsi"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\QtDesktop\RTL "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QtDesktop-RTL" "DisplayName" "QtDesktop RTL"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QtDesktop-RTL" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QtDesktop-RTL" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QtDesktop-RTL" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\QtDesktop\RTL"
  CreateShortCut "$SMPROGRAMS\QtDesktop\RTL\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\QtDesktop\RTL\Setup.lnk" "$INSTDIR\setup.nsi" "" "$INSTDIR\setup.nsi" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\QtDesktop-RTL"
  DeleteRegKey HKLM SOFTWARE\QtDesktop\RTL

  ; Remove files and uninstaller
  Delete $INSTDIR\setup.nsi
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\QtDesktop\RTL\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\QtDesktop\RTL"
  RMDir "$INSTDIR"

SectionEnd
