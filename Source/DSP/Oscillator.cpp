#include "Oscillator.h"

Prop5Oscillator::Prop5Oscillator() {}

void Prop5Oscillator::prepare(double sampleRate)
{
  currentSampleRate = sampleRate;
  updateIncrement();
  resetPhase();
}

void Prop5Oscillator::setFrequency(float freqInHz)
{
  if (frequency != freqInHz)
  {
    frequency = freqInHz;
    updateIncrement();
  }
}

void Prop5Oscillator::setPulseWidth(float pwPercentage)
{
  // Normalize 0-100% to 0.0-1.0 from specifications
  pulseWidth = juce::jlimit(0.0f, 1.0f, pwPercentage / 100.0f);
}

void Prop5Oscillator::updateIncrement()
{
  if (currentSampleRate > 0.0)
  {
    phaseIncrement = (frequency * juce::MathConstants<float>::twoPi) / (float)currentSampleRate;
  }
}

float Prop5Oscillator::processSample()
{
  float output = 0.0f;
  int activeWaveforms = 0;

  // 1. Sawtooth
  if (sawEnabled)
  {
    // Map the phase of 0 to 2pi to -1.0 to 1.0
    output += (phase / juce::MathConstants<float>::pi) - 1.0f;
    activeWaveforms++;
  }

  // 2. Square / Pulse
  if (sqrEnabled)
  {
    // 1.0 if the phase is below the Pulse Width threshold, -1.0 otherwise
    float pwPhase = pulseWidth * juce::MathConstants<float>::twoPi;
    output += (phase < pwPhase) ? 1.0f : -1.0f;
    activeWaveforms++;
  }

  // 3. Triangle - for OSC B
  if (triEnabled)
  {
    float tri = 0.0f;
    if (phase < juce::MathConstants<float>::pi)
      tri = -1.0f + (2.0f / juce::MathConstants<float>::pi) * phase;
    else
      tri = 3.0f - (2.0f / juce::MathConstants<float>::pi) * phase;

    output += tri;
    activeWaveforms++;
  }

  // Average the output if multiple waveforms are active to prevent clipping
  if (activeWaveforms > 1)
  {
    output /= static_cast<float>(activeWaveforms);
  }

  // Advance phase
  phase += phaseIncrement;
  if (phase >= juce::MathConstants<float>::twoPi)
  {
    phase -= juce::MathConstants<float>::twoPi;
  }

  lastSample = output;

  return output;
}