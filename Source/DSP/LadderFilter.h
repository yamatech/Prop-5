#pragma once
#include <JuceHeader.h>

class Prop5LadderFilter
{
public:
  Prop5LadderFilter();
  ~Prop5LadderFilter() = default;

  // Initialization
  void prepare(double sampleRate);

  // Set parameters from APVTS
  void setParameters(float cutoffHz, float resonance, float envAmount, int kbTrackMode);

  // Processes one sample. Receives envelope value and MIDI note to modulate the cutoff frequency.
  float processSample(float inputSample, float envValue, float midiNote);

private:
  juce::dsp::LadderFilter<float> filter;

  double currentSampleRate = 44100.0;
  float baseCutoff = 1000.0f;
  float res = 0.0f;
  float envAmt = 0.0f;
  float kbTrack = 0.0f;
};