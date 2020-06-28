#define SteamAppID 311730

[Setup]
AppId={#UPGRADE_GUID}
AppName={#CPACK_PACKAGE_NAME}
AppVersion={#CPACK_PACKAGE_VERSION}
AppCopyright=© {#CPACK_PACKAGE_VENDOR}
WizardStyle=modern
DefaultDirName={code:DefDirRoot}
Compression=lzma2
SolidCompression=yes
ArchitecturesInstallIn64BitMode=x64
DisableReadyPage=yes
DirExistsWarning=no
PrivilegesRequired=admin
InfoBeforeFile=..\doc\end-users\mod_resolution_readme.txt
AppPublisher={#CPACK_PACKAGE_VENDOR}
UninstallFilesDir={app}\mod_resolution
DefaultGroupName={#CPACK_PACKAGE_NAME}

[UninstallDelete]
Type: files; Name: "{app}\mod_resolution.log"

[Icons]
Name: "{group}\Configuration"; Filename: "{app}\mod_resolution.ini"
Name: "{group}\Readme"; Filename: "{app}\mod_resolution_readme.txt"
Name: "{group}\Support"; Filename: "https://www.steffenl.com/contact"
Name: "{group}\Uninstall"; Filename: "{uninstallexe}"

[Files]
Source: "..\build\Release\d3d9.dll"; DestDir: "{app}"
Source: "..\doc\end-users\mod_resolution.ini"; DestDir: "{app}"; Flags: onlyifdoesntexist
Source: "..\doc\end-users\mod_resolution_readme.txt"; DestDir: "{app}"

[Run]
Filename: "{app}\mod_resolution.ini"; Description: "Open configuration file"; Flags: postinstall shellexec
Filename: "{app}\mod_resolution_readme.txt"; Description: "Open readme"; Flags: postinstall shellexec unchecked

[Code]
const
    RootUninstallRegKeyPath = 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall';
    AppUninstallRegKeyName = '{#UPGRADE_GUID}_is1';
    GameUninstallRegKeyName = 'Steam App {#SteamAppID}';
    InstallDirRegValueName = 'InstallLocation';
    SM_CXSCREEN = 0;
    SM_CYSCREEN = 1;
    ConfigFileName = 'mod_resolution.ini';

var
    ConfigPage: TInputQueryWizardPage;

function GetSystemMetrics(nIndex: Integer): Integer;
external 'GetSystemMetrics@user32.dll stdcall setuponly';

function GetScreenResolutionString(): String;
begin
    Result := IntToStr(GetSystemMetrics(SM_CXSCREEN)) + 'x' + IntToStr(GetSystemMetrics(SM_CYSCREEN));
end;

function FileExistsInInstallDir(Name: String): Boolean;
begin
    Result := FileExists(ExpandConstant('{app}\') + Name);
end;

function GetInstallDir(KeyName: String; var Value: String): Boolean;
begin
    Result := RegQueryStringValue(HKEY_LOCAL_MACHINE, RootUninstallRegKeyPath + '\' + KeyName, InstallDirRegValueName, Value);
end;

function DefDirRoot(Param: String): String;
var
    InstallDir: String;
begin
    if GetInstallDir(GameUninstallRegKeyName, InstallDir) then
        Result := InstallDir
    else
        Result := ExpandConstant('{pf}');
end;

function IsAlreadyInstalled: Boolean;
var
    InstallDir: String;
begin
    if GetInstallDir(AppUninstallRegKeyName, InstallDir) then
        Result := DirExists(InstallDir)
    else
        Result := False;
end;

procedure InitializeWizard;
begin
    ConfigPage := CreateInputQueryPage(
        wpSelectDir,
        'Configuration',
        'Video resolution configuration.',
        'Please specify the video resolution you wish to use for the game. You''ll have the option to edit the full configuration later.');
    ConfigPage.Add('Video resolution:', False);
end;

function NextButtonClick(CurPageID: Integer): Boolean;
var
    AlreadyInstalled: Boolean;
    GameExists: Boolean;
    D3d9Exists: Boolean;
begin
    if CurPageID = wpSelectDir then
    begin
        AlreadyInstalled := IsAlreadyInstalled;
        GameExists := FileExistsInInstallDir('game.exe');
        D3d9Exists := FileExistsInInstallDir('d3d9.dll');

        if not GameExists then
        begin
            MsgBox('The game (game.exe) was not found in the chosen directory.', mbError, MB_OK);
            Result := False;
            exit;
        end;

        if D3d9Exists and not AlreadyInstalled then
        begin
            MsgBox('Please move the existing d3d9.dll file before installation. If you wish to keep using it then please set it up in the proxy configuration after installation.', mbError, MB_OK);
            Result := False;
            exit;
        end;
    end;

    if CurPageID = ConfigPage.ID then
    begin
        if ConfigPage.Values[0] = '' then
        begin
            MsgBox('Video resolution is required.', mbError, MB_OK);
            Result := False;
            exit;
        end;
    end;

    Result := True;
end;

procedure CurPageChanged(CurPageID: Integer);
var
    ConfigFilePath: String;
    Resolution: String;
begin
    if CurPageID = ConfigPage.ID then
    begin
        if ConfigPage.Values[0] = '' then
        begin
            ConfigFilePath := ExpandConstant('{app}\') + ConfigFileName;
            Resolution := GetIniString('Video', 'Resolution', GetScreenResolutionString(), ConfigFilePath);
            ConfigPage.Values[0] := Resolution
        end;
    end;
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
    ConfigFilePath: String;
begin
    if CurStep = ssPostInstall then
    begin
        ConfigFilePath := ExpandConstant('{app}\') + ConfigFileName;
        SetIniString('Video', 'Resolution', ConfigPage.Values[0], ConfigFilePath);
    end;
end;
