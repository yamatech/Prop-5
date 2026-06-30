# Prop-5

The English version of the README is here: [README.en.md](README.en.md)

## 概要

Prop-5 は、JUCE フレームワークを使用して開発されたシンセサイザー・オーディオプラグイン（VST3 / Standalone 形式）です。  
このシンセサイザーは、アナログシンセサイザーの名機 Prophet-5 に影響を受けて、モデルとして作られました。   
なお、このプロジェクトは一部AIコーディング（Gemini および Antigravity）を使用しております。

## ダウンロードとインストール手順（一般ユーザー向け）

※本パッケージは現在 **Windows専用 (64bit)** です。ビルドせずにそのまま使用したい場合は、以下の手順に従ってください。

### 1. ZIPパッケージのダウンロードと展開

1. [GitHub Releases ページ](https://github.com/yamatech/Prop-5/releases) にアクセスします。
2. 最新リリース（Latest）の「Assets」セクションから、`Prop-5_v[バージョン]_[構成]_Windows.zip` をダウンロードします。
3. ダウンロードした ZIP ファイルを任意のフォルダに解凍（展開）します。

### 2. インストーラーの実行とインストールオプション

1. 展開したフォルダ内にある `Prop-5_v[バージョン]_[構成]_Installer.exe` を実行します。
2. 画面の指示に従ってインストールを行います。
3. インストーラーの実行中、以下のインストール構成を選択できます：
   - **フルインストール**: VST3 プラグインとスタンドアロン版アプリの両方をインストールします。（推奨）
   - **スタンドアロン版のみ**: DAWなしで単体起動できるアプリのみをインストールします。
   - **VST3プラグインのみ**: DAW（Cubase, Ableton Live, Studio One, Reaper など）で使用するプラグインのみをインストールします。

### 3. アンインストール方法

アンインストールしたい場合は、Windowsの「設定」＞「アプリ」＞「インストールされているアプリ」から **Prop-5** を選択し、「アンインストール」を実行してください。

### 4. 取扱説明書 (マニュアル)

本パッケージには、取扱説明書などのドキュメント一式が同梱されています。使い方や各パラメーターの説明、およびプリセットの保存・読み込み方法などの詳細については、展開したフォルダ内の `docs/manual.md`（またはPDF版 `docs/docs_PDF/manual.pdf`）をご覧ください。
リポジトリ上のマニュアルは以下から参照できます。
- [取扱説明書 (docs/manual.md)](docs/manual.md)

---

## macOS / Linux版のご利用について

現在、公式リリースのパッケージは Windows 専用となっております。macOS および Linux 環境でご利用になりたい場合は、ソースコードからご自身でビルドしていただく必要があります。
ビルドの手順については、以下のドキュメントをご参照ください。

- [ビルド手順書 (docs/how_to_build.md)](docs/how_to_build.md)

### 概要（ビルドに必要なツール）
1. C++17 対応コンパイラ、CMake、Git を用意する。
2. 本リポジトリをクローンし、プロジェクト直下に JUCE フレームワーク（v7.0.12）を配置する。
3. CMake を使用してビルドを行う。

---

## プロジェクトの理念 (Our Philosophy)

このソフトウェアは、アマチュア個人開発者による非営利のプロジェクトであり、開発リソースには限界があります。
その代わり、GPLv3ライセンスの範囲内において、ユーザーが自由にフォークし、改良を加え、派生版（macOS版やLinux版のビルドなどを含む）を作成して配布することを大いに歓迎します。

個人デベロッパーが自由に自分のシンセサイザーを作り、それを演奏して楽しむ世界が広がることを願っています。

---

## ライセンス (License)

本プロジェクトは **GNU General Public License v3 (GPLv3)** のもとで公開されています。詳細については [LICENSE](file:///W:/vsti-projects/Prop-5/LICENSE) ファイルを参照してください。  
本プロジェクトは JUCE フレームワークを使用しており、JUCE の GPLv3 ライセンス条項に従っています。

This project is licensed under the **GNU General Public License v3 (GPLv3)**. See the [LICENSE](file:///W:/vsti-projects/Prop-5/LICENSE) file for details.  
This project uses the JUCE framework under the terms of the JUCE GPLv3 license.

## 免責事項・商標について (Disclaimer & Trademarks)

- **Prophet-5** および **Sequential** は **Sequential LLC** の登録商標です。
- 本ソフトウェアは個人が開発した非公式のクローン（トリビュート・エミュレータ）であり、**Sequential LLC およびその関連会社とは一切関係がなく、公認、推奨、または支援を受けたものではありません。**

- **Prophet-5** and **Sequential** are registered trademarks of **Sequential LLC**.
- This software is an unofficial, independent fan-made clone/emulator and is **not affiliated with, endorsed by, sponsored by, or associated with Sequential LLC or its affiliates in any way.**

---

## 連絡先 (Contact)

ご意見・ご感想、バグレポートなどがありましたら、以下のメールアドレスまでご連絡ください。

- **メールアドレス:** yamatechmusic@gmail.com
- **開発者名:** yamatech
