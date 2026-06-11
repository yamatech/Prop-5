#pragma once

#include <JuceHeader.h>
#include "Oscillator.h"
#include "Envelope.h"
#include "LadderFilter.h"

// ==============================================================================
// Prop5Sound クラス
// ==============================================================================
class Prop5Sound : public juce::SynthesiserSound
{
public:
    Prop5Sound() {}
    ~Prop5Sound() override {}
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};

// ==============================================================================
// Prop5Voice クラス
// ==============================================================================
class Prop5Voice : public juce::SynthesiserVoice
{
public:
    Prop5Voice();
    ~Prop5Voice() override;

    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    void prepare(double sampleRate, int samplesPerBlock, int numOutputChannels);
    void updateParameters(const juce::AudioProcessorValueTreeState& apvts);

private:
    double currentSampleRate = 44100.0;
    juce::Random randomGen;

    // --- DSP Modules ---
    Prop5Oscillator oscA;
    Prop5Oscillator oscB;
    Prop5Noise noise;
    Prop5LadderFilter filter;
    Prop5Envelope envA;   // Filter Envelope
    Prop5Envelope envB;   // Amp Envelope

    // --- LFO state ---
    float lfoPhase = 0.0f;
    float lfoRate = 5.0f;
    int lfoWaveform = 0; // 0: Tri, 1: Sqr, 2: Saw
    float lfoInitialAmount = 0.0f;

    // --- Wheel Mod state ---
    float wheelModSourceMix = 0.0f;
    bool wheelModDestFreqA = false;
    bool wheelModDestFreqB = false;
    bool wheelModDestPwA = false;
    bool wheelModDestPwB = false;
    bool wheelModDestFilter = false;

    float currentModWheelValue = 0.0f;

    // --- Poly Mod state ---
    float polyModEnvAmt = 0.0f;
    float polyModOscBAmt = 0.0f;
    bool polyModDestFreqA = false;
    bool polyModDestPwA = false;
    bool polyModDestVcf = false;

    // --- Parameters ---
    float oscAFreqRatio = 1.0f;
    float oscAPw = 50.0f;
    bool oscASaw = true;
    bool oscASqr = false;

    float oscBFreqRatio = 1.0f;
    float oscBFine = 0.0f;
    float oscBPw = 50.0f;
    bool oscBSaw = false;
    bool oscBSqr = false;
    bool oscBTri = false;
    bool oscBLoMode = false;
    bool oscSync = false;

    float mixOscA = 0.75f;
    float mixOscB = 0.0f;
    float mixNoise = 0.0f;
    float masterVolume = 0.8f;
    float masterTune = 0.0f;
    int pitchBendRange = 2;
    bool unison = false;

    // Filter params
    float baseCutoff = 20000.0f;
    float baseResonance = 0.0f;
    float vcfEnvAmt = 0.5f;
    int vcfKbTrack = 0; // 0: OFF, 1: HALF, 2: FULL

    // VCA mode
    bool vcaMode = false; // true: Gate mode

    // Pitch state
    int currentMidiNote = -1;
    float currentPitchWheelOffset = 1.0f;

    // Velocity state
    float currentVelocity = 1.0f;
    bool velocityToAmp = false;
    bool velocityToFilter = false;

    // Glide state
    float glideTimeMs = 0.0f;
    float currentGlideNote = 60.0f;
    float targetGlideNote = 60.0f;
    bool isFirstNote = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Prop5Voice)
};