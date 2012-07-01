; qtdesktop-rtl.nsi
; Trying to install into ProgramFiles\QtDesktop\RTL
; Sources:  ( )
; BuildRequires: mingw32-nsis mingw32-qt
; Build: ln -s /usr/i686-pc-mingw32/sys-root/mingw/bin bin && makensis qtdesktop-rtl.nsi
; Bugs:
; - Error creating MainMenu...
; - sqlite - as Sql as WebKit
; TODO:
; - macro for -Post unchecked deletion
; - Qt3Support4, QtDeclarative, QtScriptTools, QtTest, QtXmlPatterns
; - Deps (.onSelChange)
; - Install/uninstall log
; - Add/Remove in Uninstaller
; Done:
; - Невозможно найти символ: Dll[Un]RegisterServer - => system32
; - Add/Remove
; - macros

!define PRODUCT_NAME "QtDesktop-RTL"
!define PRODUCT_VERSION "4.8.0"
!define PRODUCT_BUILD "0"
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
; Helper macros
!macro TryInstLib libname
 IfFileExists "$SYSDIR\${libname}" EndTryInstLib_${libname}
  !insertmacro InstallLib DLL $0 NOREBOOT_NOTPROTECTED "bin\${libname}" "$SYSDIR\${libname}" $INSTDIR
 EndTryInstLib_${libname}:
!macroend

!macro TryUnInstLib libname
 IfFileExists "$SYSDIR\${libname}" 0 EndTryUnInstLib_${libname}
  !insertmacro UninstallLib DLL SHARED NOREBOOT_NOTPROTECTED "$SYSDIR\${libname}"
 EndTryUnInstLib_${libname}:
!macroend

!macro CheckInstalled libname SECTION
  ; check installed dll section
  IfFileExists "$SYSDIR\${libname}" 0 UnCheck_${SECTION}
   SectionSetFlags ${SECTION} ${SF_SELECTED}
   Goto EndCheck_${SECTION}
  UnCheck_${SECTION}:
   SectionSetFlags ${SECTION} 0
  EndCheck_${SECTION}:
!macroend

;--------------------------------
Section "Core" SEC_Core
  SectionIn RO
  StrCmp $INSTALLED "1" EndSec_Core
   ;!insertmacro InstallLib DLL $0 NOREBOOT_NOTPROTECTED "bin\libgcc_s_sjlj-1.dll" "$SYSDIR\libgcc_s_sjlj-1.dll" $INSTDIR
   !insertmacro TryInstLib libgcc_s_sjlj-1.dll
   !insertmacro TryInstLib libstdc++-6.dll
   !insertmacro TryInstLib zlib1.dll
   !insertmacro TryInstLib QtCore4.dll
   !insertmacro TryInstLib libpng14-14.dll
   !insertmacro TryInstLib QtGui4.dll
   ; options
   ; - core
   !insertmacro TryInstLib mingwm10.dll
   !insertmacro TryInstLib pthreadGC2.dll
   !insertmacro TryInstLib libgomp-1.dll
   !insertmacro TryInstLib pthreadGCE2.dll
   !insertmacro TryInstLib libssp-0.dll
   ; - gui
   !insertmacro TryInstLib libjpeg-62.dll
   !insertmacro TryInstLib libtiff-3.dll
   !insertmacro TryInstLib libtiffxx-3.dll
  EndSec_Core:
SectionEnd

; == Optional sections
Section "DBus" SEC_DBus
  !insertmacro TryInstLib libdbus-1-3.dll
  !insertmacro TryInstLib QtDBus4.dll
  !insertmacro TryInstLib libexpat-1.dll
  SetOutPath $SYSDIR
  File "bin\dbus-daemon.exe"
  File "bin\dbus-launch.exe"
  File "bin\dbus-monitor.exe"
  File "bin\dbus-send.exe"
SectionEnd

Section "Multimedia" SEC_MM
  !insertmacro TryInstLib QtMultimedia4.dll
SectionEnd

Section "Network" SEC_Network
  !insertmacro TryInstLib QtNetwork4.dll
SectionEnd

Section "OpenGL" SEC_OpenGL
  !insertmacro TryInstLib QtOpenGL4.dll
SectionEnd

Section "Script" SEC_Script
  !insertmacro TryInstLib QtScript4.dll
SectionEnd

Section "SQL" SEC_Sql
  !insertmacro TryInstLib QtSql4.dll
SectionEnd

Section "SVG" SEC_Svg
  !insertmacro TryInstLib QtSvg4.dll
SectionEnd

Section "WebKit" SEC_WebKit
  !insertmacro TryInstLib libsqlite3-0.dll
  !insertmacro TryInstLib QtWebKit4.dll
SectionEnd

Section "XML" SEC_Xml
  !insertmacro TryInstLib QtXml4.dll
SectionEnd

; ==
Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
  ; check installed
  ReadRegStr $1 ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" 
  StrCmp $1 "" New 0
   ; installed
   StrCpy $INSTALLED "1"
   ; Check options
   !insertmacro CheckInstalled QtDBus4.dll ${SEC_DBus}
   !insertmacro CheckInstalled QtMultimedia4.dll ${SEC_MM}
   !insertmacro CheckInstalled QtNetwork4.dll ${SEC_Network}
   !insertmacro CheckInstalled QtOpenGL4.dll ${SEC_OpenGL}
   !insertmacro CheckInstalled QtScript4.dll ${SEC_Script}
   !insertmacro CheckInstalled QtSql4.dll ${SEC_Sql}
   !insertmacro CheckInstalled QtSvg4.dll ${SEC_Svg}
   !insertmacro CheckInstalled QtWebKit4.dll ${SEC_WebKit}
   !insertmacro CheckInstalled QtXml4.dll ${SEC_Xml}
   Goto Done
  ; new installation
  New:
   StrCpy $INSTALLED "0"
   ;SectionSetFlags ${SEC_Core} ${SF_SELECTED}
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
  ;CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
  EndAI:
SectionEnd

Section -Post
  StrCmp $INSTALLED "1" PostInstalled
   WriteUninstaller "$INSTDIR\uninst.exe"
   WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "Path" "$INSTDIR"
   WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "Version" "${PRODUCT_VERSION}"
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
      Delete "$SYSDIR\dbus-*.exe"
      !insertmacro TryUnInstLib QtDBus4.dll
      !insertmacro TryUnInstLib libdbus-1-3.dll
      !insertmacro TryUnInstLib libexpat-1.dll
   ; Multimedia
   Chk_MM:
   !insertmacro SectionFlagIsSet ${SEC_MM} ${SF_SELECTED} Chk_Network Del_MM
    Del_MM:
     !insertmacro TryUnInstLib QtMultimedia4.dll
   ; Network
   Chk_Network:
   !insertmacro SectionFlagIsSet ${SEC_Network} ${SF_SELECTED} Chk_OpenGL Del_Network
    Del_Network:
      !insertmacro TryUnInstLib QtNetwork4.dll
   ; OpenGL
   Chk_OpenGL:
   !insertmacro SectionFlagIsSet ${SEC_OpenGL} ${SF_SELECTED} Chk_Script Del_OpenGL
    Del_OpenGL:
      !insertmacro TryUnInstLib QtOpenGL4.dll
   ; Script
   Chk_Script:
   !insertmacro SectionFlagIsSet ${SEC_Script} ${SF_SELECTED} Chk_Sql Del_Script
    Del_Script:
      !insertmacro TryUnInstLib QtScript4.dll
   ; SQL
   Chk_Sql:
   !insertmacro SectionFlagIsSet ${SEC_Sql} ${SF_SELECTED} Chk_Svg Del_Sql
    Del_Sql:
      !insertmacro TryUnInstLib QtSql4.dll
   ; SVG
   Chk_Svg:
   !insertmacro SectionFlagIsSet ${SEC_Svg} ${SF_SELECTED} Chk_WebKit Del_Svg
    Del_Svg:
      !insertmacro TryUnInstLib QtSvg4.dll
   ; WebKit
   Chk_WebKit:
   !insertmacro SectionFlagIsSet ${SEC_WebKit} ${SF_SELECTED} Chk_Xml Del_WebKit
    Del_WebKit:
      !insertmacro TryUnInstLib QtWebKit4.dll
      !insertmacro TryUnInstLib libsqlite3-0.dll
   ; XML
   Chk_Xml:
   !insertmacro SectionFlagIsSet ${SEC_Xml} ${SF_SELECTED} EndPost Del_Xml
    Del_Xml:
      !insertmacro TryUnInstLib QtXml4.dll
   ; End delete options
  EndPost:
  ;MessageBox MB_OK "Check result"
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

Section "Uninstall"
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
  ; dlls deregister
  ; DBus
  Delete "$SYSDIR\dbus-*.exe"
  !insertmacro TryUnInstLib QtDBus4.dll
  !insertmacro TryUnInstLib libdbus-1-3.dll
  !insertmacro TryUnInstLib libexpat-1.dll
  ; Multimedia
  !insertmacro TryUnInstLib QtMultimedia4.dll
  ; Network
  !insertmacro TryUnInstLib QtNetwork4.dll
  ; OpenGL
  !insertmacro TryUnInstLib QtOpenGL4.dll
  ; Script
  !insertmacro TryUnInstLib QtScript4.dll
  ; SQL
  !insertmacro TryUnInstLib QtSql4.dll
  ; SVG
   !insertmacro TryUnInstLib QtSvg4.dll
  ; WebKit
  !insertmacro TryUnInstLib QtWebKit4.dll
  !insertmacro TryUnInstLib libsqlite3-0.dll
  ; XML
  !insertmacro TryUnInstLib QtXml4.dll
  ; Core
  ; options
  !insertmacro TryUnInstLib libtiffxx-3.dll
  !insertmacro TryUnInstLib libtiff-3.dll
  !insertmacro TryUnInstLib libjpeg-7.dll
  !insertmacro TryUnInstLib libssp-0.dll
  !insertmacro TryUnInstLib pthreadGC2.dll
  !insertmacro TryUnInstLib libgomp-1.dll
  !insertmacro TryUnInstLib pthreadGCE2.dll
  !insertmacro TryUnInstLib mingwm10.dll
  ; mandatory
  !insertmacro TryUnInstLib QtGui4.dll
  !insertmacro TryUnInstLib libpng14-14.dll
  !insertmacro TryUnInstLib QtCore4.dll
  !insertmacro TryUnInstLib libstdc++-6.dll
  !insertmacro TryUnInstLib libgcc_s_sjlj-1.dll
  !insertmacro TryUnInstLib zlib1.dll
  ; Files
  Delete "$INSTDIR\*.*"
  RMDir "$INSTDIR"
  ; Startmenu
  Delete "$SMPROGRAMS\QtDesktop\RTL\*.*"
  RMDir "$SMPROGRAMS\QtDesktop\RTL"
  ; Registry
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose false
SectionEnd
