#pragma once
#include <JuceHeader.h>

class Prop5Envelope
{
public:
  enum class State
  {
    Idle,
    Attack,
    Decay,
    Sustain,
    Release
  };

  Prop5Envelope();
  ~Prop5Envelope() = default;

  // サンプルレートの初期化
  void prepare(double sampleRate);

  // APVTSからのパラメーターを適用（Time系はミリ秒、Sustainは0.0〜1.0）
  void setParameters(float attackMs, float decayMs, float sustainLevel, float releaseMs);

  // ノートオン（鍵盤を押した時）とノートオフ（鍵盤を離した時）のトリガー
  void noteOn();
  void noteOff();

  // 1サンプルごとのエンベロープ値（0.0 〜 1.0）を計算して返す
  float processSample();

  // エンベロープが完全に終了しているかどうかを判定
  bool isActive() const { return state != State::Idle; }

  float getOutputValue() const
  {
    return currentLevel; // 保持している現在のエンベロープ値（0.0〜1.0）を返す
  }

private:
  double currentSampleRate = 44100.0;
  State state = State::Idle;

  float attackTime = 1.0f;
  float decayTime = 1.0f;
  float sustainLevel = 1.0f;
  float releaseTime = 1.0f;

  float attackIncrement = 0.0f;
  float decayDecrement = 0.0f;
  float releaseDecrement = 0.0f;

  float currentLevel = 0.0f;

  // 時間(ms)から1サンプルあたりの変化量を計算する関数
  void updateRates();
};