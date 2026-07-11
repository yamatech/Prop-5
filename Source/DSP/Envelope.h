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

  // Initialize sample rate
  void prepare(double sampleRate);

  // Apply parameters from APVTS (Time parameters in milliseconds, Sustain level 0.0 to 1.0)
  void setParameters(float attackMs, float decayMs, float sustainLevel, float releaseMs);

  // Trigger note on (key pressed) and note off (key released)
  void noteOn();
  void noteOff();

  // Calculate and return the envelope value (0.0 to 1.0) per sample
  float processSample();

  // Determine if the envelope is completely finished
  bool isActive() const { return state != State::Idle; }

  float getOutputValue() const
  {
    return currentLevel; // Return the current envelope value (0.0 to 1.0) held
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

  // Calculate the change rate per sample from the time (ms)
  void updateRates();
};