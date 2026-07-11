#pragma once

#include <JuceHeader.h>
#include "PresetData.h"
#include "DSP/Prop5Synthesiser.h"

class Prop5Processor : public juce::AudioProcessor
{
public:
    Prop5Processor();
    ~Prop5Processor() override;

    // --- AudioProcessor core ---
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    // --- Editor ---
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    // --- State/Preset Management (APVTS) ---
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    
    void resetCurrentProgram();
    bool isProgramActive (int index);
    
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // For accessing APVTS (to reference from Editor, etc.)
    juce::AudioProcessorValueTreeState apvts;

    // Managing the state of the MIDI keyboard
    juce::MidiKeyboardState keyboardState;

    // --- Preset Folder / Settings ---
    juce::File getCurrentPresetFolder();
    juce::File getDefaultPresetFolder();
    void setCurrentPresetFolder (const juce::File& newFolder);
    void scanPresets();
    
    juce::Array<juce::File> userPresetFiles;
    std::vector<Preset> factoryPresets;
    
    void savePresetToFile (const juce::File& file);
    bool loadPresetFromFile (const juce::File& file);

    double getStoredWindowScale();
    void setStoredWindowScale (double newScale);

private:
    // Function to generate and register APVTS parameters
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // Synthesizer core engine (adds 5 voices here)
    Prop5Synthesiser synth;

    int currentProgram { 0 };

    std::vector<Preset> programStates;
    void saveCurrentStateToMemory();
    void loadStateFromMemory (int index);

    // Global settings management
    std::unique_ptr<juce::PropertiesFile> properties;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Prop5Processor)
};