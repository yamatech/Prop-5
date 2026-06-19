cmake_minimum_required(VERSION 3.22)
project(Prop-5 VERSION 0.9.8.5)

# JUCEのディレクトリを指定（環境に合わせて変更してください）
add_subdirectory(JUCE)

# プラグインの定義（VST3フォーマット、シンセサイザーとして設定）
juce_add_plugin(Prop-5
    COMPANY_NAME "yamatech"
    PLUGIN_MANUFACTURER_CODE "Prop"
    PLUGIN_CODE "Prp5"
    FORMATS VST3 Standalone
    PRODUCT_NAME "Prop-5"
    ICON_BIG "${CMAKE_CURRENT_SOURCE_DIR}/Assets/icon.png"
    ICON_SMALL "${CMAKE_CURRENT_SOURCE_DIR}/Assets/icon.png"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

# ==========================================================
# JuceHeader.h を明示的に生成してインクルードパスを通す
juce_generate_juce_header(Prop-5)

# Windows (MSVC) 環境での文字コード警告 (C4819) を抑制
if(MSVC)
    target_compile_options(Prop-5 PRIVATE /utf-8 /wd4819)
endif()

# VST2互換性チェックのエラーを回避（新規プラグインなので0を指定）
target_compile_definitions(Prop-5 PUBLIC JUCE_VST3_CAN_REPLACE_VST2=0)
# ==========================================================

# C++17標準を指定
target_compile_features(Prop-5 PUBLIC cxx_std_17)

# ソースファイルの追加
target_sources(Prop-5 PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/DSP/Voice.cpp
    Source/DSP/Oscillator.cpp
    Source/DSP/Envelope.cpp
    Source/DSP/LadderFilter.cpp
    # Source/DSP/LFO.cpp
    # Source/DSP/Arpeggiator.cpp
    Source/PropLookAndFeel.cpp
    # Source/UI/KnobComponent.cpp
)

# JUCEモジュールのリンク
target_link_libraries(Prop-5 PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_dsp
)

# ヘッダ検索パスの追加
target_include_directories(Prop-5 PRIVATE Source)

# Windows環境での VST3 フォルダアイコン化の解除
# (VST3フォルダ自体がカスタムアイコン化されるのを防ぎ、通常のフォルダ表示に戻す)
if(WIN32)
    if(TARGET Prop-5_VST3)
        set(vst3_dir "${CMAKE_BINARY_DIR}/Prop-5_artefacts/$<CONFIG>/VST3/Prop-5.vst3")
        add_custom_command(TARGET Prop-5_VST3 POST_BUILD
            COMMAND attrib -s "${vst3_dir}"
            COMMAND ${CMAKE_COMMAND} -E remove "${vst3_dir}/desktop.ini"
            COMMAND ${CMAKE_COMMAND} -E remove "${vst3_dir}/Plugin.ico"
            COMMENT "Removing custom folder icon from VST3 bundle..."
            VERBATIM
        )
    endif()
endif()