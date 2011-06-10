; qtdesktop-rtl.nsi
;
; Installer of qt4 runtime
; reg key: HKLM\Trolltech\Common\4.7.3\OpenSource\Key:RegSz
; TODO:
; - skip license/directory if installed
; - delete unchecked items if installed

!define PRODUCT_NAME "QtDesktop-RTL"
!define PRODUCT_VERSION "4.7.1"
!define PRODUCT_PUBLISHER "QtDesktop"
!define PRODUCT_WEB_SITE "http://www.qtdesktop.org/"
!define PRODUCT_DIR_REGKEY "Software\QtDesktop\RTL"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

Var /GLOBAL INSTALLED

SetCompressor lzma
BrandingText "www.qtdesktop.org"
XPStyle on
RequestExecutionLevel admin

!include "MUI.nsh"
!include "Library.nsh"
!include "Sections.nsh"

; MUI Settings
!define MUI_ABORTWARNING
;!define MUI_ICON "installer.ico"
;!define MUI_UNICON "uninstaller.ico"

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page (if new installation)
!define MUI_LICENSEPAGE_RADIOBUTTONS
!insertmacro MUI_PAGE_LICENSE "gpl.txt"
; Components page
!insertmacro MUI_PAGE_COMPONENTS
; Directory page (if new installation)
!insertmacro MUI_PAGE_DIRECTORY
; Start menu page (if new)
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "QtDesktop\RTL"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
; on new and installed
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Russian"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}-${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES\QtDesktop\RTL"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" "Path"
ShowInstDetails show
ShowUnInstDetails show

;--------------------------------
Section "Core" SEC01
  SectionIn RO
  StrCmp $INSTALLED "1" EndSec01
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\libgcc_s_sjlj-1.dll" "$INSTDIR\libgcc_s_sjlj-1.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\libstdc++-6.dll" "$INSTDIR\libstdc++-6.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\zlib1.dll" "$INSTDIR\zlib1.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\libpng14-14.dll" "$INSTDIR\libpng14-14.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtCore4.dll" "$INSTDIR\QtCore4.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtGui4.dll" "$INSTDIR\QtGui4.dll" $INSTDIR
  EndSec01:
SectionEnd

Section "Network" SEC02
  IfFileExists "$INSTDIR\QtNetwork4.dll" +2
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtNetwork4.dll" "$INSTDIR\QtNetwork4.dll" $INSTDIR
SectionEnd

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
  ; check installed
  ReadRegStr $0 HKLM "${PRODUCT_DIR_REGKEY}" "Path"
  IfErrors New
   ; installed
   StrCpy $INSTALLED "1"
   StrCpy $INSTDIR $0
   ; Network
   IfFileExists "$INSTDIR\QtNetwork4.dll" 0 +3
    SectionSetFlags ${SEC02} ${SF_SELECTED}
    Goto Chk03
   SectionSetFlags ${SEC02} 0
   Chk03:
   ; the end
   Goto Done
  ; new installation
  New:
   StrCpy $INSTALLED "0"
  Done:
FunctionEnd

Section -AdditionalIcons
  StrCmp $INSTALLED "1" EndAI
  SetOutPath $INSTDIR
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  SetShellVarContext all
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
  EndAI:
SectionEnd

Section -Post
  StrCmp $INSTALLED "1" PostInstalled
   WriteUninstaller "$INSTDIR\uninst.exe"
   WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "Path" "$INSTDIR"
   WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
   WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
   WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
   WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
   WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
   WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\uninst.exe"
   WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "NoModify" "1"
   WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "NoRepair" "1"
  Goto EndPost
  PostInstalled:
   ; delete unselected
   ; Network
   !insertmacro SectionFlagIsSet ${SEC02} ${SF_SELECTED} Chk03 Del02
    Del02:
     IfFileExists "$INSTDIR\QtNetwork4.dll" 0 Chk03
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtNetwork4.dll"
   ; DBus
   Chk03:
  EndPost:
  MessageBox MB_OK "simple message box"
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${SEC01} "Core"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC02} "Network"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
; Uninstaller

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure to delete?" IDYES +2
  Abort
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "Deleting success."
FunctionEnd

Section "Uninstall"
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
  ; dlls deregister
  IfFileExists "$INSTDIR\QtNetwork4.dll" 0 +1
   !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtNetwork4.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtGui4.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtCore4.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libpng14-14.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libstdc++-6.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libgcc_s_sjlj-1.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\zlib11.dll"
  ; Files
  Delete "$INSTDIR\*.*"
  RMDir "$INSTDIR"
  ; Startmenu
  Delete "$SMPROGRAMS\QtDesktop\RTL\*.*"
  RMDir "$SMPROGRAMS\QtDesktop\RTL"
  ; Registry
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
