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
  // 仕様書の 0〜100% を 0.0〜1.0 に正規化
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

  // 1. ノコギリ波 (Sawtooth)
  if (sawEnabled)
  {
    // 0 〜 2π の位相を -1.0 〜 1.0 にマッピング
    output += (phase / juce::MathConstants<float>::pi) - 1.0f;
    activeWaveforms++;
  }

  // 2. 矩形波 (Square / Pulse)
  if (sqrEnabled)
  {
    // 位相が Pulse Width の閾値以下なら 1.0、超えれば -1.0
    float pwPhase = pulseWidth * juce::MathConstants<float>::twoPi;
    output += (phase < pwPhase) ? 1.0f : -1.0f;
    activeWaveforms++;
  }

  // 3. 三角波 (Triangle) - OSC B用
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

  // 複数波形がオンの場合は音量が割れないように平均化
  if (activeWaveforms > 1)
  {
    output /= static_cast<float>(activeWaveforms);
  }

  // 位相を進める
  phase += phaseIncrement;
  if (phase >= juce::MathConstants<float>::twoPi)
  {
    phase -= juce::MathConstants<float>::twoPi;
  }

  lastSample = output;

  return output;
}