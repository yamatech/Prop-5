#include "Voice.h"
#include <cmath>
#include <algorithm>

Prop5Voice::Prop5Voice()
{
}

Prop5Voice::~Prop5Voice()
{
}

bool Prop5Voice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<Prop5Sound*>(sound) != nullptr;
}

void Prop5Voice::prepare(double sampleRate, int samplesPerBlock, int numOutputChannels)
{
    currentSampleRate = sampleRate;

    oscA.prepare(sampleRate);
    oscB.prepare(sampleRate);
    noise.prepare(sampleRate);
    filter.prepare(sampleRate);
    envA.prepare(sampleRate);
    envB.prepare(sampleRate);

    lfoPhase = 0.0f;
    currentMidiNote = -1;
    currentPitchWheelOffset = 0.0f;
    lastPhaseB = 0.0f;

    currentGlideNote = 60.0f;
    targetGlideNote = 60.0f;
    isFirstNote = true;
}

void Prop5Voice::updateParameters(const juce::AudioProcessorValueTreeState& apvts)
{
    // Mixer and Master
    masterVolume = apvts.getRawParameterValue("master_volume")->load();
    masterTune = apvts.getRawParameterValue("master_tune")->load();
    
    if (apvts.getRawParameterValue("glide") != nullptr)
    {
        glideTimeMs = apvts.getRawParameterValue("glide")->load();
    }

    pitchBendRange = static_cast<int>(apvts.getRawParameterValue("pitchbend_range")->load());
    unison = apvts.getRawParameterValue("unison")->load() > 0.5f;

    mixOscA = apvts.getRawParameterValue("mix_osc_a")->load();
    mixOscB = apvts.getRawParameterValue("mix_osc_b")->load();
    mixNoise = apvts.getRawParameterValue("mix_noise")->load();

    // OSC A
    oscAFreqRatio = apvts.getRawParameterValue("osc_a_freq")->load();
    oscAPw = apvts.getRawParameterValue("osc_a_pw")->load();
    oscASaw = apvts.getRawParameterValue("osc_a_saw")->load() > 0.5f;
    oscASqr = apvts.getRawParameterValue("osc_a_sqr")->load() > 0.5f;

    // OSC B
    oscBFreqRatio = apvts.getRawParameterValue("osc_b_freq")->load();
    oscBFine = apvts.getRawParameterValue("osc_b_fine")->load();
    oscBPw = apvts.getRawParameterValue("osc_b_pw")->load();
    oscBSaw = apvts.getRawParameterValue("osc_b_saw")->load() > 0.5f;
    oscBSqr = apvts.getRawParameterValue("osc_b_sqr")->load() > 0.5f;
    oscBTri = apvts.getRawParameterValue("osc_b_tri")->load() > 0.5f;
    oscBLoMode = apvts.getRawParameterValue("osc_b_lo_mode")->load() > 0.5f;

    // Sync
    oscSync = apvts.getRawParameterValue("osc_sync")->load() > 0.5f;

    // VCF
    baseCutoff = apvts.getRawParameterValue("vcf_cutoff")->load();
    baseResonance = apvts.getRawParameterValue("vcf_reson")->load();
    vcfEnvAmt = apvts.getRawParameterValue("vcf_env_amt")->load();
    vcfKbTrack = static_cast<int>(apvts.getRawParameterValue("vcf_kb_track")->load());

    // Envelopes
    envA.setParameters(
        apvts.getRawParameterValue("env_a_atk")->load(),
        apvts.getRawParameterValue("env_a_dcy")->load(),
        apvts.getRawParameterValue("env_a_sus")->load(),
        apvts.getRawParameterValue("env_a_rel")->load()
    );

    envB.setParameters(
        apvts.getRawParameterValue("env_b_atk")->load(),
        apvts.getRawParameterValue("env_b_dcy")->load(),
        apvts.getRawParameterValue("env_b_sus")->load(),
        apvts.getRawParameterValue("env_b_rel")->load()
    );

    vcaMode = apvts.getRawParameterValue("vca_mode")->load() > 0.5f;

    // Poly Mod
    polyModEnvAmt = apvts.getRawParameterValue("poly_mod_env_amt")->load();
    polyModOscBAmt = apvts.getRawParameterValue("poly_mod_osc_b_amt")->load();
    polyModDestFreqA = apvts.getRawParameterValue("poly_mod_dest_freq_a")->load() > 0.5f;
    polyModDestPwA = apvts.getRawParameterValue("poly_mod_dest_pw_a")->load() > 0.5f;
    polyModDestVcf = apvts.getRawParameterValue("poly_mod_dest_vcf")->load() > 0.5f;

    // LFO & Wheel Mod
    lfoWaveform = static_cast<int>(apvts.getRawParameterValue("lfo_wave")->load());
    lfoRate = apvts.getRawParameterValue("lfo_rate")->load();
    lfoInitialAmount = apvts.getRawParameterValue("lfo_initial_amount")->load();

    wheelModSourceMix = apvts.getRawParameterValue("wheel_mod_source_mix")->load();
    wheelModDestFreqA = apvts.getRawParameterValue("wheel_mod_dest_freq_a")->load() > 0.5f;
    wheelModDestFreqB = apvts.getRawParameterValue("wheel_mod_dest_freq_b")->load() > 0.5f;
    wheelModDestPwA = apvts.getRawParameterValue("wheel_mod_dest_pw_a")->load() > 0.5f;
    wheelModDestPwB = apvts.getRawParameterValue("wheel_mod_dest_pw_b")->load() > 0.5f;
    wheelModDestFilter = apvts.getRawParameterValue("wheel_mod_dest_filter")->load() > 0.5f;

    // Sync performance controls
    currentModWheelValue = apvts.getRawParameterValue("modwheel")->load();
    float pitchbend = apvts.getRawParameterValue("pitchbend")->load();
    currentPitchWheelOffset = pitchbend * pitchBendRange;

    // Read velocity parameters
    if (apvts.getRawParameterValue("velocity_to_amp") != nullptr)
        velocityToAmp = apvts.getRawParameterValue("velocity_to_amp")->load();
    if (apvts.getRawParameterValue("velocity_to_filter") != nullptr)
        velocityToFilter = apvts.getRawParameterValue("velocity_to_filter")->load();
}

void Prop5Voice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    juce::ignoreUnused(sound, currentPitchWheelPosition);
    currentMidiNote = midiNoteNumber;
    currentVelocity = velocity;
    
    if (isFirstNote || glideTimeMs <= 0.05f)
    {
        currentGlideNote = static_cast<float>(midiNoteNumber);
        targetGlideNote = static_cast<float>(midiNoteNumber);
        isFirstNote = false;
    }
    else
    {
        targetGlideNote = static_cast<float>(midiNoteNumber);
    }

    envA.noteOn();
    envB.noteOn();
    lastPhaseB = 0.0f;
}

void Prop5Voice::stopNote(float velocity, bool allowTailOff)
{
    juce::ignoreUnused(velocity);
    
    if (allowTailOff)
    {
        envA.noteOff();
        envB.noteOff();
    }
    else
    {
        // 即座に消音
        clearCurrentNote();
        currentMidiNote = -1;
    }
}

void Prop5Voice::pitchWheelMoved(int newPitchWheelValue)
{
    juce::ignoreUnused(newPitchWheelValue);
}

void Prop5Voice::controllerMoved(int controllerNumber, int newControllerValue)
{
    juce::ignoreUnused(controllerNumber, newControllerValue);
}

void Prop5Voice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (currentMidiNote < 0)
        return;

    float velPower = std::pow(currentVelocity, 1.5f);

    // Precalculate glide factor for this block
    double factor = 1.0;
    if (glideTimeMs > 0.05f)
    {
        double tau = glideTimeMs / 1000.0;
        factor = 1.0 - std::exp(-1.0 / (tau * currentSampleRate));
    }

    for (int sample = 0; sample < numSamples; ++sample)
    {
        // Update glide note value
        if (glideTimeMs > 0.05f)
        {
            currentGlideNote += (targetGlideNote - currentGlideNote) * static_cast<float>(factor);
        }
        else
        {
            currentGlideNote = targetGlideNote;
        }

        // Get single noise sample to share between VCM/Mixer and WHEEL-MOD
        float noiseSig = noise.processSample();

        // --- LFO Generator ---
        float lfoSig = 0.0f;
        if (lfoWaveform == 0) // Triangle
        {
            lfoSig = lfoPhase < 0.5f ? 4.0f * lfoPhase - 1.0f : 3.0f - 4.0f * lfoPhase;
        }
        else if (lfoWaveform == 1) // Square
        {
            lfoSig = lfoPhase < 0.5f ? 1.0f : -1.0f;
        }
        else if (lfoWaveform == 2) // Sawtooth
        {
            lfoSig = 2.0f * lfoPhase - 1.0f;
        }

        // LFO Phase increment
        lfoPhase += lfoRate / currentSampleRate;
        if (lfoPhase >= 1.0f)
            lfoPhase -= 1.0f;

        // WHEEL-MOD Source Mix (LFO to Noise)
        float modSource = (lfoSig * (1.0f - wheelModSourceMix)) + (noiseSig * wheelModSourceMix);

        // Total modulation amount (Initial Amount + Mod Wheel)
        float totalModAmt = lfoInitialAmount + currentModWheelValue * (1.0f - lfoInitialAmount);
        float wheelModPower = modSource * totalModAmt;

        // --- Envelopes Processing ---
        float envAVal = envA.processSample();
        float envBVal = vcaMode ? 1.0f : envB.processSample();

        // --- OSC B Processing ---
        // OSC B Frequency Calculation
        float pitchOffsetB = masterTune; // -1.0 to 1.0 semitones
        pitchOffsetB += (currentGlideNote - 60.0f); // Relative to C4 (MIDI 60)
        
        if (unison)
        {
            pitchOffsetB += (randomGen.nextFloat() * 2.0f - 1.0f) * 0.15f; // Unison detuning
        }
        pitchOffsetB += currentPitchWheelOffset;

        // Wheel Mod
        if (wheelModDestFreqB)
        {
            pitchOffsetB += wheelModPower * 12.0f; // Pitch modulation up to 1 octave
        }

        float baseFreqB = 261.63f * oscBFreqRatio;
        float freqB = baseFreqB * std::pow(2.0f, (pitchOffsetB + oscBFine) / 12.0f);
        if (oscBLoMode)
        {
            freqB *= 0.01f; // Frequency divider for Lo Mode
        }
        freqB = std::max(0.01f, freqB);

        oscB.setFrequency(freqB);

        // OSC B PW modulation
        float pwB = oscBPw;
        if (wheelModDestPwB)
        {
            pwB += wheelModPower * 40.0f;
        }
        pwB = std::clamp(pwB, 5.0f, 95.0f);
        oscB.setPulseWidth(pwB);

        oscB.setSawEnabled(oscBSaw);
        oscB.setSquareEnabled(oscBSqr);
        oscB.setTriangleEnabled(oscBTri);

        float valB = oscB.processSample();

        // --- Poly Mod Modulation Signal ---
        float polyModSignal = (envAVal * polyModEnvAmt) + (valB * polyModOscBAmt);

        // --- OSC A Processing ---
        // OSC A Frequency Calculation
        float pitchOffsetA = masterTune;
        pitchOffsetA += (currentGlideNote - 60.0f);
        
        if (unison)
        {
            pitchOffsetA += (randomGen.nextFloat() * 2.0f - 1.0f) * 0.15f;
        }
        pitchOffsetA += currentPitchWheelOffset;

        // Wheel Mod
        if (wheelModDestFreqA)
        {
            pitchOffsetA += wheelModPower * 12.0f; // Pitch modulation up to 1 octave
        }
        // Poly Mod
        if (polyModDestFreqA)
        {
            pitchOffsetA += polyModSignal * 12.0f;
        }

        float baseFreqA = 261.63f * oscAFreqRatio;
        float freqA = baseFreqA * std::pow(2.0f, pitchOffsetA / 12.0f);
        freqA = std::max(1.0f, freqA);

        oscA.setFrequency(freqA);

        // Pulse Width Mod
        float pwA = oscAPw;
        if (wheelModDestPwA)
        {
            pwA += wheelModPower * 40.0f;
        }
        if (polyModDestPwA)
        {
            pwA += polyModSignal * 40.0f;
        }
        pwA = std::clamp(pwA, 5.0f, 95.0f);
        oscA.setPulseWidth(pwA);

        oscA.setSawEnabled(oscASaw);
        oscA.setSquareEnabled(oscASqr);

        // Hard Sync
        if (oscSync)
        {
            float currentPhaseB = oscB.getPhase();
            if (currentPhaseB < lastPhaseB)
            {
                oscA.resetPhase();
            }
            lastPhaseB = currentPhaseB;
        }

        float valA = oscA.processSample();

        // --- Mixer Output ---
        float mixVal = 0.0f;
        mixVal += valA * mixOscA;
        if (!oscBLoMode)
        {
            mixVal += valB * mixOscB;
        }
        mixVal += noiseSig * mixNoise;

        // --- VCF (Ladder Filter) ---
        float filterVelocityScale = 1.0f + velocityToFilter * ((0.1f + 0.9f * velPower) - 1.0f);
        float cutoffVelocityScale = 1.0f + velocityToFilter * (std::pow(2.0f, (velPower - 1.0f) * 2.0f) - 1.0f);
        filter.setParameters(baseCutoff * cutoffVelocityScale, baseResonance, vcfEnvAmt * filterVelocityScale, vcfKbTrack);

        float totalFilterMod = envAVal;
        if (wheelModDestFilter)
        {
            totalFilterMod += wheelModPower * 0.5f;
        }
        if (polyModDestVcf)
        {
            totalFilterMod += polyModSignal * 0.5f;
        }
        totalFilterMod = std::clamp(totalFilterMod, 0.0f, 1.0f);

        float filterVal = filter.processSample(mixVal, totalFilterMod, currentGlideNote);

        // --- VCA Output ---
        float ampVelocityScale = 1.0f + velocityToAmp * ((0.05f + 0.95f * velPower) - 1.0f);
        float outputVal = filterVal * envBVal * masterVolume * 0.2f * ampVelocityScale;

        outputBuffer.addSample(0, startSample + sample, outputVal);
        outputBuffer.addSample(1, startSample + sample, outputVal);
    }

    // Check if envelope has finished
    if (!vcaMode && !envB.isActive())
    {
        clearCurrentNote();
        currentMidiNote = -1;
    }
}