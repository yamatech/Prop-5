#pragma once

#include <JuceHeader.h>
#include "Voice.h"
#include <vector>
#include <algorithm>

class Prop5Synthesiser : public juce::Synthesiser
{
public:
    Prop5Synthesiser() = default;
    ~Prop5Synthesiser() override = default;

    void setUnisonMode(bool enabled) { unisonMode = enabled; }
    bool isUnisonMode() const { return unisonMode; }

    void clearNoteStack()
    {
        noteStack.clear();
        allNotesOff(0, false);
    }

    void noteOn(int midiChannel, int midiNoteNumber, float velocity) override
    {
        if (unisonMode)
        {
            // Remove from stack if already exists
            noteStack.erase(std::remove_if(noteStack.begin(), noteStack.end(),
                [midiChannel, midiNoteNumber](const NoteInfo& n) {
                    return n.channel == midiChannel && n.noteNumber == midiNoteNumber;
                }), noteStack.end());

            // Add to stack
            noteStack.push_back({ midiChannel, midiNoteNumber, velocity });

            triggerUnisonNote(midiChannel, midiNoteNumber, velocity);
        }
        else
        {
            noteStack.clear();
            for (int i = 0; i < getNumVoices(); ++i)
            {
                if (auto* voice = dynamic_cast<Prop5Voice*>(getVoice(i)))
                {
                    voice->setUnisonParams(-1, 1);
                }
            }
            juce::Synthesiser::noteOn(midiChannel, midiNoteNumber, velocity);
        }
    }

    void noteOff(int midiChannel, int midiNoteNumber, float velocity, bool allowTailOff) override
    {
        if (unisonMode)
        {
            // Remove from stack
            noteStack.erase(std::remove_if(noteStack.begin(), noteStack.end(),
                [midiChannel, midiNoteNumber](const NoteInfo& n) {
                    return n.channel == midiChannel && n.noteNumber == midiNoteNumber;
                }), noteStack.end());

            if (noteStack.empty())
            {
                // Stop all voices (trigger release)
                for (int i = 0; i < getNumVoices(); ++i)
                {
                    if (auto* voice = getVoice(i))
                    {
                        if (voice->isVoiceActive())
                        {
                            voice->stopNote(velocity, allowTailOff);
                        }
                    }
                }
            }
            else
            {
                // Trigger the latest note if keys are still pressed
                auto& lastNote = noteStack.back();
                triggerUnisonNote(lastNote.channel, lastNote.noteNumber, lastNote.velocity);
            }
        }
        else
        {
            noteStack.clear();
            juce::Synthesiser::noteOff(midiChannel, midiNoteNumber, velocity, allowTailOff);
        }
    }

private:
    struct NoteInfo
    {
        int channel;
        int noteNumber;
        float velocity;
    };

    void triggerUnisonNote(int midiChannel, int midiNoteNumber, float velocity)
    {
        // Immediately stop all active voices (no tail-off)
        for (int i = 0; i < getNumVoices(); ++i)
        {
            if (auto* voice = getVoice(i))
            {
                if (voice->isVoiceActive())
                {
                    voice->stopNote(0.0f, false);
                }
            }
        }

        std::vector<Prop5Voice*> propVoices;
        for (int i = 0; i < getNumVoices(); ++i)
        {
            if (auto* voice = dynamic_cast<Prop5Voice*>(getVoice(i)))
            {
                propVoices.push_back(voice);
            }
        }

        int count = static_cast<int>(propVoices.size());
        for (int i = 0; i < count; ++i)
        {
            auto* voice = propVoices[i];
            voice->setUnisonParams(i, count);
            if (getNumSounds() > 0)
            {
                startVoice(voice, getSound(0).get(), midiChannel, midiNoteNumber, velocity);
            }
        }
    }

    bool unisonMode = false;
    std::vector<NoteInfo> noteStack;
};
