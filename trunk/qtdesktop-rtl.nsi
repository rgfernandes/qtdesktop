; qtdesktop-rtl.nsi
;
; Installer of qt4 runtime
; reg key: HKLM\Trolltech\Common\4.7.3\OpenSource\Key:RegSz
; TODO:
; - skip license/directory if installed
; - delete unchecked items if installed

!define PRODUCT_NAME "QtDesktop-RTL"
!define PRODUCT_VERSION "4.7.1"
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
Section "Core" SEC00
  SectionIn RO
  StrCmp $INSTALLED "1" EndSec00
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\libgcc_s_sjlj-1.dll" "$INSTDIR\libgcc_s_sjlj-1.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\libstdc++-6.dll" "$INSTDIR\libstdc++-6.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\zlib1.dll" "$INSTDIR\zlib1.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\libpng15-15.dll" "$INSTDIR\libpng15-15.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtCore4.dll" "$INSTDIR\QtCore4.dll" $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtGui4.dll" "$INSTDIR\QtGui4.dll" $INSTDIR
  EndSec00:
SectionEnd

; == Optional sections
Section "Network" SEC01
  IfFileExists "$INSTDIR\QtDBus4.dll" +2
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtDBus4.dll" "$INSTDIR\QtDBus4.dll" $INSTDIR
SectionEnd

Section "Network" SEC02
  IfFileExists "$INSTDIR\QtHelp4.dll" +2
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtHelp4.dll" "$INSTDIR\QtHelp4.dll" $INSTDIR
SectionEnd

Section "Network" SEC03
  IfFileExists "$INSTDIR\QtMultimedia4.dll" +2
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtMultimedia4.dll" "$INSTDIR\QtMultimedia4.dll" $INSTDIR
SectionEnd

Section "Network" SEC04
  IfFileExists "$INSTDIR\QtNetwork4.dll" +2
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtNetwork4.dll" "$INSTDIR\QtNetwork4.dll" $INSTDIR
SectionEnd

Section "Network" SEC05
  IfFileExists "$INSTDIR\QtOpenGL4.dll" +2
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtOpenGL4.dll" "$INSTDIR\QtOpenGL4.dll" $INSTDIR
SectionEnd

Section "Network" SEC06
  IfFileExists "$INSTDIR\QtScript4.dll" +2
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtScript4.dll" "$INSTDIR\QtScript4.dll" $INSTDIR
SectionEnd

Section "Network" SEC07
  IfFileExists "$INSTDIR\QtSql4.dll" +2
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtSql4.dll" "$INSTDIR\QtSql4.dll" $INSTDIR
SectionEnd

Section "Network" SEC08
  IfFileExists "$INSTDIR\QtSvg4.dll" +2
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtSvg4.dll" "$INSTDIR\QtSvg4.dll" $INSTDIR
SectionEnd

Section "Network" SEC09
  IfFileExists "$INSTDIR\QtWebKit4.dll" +2
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtWebKit4.dll" "$INSTDIR\QtWebKit4.dll" $INSTDIR
SectionEnd

Section "Network" SEC10
  IfFileExists "$INSTDIR\QtXml4.dll" +2
   SetOutPath $INSTDIR
   !insertmacro InstallLib REGDLL $0 NOREBOOT_NOTPROTECTED "bin\QtXml4.dll" "$INSTDIR\QtXml4.dll" $INSTDIR
SectionEnd

; ==
Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
  ; check installed
  ReadRegStr $0 HKLM "${PRODUCT_DIR_REGKEY}" "Path"
  IfErrors New
   ; installed
   StrCpy $INSTALLED "1"
   StrCpy $INSTDIR $0
   ; Check options
   ; DBus
   IfFileExists "$INSTDIR\QtDBus4.dll" 0 +3
    SectionSetFlags ${SEC01} ${SF_SELECTED}
    Goto Chk02
   SectionSetFlags ${SEC01} 0
   Chk02:
   ; Help
   IfFileExists "$INSTDIR\QtHelp4.dll" 0 +3
    SectionSetFlags ${SEC02} ${SF_SELECTED}
    Goto Chk03
   SectionSetFlags ${SEC02} 0
   Chk03:
   ; Multimedia
   IfFileExists "$INSTDIR\QtMultimedia4.dll" 0 +3
    SectionSetFlags ${SEC03} ${SF_SELECTED}
    Goto Chk04
   SectionSetFlags ${SEC03} 0
   Chk04:
   ; Network
   IfFileExists "$INSTDIR\QtNetwork4.dll" 0 +3
    SectionSetFlags ${SEC04} ${SF_SELECTED}
    Goto Chk05
   SectionSetFlags ${SEC04} 0
   Chk05:
   ; OpenGL
   IfFileExists "$INSTDIR\QtOpenGL4.dll" 0 +3
    SectionSetFlags ${SEC05} ${SF_SELECTED}
    Goto Chk06
   SectionSetFlags ${SEC05} 0
   Chk06:
   ; Script
   IfFileExists "$INSTDIR\QtScript4.dll" 0 +3
    SectionSetFlags ${SEC06} ${SF_SELECTED}
    Goto Chk07
   SectionSetFlags ${SEC06} 0
   Chk07:
   ; SQL
   IfFileExists "$INSTDIR\QtSql4.dll" 0 +3
    SectionSetFlags ${SEC07} ${SF_SELECTED}
    Goto Chk08
   SectionSetFlags ${SEC07} 0
   Chk08:
   ; SVG
   IfFileExists "$INSTDIR\QtSvg4.dll" 0 +3
    SectionSetFlags ${SEC08} ${SF_SELECTED}
    Goto Chk09
   SectionSetFlags ${SEC08} 0
   Chk09:
   ; WebKit
   IfFileExists "$INSTDIR\QtWebKit4.dll" 0 +3
    SectionSetFlags ${SEC09} ${SF_SELECTED}
    Goto Chk10
   SectionSetFlags ${SEC09} 0
   Chk10:
   ; XML
   IfFileExists "$INSTDIR\QtXml4.dll" 0 +3
    SectionSetFlags ${SEC10} ${SF_SELECTED}
    Goto Done
   SectionSetFlags ${SEC10} 0
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
   ; delete unselected options
   ; DBus
   !insertmacro SectionFlagIsSet ${SEC01} ${SF_SELECTED} Chk02 Del01
    Del01:
     IfFileExists "$INSTDIR\QtDBus4.dll" 0 Chk02
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtDBus4.dll"
   ; Help
   Chk02:
   !insertmacro SectionFlagIsSet ${SEC02} ${SF_SELECTED} Chk03 Del02
    Del02:
     IfFileExists "$INSTDIR\QtHelp4.dll" 0 Chk03
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtHelp4.dll"
   ; Multimedia
   Chk03:
   !insertmacro SectionFlagIsSet ${SEC02} ${SF_SELECTED} Chk03 Del02
    Del02:
     IfFileExists "$INSTDIR\QtNetwork4.dll" 0 Chk03
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtNetwork4.dll"
   ; DBus
   Chk03:
   ; Network
   !insertmacro SectionFlagIsSet ${SEC02} ${SF_SELECTED} Chk03 Del02
    Del02:
     IfFileExists "$INSTDIR\QtNetwork4.dll" 0 Chk03
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtNetwork4.dll"
   ; DBus
   Chk03:
   ; Network
   !insertmacro SectionFlagIsSet ${SEC02} ${SF_SELECTED} Chk03 Del02
    Del02:
     IfFileExists "$INSTDIR\QtNetwork4.dll" 0 Chk03
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtNetwork4.dll"
   ; DBus
   Chk03:
   ; Network
   !insertmacro SectionFlagIsSet ${SEC02} ${SF_SELECTED} Chk03 Del02
    Del02:
     IfFileExists "$INSTDIR\QtNetwork4.dll" 0 Chk03
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtNetwork4.dll"
   ; DBus
   Chk03:
   ; Network
   !insertmacro SectionFlagIsSet ${SEC02} ${SF_SELECTED} Chk03 Del02
    Del02:
     IfFileExists "$INSTDIR\QtNetwork4.dll" 0 Chk03
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtNetwork4.dll"
   ; DBus
   Chk03:
   ; Network
   !insertmacro SectionFlagIsSet ${SEC02} ${SF_SELECTED} Chk03 Del02
    Del02:
     IfFileExists "$INSTDIR\QtNetwork4.dll" 0 Chk03
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtNetwork4.dll"
   ; DBus
   Chk03:
   ; Network
   !insertmacro SectionFlagIsSet ${SEC02} ${SF_SELECTED} Chk03 Del02
    Del02:
     IfFileExists "$INSTDIR\QtNetwork4.dll" 0 Chk03
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtNetwork4.dll"
   ; DBus
   Chk03:
   ; Network
   !insertmacro SectionFlagIsSet ${SEC02} ${SF_SELECTED} Chk03 Del02
    Del02:
     IfFileExists "$INSTDIR\QtNetwork4.dll" 0 Chk03
      !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\QtNetwork4.dll"
   ; DBus
   Chk03:
   ; End delete options
  EndPost:
  MessageBox MB_OK "simple message box"
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${SEC00} "Core"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC01} "DBus"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC02} "Help"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC03} "Multimedia"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC04} "Network"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC05} "OpenGL"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC06} "Script"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC07} "SQL"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC08} "SVG"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC09} "WebKit"
!insertmacro MUI_DESCRIPTION_TEXT ${SEC10} "XML"
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
  !insertmacro UninstallLib REGDLL SHARED NOREBOOT_NOTPROTECTED "$INSTDIR\libpng15-15.dll"
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
