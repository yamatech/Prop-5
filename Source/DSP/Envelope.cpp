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
    // Set minimum to 0.1ms to prevent division by zero
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
        // Convert milliseconds to samples and calculate the increment/decrement per sample
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
        // Recalculate rate to release from the current level towards 0
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
            // Follow up when parameters change dynamically
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