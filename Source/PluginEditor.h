#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PropLookAndFeel.h"

//==============================================================================
class SettingsOverlay : public juce::Component
{
public:
    SettingsOverlay (Prop5Processor& p, std::function<void(double)> onScaleChanged, std::function<void()> onFolderChanged);
    ~SettingsOverlay() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;
    void updateFolderDisplay();
    void updateSizeComboSelection();
    void mouseDown (const juce::MouseEvent& e) override;

private:
    void selectFolder();
    void resetToDefaultFolder();
    void changeScale();

    Prop5Processor& processor;
    std::function<void(double)> scaleCallback;
    std::function<void()> folderCallback;

    juce::Label titleLabel;
    juce::TextButton closeButton;
    juce::Label folderSectionLabel;
    juce::Label folderPathLabel;
    juce::TextButton selectFolderButton;
    juce::TextButton defaultFolderButton;
    juce::Label sizeSectionLabel;
    juce::ComboBox sizeCombo;

    std::unique_ptr<juce::FileChooser> fileChooser;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsOverlay)
};

//==============================================================================
class AboutOverlay : public juce::Component
{
public:
    AboutOverlay();
    ~AboutOverlay() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& e) override;

private:
    juce::Label titleLabel;
    juce::TextButton closeButton;
    juce::Label logoLabel;
    juce::Label infoLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AboutOverlay)
};

//==============================================================================
class Prop5Editor : public juce::AudioProcessorEditor,
                    private juce::Timer
{
public:
    Prop5Editor (Prop5Processor&);
    ~Prop5Editor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    Prop5Processor& audioProcessor;

    // --- Preset Selection ---
    juce::ComboBox presetCombo;
    juce::Label presetLabel;
    juce::TextButton prevPresetButton;
    juce::TextButton nextPresetButton;
    juce::TextButton initButton;
    juce::TextButton saveButton;
    juce::TextButton loadButton;
    juce::TextButton settingsButton;
    void updatePresetComboBoxItems();
    std::unique_ptr<juce::FileChooser> fileChooser;

    // --- Performance Wheels ---
    juce::Slider pitchBendSlider;
    juce::Label pitchBendLabel;
    juce::Slider modWheelSlider;
    juce::Label modWheelLabel;

    // --- LFO ---
    juce::ToggleButton lfoTriButton;
    juce::ToggleButton lfoSquButton;
    juce::ToggleButton lfoSawButton;
    juce::Label lfoShapeLabel;
    juce::Slider lfoRateSlider;
    juce::Label lfoRateLabel;
    juce::Slider lfoInitialAmountSlider;
    juce::Label lfoInitialAmountLabel;

    // --- WHEEL-MOD ---
    juce::Slider wheelModSourceMixSlider;
    juce::Label wheelModSourceMixLabel;
    juce::ToggleButton wheelModDestFreqAButton;
    juce::ToggleButton wheelModDestFreqBButton;
    juce::ToggleButton wheelModDestPwAButton;
    juce::ToggleButton wheelModDestPwBButton;
    juce::ToggleButton wheelModDestFilterButton;

    // --- POLY MOD ---
    juce::Slider polyModEnvAmtSlider;
    juce::Label polyModEnvAmtLabel;
    juce::Slider polyModOscBAmtSlider;
    juce::Label polyModOscBAmtLabel;
    juce::ToggleButton polyModDestFreqAButton;
    juce::ToggleButton polyModDestPwAButton;
    juce::ToggleButton polyModDestVcfButton;

    // --- OSC A ---
    juce::Slider oscAFreqSlider;
    juce::Label oscAFreqLabel;
    juce::Slider oscAPwSlider;
    juce::Label oscAPwLabel;
    juce::ToggleButton oscASawButton;
    juce::ToggleButton oscASqrButton;

    // --- OSC B ---
    juce::Slider oscBFreqSlider;
    juce::Label oscBFreqLabel;
    juce::Slider oscBFineSlider;
    juce::Label oscBFineLabel;
    juce::Slider oscBPwSlider;
    juce::Label oscBPwLabel;
    juce::ToggleButton oscBSawButton;
    juce::ToggleButton oscBSqrButton;
    juce::ToggleButton oscBTriButton;
    juce::ToggleButton oscBLoModeButton;

    // --- OSC Sync ---
    juce::ToggleButton oscSyncButton;

    // --- Mixer / Global ---
    juce::Slider mixOscASlider;
    juce::Label mixOscALabel;
    juce::Slider mixOscBSlider;
    juce::Label mixOscBLabel;
    juce::Slider mixNoiseSlider;
    juce::Label mixNoiseLabel;
    juce::Slider masterVolumeSlider;
    juce::Label masterVolumeLabel;
    juce::Slider masterTuneSlider;
    juce::Label masterTuneLabel;
    juce::Slider pbRangeSlider;
    juce::Label pbRangeLabel;
    juce::Slider glideSlider;
    juce::Label glideLabel;
    juce::ToggleButton unisonButton;
    juce::ToggleButton holdButton;

    // --- VCF ---
    juce::Slider cutoffSlider;
    juce::Label cutoffLabel;
    juce::Slider resonanceSlider;
    juce::Label resonanceLabel;
    juce::Slider vcfEnvAmtSlider;
    juce::Label vcfEnvAmtLabel;
    juce::ToggleButton kbTrackOffButton;
    juce::ToggleButton kbTrackHalfButton;
    juce::ToggleButton kbTrackFullButton;
    juce::Label vcfKbTrackLabel;

    // --- Envelope A (Filter) ---
    juce::Slider envAAtkSlider;
    juce::Label envAAtkLabel;
    juce::Slider envADcySlider;
    juce::Label envADcyLabel;
    juce::Slider envASusSlider;
    juce::Label envASusLabel;
    juce::Slider envARelSlider;
    juce::Label envARelLabel;

    // --- Envelope B (Amp) ---
    juce::Slider envBAtkSlider;
    juce::Label envBAtkLabel;
    juce::Slider envBDcySlider;
    juce::Label envBDcyLabel;
    juce::Slider envBSusSlider;
    juce::Label envBSusLabel;
    juce::Slider envBRelSlider;
    juce::Label envBRelLabel;

    // --- Velocity ---
    juce::Slider velocityToAmpSlider;
    juce::Label velocityToAmpLabel;
    juce::Slider velocityToFilterSlider;
    juce::Label velocityToFilterLabel;

    // --- Attachments ---
    using SliderAttach = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttach = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using ComboAttach  = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    // Wheels & LFO & Wheel Mod Attachments
    std::unique_ptr<SliderAttach> pitchBendAttach;
    std::unique_ptr<SliderAttach> modWheelAttach;

    std::unique_ptr<SliderAttach> lfoRateAttach;
    std::unique_ptr<SliderAttach> lfoInitialAmountAttach;

    std::unique_ptr<SliderAttach> wheelModSourceMixAttach;
    std::unique_ptr<ButtonAttach> wheelModDestFreqAAttach;
    std::unique_ptr<ButtonAttach> wheelModDestFreqBAttach;
    std::unique_ptr<ButtonAttach> wheelModDestPwAAttach;
    std::unique_ptr<ButtonAttach> wheelModDestPwBAttach;
    std::unique_ptr<ButtonAttach> wheelModDestFilterAttach;

    // Poly Mod Attachments
    std::unique_ptr<SliderAttach> polyModEnvAmtAttach;
    std::unique_ptr<SliderAttach> polyModOscBAmtAttach;
    std::unique_ptr<ButtonAttach> polyModDestFreqAAttach;
    std::unique_ptr<ButtonAttach> polyModDestPwAAttach;
    std::unique_ptr<ButtonAttach> polyModDestVcfAttach;

    // OSC A Attachments
    std::unique_ptr<SliderAttach> oscAFreqAttach;
    std::unique_ptr<SliderAttach> oscAPwAttach;
    std::unique_ptr<ButtonAttach> oscASawAttach;
    std::unique_ptr<ButtonAttach> oscASqrAttach;

    // OSC B Attachments
    std::unique_ptr<SliderAttach> oscBFreqAttach;
    std::unique_ptr<SliderAttach> oscBFineAttach;
    std::unique_ptr<SliderAttach> oscBPwAttach;
    std::unique_ptr<ButtonAttach> oscBSawAttach;
    std::unique_ptr<ButtonAttach> oscBSqrAttach;
    std::unique_ptr<ButtonAttach> oscBTriAttach;
    std::unique_ptr<ButtonAttach> oscBLoModeAttach;

    // Sync Attachment
    std::unique_ptr<ButtonAttach> oscSyncAttach;

    // Mixer / Global Attachments
    std::unique_ptr<SliderAttach> mixOscAAttach;
    std::unique_ptr<SliderAttach> mixOscBAttach;
    std::unique_ptr<SliderAttach> mixNoiseAttach;
    std::unique_ptr<SliderAttach> masterVolumeAttach;
    std::unique_ptr<SliderAttach> masterTuneAttach;
    std::unique_ptr<SliderAttach> pbRangeAttach;
    std::unique_ptr<SliderAttach> glideAttach;
    std::unique_ptr<ButtonAttach> unisonAttach;
    std::unique_ptr<ButtonAttach> holdAttach;

    // VCF Attachments
    std::unique_ptr<SliderAttach> cutoffAttach;
    std::unique_ptr<SliderAttach> resonanceAttach;
    std::unique_ptr<SliderAttach> vcfEnvAmtAttach;

    // Env A Attachments
    std::unique_ptr<SliderAttach> envAAtkAttach;
    std::unique_ptr<SliderAttach> envADcyAttach;
    std::unique_ptr<SliderAttach> envASusAttach;
    std::unique_ptr<SliderAttach> envARelAttach;

    // Env B Attachments
    std::unique_ptr<SliderAttach> envBAtkAttach;
    std::unique_ptr<SliderAttach> envBDcyAttach;
    std::unique_ptr<SliderAttach> envBSusAttach;
    std::unique_ptr<SliderAttach> envBRelAttach;

    // Velocity Attachments
    std::unique_ptr<SliderAttach> velocityToAmpAttach;
    std::unique_ptr<SliderAttach> velocityToFilterAttach;

    // --- Window Resizing & Dialogs ---
    juce::TextButton aboutButton;
    SettingsOverlay settingsOverlay;
    AboutOverlay aboutOverlay;

    juce::MidiKeyboardComponent keyboardComponent;

    PropLookAndFeel lookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Prop5Editor)
};