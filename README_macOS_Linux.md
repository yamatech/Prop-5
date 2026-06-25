# README for macOS and Linux / macOS・Linuxユーザーの皆様へ

※ この文書はマークダウンファイルです。PDFのほうが読みやすいという方は [README_PDF](README_PDF) フォルダをご覧ください。
*Note: This is a Markdown file. If you find the PDF version easier to read, please refer to the [README_PDF](README_PDF) folder.*

## 日本語 (Japanese)

### macOSおよびLinux版の提供について

現在、開発者の手元にはWindowsの開発環境しかなく、実機での検証が行えないため、macOS（およびLogic等のAU環境）やLinux向けのバイナリ（リリースパッケージ）は公式には提供していません。

しかし、本プロジェクトはJUCEフレームワークを使用しているため、C++およびCMakeの開発環境が整っている方であれば、macOSやLinux上でもビルドが可能です。

もしご自身でビルドしてみたい場合は、以下のドキュメントを参考にビルドを行ってください。
- [ビルド手順書 (docs/how_to_build.md)](docs/how_to_build.md)

本ソフトウェアはGPLv3ライセンスに基づき、自由なフォークや改良・派生版の配布を歓迎しています。ご自身の環境でビルドして動作した、あるいはmacOS版の派生版を配布したいといった試みを心よりお待ちしております。

---

## English (English)

### About macOS and Linux Support

Currently, official binaries (release packages) are not provided for macOS and Linux because the developer only has a Windows development environment and cannot test the software on physical macOS or Linux devices.

However, since this project is built with the JUCE framework, you can compile it from source on macOS or Linux if you have a C++ and CMake development environment set up.

If you would like to build it yourself, please refer to the following guide:
- [Build Instructions (docs/how_to_build.en.md)](docs/how_to_build.en.md)

Under the GNU GPLv3 license, we highly encourage you to fork, modify, and distribute your own derived versions. Contributions and forks to support macOS or Linux are very welcome!
