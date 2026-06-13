# Prop-5

Japanese version of README is here: [README.md](README.md)

## Overview

Prop-5 is a synthesizer audio plugin (VST3 / Standalone formats) developed using the JUCE framework.  
*Note: Currently in beta testing.*  
This synthesizer was modeled after and inspired by the legendary analog synthesizer, Prophet-5.  
We plan to build release packages in the future, and development is currently active.  
Please note that this project partially utilizes AI-assisted coding (Gemini and Antigravity).

## Download and Installation Instructions (For End-Users)

*Note: This release package is currently for **Windows only (64-bit)**. If you would like to use this plugin without building it yourself, please follow the steps below:*

### 1. Download the Package
1. Go to the [GitHub Releases page](https://github.com/yamatech/Prop-5/releases).
2. Download `Prop-5_v[Version]_Windows.zip` from the "Assets" section of the latest release.
3. Extract the downloaded ZIP file to any directory.

### 2. Install VST3 Plugin
1. Copy the `Prop-5.vst3` folder (the entire folder structure) from the extracted files into the following directory:
   - **Destination Path:** `C:\Program Files\Common Files\VST3\`
   *(Note: If the `VST3` folder does not exist, please create it. You may need Administrator privileges to perform this copy)*
2. Restart your DAW (Cubase, Ableton Live, Studio One, Reaper, etc.) or trigger a plug-in rescan. The DAW will automatically detect the plugin.

### 3. Launch Standalone App
- `Prop-5.exe` is the Standalone version and does not require installation. You can place it anywhere and double-click to run it.

---

## Prerequisites

To build this project, the following tools must be installed on your system in advance:

- **C++17 Compatible Compiler**
  - Windows: Visual Studio 2022 (with "Desktop development with C++" workload)
  - macOS: Xcode 15 or higher (including Command Line Tools)
  - Linux: Clang or GCC
- **CMake** (version 3.22 or higher)

---

## Setup Instructions

This repository does not include the JUCE framework source code itself (it is excluded by `.gitignore`). Before building, you need to place JUCE in the root directory of the project.

### 1. Clone the Repository
First, clone this repository.

```bash
git clone https://github.com/yamatech/Prop-5 Prop-5
cd Prop-5
```

### 2. Place JUCE
Place the JUCE framework directly under the project root with the folder name `JUCE`.

#### Method A: Get using Git (Recommended)
Clone the official JUCE repository and check out the version verified to work with the project (e.g., `7.0.12`).

```bash
git clone https://github.com/juce-framework/JUCE.git JUCE
cd JUCE
git checkout tags/7.0.12
cd ..
```

#### Method B: Download Manually
1. Download the zip file from the [JUCE Official Website](https://juce.com/download/).
2. Extract the file and place the `JUCE` folder directly in the root of this project.
   (Make sure `Prop-5/JUCE/CMakeLists.txt` exists in this layout)

---

## How to Build

Use CMake for building. Execute the following commands in the root directory of the project.

### 1. Create and Configure Build Directory
```bash
mkdir build
cd build
cmake ..
```

### 2. Run the Compilation
To perform a release build, run the following command:

```bash
cmake --build . --config Release
```

*Note: For development debug builds, specify `--config Debug` instead.*

---

## About Build Artifacts

Upon a successful build, the plugin files for each format will be generated in the following directories:

- **VST3 Plugin / Standalone App:**
  - Generated inside `build/Prop-5_artefacts/Release/` (or `Debug/`).

On Windows, if you want to automatically copy the plug-in to a specific folder (e.g., `C:\Program Files\Common Files\VST3`) after building, change `COPY_PLUGIN_AFTER_BUILD` to `TRUE` in [CMakeLists.txt](file:///W:/vsti-projects/Prop-5/CMakeLists.txt).

---

## License

This project is licensed under the **GNU General Public License v3 (GPLv3)**. See the [LICENSE](file:///W:/vsti-projects/Prop-5/LICENSE) file for details.  
This project uses the JUCE framework under the terms of the JUCE GPLv3 license.

## Disclaimer & Trademarks

- **Prophet-5** and **Sequential** are registered trademarks of **Sequential LLC**.
- This software is an unofficial, independent fan-made clone/emulator and is **not affiliated with, endorsed by, sponsored by, or associated with Sequential LLC or its affiliates in any way.**
