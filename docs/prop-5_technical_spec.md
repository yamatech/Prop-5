
| TECHNICAL SPECIFICATION prop-5 VST3 アナログシンセサイザー  ·  Prophet 5 エミュレーション   プラグインファイル: prop-5.vst3  ·  JUCE 8.x  ·  v1.0.0-draft   |
| :---: |

# **1\. 概要**

**1.1 プロジェクト基本情報**

| プラグイン名 | prop-5 |
| :---- | :---- |
| **プラグインファイル** | prop-5.vst3 |
| **フォーマット** | VST3（Steinberg VST3 SDK 準拠） |
| **フレームワーク** | JUCE 8.x（C++17 以上） |
| **ポリフォニー** | 5 Voice Polyphony（Prophet 5 Rev.3.3 準拠） |
| **内部処理精度** | 64bit 浮動小数点（double精度） |
| **サンプルレート** | 44.1 / 48 / 88.2 / 96 / 192 kHz 対応 |
| **ステータス** | v1.0.0 DRAFT |

**1.2 対応プラットフォーム**

| OS | バージョン要件 | コンパイラ/アーキテクチャ |
| :---- | :---- | :---- |
| Windows | Windows 10 / 11 | MSVC 2022  ·  x86\_64 |
| macOS | macOS 12 Monterey 以降 | Clang  ·  Universal Binary (x86\_64 \+ Apple Silicon) |
| Linux | Ubuntu 20.04 LTS 以降 | GCC 11 / Clang 14  ·  x86\_64 |

**1.3 設計方針**

Prophet 5 (Rev.3.3) のアナログ回路の振る舞いを DSP でエミュレートする。VCO・VCF・VCA・ADSR×2・LFO・アルペジエーター/シーケンサーの各モジュールを JUCE DSP モジュールとカスタム実装で構成する。

UI は Prophet 5 の実機パネルレイアウトを忠実に再現し、左から「POLY MOD → OSC A/B → MIXER → VCF → ENV A/B → LFO → ARP/MASTER」の順に配置する。

パラメーター管理・オートメーション・プリセット保存は JUCE の AudioProcessorValueTreeState (APVTS) で一元化する。

# **2\. DSP シグナルチェーン**

**2.1 ボイスごとのシグナルフロー**

各ボイスは以下の順でシグナルを処理する：

**VCO A / VCO B  →  MIXER  →  VCF（4-pole Ladder LP）  →  VCA  →  ボイスバス集約**

**2.2 モジュール仕様**

| VCO A | 波形: SAW / SQR（PWM対応）。Poly Mod（ENV B / OSC B）でピッチおよびPWを変調可能。 |
| :---- | :---- |
| **VCO B** | 波形: SAW / SQR / TRI（PWM対応）。Fine Tune（±50セント）。Lo モードでサブオシレーター動作。OSC SYNC（A→B）対応。 |
| **NOISE** | ホワイトノイズジェネレーター。MIXERへ直結。 |
| **MIXER** | OSC A / OSC B / NOISEの独立レベルノブ。 |
| **VCF** | 4-pole Ladder Low Pass Filter（Curtis CEM3320/Moogモデル）。Cutoff 20Hz〜20kHz（対数）。Resonance 0〜自己発振。KB Track: Off / 1/2 / Full。ENV Amount: 双極性（±全域）。 |
| **VCA** | ENV B または Gate 選択。Velocity Sensitivity 対応（v1.0）。 |
| **ENV A** | フィルターエンベロープ ADSR。Attack 0.5ms〜30s、Decay 1ms〜30s、Sustain 0〜1.0、Release 5ms〜30s。 |
| **ENV B** | アンプ / Poly Mod エンベロープ ADSR。同レンジ。Poly Mod Source として VCO A Freq / PW / VCF Cutoff へ送出。 |
| **LFO** | 波形: SIN / TRI / SQR / SAW / S\&H。周波数 0.01〜50Hz。変調先: Freq A / PW A / VCF Cutoff（各スイッチ）。 |
| **ARP** | Up / Down / Up-Down / Random モード。レンジ 1〜4オクターブ。DAW テンポ同期対応（v1.0）。 |

# **3\. パラメーター一覧**

全パラメーターは AudioProcessorValueTreeState に登録し、VST3 オートメーション・プリセット保存に対応する。

| パラメーターID | パラメーター名 | グループ | レンジ | デフォルト | 種別 |
| :---- | :---- | :---- | :---- | :---- | :---- |
| osc\_a\_freq | OSC A Frequency | **OSC** | 16′〜2′（7オクターブ） | 8′ | ロータリースイッチ |
| osc\_a\_pw | OSC A Pulse Width | **OSC** | 0〜100% | 50% | ノブ |
| osc\_a\_saw | OSC A Saw On | **OSC** | Off/On | On | トグルスイッチ |
| osc\_a\_sqr | OSC A Square On | **OSC** | Off/On | Off | トグルスイッチ |
| osc\_b\_freq | OSC B Frequency | **OSC** | 16′〜2′ | 8′ | ロータリースイッチ |
| osc\_b\_fine | OSC B Fine Tune | **OSC** | −50〜+50 cent | 0 | ノブ |
| osc\_b\_pw | OSC B Pulse Width | **OSC** | 0〜100% | 50% | ノブ |
| osc\_b\_saw | OSC B Saw On | **OSC** | Off/On | On | トグルスイッチ |
| osc\_b\_sqr | OSC B Square On | **OSC** | Off/On | Off | トグルスイッチ |
| osc\_b\_tri | OSC B Triangle On | **OSC** | Off/On | Off | トグルスイッチ |
| osc\_b\_lo | OSC B Lo Mode | **OSC** | Off/On | Off | トグルスイッチ |
| osc\_sync | OSC Sync | **OSC** | Off/On | Off | トグルスイッチ |
| mix\_osc\_a | Mixer OSC A Level | **MIX** | 0〜1.0 | 0.75 | ノブ |
| mix\_osc\_b | Mixer OSC B Level | **MIX** | 0〜1.0 | 0.75 | ノブ |
| mix\_noise | Mixer Noise Level | **MIX** | 0〜1.0 | 0.0 | ノブ |
| vcf\_cutoff | VCF Cutoff | **VCF** | 20Hz〜20kHz | 1kHz | ノブ（対数） |
| vcf\_reson | VCF Resonance | **VCF** | 0〜1.0 | 0.0 | ノブ |
| vcf\_env\_amt | VCF Env Amount | **VCF** | −1.0〜+1.0 | 0.5 | ノブ（双極） |
| vcf\_kb\_track | VCF KB Track | **VCF** | Off / 1/2 / Full | Off | 3段スイッチ |
| env\_a\_atk | ENV A Attack | **ENV** | 0.5ms〜30s | 20ms | ノブ（対数） |
| env\_a\_dcy | ENV A Decay | **ENV** | 1ms〜30s | 200ms | ノブ（対数） |
| env\_a\_sus | ENV A Sustain | **ENV** | 0〜1.0 | 0.7 | ノブ |
| env\_a\_rel | ENV A Release | **ENV** | 5ms〜30s | 300ms | ノブ（対数） |
| env\_b\_atk | ENV B Attack | **ENV** | 0.5ms〜30s | 5ms | ノブ（対数） |
| env\_b\_dcy | ENV B Decay | **ENV** | 1ms〜30s | 150ms | ノブ（対数） |
| env\_b\_sus | ENV B Sustain | **ENV** | 0〜1.0 | 0.8 | ノブ |
| env\_b\_rel | ENV B Release | **ENV** | 5ms〜30s | 200ms | ノブ（対数） |
| vca\_mode | VCA Mode | **ENV** | Env / Gate | Env | トグルスイッチ |
| lfo\_freq | LFO Frequency | **LFO** | 0.01〜50Hz | 2Hz | ノブ（対数） |
| lfo\_amt | LFO Amount | **LFO** | 0〜1.0 | 0.0 | ノブ |
| lfo\_wave | LFO Waveform | **LFO** | SIN/TRI/SQR/SAW/S\&H | SIN | 5段スイッチ |
| lfo\_dest\_freq\_a | LFO Dest: Freq A | **LFO** | Off/On | Off | トグルスイッチ |
| lfo\_dest\_pw\_a | LFO Dest: PW A | **LFO** | Off/On | Off | トグルスイッチ |
| lfo\_dest\_vcf | LFO Dest: VCF | **LFO** | Off/On | Off | トグルスイッチ |
| pmod\_env\_amt | Poly Mod Env Amt | **ENV** | 0〜1.0 | 0.0 | ノブ |
| pmod\_osc\_b\_amt | Poly Mod OSC B Amt | **OSC** | 0〜1.0 | 0.0 | ノブ |
| pmod\_dest\_freq\_a | Poly Mod Dest: Freq A | **ENV** | Off/On | Off | トグルスイッチ |
| pmod\_dest\_pw\_a | Poly Mod Dest: PW A | **ENV** | Off/On | Off | トグルスイッチ |
| pmod\_dest\_vcf | Poly Mod Dest: VCF | **ENV** | Off/On | Off | トグルスイッチ |
| arp\_on | Arpeggiator On | **ARP** | Off/On | Off | トグルスイッチ |
| arp\_mode | Arp Mode | **ARP** | Up/Down/UD/Rnd | Up | 4段スイッチ |
| arp\_range | Arp Range | **ARP** | 1〜4 Oct | 1 | ロータリー |
| arp\_rate | Arp Rate | **ARP** | BPM sync / 0.5〜30Hz | 1/2音符 | ノブ |
| master\_volume | Master Volume | **MIX** | 0〜1.0 | 0.8 | ノブ |
| master\_tune | Master Tune | **MIX** | −100〜+100 cent | 0 | ノブ |
| pitchbend\_range | Pitch Bend Range | **MIX** | 1〜12 半音 | 2 | スピンボックス |
| unison | Unison Mode | **MIX** | Off/On | Off | トグルスイッチ |

# **4\. UI 実装仕様**

**4.1 ウィンドウ仕様**

| ウィンドウサイズ | 1200 × 440 px（基本） |
| :---- | :---- |
| **スケール対応** | ×0.75 〜 ×1.5（ユーザーリサイズ対応） |
| **HiDPI対応** | Retina / HiDPI 完全対応 |
| **レンダリング** | JUCE OpenGL Renderer（Software Fallback付き） |

**4.2 パネルセクション配置（左→右）**

| 順 | セクション | 主なコントロール |
| :---: | :---- | :---- |
| **1** | **POLY MOD** | ENV Amt ノブ / OSC B Amt ノブ / Dest スイッチ（Freq A / PW A / Filter） |
| **2** | **OSC A** | Freq ロータリー / PW ノブ / SAW・SQR スイッチ |
| **3** | **OSC B** | Freq ロータリー / Fine ノブ / PW ノブ / SAW・SQR・TRI スイッチ / Lo・Sync スイッチ |
| **4** | **MIXER** | OSC A / OSC B / NOISE 各レベルノブ |
| **5** | **VCF** | Cutoff / Resonance / Env Amt ノブ / KB Track スイッチ |
| **6** | **ENV A** | Attack / Decay / Sustain / Release ノブ（Filter Envelope） |
| **7** | **ENV B** | Attack / Decay / Sustain / Release ノブ（Amp Envelope）/ VCA Mode スイッチ |
| **8** | **LFO** | Freq / Amount ノブ / 波形スイッチ / Dest スイッチ |
| **9** | **ARP** | On スイッチ / Mode / Range / Rate |
| **10** | **MASTER** | Volume / Tune ノブ / Pitch Bend Range / Unison スイッチ |

**4.3 カスタム LookAndFeel 仕様**

* ノブ: ビンテージメタル風（Gold/Ivory）、ドラッグで回転、ダブルクリックでリセット、右クリックで数値入力

* スイッチ: LED 風発光トグル（点灯色 \#C8A84B）

* パネル背景色: \#1A1410（艶消しブラック）

* フォント: 実機準拠のサンセリフ（Label）

* プリセットブラウザ: オーバーレイパネル（v1.0 実装予定）

# **5\. JUCE 実装ガイド**

**5.1 プロジェクト構成**

| ファイル / ディレクトリ | 役割 |
| :---- | :---- |
| CMakeLists.txt | ビルド定義（juce\_add\_plugin、VST3指定） |
| Source/PluginProcessor.cpp | Prop5Processor：APVTS管理・MIDIルーティング・ボイスプール |
| Source/PluginEditor.cpp | Prop5Editor：UIルート・LookAndFeel適用・セクション配置 |
| Source/DSP/Voice.cpp | Prop5Voice：SynthesiserVoice継承・DSPブロック統合 |
| Source/DSP/Oscillator.cpp | VCO A/B：波形生成・PWM・Sync |
| Source/DSP/LadderFilter.cpp | VCF：4-pole Ladder（Huovilainen 2004アルゴリズム） |
| Source/DSP/Envelope.cpp | ADSR汎用クラス（ENV A / ENV B共用） |
| Source/DSP/LFO.cpp | LFO：多波形・Delay/Fade In |
| Source/DSP/Arpeggiator.cpp | ARP：モード・テンポ同期 |
| Source/UI/PropLookAndFeel.cpp | カスタムノブ・スイッチ描画 |
| Source/UI/KnobComponent.cpp | ノブ入力処理・右クリックメニュー |
| Assets/ | スキン画像・フォント |

**5.2 主要クラス設計**

| クラス | 責務・主要メンバー |
| :---- | :---- |
| **Prop5Processor** | AudioProcessor継承。APVTS管理、VoicePool（5 voices）、Arpeggiator、MidiBuffer管理、processBlock()実装。 |
| **Prop5Editor** | AudioProcessorEditor継承。PropLookAndFeel・各セクションComponent・OpenGLContext・PresetBrowserオーバーレイ。 |
| **Prop5Voice** | SynthesiserVoice継承。Oscillator oscA/oscB、LadderFilter vcf、AdsrEnvelope envA/envB、renderNextBlock()。 |
| **LadderFilter** | 4-pole Ladder（Moogモデル）。Huovilainen(2004)。熱ノイズ付加オプション。processSample(double):double。 |
| **PropLookAndFeel** | LookAndFeel\_V4継承。drawRotarySlider()・drawToggleButton()オーバーライド。ビンテージ外観実装。 |

**5.3 APVTS パラメーター登録方針**

* AudioParameterFloat: 連続値パラメーター（Cutoff, Resonance, ADSR等）。NormalisableRange に対数スケール（skewFactor）を設定。

* AudioParameterBool: トグルスイッチ（波形選択、Sync、LFO Dest等）。

* AudioParameterChoice: 多段スイッチ（KB Track Off/1/2/Full、LFO Waveform等）。StringArray でラベル定義。

* stringFromValueFunction を全パラメーターに設定し、ホスト側に単位付き文字列を表示する。

# **6\. 要件チェックリスト**

v1.0ではチェックマークの項目のみを実装し、チェックマークのない項目は実装しない。

**6.1 音源モジュール（v1.0 対象）**

* **\[✓\]** VCO A：SAW / SQR（PWM付き）

* **\[✓\]** VCO B：SAW / SQR / TRI（PWM付き）

* **\[✓\]** OSC B Lo モード（サブオシレーター動作）

* **\[✓\]** OSC SYNC（A → B ハードシンク）

* **\[✓\]** ノイズジェネレーター（ホワイト）

* **\[✓\]** Poly Mod（ENV B → VCO A / VCF）

* **\[✓\]** 4-pole Ladder LPF（自己発振対応）

* **\[✓\]** Keyboard Tracking（Off / 1/2 / Full）

* **\[✓\]** ENV A (Filter) ADSR

* **\[✓\]** ENV B (Amp) ADSR

* **\[✓\]** LFO（SIN / SQR / SAW / TRI / S\&H）

* **\[ \]** LFO Delay / Fade In

* **\[ \]** Velocity Sensitivity（VCA）

* **\[ \]** Unison モード

**6.2 UI / システム**

* **\[✓\]** Prophet 5 準拠パネルレイアウト

* **\[✓\]** カスタム LookAndFeel（ビンテージノブ）

* **\[✓\]** ノブ右クリック → 値入力ダイアログ

* **\[✓\]** HiDPI / Retina 対応

* **\[✓\]** VST3 パラメーターオートメーション

* **\[✓\]** プリセット保存・読み込み（XML）

* **\[✓\]** ピッチベンド / モジュレーションホイール

* **\[✓\]** MIDI CC マッピング

* **\[ \]** アルペジエーター（Up / Down / UD / Rnd）

* **\[ \]** ステップシーケンサー（16 step）

* **\[ \]** DAW テンポ同期（LFO / ARP）

* **\[ \]** プリセットブラウザ UI

**6.3 ビルド・テスト**

* **\[✓\]** JUCE CMake ビルドシステム

* **\[✓\]** VST3 SDK バリデーション（Steinberg Validator）

* **\[✓\]** プラグインスキャン対応（クラッシュなし）

* **\[ \]** Audio Unit v3 対応（macOS 追加）

* **\[ \]** CLAP フォーマット対応（v1.1 予定）

* **\[ \]** 単体テスト（DSP CTest）

# **7\. 用語集**

| 用語 | 説明 |
| :---- | :---- |
| **VCO** | Voltage Controlled Oscillator。電圧制御発振器。アナログシンセの音源波形を生成するモジュール。 |
| **VCF** | Voltage Controlled Filter。電圧制御フィルター。音色のブライトネスを制御する。 |
| **VCA** | Voltage Controlled Amplifier。電圧制御アンプ。音量エンベロープを適用する。 |
| **ADSR** | Attack / Decay / Sustain / Release。エンベロープの4段階パラメーター。 |
| **LFO** | Low Frequency Oscillator。低周波発振器。モジュレーションのソースとして使う。 |
| **PWM** | Pulse Width Modulation。矩形波のパルス幅を変調する技法。 |
| **Poly Mod** | Polyphonic Modulation。Prophet 5固有のボイスごとのモジュレーション機能。 |
| **OSC SYNC** | オシレーターシンク。VCO AがVCO Bをリセットし、独特の倍音を生成する。 |
| **APVTS** | AudioProcessorValueTreeState。JUCEのパラメーター管理・直列化クラス。 |
| **S\&H** | Sample & Hold。ランダムなステップ波形を生成するLFO波形。 |

— 以上 —
