# Prop-5 Build Instructions (How to Build)

*Note: This is a Markdown file. If you find the PDF version easier to read, please refer to the [docs_PDF](docs_PDF) folder.*

This document is a guide for developers and users who wish to compile and build the Prop-5 source code themselves.

> [!NOTE]
> If you want to use the pre-built plug-ins (VST3) or the standalone app without compiling them yourself, please follow the installation instructions in `README_Install.txt` included in the package.

---

## 1. Prerequisites

To build this project, the following tools must be installed on your system in advance:

* **C++17 Compatible Compiler**
  * **Windows**: Visual Studio 2022 (with "Desktop development with C++" workload)
  * **macOS**: Xcode 15 or higher (including Command Line Tools)
  * **Linux**: Clang or GCC
* **CMake** (version 3.22 or higher)
* **Git** (used for cloning the repository and retrieving JUCE)

---

## 2. Setup Instructions

This repository does not include the JUCE framework source code itself. Before building, you need to place JUCE in the root directory of the project.

### ① Clone the Repository
First, open your terminal or command prompt and clone this repository:

```bash
git clone https://github.com/yamatech/Prop-5.git Prop-5
cd Prop-5
```

### ② Place the JUCE Framework
Place the JUCE framework directly under the project root with the folder name `JUCE`.

#### [Method A] Get using Git (Recommended)
Clone the official JUCE repository and check out the version verified to work with this project (`7.0.12`):

```bash
git clone https://github.com/juce-framework/JUCE.git JUCE
cd JUCE
git checkout tags/7.0.12
cd ..
```

#### [Method B] Download Manually
1. Download the zip file from the [JUCE Official Website](https://juce.com/download/).
2. Extract the file and place the `JUCE` folder directly in the root of this project.
   *(Make sure `Prop-5/JUCE/CMakeLists.txt` exists in this layout)*

---

## 3. How to Build

Use CMake for building. Execute the following commands in the root directory of the project.

### ① Create and Configure Build Directory
Run CMake to generate the build environment:

```bash
mkdir build
cd build
cmake ..
```

### ② Run the Compilation (Build)
Build the plugin and standalone app:

* **To perform a Release Build (Recommended):**
  ```bash
  cmake --build . --config Release
  ```
* **To perform a Debug Build (for development):**
  ```bash
  cmake --build . --config Debug
  ```

---

## 4. About Build Artifacts

Upon a successful build, the plugin files for each format will be generated in the following directory:

* **Output Path:**
  * `build/Prop-5_artefacts/Release/` (or `Debug/`)

#### Convenient Setting: Copy Plugin After Build
On Windows, if you want to automatically copy the VST3 plug-in to a specific folder (e.g., `C:\Program Files\Common Files\VST3`) after a successful build, change the following line in `CMakeLists.txt` (located in the project root) from `FALSE` to `TRUE`:

```cmake
set(COPY_PLUGIN_AFTER_BUILD FALSE)
```
↓↓↓
```cmake
set(COPY_PLUGIN_AFTER_BUILD TRUE)
```

---

## 5. License & Disclaimer

* **License:**
  This project is licensed under the **GNU General Public License v3 (GPLv3)**. See the `LICENSE` file for details.
* **Disclaimer & Trademarks:**
  * **Prophet-5** and **Sequential** are registered trademarks of **Sequential LLC**.
  * This software is an unofficial, independent fan-made clone/emulator and is **not affiliated with, endorsed by, sponsored by, or associated with Sequential LLC or its affiliates in any way.**

---

## 6. Contact

If you have any feedback, suggestions, or bug reports, please feel free to reach out to us at:

* **Email:** yamatechmusic@gmail.com
* **Developer Name:** yamatech
