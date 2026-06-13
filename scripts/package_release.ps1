# scripts/package_release.ps1

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$rootDir = Resolve-Path "$scriptDir\.."
$buildDir = "$rootDir\build"
$artefactsDir = "$buildDir\Prop-5_artefacts\Release"

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

# Verify build artefacts exist
$vst3Path = "$artefactsDir\VST3\Prop-5.vst3"
$exePath = "$artefactsDir\Standalone\Prop-5.exe"

if (!(Test-Path $vst3Path)) {
    Write-Error "VST3 artefact not found: $vst3Path `nPlease build the Release target first."
    exit 1
}
if (!(Test-Path $exePath)) {
    Write-Error "Standalone exe not found: $exePath `nPlease build the Release target first."
    exit 1
}

# Prepare package directory
$zipFileName = "Prop-5_v${version}_Windows.zip"
$zipPath = "$rootDir\$zipFileName"
$tempDir = "$buildDir\package_temp"

if (Test-Path $tempDir) {
    Remove-Item -Recurse -Force $tempDir
}
New-Item -ItemType Directory -Path $tempDir | Out-Null

Write-Host "Copying files to package directory..."

# Copy standalone exe and VST3 folder
Copy-Item -Path $exePath -Destination "$tempDir\"
Copy-Item -Path $vst3Path -Destination "$tempDir\" -Recurse

# Copy license
$licensePath = "$rootDir\LICENSE"
if (Test-Path $licensePath) {
    Copy-Item -Path $licensePath -Destination "$tempDir\"
}

# Generate README_Install.txt from template
$templatePath = "$scriptDir\README_Install_template.txt"
if (Test-Path $templatePath) {
    $readmeContent = Get-Content $templatePath -Raw -Encoding utf8
    $readmeContent = $readmeContent.Replace("{VERSION}", $version)
    # Write as UTF-8 with BOM to ensure proper encoding on Windows notepad
    $readmeContent | Out-File -FilePath "$tempDir\README_Install.txt" -Encoding utf8
} else {
    Write-Warning "README_Install_template.txt not found. Packaging without it."
}

# Create ZIP archive
if (Test-Path $zipPath) {
    Remove-Item -Force $zipPath
}

Write-Host "Creating ZIP archive: $zipFileName"
Compress-Archive -Path "$tempDir\*" -DestinationPath $zipPath -Force

# Clean up
Remove-Item -Recurse -Force $tempDir

Write-Host "Packaging completed successfully!"
Write-Host "Output: $zipPath"
