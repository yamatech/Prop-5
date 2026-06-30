; Prop-5 インストーラー作成スクリプト (Inno Setup)
; Windows 64-bit 専用

[Setup]
; アプリケーションの基本情報
AppName=Prop-5
AppVersion=1.0.0
AppPublisher=yamatech
AppPublisherURL=https://github.com/yamatech/Prop-5
AppSupportURL=https://github.com/yamatech/Prop-5
AppUpdatesURL=https://github.com/yamatech/Prop-5

; デフォルトのインストール先 (C:\Program Files\Prop-5)
DefaultDirName={autopf}\Prop-5
DefaultGroupName=Prop-5

; 使用許諾契約書 (GPLv3 ライセンスファイルを指定)
LicenseFile=LICENSE

; インストーラー作成時の設定
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
OutputDir=installer_output
OutputBaseFilename=Prop-5_v1.0.0_Setup
Compression=lzma2
SolidCompression=yes
WizardStyle=modern

[Types]
Name: "full"; Description: "フルインストール (Standalone & VST3)"
Name: "standaloneonly"; Description: "スタンドアロン版のみ"
Name: "vst3only"; Description: "VST3プラグインのみ"
Name: "custom"; Description: "カスタムインストール"; Flags: iscustom

[Components]
Name: "standalone"; Description: "スタンドアロンアプリ (.exe)"; Types: full standaloneonly custom
Name: "vst3"; Description: "VST3 プラグイン (.vst3)"; Types: full vst3only custom

[Files]
; スタンドアロン版のファイル配置
Source: "build\Prop-5_artefacts\Release\Standalone\Prop-5.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: standalone
Source: "docs\manual.md"; DestDir: "{app}"; Flags: ignoreversion; Components: standalone

; VST3プラグインのファイル配置 (フォルダ丸ごとコピー)
; {commoncf} は C:\Program Files\Common Files に展開されます
Source: "build\Prop-5_artefacts\Release\VST3\Prop-5.vst3\*"; DestDir: "{commoncf}\VST3\Prop-5.vst3"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: vst3

[Icons]
; スタンドアロン版用ショートカット
Name: "{group}\Prop-5"; Filename: "{app}\Prop-5.exe"; Components: standalone
Name: "{group}\Uninstall Prop-5"; Filename: "{uninstallexe}"
Name: "{autodesktop}\Prop-5"; Filename: "{app}\Prop-5.exe"; Tasks: desktopicon; Components: standalone

[Tasks]
Name: "desktopicon"; Description: "デスクトップにショートカットを作成する"; GroupDescription: "追加のアイコン:"; Flags: unchecked

[Run]
; インストール完了後にスタンドアロン版を起動するオプション
Filename: "{app}\Prop-5.exe"; Description: "Prop-5 を起動する"; Flags: nowait postinstall skipifsilent; Components: standalone
