; qtdesktop-qrdc.nsi
; TODO:
; - Deps: QtGui, QtSvg, QtSql

!define PRODUCT_NAME "QtDesktop-QRDC"
!define PRODUCT_VERSION "0.0.1"
!define PRODUCT_BUILD "0"
!define PRODUCT_PUBLISHER "QtDesktop"
!define PRODUCT_WEB_SITE "http://www.qtdesktop.org/"
!define PRODUCT_DIR_REGKEY "Software\QtDesktop\QRDC"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

SetCompressor /SOLID lzma
BrandingText "www.qtdesktop.org"
XPStyle on
RequestExecutionLevel admin

!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!define MUI_LICENSEPAGE_RADIOBUTTONS
!insertmacro MUI_PAGE_LICENSE "gpl.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Start menu page
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "QtDesktop\QRDC"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
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
OutFile "${PRODUCT_NAME}-${PRODUCT_VERSION}-${PRODUCT_BUILD}.exe"
InstallDir "$PROGRAMFILES\QtDesktop\RTL"
ShowInstDetails show
ShowUnInstDetails show

;--------------------------------
Section ""
  SetOutPath $INSTDIR
  File "bin\qrdc.exe"
SectionEnd

; ==
Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
  ; check installed
  ReadRegStr $0 HKLM "${PRODUCT_DIR_REGKEY}" "Path"
  IfErrors Done
   ; installed
   StrCpy $INSTDIR $0
   ; Check options
   ; DBus
   IfFileExists "$INSTDIR\QtDBus4.dll" 0 +3
    SectionSetFlags ${SEC_DBus} ${SF_SELECTED}
    Goto Chk_MM
   SectionSetFlags ${SEC_DBus} 0
   ; Multimedia
   Chk_MM:
   IfFileExists "$INSTDIR\QtMultimedia4.dll" 0 +3
    SectionSetFlags ${SEC_MM} ${SF_SELECTED}
    Goto Chk_Network
   SectionSetFlags ${SEC_MM} 0
   ; Network
   Chk_Network:
   IfFileExists "$INSTDIR\QtNetwork4.dll" 0 +3
    SectionSetFlags ${SEC_Network} ${SF_SELECTED}
    Goto Chk_OpenGL
   SectionSetFlags ${SEC_Network} 0
   ; OpenGL
   Chk_OpenGL:
   IfFileExists "$INSTDIR\QtOpenGL4.dll" 0 +3
    SectionSetFlags ${SEC_OpenGL} ${SF_SELECTED}
    Goto Chk_Script
   SectionSetFlags ${SEC_OpenGL} 0
   ; Script
   Chk_Script:
   IfFileExists "$INSTDIR\QtScript4.dll" 0 +3
    SectionSetFlags ${SEC_Script} ${SF_SELECTED}
    Goto Chk_Sql
   SectionSetFlags ${SEC_Script} 0
   ; SQL
   Chk_Sql:
   IfFileExists "$INSTDIR\QtSql4.dll" 0 +3
    SectionSetFlags ${SEC_Sql} ${SF_SELECTED}
    Goto Chk_Svg
   SectionSetFlags ${SEC_Sql} 0
   ; SVG
   Chk_Svg:
   IfFileExists "$INSTDIR\QtSvg4.dll" 0 +3
    SectionSetFlags ${SEC_Svg} ${SF_SELECTED}
    Goto Chk_WebKit
   SectionSetFlags ${SEC_Svg} 0
   ; WebKit
   Chk_WebKit:
   IfFileExists "$INSTDIR\QtWebKit4.dll" 0 +3
    SectionSetFlags ${SEC_WebKit} ${SF_SELECTED}
    Goto Chk_Xml
   SectionSetFlags ${SEC_WebKit} 0
   ; XML
   Chk_Xml:
   IfFileExists "$INSTDIR\QtXml4.dll" 0 +3
    SectionSetFlags ${SEC_Xml} ${SF_SELECTED}
    Goto Done
   SectionSetFlags ${SEC_Xml} 0
  ; new installation
  Done:
FunctionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  SetShellVarContext all
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

;--------------------------------
; Uninstaller

Section "Uninstall"
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
  ; Files
  Delete "$INSTDIR\*.*"
  RMDir "$INSTDIR"
  ; Startmenu
  Delete "$SMPROGRAMS\QtDesktop\RTL\qrdc.lnk"
  ; Registry
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  MessageBox MB_OK "Check result"
;  SetAutoClose true
SectionEnd
