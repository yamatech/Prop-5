# scripts/package_release.ps1
param (
    [Parameter(Mandatory=$false)]
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Release"
)

$ErrorActionPreference = "Stop"

$scriptDir = $PSScriptRoot
$rootDir = Split-Path -Parent $scriptDir
$buildDir = Join-Path $rootDir "build"
$artefactsDir = Join-Path $buildDir "Prop-5_artefacts\$Configuration"


# Get version from CMakeLists.txt
$cmakePath = "$rootDir\CMakeLists.txt"
if (!(Test-Path $cmakePath)) {
    Write-Error "CMakeLists.txt not found."
    exit 1
}

$cmakeContent = Get-Content $cmakePath -Raw
if ($cmakeContent -match 'project\(\s*Prop-5\s+VERSION\s+([0-9\.]+)\s*\)') {
    $version = $Matches[1]
} else {
    $version = "unknown"
    Write-Warning "Failed to extract version from CMakeLists.txt. Defaulting to 'unknown'."
}

Write-Host "Project version: $version"
Write-Host "Configuration: $Configuration"

# Verify build artefacts exist
$vst3Path = "$artefactsDir\VST3\Prop-5.vst3"
$exePath = "$artefactsDir\Standalone\Prop-5.exe"

if (!(Test-Path $vst3Path)) {
    Write-Error "VST3 archetype not found: $vst3Path `nPlease build the $Configuration target first."
    exit 1
}
if (!(Test-Path $exePath)) {
    Write-Error "Standalone exe not found: $exePath `nPlease build the $Configuration target first."
    exit 1
}

# ==========================================================
# Inno Setup (ISCC.exe) によるインストーラー作成
# ==========================================================

Write-Host "Searching for Inno Setup compiler (ISCC.exe)..."
$isccPath = ""
$isccSearchPaths = @(
    "ISCC.exe",
    "$env:LOCALAPPDATA\Programs\Inno Setup 6\ISCC.exe",
    "C:\Program Files (x86)\Inno Setup 6\ISCC.exe",
    "C:\Program Files\Inno Setup 6\ISCC.exe",
    "C:\Program Files (x86)\Inno Setup 5\ISCC.exe",
    "C:\Program Files\Inno Setup 5\ISCC.exe"
)

foreach ($path in $isccSearchPaths) {
    if ($path -eq "ISCC.exe") {
        $check = Get-Command "ISCC.exe" -ErrorAction SilentlyContinue
        if ($check) {
            $isccPath = $check.Source
            break
        }
    } elseif (Test-Path $path) {
        $isccPath = $path
        break
    }
}

$installerCreated = $false
$expectedInstaller = $buildDir + "\Prop-5_v" + $version + "_" + $Configuration + "_Installer.exe"

if ($isccPath) {
    Write-Host "Inno Setup Compiler found: $isccPath"
    Write-Host "Building Installer..."
    $issPath = "$scriptDir\Prop-5.iss"
    
    # Run ISCC
    & $isccPath "/DMyAppVersion=$version" "/DConfiguration=$Configuration" $issPath
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Installer created successfully!"
        if (Test-Path $expectedInstaller) {
            $installerCreated = $true
            $releaseInstallerPath = $rootDir + "\Prop-5_v" + $version + "_" + $Configuration + "_Installer.exe"
            if (Test-Path $releaseInstallerPath) { Remove-Item -Force $releaseInstallerPath }
            Copy-Item -Path $expectedInstaller -Destination $releaseInstallerPath
            Write-Host "Installer Output: $releaseInstallerPath"
        }
    } else {
        Write-Warning "Inno Setup compiler failed with exit code $LASTEXITCODE."
    }
} else {
    Write-Warning "Inno Setup (ISCC.exe) was not found. Installer build skipped."
    Write-Host "To build the installer, please install Inno Setup 6 (https://jrsoftware.org/isinfo.php) or add ISCC.exe to your PATH."
}

# ==========================================================
# ZIP パッケージの作成（インストーラー＋マニュアル等）
# ==========================================================

$zipFileName = "Prop-5_v" + $version + "_" + $Configuration + "_Windows.zip"
$zipPath = $rootDir + "\" + $zipFileName
$tempDir = $buildDir + "\package_temp"

if (Test-Path $tempDir) {
    Remove-Item -Recurse -Force $tempDir
}
New-Item -ItemType Directory -Path $tempDir | Out-Null

Write-Host "Preparing package directory for ZIP..."

# 1. インストーラーを含める
if ($installerCreated -and (Test-Path $expectedInstaller)) {
    Copy-Item -Path $expectedInstaller -Destination "$tempDir\"
} else {
    Write-Warning "Installer executable not found or not built. Skipping installer inclusion in ZIP."
}

# 2. LICENSE コピー
$licensePath = "$rootDir\LICENSE"
if (Test-Path $licensePath) {
    Copy-Item -Path $licensePath -Destination "$tempDir\"
}

# 3. docs ディレクトリコピー
$docsDir = "$rootDir\docs"
if (Test-Path $docsDir) {
    Copy-Item -Path $docsDir -Destination $tempDir -Recurse
} else {
    Write-Warning "docs directory not found."
}

# 4. README コピー (Markdown)
$readmeFiles = @(
    "README.md",
    "README.en.md"
)

foreach ($file in $readmeFiles) {
    $filePath = "$rootDir\$file"
    if (Test-Path $filePath) {
        Copy-Item -Path $filePath -Destination "$tempDir\"
    } else {
        Write-Warning "$file not found."
    }
}

# 5. ZIP作成
if (Test-Path $zipPath) {
    Remove-Item -Force $zipPath
}

Write-Host "Creating ZIP archive: $zipFileName"
Compress-Archive -Path "$tempDir\*" -DestinationPath $zipPath -Force

# 6. クリーンアップ
Remove-Item -Recurse -Force $tempDir

Write-Host "ZIP Packaging completed successfully!"
Write-Host "ZIP Output: $zipPath"

