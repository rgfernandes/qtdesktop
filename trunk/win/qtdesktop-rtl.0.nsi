; qtdesktop-rtl0.nsi
; Trying to install into ProgramFiles\QtDesktop\RTL
; Bugs:
; - Error creating MainMenu...
; - Невозможно найти символ: Dll[Un]RegisterServer
; - deps
; TODO:
; - Install/uninstall log
; - what about Qt3Support4, QtDeclarative, QtScriptTools, QtTest, QtXmlPatterns
; - Deps:
; -- DBus < Xml
; -- WebKit < libsqlite3
; - Add/Remove in Uninstaller

!define PRODUCT_NAME "QtDesktop-RTL"
!define PRODUCT_VERSION "4.7.1"
!define PRODUCT_BUILD "0"
!define PRODUCT_PUBLISHER "QtDesktop"
!define PRODUCT_WEB_SITE "http://www.qtdesktop.org/"
!define PRODUCT_DIR_REGKEY "Software\QtDesktop\RTL"
!define PRODUCT_DIR_ROOTREGKEY "Software\QtDesktop"
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
!define MUI_PAGE_CUSTOMFUNCTION_PRE page_pre
!insertmacro MUI_PAGE_WELCOME
; License page (if new installation)
!define MUI_PAGE_CUSTOMFUNCTION_PRE page_pre
!define MUI_LICENSEPAGE_RADIOBUTTONS
!insertmacro MUI_PAGE_LICENSE "gpl.txt"
; Components page
!insertmacro MUI_PAGE_COMPONENTS
; Directory page (if new installation)
!define MUI_PAGE_CUSTOMFUNCTION_PRE page_pre
!insertmacro MUI_PAGE_DIRECTORY
; Start menu page (if new)
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "QtDesktop\RTL"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!define MUI_PAGE_CUSTOMFUNCTION_PRE page_pre
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
; allways
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
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" "Path"
ShowInstDetails show
ShowUnInstDetails show

;--------------------------------
Function page_pre
  StrCmp $INSTALLED "1" 0 EndPagePre
   Abort
  EndPagePre:
FunctionEnd
;--------------------------------
Section "Core" SEC_Core
  SectionIn RO
  StrCmp $INSTALLED "1" EndSec_Core
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\libgcc_s_sjlj-1.dll" "$INSTDIR\libgcc_s_sjlj-1.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\libstdc++-6.dll" "$INSTDIR\libstdc++-6.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\zlib1.dll" "$INSTDIR\zlib1.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\libpng14-14.dll" "$INSTDIR\libpng14-14.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtCore4.dll" "$INSTDIR\QtCore4.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtGui4.dll" "$INSTDIR\QtGui4.dll" $INSTDIR
   ; options
   ; - core
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\mingwm10.dll" "$INSTDIR\mingwm10.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\pthreadGC2.dll" "$INSTDIR\pthreadGC2.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\pthreadGCE2.dll" "$INSTDIR\pthreadGCE2.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\libgomp-1.dll" "$INSTDIR\libgomp-1.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\libssp-0.dll" "$INSTDIR\libssp-0.dll" $INSTDIR
   ; - gui
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\libjpeg-7.dll" "$INSTDIR\libjpeg-7.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\libtiff-3.dll" "$INSTDIR\libtiff-3.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\libtiffxx-3.dll" "$INSTDIR\libtiffxx-3.dll" $INSTDIR
  EndSec_Core:
SectionEnd

; == Optional sections
Section "DBus" SEC_DBus
  IfFileExists "$INSTDIR\QtDBus4.dll" EndSec_DBus
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\libdbus-1-3.dll" "$INSTDIR\libdbus-1-3.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtDBus4.dll" "$INSTDIR\QtDBus4.dll" $INSTDIR
   File "bin\dbus-daemon.exe"
   File "bin\dbus-launch.exe"
   File "bin\dbus-monitor.exe"
   File "bin\dbus-send.exe"
  EndSec_DBus:
SectionEnd

Section "Multimedia" SEC_MM
  IfFileExists "$INSTDIR\QtMultimedia4.dll" +2
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtMultimedia4.dll" "$INSTDIR\QtMultimedia4.dll" $INSTDIR
SectionEnd

Section "Network" SEC_Network
  IfFileExists "$INSTDIR\QtNetwork4.dll" +2
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtNetwork4.dll" "$INSTDIR\QtNetwork4.dll" $INSTDIR
SectionEnd

Section "OpenGL" SEC_OpenGL
  IfFileExists "$INSTDIR\QtOpenGL4.dll" +2
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtOpenGL4.dll" "$INSTDIR\QtOpenGL4.dll" $INSTDIR
SectionEnd

Section "Script" SEC_Script
  IfFileExists "$INSTDIR\QtScript4.dll" +2
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtScript4.dll" "$INSTDIR\QtScript4.dll" $INSTDIR
SectionEnd

Section "SQL" SEC_Sql
  IfFileExists "$INSTDIR\QtSql4.dll" EndSec_Sql
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\libsqlite3-0.dll" "$INSTDIR\libsqlite3-0.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtSql4.dll" "$INSTDIR\QtSql4.dll" $INSTDIR
  EndSec_Sql:
SectionEnd

Section "SVG" SEC_Svg
  IfFileExists "$INSTDIR\QtSvg4.dll" +2
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtSvg4.dll" "$INSTDIR\QtSvg4.dll" $INSTDIR
SectionEnd

Section "WebKit" SEC_WebKit
  IfFileExists "$INSTDIR\QtWebKit4.dll" +2
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtWebKit4.dll" "$INSTDIR\QtWebKit4.dll" $INSTDIR
SectionEnd

Section "XML" SEC_Xml
  IfFileExists "$INSTDIR\QtXml4.dll" EndSec_Xml
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\libexpat-1.dll" "$INSTDIR\libexpat-1.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtXml4.dll" "$INSTDIR\QtXml4.dll" $INSTDIR
  EndSec_Xml:
SectionEnd

; ==
Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
  ; check installed
  ReadRegStr $1 HKLM "${PRODUCT_DIR_REGKEY}" "Path"
  IfErrors New
   ; installed
   StrCpy $INSTALLED "1"
   StrCpy $INSTDIR $1
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
  New:
   StrCpy $INSTALLED "0"
   SectionSetFlags ${SEC_DBus} 0
   SectionSetFlags ${SEC_MM} 0
   SectionSetFlags ${SEC_Network} 0
   SectionSetFlags ${SEC_OpenGL} 0
   SectionSetFlags ${SEC_Script} 0
   SectionSetFlags ${SEC_Sql} 0
   SectionSetFlags ${SEC_Svg} 0
   SectionSetFlags ${SEC_WebKit} 0
   SectionSetFlags ${SEC_Xml} 0
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
   ; delete unselected options
   ; DBus
   !insertmacro SectionFlagIsSet ${SEC_DBus} ${SF_SELECTED} Chk_MM Del_DBus
    Del_DBus:
     IfFileExists "$INSTDIR\QtDBus4.dll" 0 Chk_MM
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libdbus-1-3.dll"
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtDBus4.dll"
      Delete "$INSTDIR\dbus-*.exe"
   ; Multimedia
   Chk_MM:
   !insertmacro SectionFlagIsSet ${SEC_MM} ${SF_SELECTED} Chk_Network Del_MM
    Del_MM:
     IfFileExists "$INSTDIR\QtMultimedia4.dll" 0 Chk_Network
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtMultimedia4.dll"
   ; Network
   Chk_Network:
   !insertmacro SectionFlagIsSet ${SEC_Network} ${SF_SELECTED} Chk_OpenGL Del_Network
    Del_Network:
     IfFileExists "$INSTDIR\QtNetwork4.dll" 0 Chk_OpenGL
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtNetwork4.dll"
   ; OpenGL
   Chk_OpenGL:
   !insertmacro SectionFlagIsSet ${SEC_OpenGL} ${SF_SELECTED} Chk_Script Del_OpenGL
    Del_OpenGL:
     IfFileExists "$INSTDIR\QtOpenGL4.dll" 0 Chk_Script
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtOpenGL4.dll"
   ; Script
   Chk_Script:
   !insertmacro SectionFlagIsSet ${SEC_Script} ${SF_SELECTED} Chk_Sql Del_Script
    Del_Script:
     IfFileExists "$INSTDIR\QtScript4.dll" 0 Chk_Sql
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtScript4.dll"
   ; SQL
   Chk_Sql:
   !insertmacro SectionFlagIsSet ${SEC_Sql} ${SF_SELECTED} Chk_Svg Del_Sql
    Del_Sql:
     IfFileExists "$INSTDIR\QtSql4.dll" 0 Chk_Svg
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libsqlite3-0.dll"
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtSql4.dll"
   ; SVG
   Chk_Svg:
   !insertmacro SectionFlagIsSet ${SEC_Svg} ${SF_SELECTED} Chk_WebKit Del_Svg
    Del_Svg:
     IfFileExists "$INSTDIR\QtSvg4.dll" 0 Chk_WebKit
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtSvg4.dll"
   ; WebKit
   Chk_WebKit:
   !insertmacro SectionFlagIsSet ${SEC_WebKit} ${SF_SELECTED} Chk_Xml Del_WebKit
    Del_WebKit:
     IfFileExists "$INSTDIR\QtWebKit4.dll" 0 Chk_Xml
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtWebKit4.dll"
   ; XML
   Chk_Xml:
   !insertmacro SectionFlagIsSet ${SEC_Xml} ${SF_SELECTED} EndPost Del_Xml
    Del_Xml:
     IfFileExists "$INSTDIR\QtXml4.dll" 0 EndPost
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtXml4.dll"
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libexpat-1.dll"
   ; End delete options
  EndPost:
  MessageBox MB_OK "Check result"
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${SEC_Core}		"Core"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC_DBus}		"DBus"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC_MM}		"Multimedia"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC_Network}	"Network"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC_OpenGL}		"OpenGL"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC_Script}		"Script"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC_Sql}		"SQL"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC_Svg}		"SVG"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC_WebKit}		"WebKit"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC_Xml}		"XML"
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
  ; DBus
  IfFileExists "$INSTDIR\QtDBus4.dll" 0 Chk_MM
   !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtDBus4.dll"
   !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libdbus-1-3.dll"
  ; Multimedia
  Chk_MM:
  IfFileExists "$INSTDIR\QtMultimedia4.dll" 0 Chk_Network
   !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtMultimedia4.dll"
  ; Network
  Chk_Network:
  IfFileExists "$INSTDIR\QtNetwork4.dll" 0 Chk_OpenGL
   !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtNetwork4.dll"
  ; OpenGL
  Chk_OpenGL:
  IfFileExists "$INSTDIR\QtOpenGL4.dll" 0 Chk_Script
   !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtOpenGL4.dll"
  ; Script
  Chk_Script:
  IfFileExists "$INSTDIR\QtScript4.dll" 0 Chk_Sql
   !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtScript4.dll"
  ; SQL
  Chk_Sql:
  IfFileExists "$INSTDIR\QtSql4.dll" 0 Chk_Svg
   !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtSql4.dll"
   !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libsqlite3-0.dll"
  ; SVG
  Chk_Svg:
  IfFileExists "$INSTDIR\QtSvg4.dll" 0 Chk_WebKit
   !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtSvg4.dll"
  ; WebKit
  Chk_WebKit:
  IfFileExists "$INSTDIR\QtWebKit4.dll" 0 Chk_Xml
   !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtWebKit4.dll"
  ; XML
  Chk_Xml:
  IfFileExists "$INSTDIR\QtXml4.dll" 0 Del_Core
   !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtXml4.dll"
   !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libexpat-1.dll"
  ; Core
  Del_Core:
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtGui4.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtCore4.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libstdc++-6.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libgcc_s_sjlj-1.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libpng14-14.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libjpeg-7.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libtiff-3.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libtiffxx-3.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\zlib1.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libssp-0.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\pthreadGC2.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libgomp-1.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\pthreadGCE2.dll"
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\mingwm10.dll"
  ; Files
  Delete "$INSTDIR\*.*"
  RMDir "$INSTDIR"
  RMDir "$INSTDIR\.."
  ; Startmenu
  Delete "$SMPROGRAMS\QtDesktop\RTL\*.*"
  RMDir "$SMPROGRAMS\QtDesktop\RTL"
  RMDir "$SMPROGRAMS\QtDesktop"
  ; Registry
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_ROOTREGKEY}"
  ;MessageBox MB_OK "Check result"
  SetAutoClose false
SectionEnd
