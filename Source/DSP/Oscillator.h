#pragma once
#include <JuceHeader.h>

class Prop5Oscillator
{
public:
    Prop5Oscillator();
    ~Prop5Oscillator() = default;

    // Initialization (set sample rate)
    void prepare(double sampleRate);

    // Set parameters
    void setFrequency(float freqInHz);
    void setPulseWidth(float pwPercentage); // 0.0 to 100.0

    // Waveform switch on/off
    void setSawEnabled(bool enabled)     { sawEnabled = enabled; }
    void setSquareEnabled(bool enabled)  { sqrEnabled = enabled; }
    void setTriangleEnabled(bool enabled){ triEnabled = enabled; } // For OSC B

    // For OSC SYNC (hard sync): Forcefully resets the phase externally
    void resetPhase() { phase = 0.0f; }
    float getPhase() const { return phase; }

    // Generates and returns one sample of the waveform
    float processSample();

    // Function to inspect the latest output value from the Voice class
    float getSample() const { return lastSample; }

private:
    double currentSampleRate = 44100.0;
    float frequency = 440.0f;
    float phase = 0.0f;
    float phaseIncrement = 0.0f;
    float pulseWidth = 0.5f; // Normalized to 0.0 - 1.0

    bool sawEnabled = true;
    bool sqrEnabled = false;
    bool triEnabled = false;

    float lastSample = 0.0f; // Variable to store the latest output value

    void updateIncrement();
};

// White noise generator
class Prop5Noise
{
public:
    void prepare(double sampleRate) { juce::ignoreUnused(sampleRate); }
    float processSample() 
    { 
        // Returns a random value between -1.0 and 1.0
        return random.nextFloat() * 2.0f - 1.0f; 
    }
private:
    juce::Random random;
};