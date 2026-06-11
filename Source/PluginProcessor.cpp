#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DSP/Voice.h"
#include "PresetData.h"

//==============================================================================
Prop5Processor::Prop5Processor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
#endif
      apvts(*this, nullptr, "Prop5State", createParameterLayout())
{
    // 5ボイス・ポリフォニック仕様に基づいてシンセボイスを追加
    for (int i = 0; i < 5; ++i)
    {
        synth.addVoice(new Prop5Voice());
    }

    // シンセサウンドを追加
    synth.addSound(new Prop5Sound());

    // PropertiesFileの初期化
    juce::PropertiesFile::Options options;
    options.applicationName = "Prop-5";
    options.filenameSuffix = ".xml";
    options.folderName = "Prop-5";
    options.storageFormat = juce::PropertiesFile::storeAsXML;
    properties = std::make_unique<juce::PropertiesFile>(options);

    // ユーザープリセットのスキャン
    scanPresets();
}

Prop5Processor::~Prop5Processor()
{
}

//==============================================================================
const juce::String Prop5Processor::getName() const
{
    return JucePlugin_Name;
}

bool Prop5Processor::acceptsMidi() const
{
    return true;
}

bool Prop5Processor::producesMidi() const
{
    return false;
}

bool Prop5Processor::isMidiEffect() const
{
    return false;
}

double Prop5Processor::getTailLengthSeconds() const
{
    return 0.0;
}

int Prop5Processor::getNumPrograms()
{
    return 13 + userPresetFiles.size();
}

int Prop5Processor::getCurrentProgram()
{
    return currentProgram;
}

void Prop5Processor::setCurrentProgram(int index)
{
    if (index >= 0 && index < getNumPrograms())
    {
        currentProgram = index;
        if (index < 13)
        {
            auto factoryPresets = getFactoryPresets();
            if (index < factoryPresets.size())
            {
                auto& preset = factoryPresets[index];
                for (auto& param : preset.parameters)
                {
                    if (auto* p = apvts.getParameter(param.first))
                    {
                        float val = param.second;
                        if (auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(p))
                        {
                            *floatParam = val;
                        }
                        else if (auto* boolParam = dynamic_cast<juce::AudioParameterBool*>(p))
                        {
                            *boolParam = (val > 0.5f);
                        }
                        else if (auto* choiceParam = dynamic_cast<juce::AudioParameterChoice*>(p))
                        {
                            *choiceParam = static_cast<int>(val);
                        }
                    }
                }
            }
        }
        else
        {
            int fileIdx = index - 13;
            if (fileIdx >= 0 && fileIdx < userPresetFiles.size())
            {
                loadPresetFromFile (userPresetFiles[fileIdx]);
            }
        }

        updateHostDisplay();
    }
}

const juce::String Prop5Processor::getProgramName(int index)
{
    if (index < 13)
    {
        auto factoryPresets = getFactoryPresets();
        if (index >= 0 && index < factoryPresets.size())
        {
            return factoryPresets[index].name;
        }
        return "Init Patch";
    }
    else
    {
        int fileIdx = index - 13;
        if (fileIdx >= 0 && fileIdx < userPresetFiles.size())
        {
            return userPresetFiles[fileIdx].getFileNameWithoutExtension();
        }
    }
    return "Empty User Patch";
}

void Prop5Processor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void Prop5Processor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    
    // 各ボイスのサンプリングレート設定などを初期化
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<Prop5Voice*>(synth.getVoice(i)))
        {
            voice->prepare(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void Prop5Processor::releaseResources()
{
}

bool Prop5Processor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
#endif
}

void Prop5Processor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // 出力バッファのクリア
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // MIDIメッセージからピッチベンド、CC、およびプログラムチェンジの変更を検出して処理
    for (const auto metadata : midiMessages)
    {
        auto msg = metadata.getMessage();
        if (msg.isPitchWheel())
        {
            if (auto* param = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("pitchbend")))
            {
                float pbValue = (msg.getPitchWheelValue() - 8192.0f) / 8192.0f;
                *param = pbValue;
            }
        }
        else if (msg.isController())
        {
            int ccNum = msg.getControllerNumber();
            float ccVal = msg.getControllerValue() / 127.0f;
            juce::String paramID = "";

            switch (ccNum)
            {
                case 7:   paramID = "master_volume"; break;
                case 5:   paramID = "glide"; break;
                case 74:  paramID = "vcf_cutoff"; break;
                case 71:  paramID = "vcf_reson"; break;
                case 89:  paramID = "vcf_env_amt"; break;
                
                // Filter Env (Env A)
                case 73:  paramID = "env_a_atk"; break;
                case 75:  paramID = "env_a_dcy"; break;
                case 79:  paramID = "env_a_sus"; break;
                case 72:  paramID = "env_a_rel"; break;

                // Amp Env (Env B)
                case 80:  paramID = "env_b_atk"; break;
                case 81:  paramID = "env_b_dcy"; break;
                case 82:  paramID = "env_b_sus"; break;
                case 83:  paramID = "env_b_rel"; break;

                // OSC
                case 102: paramID = "osc_a_freq"; break;
                case 103: paramID = "osc_a_pw"; break;
                case 104: paramID = "osc_b_freq"; break;
                case 105: paramID = "osc_b_pw"; break;

                // Mixer
                case 106: paramID = "mix_osc_a"; break;
                case 107: paramID = "mix_osc_b"; break;
                case 108: paramID = "mix_noise"; break;

                // LFO
                case 70:  paramID = "lfo_rate"; break;
                case 76:  paramID = "lfo_initial_amount"; break;

                // Modulation Wheel
                case 1:   paramID = "modwheel"; break;

                default: break;
            }

            if (paramID.isNotEmpty())
            {
                if (auto* param = apvts.getParameter(paramID))
                {
                    param->setValueNotifyingHost(ccVal);
                }
            }
        }
        else if (msg.isProgramChange())
        {
            setCurrentProgram(msg.getProgramChangeNumber());
        }
    }

    // 各ボイスに対してAPVTSの最新パラメータを適用
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<Prop5Voice*>(synth.getVoice(i)))
        {
            voice->updateParameters(apvts);
        }
    }

    // 音声生成
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool Prop5Processor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* Prop5Processor::createEditor()
{
    return new Prop5Editor(*this);
}

//==============================================================================
void Prop5Processor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    if (xml != nullptr)
    {
        xml->setAttribute("currentProgram", currentProgram);
        copyXmlToBinary(*xml, destData);
    }
}

void Prop5Processor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName(apvts.state.getType()))
        {
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
            currentProgram = xmlState->getIntAttribute("currentProgram", 0);
        }
    }
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout Prop5Processor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Skew ranges for filter cutoff and envelope rates (analog feel)
    juce::NormalisableRange<float> cutoffRange (20.0f, 20000.0f);
    cutoffRange.setSkewForCentre (1000.0f);

    juce::NormalisableRange<float> msRange (1.0f, 15000.0f);
    msRange.setSkewForCentre (1000.0f);

    juce::NormalisableRange<float> rateRange (0.1f, 30.0f);
    rateRange.setSkewForCentre (5.0f);

    // 1. Master / Global Group
    layout.add(std::make_unique<juce::AudioParameterFloat>("master_volume", "Master Volume", 0.0f, 1.0f, 0.8f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("master_tune", "Master Tune", -1.0f, 1.0f, 0.0f));
    
    juce::NormalisableRange<float> glideRange (0.0f, 5000.0f);
    glideRange.setSkewForCentre (500.0f);
    layout.add(std::make_unique<juce::AudioParameterFloat>("glide", "Glide", glideRange, 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("pitchbend_range", "Pitchbend Range", 0.0f, 12.0f, 2.0f));
    layout.add(std::make_unique<juce::AudioParameterBool>("unison", "Unison", false));

    // 2. OSC A Group
    layout.add(std::make_unique<juce::AudioParameterFloat>("osc_a_freq", "OSC A Frequency", 0.0625f, 32.0f, 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("osc_a_pw", "OSC A Pulse Width", 5.0f, 95.0f, 50.0f));
    layout.add(std::make_unique<juce::AudioParameterBool>("osc_a_saw", "OSC A Saw", true));
    layout.add(std::make_unique<juce::AudioParameterBool>("osc_a_sqr", "OSC A Square", false));

    // 3. OSC B Group
    layout.add(std::make_unique<juce::AudioParameterFloat>("osc_b_freq", "OSC B Frequency", 0.0625f, 32.0f, 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("osc_b_fine", "OSC B Fine Tune", -1.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("osc_b_pw", "OSC B Pulse Width", 5.0f, 95.0f, 50.0f));
    layout.add(std::make_unique<juce::AudioParameterBool>("osc_b_saw", "OSC B Saw", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("osc_b_sqr", "OSC B Square", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("osc_b_tri", "OSC B Triangle", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("osc_b_lo_mode", "OSC B Lo Frequency Mode", false));

    // 4. OSC Sync
    layout.add(std::make_unique<juce::AudioParameterBool>("osc_sync", "OSC Sync", false));

    // 5. Mixer Group
    layout.add(std::make_unique<juce::AudioParameterFloat>("mix_osc_a", "Mix OSC A", 0.0f, 1.0f, 0.75f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("mix_osc_b", "Mix OSC B", 0.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("mix_noise", "Mix Noise", 0.0f, 1.0f, 0.0f));

    // 6. VCF Group
    layout.add(std::make_unique<juce::AudioParameterFloat>("vcf_cutoff", "VCF Cutoff", cutoffRange, 20000.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("vcf_reson", "VCF Resonance", 0.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("vcf_env_amt", "VCF Envelope Amt", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("vcf_kb_track", "VCF Keyboard Tracking", juce::StringArray{"OFF", "HALF", "FULL"}, 0));

    // 7. Envelope A (Filter)
    layout.add(std::make_unique<juce::AudioParameterFloat>("env_a_atk", "Env A Attack", msRange, 20.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("env_a_dcy", "Env A Decay", msRange, 200.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("env_a_sus", "Env A Sustain", 0.0f, 1.0f, 0.7f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("env_a_rel", "Env A Release", msRange, 300.0f));

    // 8. Envelope B (Amp)
    layout.add(std::make_unique<juce::AudioParameterFloat>("env_b_atk", "Env B Attack", msRange, 5.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("env_b_dcy", "Env B Decay", msRange, 150.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("env_b_sus", "Env B Sustain", 0.0f, 1.0f, 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("env_b_rel", "Env B Release", msRange, 200.0f));
    layout.add(std::make_unique<juce::AudioParameterBool>("vca_mode", "VCA Mode (Gate/Env)", false));

    // 9. Poly Mod Group
    layout.add(std::make_unique<juce::AudioParameterFloat>("poly_mod_env_amt", "Poly Mod Env Amt", 0.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("poly_mod_osc_b_amt", "Poly Mod OSC B Amt", 0.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterBool>("poly_mod_dest_freq_a", "Poly Mod Dest Freq A", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("poly_mod_dest_pw_a", "Poly Mod Dest PW A", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("poly_mod_dest_vcf", "Poly Mod Dest VCF", false));

    // 10. LFO Group
    layout.add(std::make_unique<juce::AudioParameterChoice>("lfo_wave", "LFO Wave", juce::StringArray{"Triangle", "Square", "Sawtooth"}, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("lfo_rate", "LFO Rate", rateRange, 5.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("lfo_initial_amount", "LFO Initial Amount", 0.0f, 1.0f, 0.0f));

    // 11. Wheel Mod Group
    layout.add(std::make_unique<juce::AudioParameterFloat>("wheel_mod_source_mix", "Wheel Mod Source Mix", 0.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterBool>("wheel_mod_dest_freq_a", "Wheel Mod Dest Freq A", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("wheel_mod_dest_freq_b", "Wheel Mod Dest Freq B", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("wheel_mod_dest_pw_a", "Wheel Mod Dest PW A", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("wheel_mod_dest_pw_b", "Wheel Mod Dest PW B", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("wheel_mod_dest_filter", "Wheel Mod Dest Filter", false));

    // 12. Performance Controls (synced from MIDI/GUI)
    layout.add(std::make_unique<juce::AudioParameterFloat>("pitchbend", "Pitchbend", -1.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("modwheel", "Modulation Wheel", 0.0f, 1.0f, 0.0f));

    // 13. Velocity Controls
    layout.add(std::make_unique<juce::AudioParameterBool>("velocity_to_amp", "Velocity to Amp", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("velocity_to_filter", "Velocity to Filter", false));

    return layout;
}

//==============================================================================
//==============================================================================
juce::File Prop5Processor::getCurrentPresetFolder()
{
    juce::String path = properties->getValue ("presetFolderPath", "");
    if (path.isNotEmpty())
    {
        juce::File folder (path);
        if (folder.isDirectory())
            return folder;
    }
    
    // デフォルト: Documents/Prop-5/Presets
    juce::File defaultFolder = juce::File::getSpecialLocation (juce::File::userDocumentsDirectory)
                                .getChildFile ("Prop-5")
                                .getChildFile ("Presets");
    if (!defaultFolder.exists())
        defaultFolder.createDirectory();
        
    return defaultFolder;
}

void Prop5Processor::setCurrentPresetFolder (const juce::File& newFolder)
{
    if (newFolder.isDirectory())
    {
        properties->setValue ("presetFolderPath", newFolder.getFullPathName());
        properties->saveIfNeeded();
        scanPresets();
    }
}

void Prop5Processor::scanPresets()
{
    userPresetFiles.clear();
    juce::File folder = getCurrentPresetFolder();
    folder.findChildFiles (userPresetFiles, juce::File::findFiles, false, "*.prop5");
}

void Prop5Processor::savePresetToFile (const juce::File& file)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    if (xml != nullptr)
    {
        xml->writeTo (file);
    }
}

bool Prop5Processor::loadPresetFromFile (const juce::File& file)
{
    if (!file.existsAsFile())
        return false;
        
    std::unique_ptr<juce::XmlElement> xmlState (juce::XmlDocument::parse (file));
    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName (apvts.state.getType()))
        {
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
            return true;
        }
    }
    return false;
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Prop5Processor();
}