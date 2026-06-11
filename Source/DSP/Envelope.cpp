#include "Envelope.h"

Prop5Envelope::Prop5Envelope() {}

void Prop5Envelope::prepare(double sampleRate)
{
    currentSampleRate = sampleRate;
    state = State::Idle;
    currentLevel = 0.0f;
    updateRates();
}

void Prop5Envelope::setParameters(float attackMs, float decayMs, float sustainLevel, float releaseMs)
{
    // 0割りを防ぐため最小値を0.1msとする
    attackTime = juce::jmax(0.1f, attackMs);
    decayTime = juce::jmax(0.1f, decayMs);
    this->sustainLevel = juce::jlimit(0.0f, 1.0f, sustainLevel);
    releaseTime = juce::jmax(0.1f, releaseMs);

    updateRates();
}

void Prop5Envelope::updateRates()
{
    if (currentSampleRate > 0.0)
    {
        // ミリ秒をサンプル数に変換し、1サンプルあたりの増減量を計算
        attackIncrement = 1.0f / (attackTime * 0.001f * currentSampleRate);
        decayDecrement = (1.0f - sustainLevel) / (decayTime * 0.001f * currentSampleRate);
        releaseDecrement = 1.0f / (releaseTime * 0.001f * currentSampleRate); 
    }
}

void Prop5Envelope::noteOn()
{
    state = State::Attack;
}

void Prop5Envelope::noteOff()
{
    if (state != State::Idle)
    {
        state = State::Release;
        // 現在のレベルから0に向かってリリースするため、レートを再計算
        if (currentSampleRate > 0.0)
            releaseDecrement = currentLevel / (releaseTime * 0.001f * currentSampleRate);
    }
}

float Prop5Envelope::processSample()
{
    switch (state)
    {
        case State::Idle:
            currentLevel = 0.0f;
            break;

        case State::Attack:
            currentLevel += attackIncrement;
            if (currentLevel >= 1.0f)
            {
                currentLevel = 1.0f;
                state = State::Decay;
            }
            break;

        case State::Decay:
            currentLevel -= decayDecrement;
            if (currentLevel <= sustainLevel)
            {
                currentLevel = sustainLevel;
                state = State::Sustain;
            }
            break;

        case State::Sustain:
            // パラメーターが動的に変わった場合の追従
            if (currentLevel > sustainLevel) 
            {
                currentLevel -= decayDecrement;
                if (currentLevel <= sustainLevel) currentLevel = sustainLevel;
            }
            else if (currentLevel < sustainLevel)
            {
                currentLevel += attackIncrement;
                if (currentLevel >= sustainLevel) currentLevel = sustainLevel;
            }
            break;

        case State::Release:
            currentLevel -= releaseDecrement;
            if (currentLevel <= 0.0f)
            {
                currentLevel = 0.0f;
                state = State::Idle;
            }
            break;
    }

    return currentLevel;
}