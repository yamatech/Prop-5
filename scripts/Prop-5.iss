; scripts/Prop-5.iss
; Inno Setup configuration script for Prop-5

#ifndef MyAppVersion
  #define MyAppVersion "0.9.9"
#endif

#ifndef Configuration
  #define Configuration "Release"
#endif

[Setup]
; AppId is uniquely generated for Prop-5. Do not change for updates.
AppId={{5D9D454A-B823-455F-91F6-46B96CE9A8A5}
AppName=Prop-5
AppVersion={#MyAppVersion}
AppPublisher=yamatech
AppPublisherURL=https://github.com/yamatech/Prop-5
DefaultDirName={pf64}\Prop-5
DefaultGroupName=Prop-5
LicenseFile=..\LICENSE
OutputDir=..\build
OutputBaseFilename=Prop-5_v{#MyAppVersion}_{#Configuration}_Installer
Compression=lzma
SolidCompression=yes
ArchitecturesInstallIn64BitMode=x64
DisableProgramGroupPage=yes

[Languages]
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; Standalone App
Source: "..\build\Prop-5_artefacts\{#Configuration}\Standalone\Prop-5.exe"; DestDir: "{app}"; Flags: ignoreversion

; VST3 Plugin
Source: "..\build\Prop-5_artefacts\{#Configuration}\VST3\Prop-5.vst3\*"; DestDir: "{commoncf64}\VST3\Prop-5.vst3"; Flags: ignoreversion recursesubdirs createallsubdirs

; LICENSE
Source: "..\LICENSE"; DestDir: "{app}"; Flags: ignoreversion

; docs -> manual
Source: "..\docs\manual.pdf"; DestDir: "{app}\manual"; Flags: ignoreversion
Source: "..\docs\manual.en.pdf"; DestDir: "{app}\manual"; Flags: ignoreversion
Source: "..\docs\how_to_build.pdf"; DestDir: "{app}\manual"; Flags: ignoreversion
Source: "..\docs\how_to_build.en.pdf"; DestDir: "{app}\manual"; Flags: ignoreversion


[Icons]
Name: "{group}\Prop-5"; Filename: "{app}\Prop-5.exe"
Name: "{group}\{cm:UninstallProgram,Prop-5}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\Prop-5"; Filename: "{app}\Prop-5.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\Prop-5.exe"; Description: "{cm:LaunchProgram,Prop-5}"; Flags: nowait postinstall skipifsilent
