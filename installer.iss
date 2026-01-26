; Cordshot Installer Script for Inno Setup
; Download Inno Setup from: https://jrsoftware.org/isinfo.php

#define MyAppName "Cordshot"
#define MyAppVersion "1.1.0"
#define MyAppPublisher "Cordshot"
#define MyAppURL "https://github.com/williampepple1/cordshot"
#define MyAppExeName "cordshot.exe"
#define MyAppDescription "A lightweight, modern screenshot tool for Windows. Capture any region of your screen with just two clicks."

[Setup]
; App identity
AppId={{A1B2C3D4-E5F6-7890-ABCD-EF1234567890}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
AppComments={#MyAppDescription}
AppReadmeFile=https://github.com/williampepple1/cordshot#readme

; Installation settings
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes

; Output settings
OutputDir=installer_output
OutputBaseFilename=Cordshot-{#MyAppVersion}-Setup
SetupIconFile=cordshot.ico
LicenseFile=LICENSE
Compression=lzma2/ultra64
SolidCompression=yes

; Privileges
PrivilegesRequired=admin
PrivilegesRequiredOverridesAllowed=dialog

; Visual
WizardStyle=modern
WizardSizePercent=100

; Uninstaller
UninstallDisplayIcon={app}\{#MyAppExeName}
UninstallDisplayName={#MyAppName}

; Minimum Windows version (Windows 10)
MinVersion=10.0

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Messages]
WelcomeLabel2=This will install [name/ver] on your computer.%n%n{#MyAppDescription}%n%nFeatures:%n- Region Capture: Click and drag to capture any area%n- Auto-Save: Configure a folder for automatic saving%n- Clipboard Copy: Screenshots copied automatically%n- System Tray: Runs quietly in the background%n%nClick Next to continue.

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "startupicon"; Description: "Start Cordshot when Windows starts"; GroupDescription: "Startup:"; Flags: unchecked

[Files]
; Main executable
Source: "dist\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion

; Qt Core DLLs
Source: "dist\Qt6Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "dist\Qt6Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "dist\Qt6Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "dist\Qt6Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "dist\Qt6Svg.dll"; DestDir: "{app}"; Flags: ignoreversion

; Runtime DLLs
Source: "dist\libgcc_s_seh-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "dist\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "dist\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "dist\D3Dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "dist\opengl32sw.dll"; DestDir: "{app}"; Flags: ignoreversion

; Qt Plugins - Platforms
Source: "dist\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs

; Qt Plugins - Styles
Source: "dist\styles\*"; DestDir: "{app}\styles"; Flags: ignoreversion recursesubdirs

; Qt Plugins - Image Formats
Source: "dist\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs

; Qt Plugins - Icon Engines
Source: "dist\iconengines\*"; DestDir: "{app}\iconengines"; Flags: ignoreversion recursesubdirs

; Qt Plugins - Generic
Source: "dist\generic\*"; DestDir: "{app}\generic"; Flags: ignoreversion recursesubdirs

; Qt Plugins - Network
Source: "dist\networkinformation\*"; DestDir: "{app}\networkinformation"; Flags: ignoreversion recursesubdirs

; Qt Plugins - TLS
Source: "dist\tls\*"; DestDir: "{app}\tls"; Flags: ignoreversion recursesubdirs

[Icons]
; Start Menu
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"

; Desktop (optional)
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

; Startup (optional)
Name: "{userstartup}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: startupicon

[Run]
; Launch after install
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
; Clean up settings on uninstall (optional)
Type: filesandordirs; Name: "{localappdata}\Cordshot"
