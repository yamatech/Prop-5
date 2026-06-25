# Prop-5 ビルド手順書 (How to Build)

※ この文書はマークダウンファイルです。PDFのほうが読みやすいという方は [docs_PDF](docs_PDF) フォルダをご覧ください。

本ドキュメントは、Prop-5 のソースコードをご自身でコンパイル・ビルドして使用したい開発者やユーザー向けのガイドです。

> [!NOTE]
> すでにビルド済みのプラグイン（VST3）やスタンドアロンアプリをそのまま使用したい場合は、パッケージに同梱されている `README_Install.txt` のインストール手順に従ってください。

---

## 1. 前提条件 (Prerequisites)

ビルドを行うには、以下のツールが事前にシステムにインストールされている必要があります。

* **C++17 対応のコンパイラ**
  * **Windows**: Visual Studio 2022 (「C++ によるデスクトップ開発」ワークロードを推奨)
  * **macOS**: Xcode 15 以上 (Command Line Tools を含む)
  * **Linux**: Clang または GCC
* **CMake** (バージョン 3.22 以上)
* **Git** (リポジトリのクローンおよび JUCE の取得に使用)

---

## 2. セットアップ手順

本リポジトリには JUCE フレームワーク本体のソースコードは含まれていません。ビルドを開始する前に、プロジェクトのルートディレクトリに JUCE を配置する必要があります。

### ① リポジトリのクローン
まずはターミナルまたはコマンドプロンプトを開き、本リポジトリをクローンします。

```bash
git clone https://github.com/yamatech/Prop-5.git Prop-5
cd Prop-5
```

### ② JUCE フレームワークの配置
プロジェクトの直下に `JUCE` という名前で JUCE フレームワークを配置します。

#### 【方法 A】 Git を使用して取得する (推奨)
公式の JUCE リポジトリをクローンし、プロジェクトで動作確認されているバージョン（`7.0.12`）をチェックアウトします。

```bash
git clone https://github.com/juce-framework/JUCE.git JUCE
cd JUCE
git checkout tags/7.0.12
cd ..
```

#### 【方法 B】 手動でダウンロードする
1. [JUCE 公式サイトのダウンロードページ](https://juce.com/download/) から zip ファイルをダウンロードします。
2. ダウンロードしたファイルを解凍し、中にある `JUCE` フォルダをプロジェクトのルート直下に配置します。
   *(※ `Prop-5/JUCE/CMakeLists.txt` が存在する階層構成にしてください)*

---

## 3. ビルド方法

ビルドには CMake を使用します。プロジェクトのルートディレクトリで以下のコマンドを実行してください。

### ① ビルド用ディレクトリの作成と構成 (Configure)
CMake を実行してビルド環境を生成します。

```bash
mkdir build
cd build
cmake ..
```

### ② コンパイルの実行 (Build)
プラグインおよびスタンドアロンアプリをビルドします。

* **リリースビルドを実行する場合（通常はこちらを推奨）:**
  ```bash
  cmake --build . --config Release
  ```
* **開発・デバッグ用ビルドを実行する場合:**
  ```bash
  cmake --build . --config Debug
  ```

---

## 4. ビルド成果物について

ビルドが正常に完了すると、以下のディレクトリにフォーマットごとのプラグインファイルが生成されます。

* **生成先パス:**
  * `build/Prop-5_artefacts/Release/` (または `Debug/`)

#### 便利な設定：ビルド後の自動コピー
Windows 環境で、ビルドが成功した後に特定のフォルダ（例: `C:\Program Files\Common Files\VST3`）へ VST3 プラグインファイルを自動でコピーさせたい場合は、プロジェクトルートにある `CMakeLists.txt` 内の以下の行を `TRUE` に書き換えてください。

```cmake
set(COPY_PLUGIN_AFTER_BUILD FALSE)
```
↓↓↓
```cmake
set(COPY_PLUGIN_AFTER_BUILD TRUE)
```

---

## 5. ライセンスと免責事項

* **ライセンス:**
  本プロジェクトは **GNU General Public License v3 (GPLv3)** に基づいて公開されています。詳細はパッケージ内の `LICENSE` ファイルを参照してください。
* **商標について:**
  * **Prophet-5** および **Sequential** は **Sequential LLC** の登録商標です。
  * 本ソフトウェアは個人が開発した非公式のクローン（トリビュート・エミュレータ）であり、**Sequential LLC およびその関連会社とは一切関係がなく、公認、推奨、または支援を受けたものではありません。**

---

## 6. 連絡先 (Contact)

ご意見・ご感想、バグレポートなどがありましたら、以下のメールアドレスまでご連絡ください。

* **メールアドレス:** yamatechmusic@gmail.com
* **開発者名:** yamatech
