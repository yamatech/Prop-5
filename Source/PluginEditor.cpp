#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Prop5Editor::Prop5Editor (Prop5Processor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel (&lookAndFeel);

    // Set window size to 1380 x 575 (1.15x of 1200x500) for Prophet-5 Grid layout
    setSize (1380, 575);

    // Helpers to setup common components
    auto setupSlider = [this] (juce::Slider& slider, juce::Label& label, const juce::String& name)
    {
        slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 15);
        addAndMakeVisible (slider);
        label.setText (name, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centred);
        label.attachToComponent (&slider, false);
    };

    auto setupToggle = [this] (juce::ToggleButton& button, const juce::String& name)
    {
        button.setButtonText (name);
        addAndMakeVisible (button);
    };

    auto setupCombo = [this] (juce::ComboBox& combo, juce::Label& label, const juce::String& name, const juce::StringArray& choices)
    {
        combo.addItemList (choices, 1);
        addAndMakeVisible (combo);
        label.setText (name, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centred);
        label.attachToComponent (&combo, false);
    };

    // --- Preset Navigation ---
    prevPresetButton.setButtonText ("<");
    addAndMakeVisible (prevPresetButton);
    prevPresetButton.onClick = [this]
    {
        int activeProgram = audioProcessor.getCurrentProgram();
        if (activeProgram > 0)
            audioProcessor.setCurrentProgram (activeProgram - 1);
    };

    nextPresetButton.setButtonText (">");
    addAndMakeVisible (nextPresetButton);
    nextPresetButton.onClick = [this]
    {
        int activeProgram = audioProcessor.getCurrentProgram();
        if (activeProgram < audioProcessor.getNumPrograms() - 1)
            audioProcessor.setCurrentProgram (activeProgram + 1);
    };

    initButton.setButtonText ("INIT");
    addAndMakeVisible (initButton);
    initButton.onClick = [this]
    {
        audioProcessor.setCurrentProgram (0);
    };

    addAndMakeVisible (presetCombo);
    updatePresetComboBoxItems();
    
    presetCombo.onChange = [this]
    {
        int id = presetCombo.getSelectedId();
        if (id > 0)
        {
            audioProcessor.setCurrentProgram (id - 1);
        }
    };
    
    presetLabel.setText ("PRESET", juce::dontSendNotification);
    addAndMakeVisible (presetLabel);

    // SAVE button
    saveButton.setButtonText ("SAVE");
    saveButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xffff2a1a)); // 実機風赤
    saveButton.setColour (juce::TextButton::textColourOffId, juce::Colours::white);
    addAndMakeVisible (saveButton);
    saveButton.onClick = [this]
    {
        fileChooser = std::make_unique<juce::FileChooser> (
            "Save Preset",
            audioProcessor.getCurrentPresetFolder(),
            "*.prop5"
        );
        
        fileChooser->launchAsync (juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles,
            [this] (const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file != juce::File{})
                {
                    if (file.getFileExtension() != ".prop5")
                        file = file.withFileExtension (".prop5");
                        
                    audioProcessor.savePresetToFile (file);
                    audioProcessor.scanPresets();
                    updatePresetComboBoxItems();
                    
                    int newIndex = audioProcessor.userPresetFiles.indexOf (file);
                    if (newIndex >= 0)
                    {
                        int progIdx = 13 + newIndex;
                        audioProcessor.setCurrentProgram (progIdx);
                        presetCombo.setSelectedId (progIdx + 1, juce::dontSendNotification);
                    }
                }
            });
    };

    // LOAD button
    loadButton.setButtonText ("LOAD");
    loadButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xff1c75bc)); // バランスをとるための青系（SAVE赤との対比）
    loadButton.setColour (juce::TextButton::textColourOffId, juce::Colours::white);
    addAndMakeVisible (loadButton);
    loadButton.onClick = [this]
    {
        fileChooser = std::make_unique<juce::FileChooser> (
            "Load Preset",
            audioProcessor.getCurrentPresetFolder(),
            "*.prop5"
        );
        
        fileChooser->launchAsync (juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this] (const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file != juce::File{})
                {
                    if (audioProcessor.loadPresetFromFile (file))
                    {
                        audioProcessor.scanPresets();
                        updatePresetComboBoxItems();
                        
                        int fileIndex = audioProcessor.userPresetFiles.indexOf (file);
                        if (fileIndex >= 0)
                        {
                            int progIdx = 13 + fileIndex;
                            audioProcessor.setCurrentProgram (progIdx);
                            presetCombo.setSelectedId (progIdx + 1, juce::dontSendNotification);
                        }
                        else
                        {
                            presetCombo.setSelectedId (0, juce::dontSendNotification);
                            presetCombo.setText (file.getFileNameWithoutExtension(), juce::dontSendNotification);
                        }
                    }
                }
            });
    };

    // SET FOLDER button
    setFolderButton.setButtonText ("SET FOLDER");
    addAndMakeVisible (setFolderButton);
    setFolderButton.onClick = [this]
    {
        fileChooser = std::make_unique<juce::FileChooser> (
            "Select Preset Folder",
            audioProcessor.getCurrentPresetFolder(),
            ""
        );
        
        fileChooser->launchAsync (juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories,
            [this] (const juce::FileChooser& fc)
            {
                auto folder = fc.getResult();
                if (folder != juce::File{} && folder.isDirectory())
                {
                    audioProcessor.setCurrentPresetFolder (folder);
                    updatePresetComboBoxItems();
                }
            });
    };

    // Timer to sync Preset Combo Box from processor (DAW program selection)
    startTimer (100);

    // --- Performance Wheels ---
    // Pitch Bend
    pitchBendSlider.setSliderStyle (juce::Slider::LinearVertical);
    pitchBendSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible (pitchBendSlider);
    pitchBendLabel.setText ("PB", juce::dontSendNotification);
    pitchBendLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (pitchBendLabel);
    // Mouse release triggers spring-back-to-center behavior
    pitchBendSlider.onDragEnd = [this] { pitchBendSlider.setValue (0.0, juce::sendNotificationAsync); };

    // Mod Wheel
    modWheelSlider.setSliderStyle (juce::Slider::LinearVertical);
    modWheelSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible (modWheelSlider);
    modWheelLabel.setText ("MW", juce::dontSendNotification);
    modWheelLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (modWheelLabel);

    pitchBendAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "pitchbend", pitchBendSlider);
    modWheelAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "modwheel", modWheelSlider);

    // --- LFO ---
    lfoShapeLabel.setText ("SHAPE", juce::dontSendNotification);
    lfoShapeLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (lfoShapeLabel);

    setupToggle (lfoTriButton, "Tri");
    setupToggle (lfoSquButton, "Squ");
    setupToggle (lfoSawButton, "Saw");

    auto setLfoWave = [this] (int index)
    {
        if (auto* param = dynamic_cast<juce::AudioParameterChoice*>(audioProcessor.apvts.getParameter ("lfo_wave")))
        {
            *param = index;
        }
    };
    lfoTriButton.onClick = [this, setLfoWave] { setLfoWave (0); };
    lfoSquButton.onClick = [this, setLfoWave] { setLfoWave (1); };
    lfoSawButton.onClick = [this, setLfoWave] { setLfoWave (2); };

    setupSlider (lfoRateSlider, lfoRateLabel, "FREQ");
    setupSlider (lfoInitialAmountSlider, lfoInitialAmountLabel, "INIT AMT");

    lfoRateAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "lfo_rate", lfoRateSlider);
    lfoInitialAmountAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "lfo_initial_amount", lfoInitialAmountSlider);

    // --- WHEEL-MOD ---
    setupSlider (wheelModSourceMixSlider, wheelModSourceMixLabel, "SRC MIX");
    setupToggle (wheelModDestFreqAButton, "FREQ A");
    setupToggle (wheelModDestFreqBButton, "FREQ B");
    setupToggle (wheelModDestPwAButton, "PW A");
    setupToggle (wheelModDestPwBButton, "PW B");
    setupToggle (wheelModDestFilterButton, "FILTER");

    wheelModSourceMixAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "wheel_mod_source_mix", wheelModSourceMixSlider);
    wheelModDestFreqAAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "wheel_mod_dest_freq_a", wheelModDestFreqAButton);
    wheelModDestFreqBAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "wheel_mod_dest_freq_b", wheelModDestFreqBButton);
    wheelModDestPwAAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "wheel_mod_dest_pw_a", wheelModDestPwAButton);
    wheelModDestPwBAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "wheel_mod_dest_pw_b", wheelModDestPwBButton);
    wheelModDestFilterAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "wheel_mod_dest_filter", wheelModDestFilterButton);

    // --- POLY MOD ---
    setupSlider (polyModEnvAmtSlider, polyModEnvAmtLabel, "ENV AMT");
    setupSlider (polyModOscBAmtSlider, polyModOscBAmtLabel, "OSC B AMT");
    setupToggle (polyModDestFreqAButton, "Freq A");
    setupToggle (polyModDestPwAButton, "PW A");
    setupToggle (polyModDestVcfButton, "VCF");

    polyModEnvAmtAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "poly_mod_env_amt", polyModEnvAmtSlider);
    polyModOscBAmtAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "poly_mod_osc_b_amt", polyModOscBAmtSlider);
    polyModDestFreqAAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "poly_mod_dest_freq_a", polyModDestFreqAButton);
    polyModDestPwAAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "poly_mod_dest_pw_a", polyModDestPwAButton);
    polyModDestVcfAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "poly_mod_dest_vcf", polyModDestVcfButton);

    // --- OSC A ---
    setupSlider (oscAFreqSlider, oscAFreqLabel, "FREQ A");
    setupSlider (oscAPwSlider, oscAPwLabel, "PW A");
    setupToggle (oscASawButton, "Saw");
    setupToggle (oscASqrButton, "Square");

    oscAFreqAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "osc_a_freq", oscAFreqSlider);
    oscAPwAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "osc_a_pw", oscAPwSlider);
    oscASawAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "osc_a_saw", oscASawButton);
    oscASqrAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "osc_a_sqr", oscASqrButton);

    // --- OSC B ---
    setupSlider (oscBFreqSlider, oscBFreqLabel, "FREQ B");
    setupSlider (oscBFineSlider, oscBFineLabel, "FINE B");
    setupSlider (oscBPwSlider, oscBPwLabel, "PW B");
    setupToggle (oscBSawButton, "Saw");
    setupToggle (oscBSqrButton, "Square");
    setupToggle (oscBTriButton, "Triangle");
    setupToggle (oscBLoModeButton, "Lo Mode");

    oscBFreqAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "osc_b_freq", oscBFreqSlider);
    oscBFineAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "osc_b_fine", oscBFineSlider);
    oscBPwAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "osc_b_pw", oscBPwSlider);
    oscBSawAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "osc_b_saw", oscBSawButton);
    oscBSqrAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "osc_b_sqr", oscBSqrButton);
    oscBTriAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "osc_b_tri", oscBTriButton);
    oscBLoModeAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "osc_b_lo_mode", oscBLoModeButton);

    // --- OSC Sync ---
    setupToggle (oscSyncButton, "Sync");
    oscSyncAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "osc_sync", oscSyncButton);

    // --- Mixer / Global ---
    setupSlider (mixOscASlider, mixOscALabel, "OSC A");
    setupSlider (mixOscBSlider, mixOscBLabel, "OSC B");
    setupSlider (mixNoiseSlider, mixNoiseLabel, "NOISE");
    setupSlider (masterVolumeSlider, masterVolumeLabel, "VOLUME");
    setupSlider (masterTuneSlider, masterTuneLabel, "TUNE");
    setupSlider (pbRangeSlider, pbRangeLabel, "PB RANGE");
    setupSlider (glideSlider, glideLabel, "GLIDE");
    setupToggle (unisonButton, "UNISON");
    setupToggle (holdButton, "HOLD");

    mixOscAAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "mix_osc_a", mixOscASlider);
    mixOscBAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "mix_osc_b", mixOscBSlider);
    mixNoiseAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "mix_noise", mixNoiseSlider);
    masterVolumeAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "master_volume", masterVolumeSlider);
    masterTuneAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "master_tune", masterTuneSlider);
    pbRangeAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "pitchbend_range", pbRangeSlider);
    glideAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "glide", glideSlider);
    unisonAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "unison", unisonButton);
    holdAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "vca_mode", holdButton);

    // --- VCF ---
    setupSlider (cutoffSlider, cutoffLabel, "CUTOFF");
    setupSlider (resonanceSlider, resonanceLabel, "RESON");
    setupSlider (vcfEnvAmtSlider, vcfEnvAmtLabel, "ENV AMT");
    
    vcfKbTrackLabel.setText ("Kb Track", juce::dontSendNotification);
    vcfKbTrackLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (vcfKbTrackLabel);

    setupToggle (kbTrackOffButton, "OFF");
    setupToggle (kbTrackHalfButton, "HALF");
    setupToggle (kbTrackFullButton, "FULL");

    auto setKbTrack = [this] (int index)
    {
        if (auto* param = dynamic_cast<juce::AudioParameterChoice*>(audioProcessor.apvts.getParameter ("vcf_kb_track")))
        {
            *param = index;
        }
    };
    kbTrackOffButton.onClick = [this, setKbTrack] { setKbTrack (0); };
    kbTrackHalfButton.onClick = [this, setKbTrack] { setKbTrack (1); };
    kbTrackFullButton.onClick = [this, setKbTrack] { setKbTrack (2); };

    cutoffAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "vcf_cutoff", cutoffSlider);
    resonanceAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "vcf_reson", resonanceSlider);
    vcfEnvAmtAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "vcf_env_amt", vcfEnvAmtSlider);

    // --- Envelope A ---
    setupSlider (envAAtkSlider, envAAtkLabel, "ATTACK");
    setupSlider (envADcySlider, envADcyLabel, "DECAY");
    setupSlider (envASusSlider, envASusLabel, "SUSTAIN");
    setupSlider (envARelSlider, envARelLabel, "RELEASE");

    envAAtkAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "env_a_atk", envAAtkSlider);
    envADcyAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "env_a_dcy", envADcySlider);
    envASusAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "env_a_sus", envASusSlider);
    envARelAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "env_a_rel", envARelSlider);

    // --- Envelope B ---
    setupSlider (envBAtkSlider, envBAtkLabel, "ATTACK");
    setupSlider (envBDcySlider, envBDcyLabel, "DECAY");
    setupSlider (envBSusSlider, envBSusLabel, "SUSTAIN");
    setupSlider (envBRelSlider, envBRelLabel, "RELEASE");

    envBAtkAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "env_b_atk", envBAtkSlider);
    envBDcyAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "env_b_dcy", envBDcySlider);
    envBSusAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "env_b_sus", envBSusSlider);
    envBRelAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "env_b_rel", envBRelSlider);

    // --- Velocity Setup ---
    setupToggle (velocityToAmpButton, "AMP");
    setupToggle (velocityToFilterButton, "FILT");

    velocityToAmpAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "velocity_to_amp", velocityToAmpButton);
    velocityToFilterAttach = std::make_unique<ButtonAttach>(audioProcessor.apvts, "velocity_to_filter", velocityToFilterButton);
}

Prop5Editor::~Prop5Editor()
{
    setLookAndFeel (nullptr);
    stopTimer();
}

//==============================================================================
void Prop5Editor::paint (juce::Graphics& g)
{
    float scale = getWidth() / 1200.0f;
    g.addTransform (juce::AffineTransform::scale (scale));

    // 1. 全体背景（フラットなダークグレー/ブラック）
    g.fillAll (juce::Colour (0xff0f0f11));

    // 2. ウッドフレーム（左右・下部）
    juce::ColourGradient leftWood (juce::Colour (0xff6a3b16), 0.0f, 0.0f,
                                   juce::Colour (0xff361c08), 10.0f, 0.0f, false);
    g.setGradientFill (leftWood);
    g.fillRect (0, 0, 10, 500);

    juce::ColourGradient rightWood (juce::Colour (0xff361c08), 1190.0f, 0.0f,
                                    juce::Colour (0xff6a3b16), 1200.0f, 0.0f, false);
    g.setGradientFill (rightWood);
    g.fillRect (1190, 0, 10, 500);

    juce::ColourGradient bottomWood (juce::Colour (0xff361c08), 0.0f, 490.0f,
                                     juce::Colour (0xff502b10), 0.0f, 500.0f, false);
    g.setGradientFill (bottomWood);
    g.fillRect (0, 490, 1200, 10);

    // 3. 各セクションの枠線とタイトルの描画
    auto drawSection = [&g] (const juce::Rectangle<int>& area, const juce::String& name, const juce::Colour& titleColor)
    {
        float cornerSize = 4.0f; // 角丸の半径

        // セクション背景（角丸のフラットな背景色で塗りつぶし）
        g.setColour (juce::Colour (0xff0f0f11));
        g.fillRoundedRectangle (area.toFloat(), cornerSize);

        // 細いオフホワイトの角丸枠線（太さ1.0f）
        g.setColour (juce::Colour (0xffd1d1d6));
        g.drawRoundedRectangle (area.toFloat(), cornerSize, 1.0f);

        // タイトル（細身でフラットなフォント、太字ではなくPlain）
        g.setFont (juce::Font (12.0f, juce::Font::plain));
        g.setColour (titleColor);
        g.drawText (name, area.getX() + 12, area.getY() + 8, area.getWidth() - 24, 18, juce::Justification::left);
    };

    // Prophet-5の無機質なデザインに合わせ、セクションタイトル色はオフホワイトに統一
    juce::Colour titleGold = juce::Colour (0xffeae6df);
    juce::Colour titleWhite = juce::Colour (0xffeae6df);

    // WHEELS
    drawSection (juce::Rectangle<int> (15, 15, 75, 420), "WHEELS", titleWhite);

    // COLUMN 1
    drawSection (juce::Rectangle<int> (100, 15, 265, 130), "POLY-MOD", titleGold);
    drawSection (juce::Rectangle<int> (100, 155, 265, 130), "LFO", titleGold);
    drawSection (juce::Rectangle<int> (100, 295, 265, 140), "WHEEL-MOD", titleGold);

    // COLUMN 2
    drawSection (juce::Rectangle<int> (375, 15, 265, 200), "OSCILLATOR A", titleGold);
    drawSection (juce::Rectangle<int> (375, 225, 265, 210), "OSCILLATOR B", titleGold);

    // COLUMN 3
    drawSection (juce::Rectangle<int> (650, 15, 265, 110), "OSC MIX", titleGold);
    
    // FILTER (VCF) は内部に線を描く
    juce::Rectangle<int> filterArea (650, 135, 265, 300);
    drawSection (filterArea, "FILTER", titleGold);
    
    // VCF ENV 用の内部境界線（立体感をなくしたフラットなライン）
    g.setColour (juce::Colour (0xff3a3a40));
    g.drawHorizontalLine (filterArea.getY() + 105, filterArea.getX() + 5, filterArea.getRight() - 5);
    g.setFont (juce::Font (11.0f, juce::Font::plain));
    g.setColour (titleWhite.withAlpha (0.7f));
    g.drawText ("VCF ENV", filterArea.getX() + 12, filterArea.getY() + 110, filterArea.getWidth() - 24, 15, juce::Justification::left);

    // Kb Track 用の内部境界線
    g.drawHorizontalLine (filterArea.getY() + 205, filterArea.getX() + 5, filterArea.getRight() - 5);

    // COLUMN 4
    drawSection (juce::Rectangle<int> (925, 15, 265, 130), "VCA ENV", titleGold);
    
    // GLOBAL は内部に線を描く
    juce::Rectangle<int> globalArea (925, 155, 265, 280);
    drawSection (globalArea, "GLOBAL", titleGold);
    
    // VELOCITY 用の内部境界線（立体感をなくしたフラットなライン）
    g.setColour (juce::Colour (0xff3a3a40));
    g.drawHorizontalLine (globalArea.getY() + 170, globalArea.getX() + 5, globalArea.getRight() - 5);
    g.setFont (juce::Font (11.0f, juce::Font::plain));
    g.setColour (titleWhite.withAlpha (0.7f));
    g.drawText ("VELOCITY", globalArea.getX() + 12, globalArea.getY() + 175, globalArea.getWidth() - 24, 15, juce::Justification::left);

    // PRESET BAR (白線の枠なしでスッキリさせるため、描画を省略)
    // drawSection (juce::Rectangle<int> (15, 445, 1170, 45), "", titleWhite);

    // --- プラグインロゴ・エンブレム描画 ---
    // プリセットバーの左側の空きスペース（x = 30付近）に、実機風のオレンジ/レッドの太字イタリックロゴを描画
    g.setColour (juce::Colour (0xfff25f38)); // ヴィンテージ・オレンジ（実機ロゴインスパイア）
    g.setFont (juce::Font ("Arial", 18.0f, juce::Font::bold | juce::Font::italic));
    g.drawText ("Prop-5", 30, 448, 65, 22, juce::Justification::left);

    // ロゴの右隣に「BETA」をオレンジ色で小さく描画
    g.setFont (juce::Font ("Arial", 10.0f, juce::Font::bold));
    g.drawText ("BETA", 98, 448, 40, 22, juce::Justification::left);

    g.setColour (juce::Colour (0xffeae6df).withAlpha (0.5f)); // 薄いオフホワイト
    g.setFont (juce::Font ("Arial", 9.0f, juce::Font::plain));
    g.drawText ("POLYPHONIC SYNTHESIZER", 30, 469, 140, 12, juce::Justification::left);

    // プリセットバーの右側の空きスペース（x = 970〜1170）に、バージョン情報を右寄せで描画
    g.setColour (juce::Colour (0xffeae6df).withAlpha (0.4f));
    g.setFont (juce::Font ("Arial", 9.0f, juce::Font::plain));
    g.drawText ("VERSION 0.9.0", 970, 458, 200, 20, juce::Justification::right);
}

void Prop5Editor::resized()
{
    float scale = getWidth() / 1200.0f;
    auto s = [scale] (int val) -> int { return juce::roundToInt (val * scale); };
    auto sRect = [s] (int x, int y, int w, int h) -> juce::Rectangle<int>
    {
        return { s(x), s(y), s(w), s(h) };
    };

    // --- PRESET BAR Placement ---
    int prY = 445;
    presetLabel.setBounds (sRect (180, prY + 10, 70, 25));
    prevPresetButton.setBounds (sRect (250, prY + 10, 30, 25));
    presetCombo.setBounds (sRect (290, prY + 10, 220, 25));
    nextPresetButton.setBounds (sRect (520, prY + 10, 30, 25));
    initButton.setBounds (sRect (570, prY + 10, 50, 25));
    saveButton.setBounds (sRect (630, prY + 10, 60, 25));
    loadButton.setBounds (sRect (700, prY + 10, 60, 25));
    setFolderButton.setBounds (sRect (770, prY + 10, 100, 25));

    // --- WHEELS Placement ---
    pitchBendSlider.setBounds (sRect (25, 60, 25, 360));
    modWheelSlider.setBounds (sRect (55, 60, 25, 360));
    pitchBendLabel.setBounds (sRect (15, 35, 45, 20));
    modWheelLabel.setBounds (sRect (50, 35, 45, 20));

    // --- POLY MOD Placement ---
    int pmX = 100, pmY = 15;
    polyModEnvAmtSlider.setBounds (sRect (pmX + 25, pmY + 42, 55, 55));
    polyModOscBAmtSlider.setBounds (sRect (pmX + 95, pmY + 42, 55, 55));
    polyModDestFreqAButton.setBounds (sRect (pmX + 20, pmY + 102, 70, 25));
    polyModDestPwAButton.setBounds (sRect (pmX + 97, pmY + 102, 70, 25));
    polyModDestVcfButton.setBounds (sRect (pmX + 174, pmY + 102, 70, 25));

    // --- LFO Placement ---
    int lfoX = 100, lfoY = 155;
    lfoRateSlider.setBounds (sRect (lfoX + 15, lfoY + 45, 55, 55));
    lfoInitialAmountSlider.setBounds (sRect (lfoX + 85, lfoY + 45, 55, 55));
    lfoShapeLabel.setBounds (sRect (lfoX + 150, lfoY + 22, 100, 15));
    lfoTriButton.setBounds (sRect (lfoX + 150, lfoY + 42, 32, 45));
    lfoSquButton.setBounds (sRect (lfoX + 185, lfoY + 42, 32, 45));
    lfoSawButton.setBounds (sRect (lfoX + 220, lfoY + 42, 32, 45));

    // --- WHEEL-MOD Placement ---
    int wmX = 100, wmY = 295;
    wheelModSourceMixSlider.setBounds (sRect (wmX + 15, wmY + 55, 55, 55));
    wheelModDestFreqAButton.setBounds (sRect (wmX + 85, wmY + 40, 80, 25));
    wheelModDestFreqBButton.setBounds (sRect (wmX + 170, wmY + 40, 80, 25));
    wheelModDestPwAButton.setBounds (sRect (wmX + 85, wmY + 80, 50, 25));
    wheelModDestPwBButton.setBounds (sRect (wmX + 140, wmY + 80, 50, 25));
    wheelModDestFilterButton.setBounds (sRect (wmX + 195, wmY + 80, 60, 25));

    // --- OSC A Placement ---
    int oscAX = 375, oscAY = 15;
    // 上段：左列に Sync ボタンを配置し、中列・右列に Freq A と Pw A ノブを配置
    oscSyncButton.setBounds (sRect (oscAX + 20, oscAY + 68, 60, 40));
    oscAFreqSlider.setBounds (sRect (oscAX + 95, oscAY + 55, 65, 65));
    oscAPwSlider.setBounds (sRect (oscAX + 175, oscAY + 55, 65, 65));
    // 下段：中列・右列に波形選択ボタン（Saw, Square）を配置（Sync との混同を防ぐ）
    oscASawButton.setBounds (sRect (oscAX + 97, oscAY + 138, 60, 40));
    oscASqrButton.setBounds (sRect (oscAX + 177, oscAY + 138, 60, 40));

    // --- OSC B Placement ---
    int oscBX = 375, oscBY = 225;
    oscBFreqSlider.setBounds (sRect (oscBX + 20, oscBY + 50, 60, 60));
    oscBFineSlider.setBounds (sRect (oscBX + 90, oscBY + 50, 60, 60));
    oscBPwSlider.setBounds (sRect (oscBX + 160, oscBY + 50, 60, 60));
    oscBSawButton.setBounds (sRect (oscBX + 20, oscBY + 122, 60, 40));
    oscBSqrButton.setBounds (sRect (oscBX + 90, oscBY + 122, 60, 40));
    oscBTriButton.setBounds (sRect (oscBX + 160, oscBY + 122, 60, 40));
    oscBLoModeButton.setBounds (sRect (oscBX + 70, oscBY + 172, 125, 30));

    // --- OSC MIX Placement ---
    int mixX = 650, mixY = 15;
    mixOscASlider.setBounds (sRect (mixX + 25, mixY + 42, 55, 55));
    mixOscBSlider.setBounds (sRect (mixX + 95, mixY + 42, 55, 55));
    mixNoiseSlider.setBounds (sRect (mixX + 165, mixY + 42, 55, 55));

    // --- FILTER Placement ---
    int vcfX = 650, vcfY = 135;
    cutoffSlider.setBounds (sRect (vcfX + 25, vcfY + 45, 60, 60));
    resonanceSlider.setBounds (sRect (vcfX + 95, vcfY + 45, 60, 60));
    vcfEnvAmtSlider.setBounds (sRect (vcfX + 165, vcfY + 45, 60, 60));

    envAAtkSlider.setBounds (sRect (vcfX + 15, vcfY + 145, 50, 50));
    envADcySlider.setBounds (sRect (vcfX + 75, vcfY + 145, 50, 50));
    envASusSlider.setBounds (sRect (vcfX + 135, vcfY + 145, 50, 50));
    envARelSlider.setBounds (sRect (vcfX + 195, vcfY + 145, 50, 50));

    vcfKbTrackLabel.setBounds (sRect (vcfX + 10, vcfY + 210, 245, 15));
    kbTrackOffButton.setBounds (sRect (vcfX + 25, vcfY + 228, 60, 35));
    kbTrackHalfButton.setBounds (sRect (vcfX + 95, vcfY + 228, 60, 35));
    kbTrackFullButton.setBounds (sRect (vcfX + 165, vcfY + 228, 60, 35));

    // --- VCA ENV Placement ---
    int vcaX = 925, vcaY = 15;
    envBAtkSlider.setBounds (sRect (vcaX + 15, vcaY + 45, 50, 50));
    envBDcySlider.setBounds (sRect (vcaX + 75, vcaY + 45, 50, 50));
    envBSusSlider.setBounds (sRect (vcaX + 135, vcaY + 45, 50, 50));
    envBRelSlider.setBounds (sRect (vcaX + 195, vcaY + 45, 50, 50));

    // --- GLOBAL Placement ---
    int glX = 925, glY = 155;
    glideSlider.setBounds (sRect (glX + 15, glY + 45, 50, 50));
    masterTuneSlider.setBounds (sRect (glX + 75, glY + 45, 50, 50));
    pbRangeSlider.setBounds (sRect (glX + 135, glY + 45, 50, 50));
    masterVolumeSlider.setBounds (sRect (glX + 195, glY + 45, 50, 50));

    unisonButton.setBounds (sRect (glX + 45, glY + 112, 75, 40));
    holdButton.setBounds (sRect (glX + 135, glY + 112, 75, 40));

    velocityToFilterButton.setBounds (sRect (glX + 45, glY + 202, 75, 40));
    velocityToAmpButton.setBounds (sRect (glX + 135, glY + 202, 75, 40));
}

void Prop5Editor::timerCallback()
{
    // Sync UI preset dropdown with the processor's current program
    int activeProgram = audioProcessor.getCurrentProgram();
    if (presetCombo.getSelectedId() != activeProgram + 1)
    {
        presetCombo.setSelectedId (activeProgram + 1, juce::dontSendNotification);
    }

    // Sync LFO Wave selection
    if (auto* lfoParam = dynamic_cast<juce::AudioParameterChoice*>(audioProcessor.apvts.getParameter ("lfo_wave")))
    {
        int idx = lfoParam->getIndex();
        lfoTriButton.setToggleState (idx == 0, juce::dontSendNotification);
        lfoSquButton.setToggleState (idx == 1, juce::dontSendNotification);
        lfoSawButton.setToggleState (idx == 2, juce::dontSendNotification);
    }

    // Sync VCF KB Track selection
    if (auto* kbParam = dynamic_cast<juce::AudioParameterChoice*>(audioProcessor.apvts.getParameter ("vcf_kb_track")))
    {
        int idx = kbParam->getIndex();
        kbTrackOffButton.setToggleState (idx == 0, juce::dontSendNotification);
        kbTrackHalfButton.setToggleState (idx == 1, juce::dontSendNotification);
        kbTrackFullButton.setToggleState (idx == 2, juce::dontSendNotification);
    }
}

void Prop5Editor::updatePresetComboBoxItems()
{
    presetCombo.clear();
    
    // Factory presets
    for (int i = 0; i < 13; ++i)
    {
        presetCombo.addItem (audioProcessor.getProgramName (i), i + 1);
    }
    
    // Separator
    presetCombo.addSeparator();
    
    // User presets
    for (int i = 0; i < audioProcessor.userPresetFiles.size(); ++i)
    {
        int progIndex = 13 + i;
        presetCombo.addItem (audioProcessor.getProgramName (progIndex), progIndex + 1);
    }
}