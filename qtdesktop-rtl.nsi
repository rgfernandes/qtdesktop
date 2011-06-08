; qtdesktop-rtl.nsi
;
; Installer of qt4 runtime
; reg key: HKLM\Trolltech\Common\4.7.3\OpenSource\Key:RegSz

!define PRODUCT_NAME "QtDesktop-RTL"
!define PRODUCT_VERSION "4.7.1"
!define PRODUCT_PUBLISHER "QtDesktop"
!define PRODUCT_WEB_SITE "http://www.qtdesktop.org/"
!define PRODUCT_DIR_REGKEY "Software\QtDesktop\RTL"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

!define INSTALLED "0"

SetCompressor /SOLID lzma
BrandingText "www.qtdesktop.org"
XPStyle on
RequestExecutionLevel admin

!include "MUI.nsh"
!include "Library.nsh"

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
  SetOverwrite ifnewer
  SetOutPath $INSTDIR
  File "bin\QtCore4.dll"
  !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtCore4.dll" "$INSTDIR\QtCore4.dll" $INSTDIR
  File "bin\QtGui4.dll"
  !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtGui4.dll" "$INSTDIR\QtGui.dll" $INSTDIR
SectionEnd

Section "Network" SEC02
  SetOverwrite ifnewer
  SetOutPath $INSTDIR
  File "bin\QtNetwork4.dll"
;  IfFileExists "$INSTDIR\QtNetwork4.dll" +2
;   StrCpy $0 1
;  !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtNetwork4.dll" "$INSTDIR\QtNetwork4.dll" $INSTDIR
SectionEnd

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
  ; check installed
  ReadRegStr $0 HKLM "${PRODUCT_DIR_REGKEY}" "Path"
  IfErrors New
  ; installed
  ;StrCpy $INSTALLED "1"
  StrCpy $INSTDIR $0
  ; Network
  IfFileExists "$INSTDIR\QtNetwork4.dll" 0 +3
   SectionSetFlags ${SEC02} ${SF_SELECTED}
   Goto Chk03
  SectionSetFlags ${SEC02} $0
  Chk03:
  ; the end
  Goto Done
  ; new installation
  New:
  Done:
FunctionEnd

;Function .onSelChange
;  # keep section 'test' selected
;  SectionGetFlags ${test_section_id} $0
;  IntOp $0 $0 | ${SF_SELECTED}
;  SectionSetFlags ${test_section_id} $0
;FunctionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  SetShellVarContext all
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -Post
  ; check exists
  ; delete unselected
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "Path" "$INSTDIR"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\qutim.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "NoModify" "1"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "NoRepair" "1"
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${SEC01} "Core / Ядро"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC02} "Networ / Сеть"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------

; Uninstaller

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "Deleting success."
FunctionEnd

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure to delete?" IDYES +2
  Abort
FunctionEnd

Section "Uninstall"
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
  ; Files
  Delete "$INSTDIR\*.*"
  RMDir "$INSTDIR"
  ; dlls deregister
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtCore4.dll"
  ; Startmenu
  Delete "$SMPROGRAMS\QtDesktop\RTL\*.*"
  RMDir "$SMPROGRAMS\QtDesktop\RTL"
  ; Registry
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
