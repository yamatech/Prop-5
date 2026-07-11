#include "LadderFilter.h"

Prop5LadderFilter::Prop5LadderFilter()
{
  filter.setMode(juce::dsp::LadderFilter<float>::Mode::LPF24);
}

void Prop5LadderFilter::prepare(double sampleRate)
{
  currentSampleRate = sampleRate;

  juce::dsp::ProcessSpec spec;
  spec.sampleRate = sampleRate;
  spec.maximumBlockSize = 1;
  spec.numChannels = 1;

  filter.prepare(spec);
}

void Prop5LadderFilter::setParameters(float cutoffHz, float resonance, float envAmount, int kbTrackMode)
{
  baseCutoff = cutoffHz;
  res = juce::jlimit(0.0f, 1.0f, resonance);
  envAmt = envAmount;

  if (kbTrackMode == 1)
    kbTrack = 0.5f;
  else if (kbTrackMode == 2)
    kbTrack = 1.0f;
  else
    kbTrack = 0.0f;
}

float Prop5LadderFilter::processSample(float inputSample, float envValue, float midiNote)
{
  // Calculate parameters
  float envMod = envAmt * envValue * 8.0f;
  float kbMod = (midiNote - 60.0f) / 12.0f * kbTrack;
  float totalMod = envMod + kbMod;
  float currentCutoff = baseCutoff * std::pow(2.0f, totalMod);
  currentCutoff = juce::jlimit(20.0f, (float)currentSampleRate / 2.0f - 100.0f, currentCutoff);

  // Configure filter settings
  filter.setCutoffFrequencyHz(currentCutoff);
  filter.setResonance(res);

  float output = inputSample;
  // juce::dsp::AudioBlock expects an array of channel pointers. Create a small
  // temporary buffer and channel pointer array for single-sample, mono processing.
  float sampleBuffer[1] = { output };
  float* channels[1] = { sampleBuffer };
  juce::dsp::AudioBlock<float> block(channels, 1, 1);
  juce::dsp::ProcessContextReplacing<float> context(block);
  filter.process(context);

  return sampleBuffer[0] * 3.0f;
}