# A. Registered Categories #

This section contains a number of well known categories and suggestions on how to use them. The list of Main Categories consist of those categories that every conforming desktop environment MUST support. By including one of these categories in an application's desktop entry file the application will be ensured that it will show up in a section of the application menu dedicated to this category. The list of Additional Categories provides categories that can be used to provide more fine grained information about the application. Additional Categories should always be used in combination with one of the Main Categories.

The table below lists all Main Categories. Note that category names are case-sensitive.

| **Main Category** | **Description** | **Notes** |
|:------------------|:----------------|:----------|
| AudioVideo        | A multimedia (audio/video) application |           |
| Audio             | An audio application | Desktop entry must include AudioVideo as well |
| Video             | A video application | Desktop entry must include AudioVideo as well |
| Development       | An application for development |           |
| Education         | Educational software |           |
| Game              | A game          |           |
| Graphics          | Graphical application |           |
| Network           | Network application such as a web browser |           |
| Office            | An office type application |           |
| Settings          | Settings applications | Entries may appear in a separate menu or as part of a "Control Center" |
| System            | System application, "System Tools" such as say a log viewer or network monitor |           |
| Utility           | Small utility application, "Accessories" |           |

The table below describes Additional Categories. The Related Categories column lists one or more categories that are suggested to be used in conjunction with the Additional Category. Note that at least one Main Category must be included in the desktop entry's list of categories. If multiple Main Categories are included in a single desktop entry file, the entry may appear more than once in the menu. If the Related Categories column is blank, the Additional Category can be used with any Main Category.

| **Category** | **Description** | **Related Categories** |
|:-------------|:----------------|:-----------------------|
| Building     | A tool to build applications | Development            |
| Debugger     | A tool to debug applications | Development            |
| IDE          | IDE application | Development            |
| GUIDesigner  | A GUI designer application | Development            |
| Profiling    | A profiling tool | Development            |
| RevisionControl | Applications like cvs or subversion | Development            |
| Translation  | A translation tool | Development            |
| Calendar     | Calendar application | Office                 |
| ContactManagement | E.g. an address book | Office                 |
| Database     | Application to manage a database | Office or Development or AudioVideo |
| Dictionary   | A dictionary    | Office;TextTools       |
| Chart        | Chart application | Office                 |
| Email        | Email application | Office;Network         |
| Finance      | Application to manage your finance | Office                 |
| FlowChart    | A flowchart application | Office                 |
| PDA          | Tool to manage your PDA | Office                 |
| ProjectManagement | Project management application | Office;Development     |
| Presentation | Presentation software | Office                 |
| Spreadsheet  | A spreadsheet   | Office                 |
| WordProcessor | A word processor | Office                 |
| 2DGraphics   | 2D based graphical application | Graphics               |
| VectorGraphics | Vector based graphical application | Graphics;2DGraphics    |
| RasterGraphics | Raster based graphical application | Graphics;2DGraphics    |
| 3DGraphics   | 3D based graphical application | Graphics               |
| Scanning     | Tool to scan a file/text | Graphics               |
| OCR          | Optical character recognition application | Graphics;Scanning      |
| Photography  | Camera tools, etc. | Graphics or Office     |
| Publishing   | Desktop Publishing applications and Color Management tools | Graphics or Office     |
| Viewer       | Tool to view e.g. a graphic or pdf file | Graphics or Office     |
| TextTools    | A text tool utiliy | Utility                |
| DesktopSettings | Configuration tool for the GUI | Settings               |
| HardwareSettings | A tool to manage hardware components, like sound cards, video cards or printers | Settings               |
| Printing     | A tool to manage printers | HardwareSettings;Settings |
| PackageManager | A package manager application | Settings               |
| Dialup       | A dial-up program | Network                |
| InstantMessaging | An instant messaging client | Network                |
| Chat         | A chat client   | Network                |
| IRCClient    | An IRC client   | Network                |
| FileTransfer | Tools like FTP or P2P programs | Network                |
| HamRadio     | HAM radio software | Network or Audio       |
| News         | A news reader or a news ticker | Network                |
| P2P          | A P2P program   | Network                |
| RemoteAccess | A tool to remotely manage your PC | Network                |
| Telephony    | Telephony via PC | Network                |
| TelephonyTools | Telephony tools, to dial a number, manage PBX, ... | Utility                |
| VideoConference | Video Conference software | Network                |
| WebBrowser   | A web browser   | Network                |
| WebDevelopment | A tool for web developers | Network or Development |
| Midi         | An app related to MIDI | AudioVideo;Audio       |
| Mixer        | Just a mixer    | AudioVideo;Audio       |
| Sequencer    | A sequencer     | AudioVideo;Audio       |
| Tuner        | A tuner         | AudioVideo;Audio       |
| TV           | A TV application | AudioVideo;Video       |
| AudioVideoEditing | Application to edit audio/video files | Audio or Video or AudioVideo |
| Player       | Application to play audio/video files | Audio or Video or AudioVideo |
| Recorder     | Application to record audio/video files | Audio or Video or AudioVideo |
| DiscBurning  | Application to burn a disc | AudioVideo             |
| ActionGame   | An action game  | Game                   |
| AdventureGame | Adventure style game | Game                   |
| ArcadeGame   | Arcade style game | Game                   |
| BoardGame    | A board game    | Game                   |
| BlocksGame   | Falling blocks game | Game                   |
| CardGame     | A card game     | Game                   |
| KidsGame     | A game for kids | Game                   |
| LogicGame    | Logic games like puzzles, etc | Game                   |
| RolePlaying  | A role playing game | Game                   |
| Simulation   | A simulation game | Game                   |
| SportsGame   | A sports game   | Game                   |
| StrategyGame | A strategy game | Game                   |
| Art          | Software to teach arts | Education              |
| Construction |                 | Education              |
| Music        | Musical software | AudioVideo;Education   |
| Languages    | Software to learn foreign languages | Education              |
| Science      | Scientific software | Education              |
| ArtificialIntelligence | Artificial Intelligence software | Education;Science      |
| Astronomy    | Astronomy software | Education;Science      |
| Biology      | Biology software | Education;Science      |
| Chemistry    | Chemistry software | Education;Science      |
| ComputerScience | ComputerSience software | Education;Science      |
| DataVisualization | Data visualization software | Education;Science      |
| Economy      | Economy software | Education              |
| Electricity  | Electricity software | Education;Science      |
| Geography    | Geography software | Education              |
| Geology      | Geology software | Education;Science      |
| Geoscience   | Geoscience software | Education;Science      |
| History      | History software | Education              |
| ImageProcessing | Image Processing software | Education;Science      |
| Literature   | Literature software | Education              |
| Math         | Math software   | Education;Science      |
| NumericalAnalysis | Numerical analysis software | Education;Science;Math |
| MedicalSoftware | Medical software | Education;Science      |
| Physics      | Physics software | Education;Science      |
| Robotics     | Robotics software | Education;Science      |
| Sports       | Sports software | Education              |
| ParallelComputing | Parallel computing software | Education;Science;ComputerScience |
| Amusement    | A simple amusement |                        |
| Archiving    | A tool to archive/backup data | Utility                |
| Compression  | A tool to manage compressed data/archives | Utility;Archiving      |
| Electronics  | Electronics software, e.g. a circuit designer |                        |
| Emulator     | Emulator of another platform, such as a DOS emulator | System or Game         |
| Engineering  | Engineering software, e.g. CAD programs |                        |
| FileTools    | A file tool utility | Utility or System      |
| FileManager  | A file manager  | System;FileTools       |
| TerminalEmulator | A terminal emulator application | System                 |
| Filesystem   | A file system tool | System                 |
| Monitor      | Monitor application/applet that monitors some resource or activity | System                 |
| Security     | A security tool | Settings or System     |
| Accessibility | Accessibility   | Settings or Utility    |
| Calculator   | A calculator    | Utility                |
| Clock        | A clock application/applet | Utility                |
| TextEditor   | A text editor   | Utility                |
| Documentation | Help or documentation |                        |
| Core         | Important application, core to the desktop such as a file manager or a help browser |                        |
| KDE          | Application based on KDE libraries | QT                     |
| GNOME        | Application based on GNOME libraries | GTK                    |
| GTK          | Application based on GTK+ libraries |                        |
| Qt           | Application based on Qt libraries |                        |
| Motif        | Application based on Motif libraries |                        |
| Java         | Application based on Java GUI libraries, such as AWT or Swing |                        |
| ConsoleOnly  | Application that only works inside a terminal (text-based or command line application) |                        |

The table below describes Reserved Categories. Reserved Categories have a specific desktop specific meaning that has not been standardized (yet). Desktop entry files that use a reserved category MUST also include an appropriate OnlyShowIn= entry to restrict themselves to those environments that properly support the reserved category as used.

| **Reserved Category** | **Description** |
|:----------------------|:----------------|
| Screensaver           | A screen saver (launching this desktop entry should activate the screen saver) |
| TrayIcon              | An application that is primarily an icon for the "system tray" or "notification area" (apps that open a normal window and just happen to have a tray icon as well should not list this category) |
| Applet                | An applet that will run inside a panel or another such application, likely desktop specific |
| Shell                 | A shell (an actual specific shell such as bash or tcsh, not a TerminalEmulator) |

# B.Registered OnlyShowIn Environments #

Remember, these are case-sensitive. "KDE" not "kde" should be used.

| **OnlyShowIn Value** | **Environment** |
|:---------------------|:----------------|
| GNOME                | GNOME Desktop   |
| KDE                  | KDE Desktop     |
| ROX                  | ROX Desktop     |
| XFCE                 | XFCE Desktop    |
| Old                  | Legacy menu systems |