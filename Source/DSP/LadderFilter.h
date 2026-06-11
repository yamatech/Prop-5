#pragma once
#include <JuceHeader.h>

class Prop5LadderFilter
{
public:
  Prop5LadderFilter();
  ~Prop5LadderFilter() = default;

  // 初期化
  void prepare(double sampleRate);

  // APVTSからのパラメータ設定
  void setParameters(float cutoffHz, float resonance, float envAmount, int kbTrackMode);

  // 1サンプル処理。エンベロープの値とMIDIノートを受け取ってカットオフを動かします
  float processSample(float inputSample, float envValue, float midiNote);

private:
  juce::dsp::LadderFilter<float> filter;

  double currentSampleRate = 44100.0;
  float baseCutoff = 1000.0f;
  float res = 0.0f;
  float envAmt = 0.0f;
  float kbTrack = 0.0f;
};