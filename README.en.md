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

### 4. User Manual

For details on how to use Prop-5, explanations of each parameter, and how to save/load presets, please refer to the user manual:
- [User Manual (docs/manual.en.md)](docs/manual.en.md)

---

## For macOS / Linux Users

Currently, the official release packages are only available for Windows. If you wish to use this plugin on macOS or Linux, you will need to build it from the source code.
Please refer to the following document for build instructions:

- [Build Instructions (docs/how_to_build.en.md)](docs/how_to_build.en.md)

### Overview (Tools Required for Building)
1. Prepare a C++17 compatible compiler, CMake, and Git.
2. Clone this repository and place the JUCE framework (v7.0.12) directly under the project root.
3. Build the project using CMake.

---

## Our Philosophy

This software is a non-commercial project developed by an amateur individual developer, and development resources are limited.
In return, within the scope of the GNU GPLv3 license, we highly welcome users to freely fork, improve, and create/distribute derived versions (including builds for macOS, Linux, etc.).

We hope to see a world where individual developers can freely create, play, and enjoy their own synthesizers.

---

## License

This project is licensed under the **GNU General Public License v3 (GPLv3)**. See the [LICENSE](file:///W:/vsti-projects/Prop-5/LICENSE) file for details.  
This project uses the JUCE framework under the terms of the JUCE GPLv3 license.

## Disclaimer & Trademarks

- **Prophet-5** and **Sequential** are registered trademarks of **Sequential LLC**.
- This software is an unofficial, independent fan-made clone/emulator and is **not affiliated with, endorsed by, sponsored by, or associated with Sequential LLC or its affiliates in any way.**

---

## Contact

If you have any feedback, suggestions, or bug reports, please feel free to reach out to us at:

- **Email:** yamatechmusic@gmail.com
- **Developer Name:** yamatech

