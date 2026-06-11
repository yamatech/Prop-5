# Prop-5

Prop-5 は、JUCE フレームワークを使用して開発されたシンセサイザー・オーディオプラグイン（VST3 / Standalone 形式）です。

## 前提条件 (Prerequisites)

ビルドを行うには、以下のツールが事前にシステムにインストールされている必要があります。

- **C++17 対応のコンパイラ**
  - Windows: Visual Studio 2022 (C++ によるデスクトップ開発ワークロード)
  - macOS: Xcode 15 以上 (Command Line Tools を含む)
  - Linux: Clang または GCC
- **CMake** (バージョン 3.22 以上)

---

## セットアップ手順

本リポジトリには JUCE フレームワーク本体のソースコードは含まれていません（`.gitignore` により除外されています）。ビルドする前に、プロジェクトのルートディレクトリに JUCE を配置する必要があります。

### 1. リポジトリのクローン
まずは本リポジトリをクローンします。

```bash
git clone <本リポジトリのURL> Prop-5
cd Prop-5
```

### 2. JUCE の配置
プロジェクトの直下に `JUCE` という名前で JUCE フレームワークを配置します。

#### 方法 A: Git を使用して取得する (推奨)
公式の JUCE リポジトリをクローンし、プロジェクトで動作確認されているバージョン（例: `7.0.12`）をチェックアウトします。

```bash
git clone https://github.com/juce-framework/JUCE.git JUCE
cd JUCE
git checkout tags/7.0.12
cd ..
```

#### 方法 B: 手動でダウンロードする
1. [JUCE 公式サイト](https://juce.com/download/) から zip ファイルをダウンロードします。
2. 解凍してできた `JUCE` フォルダを、本プロジェクトのルート直下に配置します。
   （`Prop-5/JUCE/CMakeLists.txt` が存在する構成にしてください）

---

## ビルド方法

ビルドには CMake を使用します。以下のコマンドをプロジェクトのルートディレクトリで実行してください。

### 1. ビルド用のディレクトリ作成と構成
```bash
mkdir build
cd build
cmake ..
```

### 2. コンパイルの実行
リリースビルドを行う場合は、以下のコマンドを実行します。

```bash
cmake --build . --config Release
```

*※開発用のデバッグビルドを行う場合は `--config Debug` を指定してください。*

---

## ビルド成果物について

ビルドが成功すると、以下のディレクトリにフォーマットごとのプラグインファイルが生成されます。

- **VST3 プラグイン / スタンドアロンアプリ:**
  - `build/Prop-5_artefacts/Release/` (または `Debug/`) 内に生成されます。

Windows 環境で、ビルド後に特定のフォルダ（例: `C:\Program Files\Common Files\VST3`）へ自動コピーしたい場合は、[CMakeLists.txt](file:///W:/vsti-projects/Prop-5/CMakeLists.txt) 内の `COPY_PLUGIN_AFTER_BUILD` を `TRUE` に変更してください。
