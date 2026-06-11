#pragma once

#include <JuceHeader.h>

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
    
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // APVTSへのアクセス用（Editor等から参照するため）
    juce::AudioProcessorValueTreeState apvts;

    // --- Preset Folder / Settings ---
    juce::File getCurrentPresetFolder();
    void setCurrentPresetFolder (const juce::File& newFolder);
    void scanPresets();
    
    juce::Array<juce::File> userPresetFiles;
    
    void savePresetToFile (const juce::File& file);
    bool loadPresetFromFile (const juce::File& file);

private:
    // APVTSのパラメーターを生成・登録する関数
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // シンセサイザーのコアエンジン（ここに5ボイスを追加します）
    juce::Synthesiser synth;

    int currentProgram { 0 };

    // グローバル設定管理
    std::unique_ptr<juce::PropertiesFile> properties;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Prop5Processor)
};