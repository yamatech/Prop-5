#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <algorithm>

//==============================================================================
Prop5Editor::Prop5Editor (Prop5Processor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
      settingsOverlay (p,
          [this] (double scale) {
              audioProcessor.setStoredWindowScale (scale);
              setSize (juce::roundToInt (1200 * scale), juce::roundToInt (560 * scale));
              settingsOverlay.updateSizeComboSelection();
          },
          [this] {
              updatePresetComboBoxItems();
          })
{
    setLookAndFeel (&lookAndFeel);

    // Set window size based on stored scale (default is 1.15x of 1200x560)
    double initialScale = audioProcessor.getStoredWindowScale();
    setSize (juce::roundToInt (1200 * initialScale), juce::roundToInt (560 * initialScale));

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
        int newProgram = activeProgram;
        while (newProgram > 0)
        {
            newProgram--;
            if (audioProcessor.isProgramActive (newProgram))
            {
                audioProcessor.setCurrentProgram (newProgram);
                break;
            }
        }
    };

    nextPresetButton.setButtonText (">");
    addAndMakeVisible (nextPresetButton);
    nextPresetButton.onClick = [this]
    {
        int activeProgram = audioProcessor.getCurrentProgram();
        int newProgram = activeProgram;
        int numProgs = audioProcessor.getNumPrograms();
        while (newProgram < numProgs - 1)
        {
            newProgram++;
            if (audioProcessor.isProgramActive (newProgram))
            {
                audioProcessor.setCurrentProgram (newProgram);
                break;
            }
        }
    };

    initButton.setButtonText ("RESET");
    addAndMakeVisible (initButton);
    initButton.onClick = [this]
    {
        audioProcessor.resetCurrentProgram();
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
                        int factorySize = static_cast<int>(audioProcessor.factoryPresets.size());
                        int progIdx = factorySize + newIndex;
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
                            int factorySize = static_cast<int>(audioProcessor.factoryPresets.size());
                            int progIdx = factorySize + fileIndex;
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

    // SETTINGS button
    settingsButton.setButtonText ("SETTINGS");
    addAndMakeVisible (settingsButton);
    settingsButton.onClick = [this]
    {
        settingsOverlay.updateFolderDisplay();
        settingsOverlay.setVisible (true);
        settingsOverlay.toFront (true);
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
    setupSlider (velocityToFilterSlider, velocityToFilterLabel, "FILT");
    setupSlider (velocityToAmpSlider, velocityToAmpLabel, "AMP");

    velocityToFilterAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "velocity_to_filter", velocityToFilterSlider);
    velocityToAmpAttach = std::make_unique<SliderAttach>(audioProcessor.apvts, "velocity_to_amp", velocityToAmpSlider);

    // ABOUT button
    aboutButton.setButtonText ("About");
    aboutButton.setColour (juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
    aboutButton.setColour (juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
    aboutButton.setColour (juce::TextButton::textColourOffId, juce::Colour (0xffeae6df).withAlpha (0.6f));
    aboutButton.setColour (juce::TextButton::textColourOnId, juce::Colour (0xffeae6df));
    addAndMakeVisible (aboutButton);
    aboutButton.onClick = [this]
    {
        aboutOverlay.setVisible (true);
        aboutOverlay.toFront (true);
    };

    // Dialog overlays
    addChildComponent (settingsOverlay);
    addChildComponent (aboutOverlay);
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
    g.fillRect (0, 0, 10, 560);

    juce::ColourGradient rightWood (juce::Colour (0xff361c08), 1190.0f, 0.0f,
                                    juce::Colour (0xff6a3b16), 1200.0f, 0.0f, false);
    g.setGradientFill (rightWood);
    g.fillRect (1190, 0, 10, 560);

    juce::ColourGradient bottomWood (juce::Colour (0xff7a431c), 0.0f, 500.0f,
                                     juce::Colour (0xff321808), 0.0f, 560.0f, false);
    g.setGradientFill (bottomWood);
    g.fillRect (0, 500, 1200, 60);

    // 木目テクスチャの細い横線をシミュレート
    for (int y = 503; y < 557; y += 4)
    {
        float alpha = 0.03f + 0.04f * std::sin (y * 0.5f);
        g.setColour (juce::Colour (0xff201005).withAlpha (alpha));
        g.fillRect (10, y, 1180, 1);
    }

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
    
    // VELOCITY
    drawSection (juce::Rectangle<int> (925, 155, 265, 110), "VELOCITY", titleGold);
    
    // GLOBAL
    drawSection (juce::Rectangle<int> (925, 275, 265, 160), "GLOBAL", titleGold);

    // PRESET BAR (白線の枠で囲む)
    drawSection (juce::Rectangle<int> (260, 445, 680, 45), "", titleWhite);

    // --- 左下の情報表示（木目パネルの上に描画） ---
    g.setColour (juce::Colour (0xffeae6df).withAlpha (0.5f));
    g.setFont (juce::Font ("Arial", 9.0f, juce::Font::plain));
    g.drawText ("POLYPHONIC SYNTHESIZER", 25, 512, 160, 15, juce::Justification::left);

    g.setColour (juce::Colour (0xffeae6df).withAlpha (0.7f));
    g.setFont (juce::Font ("Arial", 10.0f, juce::Font::plain));
    g.drawText ("VERSION 0.9.6", 25, 532, 160, 15, juce::Justification::left);

    // --- 右下のシルバーエンブレムプレート（ロゴ） ---
    juce::Rectangle<float> plateArea (1030.0f, 510.0f, 145.0f, 40.0f);
    float plateCornerSize = 4.0f;

    // 1. プレートの凹み影（木目に埋め込まれた立体感を表現）
    g.setColour (juce::Colour (0xffaa6633).withAlpha (0.4f));
    g.fillRoundedRectangle (plateArea.translated (0.0f, 1.0f), plateCornerSize);
    g.setColour (juce::Colour (0xff150b05));
    g.drawRoundedRectangle (plateArea.translated (-0.5f, -0.5f), plateCornerSize, 1.0f);

    // 2. メタリックシルバー背景グラデーション
    juce::ColourGradient metalGradient (
        juce::Colour (0xfff2f2f4), plateArea.getX(), plateArea.getY(),
        juce::Colour (0xffbcbcc0), plateArea.getRight(), plateArea.getBottom(), false);
    g.setGradientFill (metalGradient);
    g.fillRoundedRectangle (plateArea, plateCornerSize);

    // 3. プレートの内枠線と立体感（ベベル効果）
    g.setColour (juce::Colour (0xffffffff).withAlpha (0.7f));
    g.drawRoundedRectangle (plateArea, plateCornerSize, 0.5f);
    g.setColour (juce::Colour (0xff8c8c90));
    g.drawRoundedRectangle (plateArea.translated (0.5f, 0.5f), plateCornerSize, 0.5f);

    // 暗いグレーの境界線
    g.setColour (juce::Colour (0xff4a4a4e));
    g.drawRoundedRectangle (plateArea, plateCornerSize, 1.0f);

    // 4. ロゴテキスト "Prop-5" の描画
    // フォントは Arial Black または Arial Bold を横に引き伸ばす
    juce::Font logoFont (juce::Font::getDefaultSerifFontName(), 24.0f, juce::Font::bold);
    logoFont.setHorizontalScale (1.30f); // 横長にして Eurostile 風にする
    g.setFont (logoFont);

    // 立体感を出すためのシャドウ
    g.setColour (juce::Colour (0xffffffff).withAlpha (0.7f));
    g.drawText ("Prop-5", plateArea.translated (0.5f, 1.0f), juce::Justification::centred);

    // メインテキスト
    g.setColour (juce::Colour (0xff1f1f23));
    g.drawText ("Prop-5", plateArea, juce::Justification::centred);
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
    presetLabel.setBounds (sRect (260 + 45, prY + 10, 60, 25));
    prevPresetButton.setBounds (sRect (260 + 115, prY + 10, 30, 25));
    presetCombo.setBounds (sRect (260 + 155, prY + 10, 220, 25));
    nextPresetButton.setBounds (sRect (260 + 385, prY + 10, 30, 25));
    initButton.setBounds (sRect (260 + 435, prY + 10, 60, 25));
    saveButton.setBounds (sRect (260 + 505, prY + 10, 60, 25));
    loadButton.setBounds (sRect (260 + 575, prY + 10, 60, 25));
    settingsButton.setBounds (sRect (1095, prY + 8, 80, 28));

    // About Button Placement
    aboutButton.setBounds (sRect (105, 530, 40, 18));

    // Overlays cover the whole screen
    settingsOverlay.setBounds (sRect (0, 0, 1200, 560));
    aboutOverlay.setBounds (sRect (0, 0, 1200, 560));

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

    // --- VELOCITY Placement ---
    int velX = 925, velY = 155;
    velocityToFilterSlider.setBounds (sRect (velX + 45, velY + 45, 50, 50));
    velocityToAmpSlider.setBounds (sRect (velX + 135, velY + 45, 50, 50));

    // --- GLOBAL Placement ---
    int glX = 925, glY = 275;
    glideSlider.setBounds (sRect (glX + 15, glY + 45, 50, 50));
    masterTuneSlider.setBounds (sRect (glX + 75, glY + 45, 50, 50));
    pbRangeSlider.setBounds (sRect (glX + 135, glY + 45, 50, 50));
    masterVolumeSlider.setBounds (sRect (glX + 195, glY + 45, 50, 50));

    unisonButton.setBounds (sRect (glX + 45, glY + 112, 75, 40));
    holdButton.setBounds (sRect (glX + 135, glY + 112, 75, 40));
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
    presetCombo.clear (juce::dontSendNotification);
    
    int factorySize = static_cast<int>(audioProcessor.factoryPresets.size());
    
    // 1. ComboBoxの内部検索用リストへの追加（setSelectedId用）
    for (int i = 0; i < factorySize; ++i)
    {
        if (audioProcessor.isProgramActive (i))
        {
            presetCombo.addItem (audioProcessor.getProgramName (i), i + 1);
        }
    }
    
    for (int i = 0; i < audioProcessor.userPresetFiles.size(); ++i)
    {
        int progIndex = factorySize + i;
        presetCombo.addItem (audioProcessor.getProgramName (progIndex), progIndex + 1);
    }
    
    // 2. 表示用ポップアップメニューのカスタマイズ（サブメニュー化）
    if (auto* rootMenu = presetCombo.getRootMenu())
    {
        rootMenu->clear();
        
        // カテゴリーごとのPopupMenuを格納する構造体
        struct CategoryMenu
        {
            juce::String name;
            juce::PopupMenu menu;
        };
        std::vector<CategoryMenu> categories;
        
        auto getOrCreateMenu = [&](const juce::String& name) -> juce::PopupMenu& {
            for (auto& cat : categories)
            {
                if (cat.name == name)
                    return cat.menu;
            }
            categories.push_back ({ name, juce::PopupMenu() });
            return categories.back().menu;
        };
        
        // ファクトリープリセットをカテゴリーごとに分類して追加
        for (int i = 0; i < factorySize; ++i)
        {
            if (audioProcessor.isProgramActive (i))
            {
                const auto& preset = audioProcessor.factoryPresets[i];
                juce::String categoryName = preset.category.isNotEmpty() ? preset.category : "Others";
                if (categoryName == "Reserved")
                    continue;
                auto& catMenu = getOrCreateMenu (categoryName);
                catMenu.addItem (i + 1, preset.name);
            }
        }
        
        // ルートメニューにファクトリープリセットのサブメニューを追加
        std::sort (categories.begin(), categories.end(), [](const CategoryMenu& a, const CategoryMenu& b) {
            return a.name < b.name;
        });

        for (auto& cat : categories)
        {
            rootMenu->addSubMenu (cat.name, cat.menu);
        }
        
        // ユーザープリセットがある場合は「User Presets」サブメニューを追加
        if (audioProcessor.userPresetFiles.size() > 0)
        {
            rootMenu->addSeparator();
            
            juce::PopupMenu userMenu;
            for (int i = 0; i < audioProcessor.userPresetFiles.size(); ++i)
            {
                int progIndex = factorySize + i;
                userMenu.addItem (progIndex + 1, audioProcessor.getProgramName (progIndex));
            }
            rootMenu->addSubMenu ("User Presets", userMenu);
        }
    }
}

//==============================================================================
SettingsOverlay::SettingsOverlay (Prop5Processor& p, std::function<void(double)> onScaleChanged, std::function<void()> onFolderChanged)
    : processor (p), scaleCallback (onScaleChanged), folderCallback (onFolderChanged)
{
    // 閉じるボタン
    closeButton.setButtonText ("X");
    closeButton.onClick = [this] { setVisible (false); };
    addAndMakeVisible (closeButton);

    // タイトル
    titleLabel.setText ("SETTINGS", juce::dontSendNotification);
    titleLabel.setFont (juce::Font (18.0f, juce::Font::bold));
    titleLabel.setJustificationType (juce::Justification::left);
    titleLabel.setColour (juce::Label::textColourId, juce::Colour (0xffeae6df));
    addAndMakeVisible (titleLabel);

    // Preset Folder 項目
    folderSectionLabel.setText ("Preset Folder:", juce::dontSendNotification);
    folderSectionLabel.setFont (juce::Font (13.0f, juce::Font::bold));
    folderSectionLabel.setColour (juce::Label::textColourId, juce::Colour (0xffeae6df).withAlpha (0.8f));
    addAndMakeVisible (folderSectionLabel);

    folderPathLabel.setColour (juce::Label::backgroundColourId, juce::Colour (0xff0f0f11));
    folderPathLabel.setColour (juce::Label::outlineColourId, juce::Colour (0xff3a3a40));
    folderPathLabel.setColour (juce::Label::textColourId, juce::Colour (0xffeae6df).withAlpha (0.7f));
    folderPathLabel.setFont (juce::Font (11.0f));
    addAndMakeVisible (folderPathLabel);
    updateFolderDisplay();

    selectFolderButton.setButtonText ("Select Folder");
    selectFolderButton.onClick = [this] { selectFolder(); };
    addAndMakeVisible (selectFolderButton);

    defaultFolderButton.setButtonText ("Default");
    defaultFolderButton.onClick = [this] { resetToDefaultFolder(); };
    addAndMakeVisible (defaultFolderButton);

    // Window size 項目
    sizeSectionLabel.setText ("Window Size:", juce::dontSendNotification);
    sizeSectionLabel.setFont (juce::Font (13.0f, juce::Font::bold));
    sizeSectionLabel.setColour (juce::Label::textColourId, juce::Colour (0xffeae6df).withAlpha (0.8f));
    addAndMakeVisible (sizeSectionLabel);

    sizeCombo.addItem ("80%", 1);
    sizeCombo.addItem ("100%", 2);
    sizeCombo.addItem ("115%", 3);
    sizeCombo.addItem ("130%", 4);
    sizeCombo.addItem ("150%", 5);
    sizeCombo.addItem ("175%", 6);
    sizeCombo.addItem ("200%", 7);
    addAndMakeVisible (sizeCombo);

    updateSizeComboSelection();

    sizeCombo.onChange = [this] { changeScale(); };
}

void SettingsOverlay::updateFolderDisplay()
{
    juce::File currentFolder = processor.getCurrentPresetFolder();
    folderPathLabel.setText (currentFolder.getFullPathName(), juce::dontSendNotification);
}

void SettingsOverlay::updateSizeComboSelection()
{
    double initialScale = processor.getStoredWindowScale();
    int selectedId = 3;
    if (juce::exactlyEqual (initialScale, 0.80))       selectedId = 1;
    else if (juce::exactlyEqual (initialScale, 1.00))  selectedId = 2;
    else if (juce::exactlyEqual (initialScale, 1.15))  selectedId = 3;
    else if (juce::exactlyEqual (initialScale, 1.30))  selectedId = 4;
    else if (juce::exactlyEqual (initialScale, 1.50))  selectedId = 5;
    else if (juce::exactlyEqual (initialScale, 1.75))  selectedId = 6;
    else if (juce::exactlyEqual (initialScale, 2.00))  selectedId = 7;
    sizeCombo.setSelectedId (selectedId, juce::dontSendNotification);
}

void SettingsOverlay::mouseDown (const juce::MouseEvent& e)
{
    float scale = getWidth() / 1200.0f;
    auto s = [scale] (float val) -> float { return val * scale; };
    
    juce::Rectangle<int> dialogBounds = juce::Rectangle<float> (s(350.0f), s(130.0f), s(500.0f), s(300.0f)).toNearestInt();

    if (!dialogBounds.contains (e.getPosition()))
    {
        setVisible (false);
    }
}

void SettingsOverlay::paint (juce::Graphics& g)
{
    float scale = getWidth() / 1200.0f;
    auto s = [scale] (float val) -> float { return val * scale; };

    g.fillAll (juce::Colours::black.withAlpha (0.6f));

    juce::Rectangle<float> dialogBounds (s(350.0f), s(130.0f), s(500.0f), s(300.0f));
    
    g.setColour (juce::Colour (0xff1f1f23));
    g.fillRoundedRectangle (dialogBounds, s(8.0f));

    g.setColour (juce::Colour (0xffffffff).withAlpha (0.4f));
    g.drawRoundedRectangle (dialogBounds.translated(0.0f, -0.5f), s(8.0f), s(1.0f));
    
    g.setColour (juce::Colour (0xff8c8c90));
    g.drawRoundedRectangle (dialogBounds.translated(0.5f, 0.5f), s(8.0f), s(1.0f));

    g.setColour (juce::Colour (0xff4a4a4e));
    g.drawRoundedRectangle (dialogBounds, s(8.0f), s(1.5f));
}

void SettingsOverlay::resized()
{
    float scale = getWidth() / 1200.0f;
    auto s = [scale] (int val) -> int { return juce::roundToInt (val * scale); };
    auto sRect = [s] (int x, int y, int w, int h) -> juce::Rectangle<int>
    {
        return { s(x), s(y), s(w), s(h) };
    };

    int dx = 350, dy = 130;

    titleLabel.setBounds (sRect (dx + 25, dy + 20, 200, 30));
    closeButton.setBounds (sRect (dx + 445, dy + 20, 30, 30));

    folderSectionLabel.setBounds (sRect (dx + 25, dy + 70, 200, 20));
    folderPathLabel.setBounds (sRect (dx + 25, dy + 95, 450, 25));
    selectFolderButton.setBounds (sRect (dx + 25, dy + 130, 120, 30));
    defaultFolderButton.setBounds (sRect (dx + 160, dy + 130, 80, 30));

    sizeSectionLabel.setBounds (sRect (dx + 25, dy + 180, 120, 30));
    sizeCombo.setBounds (sRect (dx + 160, dy + 180, 120, 30));
}

void SettingsOverlay::selectFolder()
{
    fileChooser = std::make_unique<juce::FileChooser> (
        "Select Preset Folder",
        processor.getCurrentPresetFolder(),
        ""
    );
    
    fileChooser->launchAsync (juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories,
        [this] (const juce::FileChooser& fc)
        {
            auto folder = fc.getResult();
            if (folder != juce::File{} && folder.isDirectory())
            {
                processor.setCurrentPresetFolder (folder);
                updateFolderDisplay();
                if (folderCallback)
                    folderCallback();
            }
        });
}

void SettingsOverlay::resetToDefaultFolder()
{
    processor.setCurrentPresetFolder (processor.getDefaultPresetFolder());
    updateFolderDisplay();
    if (folderCallback)
        folderCallback();
}

void SettingsOverlay::changeScale()
{
    double scale = 1.15;
    switch (sizeCombo.getSelectedId())
    {
        case 1: scale = 0.80; break;
        case 2: scale = 1.00; break;
        case 3: scale = 1.15; break;
        case 4: scale = 1.30; break;
        case 5: scale = 1.50; break;
        case 6: scale = 1.75; break;
        case 7: scale = 2.00; break;
    }
    if (scaleCallback)
        scaleCallback (scale);
}

//==============================================================================
AboutOverlay::AboutOverlay()
{
    closeButton.setButtonText ("X");
    closeButton.onClick = [this] { setVisible (false); };
    addAndMakeVisible (closeButton);

    titleLabel.setText ("ABOUT", juce::dontSendNotification);
    titleLabel.setFont (juce::Font (18.0f, juce::Font::bold));
    titleLabel.setJustificationType (juce::Justification::left);
    titleLabel.setColour (juce::Label::textColourId, juce::Colour (0xffeae6df));
    addAndMakeVisible (titleLabel);

    logoLabel.setText ("Prop-5", juce::dontSendNotification);
    juce::Font logoFont (juce::Font::getDefaultSerifFontName(), 36.0f, juce::Font::bold);
    logoFont.setHorizontalScale (1.30f);
    logoLabel.setFont (logoFont);
    logoLabel.setJustificationType (juce::Justification::centred);
    logoLabel.setColour (juce::Label::textColourId, juce::Colour (0xffeae6df));
    addAndMakeVisible (logoLabel);

    juce::String infoText;
    infoText << "Polyphonic Synthesizer\n"
             << "Version 0.9.6\n\n"
             << "Developed by yamatech\n"
             << "Copyright (C) 2026 yamatech. All rights reserved.\n\n"
             << "Released under the GNU GPL v3 License.\n\n"
             << "Based on JUCE Framework.";
    infoLabel.setText (infoText, juce::dontSendNotification);
    infoLabel.setFont (juce::Font (12.0f, juce::Font::plain));
    infoLabel.setJustificationType (juce::Justification::centred);
    infoLabel.setColour (juce::Label::textColourId, juce::Colour (0xffeae6df).withAlpha (0.8f));
    addAndMakeVisible (infoLabel);
}

void AboutOverlay::mouseDown (const juce::MouseEvent& e)
{
    float scale = getWidth() / 1200.0f;
    auto s = [scale] (float val) -> float { return val * scale; };
    
    juce::Rectangle<int> dialogBounds = juce::Rectangle<float> (s(375.0f), s(150.0f), s(450.0f), s(260.0f)).toNearestInt();

    if (!dialogBounds.contains (e.getPosition()))
    {
        setVisible (false);
    }
}

void AboutOverlay::paint (juce::Graphics& g)
{
    float scale = getWidth() / 1200.0f;
    auto s = [scale] (float val) -> float { return val * scale; };

    g.fillAll (juce::Colours::black.withAlpha (0.6f));

    juce::Rectangle<float> dialogBounds (s(375.0f), s(150.0f), s(450.0f), s(260.0f));
    
    g.setColour (juce::Colour (0xff1f1f23));
    g.fillRoundedRectangle (dialogBounds, s(8.0f));

    g.setColour (juce::Colour (0xffffffff).withAlpha (0.4f));
    g.drawRoundedRectangle (dialogBounds.translated(0.0f, -0.5f), s(8.0f), s(1.0f));
    
    g.setColour (juce::Colour (0xff8c8c90));
    g.drawRoundedRectangle (dialogBounds.translated(0.5f, 0.5f), s(8.0f), s(1.0f));

    g.setColour (juce::Colour (0xff4a4a4e));
    g.drawRoundedRectangle (dialogBounds, s(8.0f), s(1.5f));
}

void AboutOverlay::resized()
{
    float scale = getWidth() / 1200.0f;
    auto s = [scale] (int val) -> int { return juce::roundToInt (val * scale); };
    auto sRect = [s] (int x, int y, int w, int h) -> juce::Rectangle<int>
    {
        return { s(x), s(y), s(w), s(h) };
    };

    int dx = 375, dy = 150;

    titleLabel.setBounds (sRect (dx + 25, dy + 20, 200, 30));
    closeButton.setBounds (sRect (dx + 395, dy + 20, 30, 30));

    logoLabel.setBounds (sRect (dx + 50, dy + 60, 350, 45));
    infoLabel.setBounds (sRect (dx + 50, dy + 115, 350, 120));
}