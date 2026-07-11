#pragma once
#include <JuceHeader.h>
#include <unordered_map>
#include <vector>

struct Preset
{
    juce::String name;
    juce::String category;
    std::unordered_map<juce::String, float> parameters;
};

inline std::vector<Preset> getFactoryPresets()
{
    std::vector<Preset> presets;

    // Base parameters (Init Patch)
    Preset initPatch;
    initPatch.name = "Init Patch";
    initPatch.parameters = {
        {"glide", 0.0f},
        {"master_volume", 0.8f},
        {"master_tune", 0.0f},
        {"pitchbend_range", 2.0f},
        {"unison", 0.0f},
        {"osc_a_freq", 1.0f},
        {"osc_a_pw", 50.0f},
        {"osc_a_saw", 1.0f},
        {"osc_a_sqr", 0.0f},
        {"osc_b_freq", 1.0f},
        {"osc_b_fine", 0.0f},
        {"osc_b_pw", 50.0f},
        {"osc_b_saw", 0.0f},
        {"osc_b_sqr", 0.0f},
        {"osc_b_tri", 0.0f},
        {"osc_b_lo_mode", 0.0f},
        {"osc_sync", 0.0f},
        {"mix_osc_a", 0.75f},
        {"mix_osc_b", 0.0f},
        {"mix_noise", 0.0f},
        {"vcf_cutoff", 20000.0f},
        {"vcf_reson", 0.0f},
        {"vcf_env_amt", 0.5f},
        {"vcf_kb_track", 0.0f}, // 0: OFF, 1: HALF, 2: FULL
        {"env_a_atk", 20.0f},
        {"env_a_dcy", 200.0f},
        {"env_a_sus", 0.7f},
        {"env_a_rel", 300.0f},
        {"env_b_atk", 5.0f},
        {"env_b_dcy", 150.0f},
        {"env_b_sus", 1.0f},
        {"env_b_rel", 200.0f},
        {"vca_mode", 0.0f}, // 0: Env Mode, 1: Gate Mode
        {"poly_mod_env_amt", 0.0f},
        {"poly_mod_osc_b_amt", 0.0f},
        {"poly_mod_dest_freq_a", 0.0f},
        {"poly_mod_dest_pw_a", 0.0f},
        {"poly_mod_dest_vcf", 0.0f},
        {"lfo_wave", 0.0f}, // 0: Triangle, 1: Square, 2: Sawtooth
        {"lfo_rate", 5.0f},
        {"lfo_initial_amount", 0.0f},
        {"wheel_mod_source_mix", 0.0f}, // 0.0: LFO, 1.0: Noise
        {"wheel_mod_dest_freq_a", 0.0f},
        {"wheel_mod_dest_freq_b", 0.0f},
        {"wheel_mod_dest_pw_a", 0.0f},
        {"wheel_mod_dest_pw_b", 0.0f},
        {"wheel_mod_dest_filter", 0.0f},
        {"pitchbend", 0.0f},
        {"modwheel", 0.0f},
        {"velocity_to_amp", 0.0f},
        {"velocity_to_filter", 0.0f}
    };

    // =========================================================================
    // 1. Bass (6 voices)
    // =========================================================================

    // [1] Classic Saw Bass
    Preset sawBass = initPatch;
    sawBass.name = "Classic Saw Bass";
    sawBass.parameters["osc_a_freq"] = 0.5f; // One octave down
    sawBass.parameters["osc_a_saw"] = 1.0f;
    sawBass.parameters["osc_b_freq"] = 0.5f;
    sawBass.parameters["osc_b_fine"] = 0.08f; // Detune
    sawBass.parameters["osc_b_saw"] = 1.0f;
    sawBass.parameters["mix_osc_a"] = 0.7f;
    sawBass.parameters["mix_osc_b"] = 0.7f;
    sawBass.parameters["vcf_cutoff"] = 250.0f;
    sawBass.parameters["vcf_reson"] = 0.3f;
    sawBass.parameters["vcf_env_amt"] = 0.7f;
    sawBass.parameters["env_a_atk"] = 2.0f;
    sawBass.parameters["env_a_dcy"] = 180.0f;
    sawBass.parameters["env_a_sus"] = 0.1f;
    sawBass.parameters["env_a_rel"] = 100.0f;
    sawBass.parameters["env_b_atk"] = 2.0f;
    sawBass.parameters["env_b_dcy"] = 250.0f;
    sawBass.parameters["env_b_sus"] = 0.8f;
    sawBass.parameters["env_b_rel"] = 150.0f;
    presets.push_back(sawBass);

    // [2] Reso Sweeper Bass
    Preset resoBass = initPatch;
    resoBass.name = "Reso Sweeper Bass";
    resoBass.parameters["osc_a_freq"] = 0.5f;
    resoBass.parameters["osc_a_saw"] = 1.0f;
    resoBass.parameters["osc_b_freq"] = 0.5f;
    resoBass.parameters["osc_b_fine"] = 0.06f;
    resoBass.parameters["osc_b_sqr"] = 1.0f;
    resoBass.parameters["mix_osc_a"] = 0.7f;
    resoBass.parameters["mix_osc_b"] = 0.6f;
    resoBass.parameters["vcf_cutoff"] = 150.0f;
    resoBass.parameters["vcf_reson"] = 0.75f; // Higher resonance
    resoBass.parameters["vcf_env_amt"] = 0.85f;
    resoBass.parameters["env_a_atk"] = 1.0f;
    resoBass.parameters["env_a_dcy"] = 120.0f;
    resoBass.parameters["env_a_sus"] = 0.0f;
    resoBass.parameters["env_a_rel"] = 80.0f;
    resoBass.parameters["env_b_atk"] = 1.0f;
    resoBass.parameters["env_b_dcy"] = 200.0f;
    resoBass.parameters["env_b_sus"] = 0.8f;
    resoBass.parameters["env_b_rel"] = 100.0f;
    presets.push_back(resoBass);

    // [3] Uni Square Bass
    Preset uniBass = initPatch;
    uniBass.name = "Uni Square Bass";
    uniBass.parameters["unison"] = 1.0f; // Unison ON
    uniBass.parameters["osc_a_freq"] = 0.5f;
    uniBass.parameters["osc_a_sqr"] = 1.0f;
    uniBass.parameters["osc_b_freq"] = 0.5f;
    uniBass.parameters["osc_b_fine"] = 0.07f;
    uniBass.parameters["osc_b_sqr"] = 1.0f;
    uniBass.parameters["mix_osc_a"] = 0.7f;
    uniBass.parameters["mix_osc_b"] = 0.7f;
    uniBass.parameters["vcf_cutoff"] = 350.0f;
    uniBass.parameters["vcf_reson"] = 0.2f;
    uniBass.parameters["vcf_env_amt"] = 0.6f;
    uniBass.parameters["env_a_atk"] = 5.0f;
    uniBass.parameters["env_a_dcy"] = 300.0f;
    uniBass.parameters["env_a_sus"] = 0.2f;
    uniBass.parameters["env_a_rel"] = 150.0f;
    uniBass.parameters["env_b_atk"] = 2.0f;
    uniBass.parameters["env_b_dcy"] = 400.0f;
    uniBass.parameters["env_b_sus"] = 0.8f;
    uniBass.parameters["env_b_rel"] = 200.0f;
    uniBass.parameters["velocity_to_filter"] = 0.3f;
    presets.push_back(uniBass);

    // [4] Sub Double Bass
    Preset subBass = initPatch;
    subBass.name = "Sub Double Bass";
    subBass.parameters["osc_a_freq"] = 0.5f;
    subBass.parameters["osc_a_saw"] = 1.0f;
    subBass.parameters["osc_b_freq"] = 0.25f; // Further one octave down
    subBass.parameters["osc_b_sqr"] = 1.0f;
    subBass.parameters["mix_osc_a"] = 0.5f;
    subBass.parameters["mix_osc_b"] = 0.8f;
    subBass.parameters["vcf_cutoff"] = 80.0f;
    subBass.parameters["vcf_reson"] = 0.0f;
    subBass.parameters["vcf_env_amt"] = 0.4f;
    subBass.parameters["env_a_atk"] = 10.0f;
    subBass.parameters["env_a_dcy"] = 400.0f;
    subBass.parameters["env_a_sus"] = 0.5f;
    subBass.parameters["env_a_rel"] = 300.0f;
    subBass.parameters["env_b_atk"] = 5.0f;
    subBass.parameters["env_b_dcy"] = 500.0f;
    subBass.parameters["env_b_sus"] = 0.9f;
    subBass.parameters["env_b_rel"] = 300.0f;
    presets.push_back(subBass);

    // [5] Modern Poly Bass
    Preset polyBass = initPatch;
    polyBass.name = "Modern Poly Bass";
    polyBass.parameters["osc_a_freq"] = 0.5f;
    polyBass.parameters["osc_a_saw"] = 1.0f;
    polyBass.parameters["osc_b_freq"] = 0.5f;
    polyBass.parameters["osc_b_fine"] = 0.05f;
    polyBass.parameters["osc_b_saw"] = 1.0f;
    polyBass.parameters["mix_osc_a"] = 0.6f;
    polyBass.parameters["mix_osc_b"] = 0.6f;
    polyBass.parameters["vcf_cutoff"] = 200.0f;
    polyBass.parameters["vcf_reson"] = 0.2f;
    polyBass.parameters["vcf_env_amt"] = 0.6f;
    polyBass.parameters["env_a_atk"] = 2.0f;
    polyBass.parameters["env_a_dcy"] = 500.0f;
    polyBass.parameters["env_a_sus"] = 0.3f;
    polyBass.parameters["env_a_rel"] = 200.0f;
    polyBass.parameters["env_b_atk"] = 2.0f;
    polyBass.parameters["env_b_dcy"] = 600.0f;
    polyBass.parameters["env_b_sus"] = 0.9f;
    polyBass.parameters["env_b_rel"] = 250.0f;
    polyBass.parameters["wheel_mod_dest_filter"] = 1.0f; // Filter can be controlled by mod wheel
    polyBass.parameters["velocity_to_filter"] = 0.4f;
    presets.push_back(polyBass);

    // [6] Acid Square Bass
    Preset acidBass = initPatch;
    acidBass.name = "Acid Square Bass";
    acidBass.parameters["osc_a_saw"] = 0.0f;
    acidBass.parameters["osc_a_sqr"] = 1.0f;
    acidBass.parameters["osc_a_pw"] = 45.0f;
    acidBass.parameters["vcf_cutoff"] = 120.0f;
    acidBass.parameters["vcf_reson"] = 0.8f;
    acidBass.parameters["vcf_env_amt"] = 0.7f;
    acidBass.parameters["env_a_atk"] = 1.0f;
    acidBass.parameters["env_a_dcy"] = 150.0f;
    acidBass.parameters["env_a_sus"] = 0.0f;
    acidBass.parameters["env_a_rel"] = 100.0f;
    acidBass.parameters["env_b_atk"] = 1.0f;
    acidBass.parameters["env_b_dcy"] = 200.0f;
    acidBass.parameters["env_b_sus"] = 0.0f;
    acidBass.parameters["env_b_rel"] = 100.0f;
    presets.push_back(acidBass);

    // =========================================================================
    // 2. Lead (6 voices)
    // =========================================================================

    // [7] Pro-Solo Lead (OSC Sync)
    Preset syncLead = initPatch;
    syncLead.name = "Pro-Solo Lead";
    syncLead.parameters["unison"] = 1.0f;
    syncLead.parameters["osc_a_freq"] = 2.0f; // User adjustment: Raised to 2.0f for a soft, flute-like high-range lead
    syncLead.parameters["osc_a_saw"] = 1.0f;
    syncLead.parameters["osc_b_freq"] = 2.0f;
    syncLead.parameters["osc_b_saw"] = 1.0f;
    syncLead.parameters["osc_sync"] = 0.0f; // User adjustment: Disabled sync to completely eliminate short noises
    syncLead.parameters["mix_osc_a"] = 0.8f;
    syncLead.parameters["mix_osc_b"] = 0.0f; // Volume is zero
    syncLead.parameters["mix_noise"] = 0.0f;
    syncLead.parameters["vcf_cutoff"] = 1200.0f;
    syncLead.parameters["vcf_reson"] = 0.0f;
    syncLead.parameters["vcf_env_amt"] = 0.0f;
    syncLead.parameters["poly_mod_env_amt"] = 0.0f;
    syncLead.parameters["poly_mod_dest_freq_a"] = 0.0f;
    syncLead.parameters["env_a_atk"] = 20.0f;
    syncLead.parameters["env_a_dcy"] = 600.0f;
    syncLead.parameters["env_a_sus"] = 0.2f;
    syncLead.parameters["env_a_rel"] = 200.0f;
    syncLead.parameters["env_b_atk"] = 15.0f; // Since the sync noise was resolved, returned attack to the original 15ms to improve play response
    syncLead.parameters["env_b_dcy"] = 400.0f;
    syncLead.parameters["env_b_sus"] = 0.8f;
    syncLead.parameters["env_b_rel"] = 200.0f;
    syncLead.parameters["velocity_to_filter"] = 0.16f;
    syncLead.parameters["vcf_kb_track"] = 1.0f; // Kb Track: HALF
    presets.push_back(syncLead);

    // [8] Polyphonic Lead
    Preset polyLead = initPatch;
    polyLead.name = "Polyphonic Lead";
    polyLead.parameters["osc_a_saw"] = 1.0f;
    polyLead.parameters["osc_b_freq"] = 1.0f;
    polyLead.parameters["osc_b_fine"] = 0.06f;
    polyLead.parameters["osc_b_saw"] = 1.0f;
    polyLead.parameters["mix_osc_a"] = 0.6f;
    polyLead.parameters["mix_osc_b"] = 0.6f;
    polyLead.parameters["vcf_cutoff"] = 1500.0f;
    polyLead.parameters["vcf_reson"] = 0.2f;
    polyLead.parameters["vcf_env_amt"] = 0.4f;
    polyLead.parameters["env_a_atk"] = 10.0f;
    polyLead.parameters["env_a_dcy"] = 300.0f;
    polyLead.parameters["env_a_sus"] = 0.5f;
    polyLead.parameters["env_a_rel"] = 250.0f;
    polyLead.parameters["env_b_atk"] = 5.0f;
    polyLead.parameters["env_b_dcy"] = 400.0f;
    polyLead.parameters["env_b_sus"] = 0.8f;
    polyLead.parameters["env_b_rel"] = 250.0f;
    polyLead.parameters["lfo_initial_amount"] = 0.03f;
    polyLead.parameters["wheel_mod_dest_freq_a"] = 1.0f;
    polyLead.parameters["wheel_mod_dest_freq_b"] = 1.0f;
    presets.push_back(polyLead);

    // [9] Nasty Feedback Lead
    Preset feedbackLead = initPatch;
    feedbackLead.name = "Nasty Feedback Lead";
    feedbackLead.parameters["osc_a_saw"] = 1.0f;
    feedbackLead.parameters["osc_b_freq"] = 1.0f;
    feedbackLead.parameters["osc_b_fine"] = 0.08f;
    feedbackLead.parameters["osc_b_saw"] = 1.0f;
    feedbackLead.parameters["mix_osc_a"] = 0.7f;
    feedbackLead.parameters["mix_osc_b"] = 0.6f;
    feedbackLead.parameters["vcf_cutoff"] = 300.0f;
    feedbackLead.parameters["vcf_reson"] = 0.75f;
    feedbackLead.parameters["vcf_env_amt"] = 0.7f;
    feedbackLead.parameters["poly_mod_osc_b_amt"] = 0.45f; // Modulate OSC A and VCF with OSC B
    feedbackLead.parameters["poly_mod_dest_freq_a"] = 1.0f;
    feedbackLead.parameters["poly_mod_dest_vcf"] = 1.0f;
    feedbackLead.parameters["env_a_atk"] = 5.0f;
    feedbackLead.parameters["env_a_dcy"] = 500.0f;
    feedbackLead.parameters["env_a_sus"] = 0.3f;
    feedbackLead.parameters["env_a_rel"] = 200.0f;
    presets.push_back(feedbackLead);

    // [10] Soft Triangle Lead
    Preset softLead = initPatch;
    softLead.name = "Soft Triangle Lead";
    softLead.parameters["osc_a_saw"] = 0.0f;
    softLead.parameters["osc_a_sqr"] = 1.0f;
    softLead.parameters["osc_a_pw"] = 50.0f;
    softLead.parameters["osc_b_tri"] = 1.0f;
    softLead.parameters["osc_b_fine"] = 0.02f;
    softLead.parameters["mix_osc_a"] = 0.4f;
    softLead.parameters["mix_osc_b"] = 0.8f;
    softLead.parameters["vcf_cutoff"] = 600.0f;
    softLead.parameters["vcf_reson"] = 0.1f;
    softLead.parameters["vcf_env_amt"] = 0.3f;
    softLead.parameters["env_a_atk"] = 80.0f;
    softLead.parameters["env_a_dcy"] = 400.0f;
    softLead.parameters["env_a_sus"] = 0.7f;
    softLead.parameters["env_a_rel"] = 300.0f;
    softLead.parameters["env_b_atk"] = 50.0f;
    softLead.parameters["env_b_dcy"] = 400.0f;
    softLead.parameters["env_b_sus"] = 0.9f;
    softLead.parameters["env_b_rel"] = 300.0f;
    softLead.parameters["lfo_initial_amount"] = 0.04f;
    softLead.parameters["wheel_mod_dest_freq_a"] = 1.0f;
    presets.push_back(softLead);

    // [11] Classic Mono Lead
    Preset waveLead = initPatch;
    waveLead.name = "Classic Mono Lead";
    waveLead.parameters["unison"] = 1.0f;
    waveLead.parameters["osc_a_sqr"] = 1.0f;
    waveLead.parameters["osc_a_pw"] = 50.0f;
    waveLead.parameters["osc_b_saw"] = 1.0f;
    waveLead.parameters["osc_b_fine"] = 0.04f;
    waveLead.parameters["mix_osc_a"] = 0.6f;
    waveLead.parameters["mix_osc_b"] = 0.6f;
    waveLead.parameters["vcf_cutoff"] = 800.0f;
    waveLead.parameters["vcf_reson"] = 0.2f;
    waveLead.parameters["vcf_env_amt"] = 0.5f;
    waveLead.parameters["env_a_atk"] = 10.0f;
    waveLead.parameters["env_a_dcy"] = 300.0f;
    waveLead.parameters["env_a_sus"] = 0.4f;
    waveLead.parameters["env_a_rel"] = 250.0f;
    waveLead.parameters["env_b_atk"] = 5.0f;
    waveLead.parameters["env_b_dcy"] = 400.0f;
    waveLead.parameters["env_b_sus"] = 0.8f;
    waveLead.parameters["env_b_rel"] = 250.0f;
    waveLead.parameters["wheel_mod_dest_pw_a"] = 1.0f;
    waveLead.parameters["wheel_mod_dest_filter"] = 1.0f;
    waveLead.parameters["velocity_to_amp"] = 0.5f;
    waveLead.parameters["velocity_to_filter"] = 0.4f;
    waveLead.parameters["glide"] = 20.0f;
    presets.push_back(waveLead);

    // [12] Sync Hard Lead
    Preset syncHard = initPatch;
    syncHard.name = "Sync Hard Lead";
    syncHard.parameters["osc_sync"] = 1.0f;
    syncHard.parameters["osc_a_saw"] = 1.0f;
    syncHard.parameters["osc_b_saw"] = 1.0f;
    syncHard.parameters["osc_b_freq"] = 2.5f;
    syncHard.parameters["mix_osc_a"] = 0.8f;
    syncHard.parameters["mix_osc_b"] = 0.0f;
    syncHard.parameters["vcf_cutoff"] = 2000.0f;
    syncHard.parameters["vcf_reson"] = 0.2f;
    syncHard.parameters["env_a_atk"] = 20.0f;
    syncHard.parameters["env_a_dcy"] = 800.0f;
    syncHard.parameters["env_a_sus"] = 0.5f;
    syncHard.parameters["env_a_rel"] = 400.0f;
    syncHard.parameters["env_b_atk"] = 5.0f;
    syncHard.parameters["env_b_dcy"] = 800.0f;
    syncHard.parameters["env_b_sus"] = 0.8f;
    syncHard.parameters["env_b_rel"] = 300.0f;
    syncHard.parameters["poly_mod_env_amt"] = 0.4f;
    syncHard.parameters["poly_mod_dest_freq_a"] = 1.0f;
    syncHard.parameters["unison"] = 1.0f; // UNISON: ON
    presets.push_back(syncHard);

    // =========================================================================
    // 3. Pad / Strings (6 voices)
    // =========================================================================

    // [13] Vintage Prophet Strings
    Preset prophetStrings = initPatch;
    prophetStrings.name = "Vintage Prophet Strings";
    prophetStrings.parameters["osc_a_saw"] = 1.0f;
    prophetStrings.parameters["osc_b_freq"] = 1.0f;
    prophetStrings.parameters["osc_b_fine"] = 0.08f;
    prophetStrings.parameters["osc_b_saw"] = 1.0f;
    prophetStrings.parameters["mix_osc_a"] = 0.65f;
    prophetStrings.parameters["mix_osc_b"] = 0.65f;
    prophetStrings.parameters["vcf_cutoff"] = 1200.0f;
    prophetStrings.parameters["vcf_reson"] = 0.1f;
    prophetStrings.parameters["vcf_env_amt"] = 0.3f;
    prophetStrings.parameters["env_a_atk"] = 250.0f;
    prophetStrings.parameters["env_a_dcy"] = 800.0f;
    prophetStrings.parameters["env_a_sus"] = 0.7f;
    prophetStrings.parameters["env_a_rel"] = 800.0f;
    prophetStrings.parameters["env_b_atk"] = 150.0f;
    prophetStrings.parameters["env_b_dcy"] = 600.0f;
    prophetStrings.parameters["env_b_sus"] = 0.9f;
    prophetStrings.parameters["env_b_rel"] = 700.0f;
    prophetStrings.parameters["wheel_mod_dest_filter"] = 0.5f;
    prophetStrings.parameters["wheel_mod_dest_pw_a"] = 0.5f;
    presets.push_back(prophetStrings);

    // [14] Warm Slowly Pad
    Preset slowlyPad = initPatch;
    slowlyPad.name = "Warm Slowly Pad";
    slowlyPad.parameters["osc_a_saw"] = 1.0f;
    slowlyPad.parameters["osc_b_tri"] = 1.0f;
    slowlyPad.parameters["osc_b_fine"] = 0.04f;
    slowlyPad.parameters["mix_osc_a"] = 0.5f;
    slowlyPad.parameters["mix_osc_b"] = 0.6f;
    slowlyPad.parameters["vcf_cutoff"] = 400.0f;
    slowlyPad.parameters["vcf_reson"] = 0.0f;
    slowlyPad.parameters["vcf_env_amt"] = 0.2f;
    slowlyPad.parameters["env_a_atk"] = 2000.0f;
    slowlyPad.parameters["env_a_dcy"] = 4000.0f;
    slowlyPad.parameters["env_a_sus"] = 0.8f;
    slowlyPad.parameters["env_a_rel"] = 2500.0f;
    slowlyPad.parameters["env_b_atk"] = 1500.0f;
    slowlyPad.parameters["env_b_dcy"] = 3000.0f;
    slowlyPad.parameters["env_b_sus"] = 1.0f;
    slowlyPad.parameters["env_b_rel"] = 2000.0f;
    presets.push_back(slowlyPad);

    // [15] Resonant Sweeper Pad
    Preset sweeperPad = initPatch;
    sweeperPad.name = "Resonant Sweeper Pad";
    sweeperPad.parameters["osc_a_saw"] = 1.0f;
    sweeperPad.parameters["osc_b_saw"] = 1.0f;
    sweeperPad.parameters["osc_b_fine"] = 0.06f;
    sweeperPad.parameters["mix_osc_a"] = 0.6f;
    sweeperPad.parameters["mix_osc_b"] = 0.6f;
    sweeperPad.parameters["vcf_cutoff"] = 300.0f;
    sweeperPad.parameters["vcf_reson"] = 0.6f;
    sweeperPad.parameters["vcf_env_amt"] = 0.5f;
    sweeperPad.parameters["env_a_atk"] = 1500.0f;
    sweeperPad.parameters["env_a_dcy"] = 2500.0f;
    sweeperPad.parameters["env_a_sus"] = 0.4f;
    sweeperPad.parameters["env_a_rel"] = 1200.0f;
    sweeperPad.parameters["env_b_atk"] = 800.0f;
    sweeperPad.parameters["env_b_dcy"] = 2000.0f;
    sweeperPad.parameters["env_b_sus"] = 0.9f;
    sweeperPad.parameters["env_b_rel"] = 1000.0f;
    sweeperPad.parameters["lfo_rate"] = 0.3f;
    sweeperPad.parameters["lfo_initial_amount"] = 0.2f;
    sweeperPad.parameters["wheel_mod_dest_filter"] = 1.0f;
    presets.push_back(sweeperPad);

    // [16] Majestic Poly Pad
    Preset majesticPad = initPatch;
    majesticPad.name = "Majestic Poly Pad";
    majesticPad.parameters["osc_a_saw"] = 1.0f;
    majesticPad.parameters["osc_b_sqr"] = 1.0f;
    majesticPad.parameters["osc_b_fine"] = 0.05f;
    majesticPad.parameters["osc_b_pw"] = 45.0f;
    majesticPad.parameters["mix_osc_a"] = 0.7f;
    majesticPad.parameters["mix_osc_b"] = 0.5f;
    majesticPad.parameters["vcf_cutoff"] = 800.0f;
    majesticPad.parameters["vcf_reson"] = 0.15f;
    majesticPad.parameters["vcf_env_amt"] = 0.4f;
    majesticPad.parameters["env_a_atk"] = 600.0f;
    majesticPad.parameters["env_a_dcy"] = 1500.0f;
    majesticPad.parameters["env_a_sus"] = 0.7f;
    majesticPad.parameters["env_a_rel"] = 1000.0f;
    majesticPad.parameters["env_b_atk"] = 400.0f;
    majesticPad.parameters["env_b_dcy"] = 1200.0f;
    majesticPad.parameters["env_b_sus"] = 0.9f;
    majesticPad.parameters["env_b_rel"] = 800.0f;
    majesticPad.parameters["wheel_mod_dest_freq_a"] = 0.2f;
    majesticPad.parameters["wheel_mod_dest_pw_a"] = 0.5f;
    presets.push_back(majesticPad);

    // [17] Dark Drone Pad
    Preset dronePad = initPatch;
    dronePad.name = "Dark Drone Pad";
    dronePad.parameters["osc_a_freq"] = 0.5f;
    dronePad.parameters["osc_a_saw"] = 1.0f;
    dronePad.parameters["osc_b_freq"] = 0.5f;
    dronePad.parameters["osc_b_fine"] = 0.04f;
    dronePad.parameters["osc_b_tri"] = 1.0f;
    dronePad.parameters["mix_osc_a"] = 0.4f;
    dronePad.parameters["mix_osc_b"] = 0.5f;
    dronePad.parameters["mix_noise"] = 0.2f; // Mix noise
    dronePad.parameters["vcf_cutoff"] = 250.0f;
    dronePad.parameters["vcf_reson"] = 0.3f;
    dronePad.parameters["vcf_env_amt"] = 0.3f;
    dronePad.parameters["env_a_atk"] = 3000.0f;
    dronePad.parameters["env_a_dcy"] = 5000.0f;
    dronePad.parameters["env_a_sus"] = 0.8f;
    dronePad.parameters["env_a_rel"] = 3000.0f;
    dronePad.parameters["env_b_atk"] = 2500.0f;
    dronePad.parameters["env_b_dcy"] = 4000.0f;
    dronePad.parameters["env_b_sus"] = 1.0f;
    dronePad.parameters["env_b_rel"] = 2500.0f;
    dronePad.parameters["poly_mod_osc_b_amt"] = 0.25f;
    dronePad.parameters["poly_mod_dest_vcf"] = 1.0f;
    dronePad.parameters["lfo_rate"] = 0.5f;
    dronePad.parameters["lfo_initial_amount"] = 0.1f;
    dronePad.parameters["wheel_mod_dest_filter"] = 1.0f;
    presets.push_back(dronePad);

    // [18] Slow Cinematic Sweep
    Preset cineSweep = initPatch;
    cineSweep.name = "Slow Cinematic Sweep";
    cineSweep.parameters["osc_a_saw"] = 1.0f;
    cineSweep.parameters["osc_b_tri"] = 1.0f;
    cineSweep.parameters["osc_b_fine"] = 0.05f;
    cineSweep.parameters["mix_osc_a"] = 0.5f;
    cineSweep.parameters["mix_osc_b"] = 0.5f;
    cineSweep.parameters["vcf_cutoff"] = 150.0f;
    cineSweep.parameters["vcf_reson"] = 0.3f;
    cineSweep.parameters["vcf_env_amt"] = 0.6f;
    cineSweep.parameters["env_a_atk"] = 3500.0f;
    cineSweep.parameters["env_a_dcy"] = 2000.0f;
    cineSweep.parameters["env_a_sus"] = 0.7f;
    cineSweep.parameters["env_a_rel"] = 2500.0f;
    cineSweep.parameters["env_b_atk"] = 3000.0f;
    cineSweep.parameters["env_b_dcy"] = 2000.0f;
    cineSweep.parameters["env_b_sus"] = 0.8f;
    cineSweep.parameters["env_b_rel"] = 2500.0f;
    cineSweep.parameters["lfo_rate"] = 1.2f;
    cineSweep.parameters["lfo_initial_amount"] = 0.2f;
    cineSweep.parameters["wheel_mod_dest_filter"] = 1.0f;
    presets.push_back(cineSweep);

    // =========================================================================
    // 4. Brass (6 voices)
    // =========================================================================

    // [19] Prophet Brass 5 (Famous "33 Brass" type)
    Preset prophetBrass = initPatch;
    prophetBrass.name = "Prophet Brass 5";
    prophetBrass.parameters["osc_a_saw"] = 1.0f;
    prophetBrass.parameters["osc_b_freq"] = 1.0f;
    prophetBrass.parameters["osc_b_fine"] = 0.06f;
    prophetBrass.parameters["osc_b_saw"] = 1.0f;
    prophetBrass.parameters["mix_osc_a"] = 0.6f;
    prophetBrass.parameters["mix_osc_b"] = 0.6f;
    prophetBrass.parameters["vcf_cutoff"] = 350.0f;
    prophetBrass.parameters["vcf_reson"] = 0.15f;
    prophetBrass.parameters["vcf_env_amt"] = 0.65f;
    prophetBrass.parameters["vcf_kb_track"] = 2.0f; // FULL
    prophetBrass.parameters["env_a_atk"] = 80.0f;
    prophetBrass.parameters["env_a_dcy"] = 600.0f;
    prophetBrass.parameters["env_a_sus"] = 0.6f;
    prophetBrass.parameters["env_a_rel"] = 300.0f;
    prophetBrass.parameters["env_b_atk"] = 10.0f;
    prophetBrass.parameters["env_b_dcy"] = 800.0f;
    prophetBrass.parameters["env_b_sus"] = 0.9f;
    prophetBrass.parameters["env_b_rel"] = 300.0f;
    presets.push_back(prophetBrass);

    // [20] Swell Synth Brass
    Preset swellBrass = initPatch;
    swellBrass.name = "Swell Synth Brass";
    swellBrass.parameters["osc_a_saw"] = 1.0f;
    swellBrass.parameters["osc_b_freq"] = 1.0f;
    swellBrass.parameters["osc_b_fine"] = 0.05f;
    swellBrass.parameters["osc_b_saw"] = 1.0f;
    swellBrass.parameters["mix_osc_a"] = 0.6f;
    swellBrass.parameters["mix_osc_b"] = 0.6f;
    swellBrass.parameters["vcf_cutoff"] = 200.0f;
    swellBrass.parameters["vcf_reson"] = 0.1f;
    swellBrass.parameters["vcf_env_amt"] = 0.5f;
    swellBrass.parameters["vcf_kb_track"] = 2.0f;
    swellBrass.parameters["env_a_atk"] = 250.0f;
    swellBrass.parameters["env_a_dcy"] = 1000.0f;
    swellBrass.parameters["env_a_sus"] = 0.5f;
    swellBrass.parameters["env_a_rel"] = 500.0f;
    swellBrass.parameters["env_b_atk"] = 150.0f;
    swellBrass.parameters["env_b_dcy"] = 1200.0f;
    swellBrass.parameters["env_b_sus"] = 0.9f;
    swellBrass.parameters["env_b_rel"] = 500.0f;
    presets.push_back(swellBrass);

    // [21] Muted Trumpet
    Preset mutedBrass = initPatch;
    mutedBrass.name = "Muted Trumpet";
    mutedBrass.parameters["osc_a_saw"] = 1.0f;
    mutedBrass.parameters["osc_b_sqr"] = 1.0f;
    mutedBrass.parameters["osc_b_fine"] = 0.03f;
    mutedBrass.parameters["osc_b_pw"] = 55.0f;
    mutedBrass.parameters["mix_osc_a"] = 0.5f;
    mutedBrass.parameters["mix_osc_b"] = 0.5f;
    mutedBrass.parameters["vcf_cutoff"] = 180.0f;
    mutedBrass.parameters["vcf_reson"] = 0.4f;
    mutedBrass.parameters["vcf_env_amt"] = 0.55f;
    mutedBrass.parameters["env_a_atk"] = 20.0f;
    mutedBrass.parameters["env_a_dcy"] = 150.0f;
    mutedBrass.parameters["env_a_sus"] = 0.0f;
    mutedBrass.parameters["env_a_rel"] = 100.0f;
    mutedBrass.parameters["env_b_atk"] = 5.0f;
    mutedBrass.parameters["env_b_dcy"] = 250.0f;
    mutedBrass.parameters["env_b_sus"] = 0.8f;
    mutedBrass.parameters["env_b_rel"] = 120.0f;
    mutedBrass.parameters["velocity_to_filter"] = 0.4f;
    mutedBrass.parameters["velocity_to_amp"] = 0.3f;
    presets.push_back(mutedBrass);

    // [22] Fat Poly Brass
    Preset uniBrassPatch = initPatch;
    uniBrassPatch.name = "Fat Poly Brass";
    uniBrassPatch.parameters["unison"] = 0.0f;
    uniBrassPatch.parameters["osc_a_saw"] = 1.0f;
    uniBrassPatch.parameters["osc_b_freq"] = 1.0f;
    uniBrassPatch.parameters["osc_b_fine"] = 0.07f;
    uniBrassPatch.parameters["osc_b_saw"] = 1.0f;
    uniBrassPatch.parameters["mix_osc_a"] = 0.7f;
    uniBrassPatch.parameters["mix_osc_b"] = 0.7f;
    uniBrassPatch.parameters["vcf_cutoff"] = 400.0f;
    uniBrassPatch.parameters["vcf_reson"] = 0.2f;
    uniBrassPatch.parameters["vcf_env_amt"] = 0.6f;
    uniBrassPatch.parameters["vcf_kb_track"] = 2.0f;
    uniBrassPatch.parameters["env_a_atk"] = 60.0f;
    uniBrassPatch.parameters["env_a_dcy"] = 500.0f;
    uniBrassPatch.parameters["env_a_sus"] = 0.7f;
    uniBrassPatch.parameters["env_a_rel"] = 300.0f;
    uniBrassPatch.parameters["env_b_atk"] = 15.0f;
    uniBrassPatch.parameters["env_b_dcy"] = 600.0f;
    uniBrassPatch.parameters["env_b_sus"] = 0.9f;
    uniBrassPatch.parameters["env_b_rel"] = 300.0f;
    uniBrassPatch.parameters["poly_mod_osc_b_amt"] = 0.42f;
    uniBrassPatch.parameters["poly_mod_dest_freq_a"] = 1.0f;
    presets.push_back(uniBrassPatch);

    // [23] Modern Hybrid Brass
    Preset hybridBrass = initPatch;
    hybridBrass.name = "Modern Hybrid Brass";
    hybridBrass.parameters["osc_a_saw"] = 1.0f;
    hybridBrass.parameters["osc_b_sqr"] = 1.0f;
    hybridBrass.parameters["osc_b_fine"] = 0.04f;
    hybridBrass.parameters["osc_b_pw"] = 50.0f;
    hybridBrass.parameters["mix_osc_a"] = 0.7f;
    hybridBrass.parameters["mix_osc_b"] = 0.5f;
    hybridBrass.parameters["vcf_cutoff"] = 500.0f;
    hybridBrass.parameters["vcf_reson"] = 0.1f;
    hybridBrass.parameters["vcf_env_amt"] = 0.7f;
    hybridBrass.parameters["env_a_atk"] = 5.0f;
    hybridBrass.parameters["env_a_dcy"] = 350.0f;
    hybridBrass.parameters["env_a_sus"] = 0.5f;
    hybridBrass.parameters["env_a_rel"] = 200.0f;
    hybridBrass.parameters["env_b_atk"] = 2.0f;
    hybridBrass.parameters["env_b_dcy"] = 400.0f;
    hybridBrass.parameters["env_b_sus"] = 0.8f;
    hybridBrass.parameters["env_b_rel"] = 200.0f;
    hybridBrass.parameters["velocity_to_filter"] = 0.5f;
    hybridBrass.parameters["velocity_to_amp"] = 0.4f;
    presets.push_back(hybridBrass);

    // [24] Analog Horn Solo
    Preset hornSolo = initPatch;
    hornSolo.name = "Analog Horn Solo";
    hornSolo.parameters["osc_a_saw"] = 1.0f;
    hornSolo.parameters["osc_b_saw"] = 1.0f;
    hornSolo.parameters["osc_b_fine"] = 0.03f;
    hornSolo.parameters["mix_osc_a"] = 0.6f;
    hornSolo.parameters["mix_osc_b"] = 0.5f;
    hornSolo.parameters["vcf_cutoff"] = 300.0f; // Cutoff: 450 -> 300
    hornSolo.parameters["vcf_reson"] = 0.1f;
    hornSolo.parameters["vcf_env_amt"] = 0.5f;
    hornSolo.parameters["env_a_atk"] = 80.0f;
    hornSolo.parameters["env_a_dcy"] = 1200.0f;
    hornSolo.parameters["env_a_sus"] = 0.7f;
    hornSolo.parameters["env_a_rel"] = 500.0f;
    hornSolo.parameters["env_b_atk"] = 60.0f;
    hornSolo.parameters["env_b_dcy"] = 1200.0f;
    hornSolo.parameters["env_b_sus"] = 0.8f;
    hornSolo.parameters["env_b_rel"] = 400.0f;
    hornSolo.parameters["lfo_rate"] = 4.5f;
    hornSolo.parameters["lfo_initial_amount"] = 0.02f;
    hornSolo.parameters["wheel_mod_dest_freq_a"] = 0.5f;
    hornSolo.parameters["wheel_mod_dest_freq_b"] = 0.5f;
    presets.push_back(hornSolo);

    // =========================================================================
    // 5. Keyboards (6 voices)
    // =========================================================================

    // [25] Classic E.Piano
    Preset analogEp = initPatch;
    analogEp.name = "Classic E.Piano";
    analogEp.parameters["osc_a_saw"] = 1.0f;
    analogEp.parameters["osc_a_sqr"] = 1.0f;
    analogEp.parameters["osc_a_pw"] = 65.0f;
    analogEp.parameters["osc_b_freq"] = 1.0f;
    analogEp.parameters["osc_b_tri"] = 1.0f;
    analogEp.parameters["osc_b_fine"] = 0.02f;
    analogEp.parameters["mix_osc_a"] = 0.7f;
    analogEp.parameters["mix_osc_b"] = 0.1f;
    analogEp.parameters["vcf_cutoff"] = 600.0f;
    analogEp.parameters["vcf_reson"] = 0.2f;
    analogEp.parameters["vcf_env_amt"] = 0.0f;
    analogEp.parameters["vcf_kb_track"] = 2.0f; // HALF -> FULL
    analogEp.parameters["env_a_atk"] = 1.0f;
    analogEp.parameters["env_a_dcy"] = 1.0f;
    analogEp.parameters["env_a_sus"] = 0.0f;
    analogEp.parameters["env_a_rel"] = 1.0f;
    analogEp.parameters["env_b_atk"] = 1.0f;
    analogEp.parameters["env_b_dcy"] = 1900.0f; // VCA Decay: 1500 -> 1900
    analogEp.parameters["env_b_sus"] = 0.0f;
    analogEp.parameters["env_b_rel"] = 430.0f; // VCA Release: 600 -> 430
    analogEp.parameters["poly_mod_osc_b_amt"] = 0.25f;
    analogEp.parameters["poly_mod_dest_freq_a"] = 1.0f;
    analogEp.parameters["velocity_to_amp"] = 0.5f;
    analogEp.parameters["velocity_to_filter"] = 0.5f; // Set filter response to velocity to 0.5
    presets.push_back(analogEp);

    // [26] Drawbar Organ
    Preset organ = initPatch;
    organ.name = "Drawbar Organ";
    organ.parameters["osc_a_saw"] = 0.0f;
    organ.parameters["osc_a_sqr"] = 1.0f;
    organ.parameters["osc_a_pw"] = 50.0f;
    organ.parameters["osc_b_freq"] = 1.5f; // Harmonic overtone a third above
    organ.parameters["osc_b_tri"] = 1.0f;
    organ.parameters["mix_osc_a"] = 0.7f;
    organ.parameters["mix_osc_b"] = 0.5f;
    organ.parameters["vcf_cutoff"] = 20000.0f;
    organ.parameters["vca_mode"] = 0.0f;
    organ.parameters["env_b_atk"] = 2.0f;
    organ.parameters["env_b_dcy"] = 200.0f;
    organ.parameters["env_b_sus"] = 1.0f;
    organ.parameters["env_b_rel"] = 15.0f;
    presets.push_back(organ);

    // [27] Prophet Clav
    Preset clav = initPatch;
    clav.name = "Prophet Clav";
    clav.parameters["osc_a_saw"] = 0.0f;
    clav.parameters["osc_a_sqr"] = 1.0f;
    clav.parameters["osc_a_pw"] = 75.0f; // Narrow pulse width
    clav.parameters["osc_b_sqr"] = 1.0f;
    clav.parameters["osc_b_fine"] = 0.03f;
    clav.parameters["osc_b_pw"] = 75.0f;
    clav.parameters["mix_osc_a"] = 0.6f;
    clav.parameters["mix_osc_b"] = 0.6f;
    clav.parameters["vcf_cutoff"] = 400.0f;
    clav.parameters["vcf_reson"] = 0.3f;
    clav.parameters["vcf_env_amt"] = 0.7f;
    clav.parameters["env_a_atk"] = 2.0f;
    clav.parameters["env_a_dcy"] = 120.0f;
    clav.parameters["env_a_sus"] = 0.0f;
    clav.parameters["env_a_rel"] = 80.0f;
    clav.parameters["env_b_atk"] = 2.0f;
    clav.parameters["env_b_dcy"] = 200.0f;
    clav.parameters["env_b_sus"] = 0.0f;
    clav.parameters["env_b_rel"] = 80.0f;
    clav.parameters["velocity_to_filter"] = 0.5f;
    clav.parameters["velocity_to_amp"] = 0.4f;
    presets.push_back(clav);

    // [28] Spaced Out EP
    Preset spacedEp = initPatch;
    spacedEp.name = "Spaced Out EP";
    spacedEp.parameters["osc_a_saw"] = 0.0f;
    spacedEp.parameters["osc_a_sqr"] = 1.0f;
    spacedEp.parameters["osc_a_pw"] = 50.0f;
    spacedEp.parameters["osc_b_tri"] = 1.0f;
    spacedEp.parameters["osc_b_fine"] = 0.05f;
    spacedEp.parameters["mix_osc_a"] = 0.5f;
    spacedEp.parameters["mix_osc_b"] = 0.6f;
    spacedEp.parameters["vcf_cutoff"] = 1000.0f;
    spacedEp.parameters["vcf_reson"] = 0.1f;
    spacedEp.parameters["vcf_env_amt"] = 0.3f;
    spacedEp.parameters["env_a_atk"] = 5.0f;
    spacedEp.parameters["env_a_dcy"] = 400.0f;
    spacedEp.parameters["env_a_sus"] = 0.0f;
    spacedEp.parameters["env_a_rel"] = 800.0f;
    spacedEp.parameters["env_b_atk"] = 5.0f;
    spacedEp.parameters["env_b_dcy"] = 600.0f;
    spacedEp.parameters["env_b_sus"] = 0.0f;
    spacedEp.parameters["env_b_rel"] = 800.0f;
    spacedEp.parameters["lfo_initial_amount"] = 0.06f; // Modulation / swell
    spacedEp.parameters["wheel_mod_dest_freq_a"] = 1.0f;
    spacedEp.parameters["wheel_mod_dest_freq_b"] = 1.0f;
    presets.push_back(spacedEp);

    // [29] Analog Harpsichord
    Preset harpsichord = initPatch;
    harpsichord.name = "Analog Harpsichord";
    harpsichord.parameters["osc_a_saw"] = 0.0f;
    harpsichord.parameters["osc_a_sqr"] = 1.0f;
    harpsichord.parameters["osc_a_pw"] = 90.0f; // Extremely narrow
    harpsichord.parameters["osc_b_freq"] = 2.0f; // One octave up
    harpsichord.parameters["osc_b_sqr"] = 1.0f;
    harpsichord.parameters["osc_b_fine"] = 0.02f;
    harpsichord.parameters["osc_b_pw"] = 90.0f;
    harpsichord.parameters["mix_osc_a"] = 0.8f;
    harpsichord.parameters["mix_osc_b"] = 0.4f;
    harpsichord.parameters["vcf_cutoff"] = 2500.0f;
    harpsichord.parameters["vcf_reson"] = 0.1f;
    harpsichord.parameters["vcf_env_amt"] = 0.0f;
    harpsichord.parameters["env_b_atk"] = 1.0f;
    harpsichord.parameters["env_b_dcy"] = 300.0f;
    harpsichord.parameters["env_b_sus"] = 0.0f;
    harpsichord.parameters["env_b_rel"] = 200.0f;
    presets.push_back(harpsichord);

    // [30] Wurli Reed EP
    Preset wurliEp = initPatch;
    wurliEp.name = "Wurli Reed EP";
    wurliEp.parameters["osc_a_sqr"] = 1.0f;
    wurliEp.parameters["osc_a_pw"] = 30.0f;
    wurliEp.parameters["osc_b_tri"] = 1.0f;
    wurliEp.parameters["mix_osc_a"] = 0.4f;
    wurliEp.parameters["mix_osc_b"] = 0.7f;
    wurliEp.parameters["poly_mod_osc_b_amt"] = 0.36f;
    wurliEp.parameters["poly_mod_dest_freq_a"] = 1.0f;
    wurliEp.parameters["vcf_cutoff"] = 468.00f;
    wurliEp.parameters["vcf_reson"] = 0.15f;
    wurliEp.parameters["vcf_env_amt"] = 0.4f;
    wurliEp.parameters["vcf_kb_track"] = 2.0f; // FULL
    wurliEp.parameters["env_a_atk"] = 2.0f;
    wurliEp.parameters["env_a_dcy"] = 350.0f;
    wurliEp.parameters["env_a_sus"] = 0.0f;
    wurliEp.parameters["env_a_rel"] = 300.0f;
    wurliEp.parameters["env_b_atk"] = 1.0f;
    wurliEp.parameters["env_b_dcy"] = 700.0f;
    wurliEp.parameters["env_b_sus"] = 0.0f;
    wurliEp.parameters["env_b_rel"] = 300.0f;
    wurliEp.parameters["lfo_rate"] = 5.5f;
    wurliEp.parameters["lfo_initial_amount"] = 0.03f;
    wurliEp.parameters["wheel_mod_dest_freq_a"] = 0.3f;
    wurliEp.parameters["velocity_to_filter"] = 0.38f;
    wurliEp.parameters["velocity_to_amp"] = 0.50f;
    presets.push_back(wurliEp);

    // =========================================================================
    // 6. Guitar / Pluck (6 voices)
    // =========================================================================

    // [31] Synthesized Acoustic
    Preset acousticGit = initPatch;
    acousticGit.name = "Synthesized Acoustic";
    acousticGit.parameters["osc_a_saw"] = 1.0f;
    acousticGit.parameters["osc_b_freq"] = 2.0f;
    acousticGit.parameters["osc_b_sqr"] = 1.0f;
    acousticGit.parameters["osc_b_fine"] = 0.05f;
    acousticGit.parameters["osc_b_pw"] = 80.0f;
    acousticGit.parameters["mix_osc_a"] = 0.5f;
    acousticGit.parameters["mix_osc_b"] = 0.5f;
    acousticGit.parameters["vcf_cutoff"] = 1300.0f;
    acousticGit.parameters["vcf_reson"] = 0.15f;
    acousticGit.parameters["vcf_env_amt"] = 0.4f;
    acousticGit.parameters["env_a_atk"] = 5.0f;
    acousticGit.parameters["env_a_dcy"] = 250.0f;
    acousticGit.parameters["env_a_sus"] = 0.0f;
    acousticGit.parameters["env_a_rel"] = 300.0f;
    acousticGit.parameters["env_b_atk"] = 2.0f;
    acousticGit.parameters["env_b_dcy"] = 400.0f;
    acousticGit.parameters["env_b_sus"] = 0.0f;
    acousticGit.parameters["env_b_rel"] = 300.0f;
    acousticGit.parameters["velocity_to_amp"] = 0.4f;
    acousticGit.parameters["velocity_to_filter"] = 0.3f;
    presets.push_back(acousticGit);

    // [32] Classic Pluck Arp
    Preset pluckArp = initPatch;
    pluckArp.name = "Classic Pluck Arp";
    pluckArp.parameters["osc_a_saw"] = 1.0f;
    pluckArp.parameters["osc_b_sqr"] = 1.0f;
    pluckArp.parameters["osc_b_fine"] = 0.04f;
    pluckArp.parameters["mix_osc_a"] = 0.6f;
    pluckArp.parameters["mix_osc_b"] = 0.5f;
    pluckArp.parameters["vcf_cutoff"] = 180.0f;
    pluckArp.parameters["vcf_reson"] = 0.3f;
    pluckArp.parameters["vcf_env_amt"] = 0.8f;
    pluckArp.parameters["env_a_atk"] = 1.0f;
    pluckArp.parameters["env_a_dcy"] = 110.0f;
    pluckArp.parameters["env_a_sus"] = 0.0f;
    pluckArp.parameters["env_a_rel"] = 80.0f;
    pluckArp.parameters["env_b_atk"] = 1.0f;
    pluckArp.parameters["env_b_dcy"] = 180.0f;
    pluckArp.parameters["env_b_sus"] = 0.0f;
    pluckArp.parameters["env_b_rel"] = 80.0f;
    pluckArp.parameters["velocity_to_filter"] = 0.4f;
    presets.push_back(pluckArp);

    // [33] Muted Pluck
    Preset mutedPluck = initPatch;
    mutedPluck.name = "Muted Pluck";
    mutedPluck.parameters["osc_a_saw"] = 0.0f;
    mutedPluck.parameters["osc_a_sqr"] = 1.0f;
    mutedPluck.parameters["osc_a_pw"] = 70.0f;
    mutedPluck.parameters["osc_b_sqr"] = 1.0f;
    mutedPluck.parameters["osc_b_fine"] = 0.03f;
    mutedPluck.parameters["osc_b_pw"] = 70.0f;
    mutedPluck.parameters["mix_osc_a"] = 0.6f;
    mutedPluck.parameters["mix_osc_b"] = 0.4f;
    mutedPluck.parameters["vcf_cutoff"] = 120.0f;
    mutedPluck.parameters["vcf_reson"] = 0.5f;
    mutedPluck.parameters["vcf_env_amt"] = 0.5f;
    mutedPluck.parameters["env_a_atk"] = 2.0f;
    mutedPluck.parameters["env_a_dcy"] = 80.0f;
    mutedPluck.parameters["env_a_sus"] = 0.0f;
    mutedPluck.parameters["env_a_rel"] = 60.0f;
    mutedPluck.parameters["env_b_atk"] = 2.0f;
    mutedPluck.parameters["env_b_dcy"] = 120.0f;
    mutedPluck.parameters["env_b_sus"] = 0.0f;
    mutedPluck.parameters["env_b_rel"] = 60.0f;
    presets.push_back(mutedPluck);

    // [34] Dreamy Pluck Pad
    Preset pluckPad = initPatch;
    pluckPad.name = "Dreamy Pluck Pad";
    pluckPad.parameters["osc_a_saw"] = 1.0f;
    pluckPad.parameters["osc_b_tri"] = 1.0f;
    pluckPad.parameters["osc_b_fine"] = 0.04f;
    pluckPad.parameters["mix_osc_a"] = 0.5f;
    pluckPad.parameters["mix_osc_b"] = 0.6f;
    pluckPad.parameters["vcf_cutoff"] = 300.0f;
    pluckPad.parameters["vcf_reson"] = 0.2f;
    pluckPad.parameters["vcf_env_amt"] = 0.5f;
    pluckPad.parameters["env_a_atk"] = 5.0f;
    pluckPad.parameters["env_a_dcy"] = 1000.0f;
    pluckPad.parameters["env_a_sus"] = 0.3f;
    pluckPad.parameters["env_a_rel"] = 1500.0f;
    pluckPad.parameters["env_b_atk"] = 2.5f;
    pluckPad.parameters["env_b_dcy"] = 800.0f;
    pluckPad.parameters["env_b_sus"] = 0.8f;
    pluckPad.parameters["env_b_rel"] = 1200.0f;
    pluckPad.parameters["wheel_mod_dest_filter"] = 0.5f;
    presets.push_back(pluckPad);

    // [35] Unison Heavy Pluck
    Preset heavyPluck = initPatch;
    heavyPluck.name = "Unison Heavy Pluck";
    heavyPluck.parameters["unison"] = 1.0f;
    heavyPluck.parameters["osc_a_saw"] = 1.0f;
    heavyPluck.parameters["osc_b_sqr"] = 1.0f;
    heavyPluck.parameters["osc_b_fine"] = 0.06f;
    heavyPluck.parameters["mix_osc_a"] = 0.7f;
    heavyPluck.parameters["mix_osc_b"] = 0.7f;
    heavyPluck.parameters["vcf_cutoff"] = 350.0f;
    heavyPluck.parameters["vcf_reson"] = 0.1f;
    heavyPluck.parameters["vcf_env_amt"] = 0.7f;
    heavyPluck.parameters["env_a_atk"] = 2.0f;
    heavyPluck.parameters["env_a_dcy"] = 250.0f;
    heavyPluck.parameters["env_a_sus"] = 0.0f;
    heavyPluck.parameters["env_a_rel"] = 150.0f;
    heavyPluck.parameters["env_b_atk"] = 2.0f;
    heavyPluck.parameters["env_b_dcy"] = 300.0f;
    heavyPluck.parameters["env_b_sus"] = 0.0f;
    heavyPluck.parameters["env_b_rel"] = 150.0f;
    heavyPluck.parameters["velocity_to_filter"] = 0.4f;
    presets.push_back(heavyPluck);

    // [36] Harp Pluck
    Preset harpPluck = initPatch;
    harpPluck.name = "Harp Pluck";
    harpPluck.parameters["osc_sync"] = 1.0f;
    harpPluck.parameters["osc_a_sqr"] = 1.0f;
    harpPluck.parameters["osc_a_pw"] = 50.0f;
    harpPluck.parameters["osc_b_tri"] = 1.0f;
    harpPluck.parameters["mix_osc_a"] = 0.3f;
    harpPluck.parameters["mix_osc_b"] = 0.8f;
    harpPluck.parameters["vcf_cutoff"] = 955.0f;
    harpPluck.parameters["vcf_reson"] = 0.1f;
    harpPluck.parameters["vcf_env_amt"] = 0.5f;
    harpPluck.parameters["env_a_atk"] = 1.0f;
    harpPluck.parameters["env_a_dcy"] = 180.0f;
    harpPluck.parameters["env_a_sus"] = 0.0f;
    harpPluck.parameters["env_a_rel"] = 250.0f;
    harpPluck.parameters["env_b_atk"] = 1.0f;
    harpPluck.parameters["env_b_dcy"] = 510.0f;
    harpPluck.parameters["env_b_sus"] = 0.0f;
    harpPluck.parameters["env_b_rel"] = 1000.0f;
    presets.push_back(harpPluck);

    // =========================================================================
    // 7. Bell / Mallet (6 voices)
    // =========================================================================

    // [37] Crystal Bell (Metal FM)
    Preset crystalBell = initPatch;
    crystalBell.name = "Crystal Bell";
    crystalBell.parameters["osc_a_saw"] = 0.0f;
    crystalBell.parameters["osc_a_sqr"] = 1.0f;
    crystalBell.parameters["osc_a_pw"] = 50.0f;
    crystalBell.parameters["osc_b_freq"] = 3.0f; // One octave + a fifth above
    crystalBell.parameters["osc_b_tri"] = 1.0f;
    crystalBell.parameters["mix_osc_a"] = 0.7f;
    crystalBell.parameters["mix_osc_b"] = 0.0f; // OSC B volume is cut (used as modulator)
    crystalBell.parameters["vcf_cutoff"] = 800.0f;
    crystalBell.parameters["vcf_reson"] = 0.2f;
    crystalBell.parameters["vcf_env_amt"] = 0.6f;
    crystalBell.parameters["vcf_kb_track"] = 2.0f;
    crystalBell.parameters["env_a_atk"] = 1.0f;
    crystalBell.parameters["env_a_dcy"] = 250.0f;
    crystalBell.parameters["env_a_sus"] = 0.0f;
    crystalBell.parameters["env_a_rel"] = 1500.0f;
    crystalBell.parameters["env_b_atk"] = 1.0f;
    crystalBell.parameters["env_b_dcy"] = 400.0f;
    crystalBell.parameters["env_b_sus"] = 0.0f;
    crystalBell.parameters["env_b_rel"] = 1500.0f;
    crystalBell.parameters["poly_mod_env_amt"] = 0.0f;
    crystalBell.parameters["poly_mod_osc_b_amt"] = 0.65f; // Deep modulation to create metallic FM sound
    crystalBell.parameters["poly_mod_dest_freq_a"] = 0.0f;
    crystalBell.parameters["poly_mod_dest_vcf"] = 1.0f;
    presets.push_back(crystalBell);

    // [38] Toy Glockenspiel
    Preset glockenspiel = initPatch;
    glockenspiel.name = "Toy Glockenspiel";
    glockenspiel.parameters["osc_a_freq"] = 4.0f; // High pitch
    glockenspiel.parameters["osc_a_saw"] = 0.0f;
    glockenspiel.parameters["osc_a_sqr"] = 1.0f;
    glockenspiel.parameters["osc_a_pw"] = 50.0f;
    glockenspiel.parameters["osc_b_freq"] = 4.0f;
    glockenspiel.parameters["osc_b_fine"] = 0.03f;
    glockenspiel.parameters["osc_b_tri"] = 1.0f;
    glockenspiel.parameters["mix_osc_a"] = 0.8f;
    glockenspiel.parameters["mix_osc_b"] = 0.3f;
    glockenspiel.parameters["vcf_cutoff"] = 1500.0f;
    glockenspiel.parameters["vcf_reson"] = 0.1f;
    glockenspiel.parameters["vcf_env_amt"] = 0.3f;
    glockenspiel.parameters["vcf_kb_track"] = 2.0f;
    glockenspiel.parameters["env_a_atk"] = 1.0f;
    glockenspiel.parameters["env_a_dcy"] = 150.0f;
    glockenspiel.parameters["env_a_sus"] = 0.0f;
    glockenspiel.parameters["env_a_rel"] = 600.0f;
    glockenspiel.parameters["env_b_atk"] = 1.0f;
    glockenspiel.parameters["env_b_dcy"] = 200.0f;
    glockenspiel.parameters["env_b_sus"] = 0.0f;
    glockenspiel.parameters["env_b_rel"] = 600.0f;
    presets.push_back(glockenspiel);

    // [39] Vibraphone Warm
    Preset vibraphone = initPatch;
    vibraphone.name = "Vibraphone Warm";
    vibraphone.parameters["osc_a_saw"] = 0.0f;
    vibraphone.parameters["osc_a_sqr"] = 1.0f;
    vibraphone.parameters["osc_a_pw"] = 50.0f;
    vibraphone.parameters["osc_b_tri"] = 1.0f;
    vibraphone.parameters["osc_b_fine"] = 0.04f;
    vibraphone.parameters["mix_osc_a"] = 0.4f;
    vibraphone.parameters["mix_osc_b"] = 0.8f;
    vibraphone.parameters["vcf_cutoff"] = 500.0f;
    vibraphone.parameters["vcf_reson"] = 0.1f;
    vibraphone.parameters["vcf_env_amt"] = 0.3f;
    vibraphone.parameters["vcf_kb_track"] = 2.0f;
    vibraphone.parameters["env_a_atk"] = 5.0f;
    vibraphone.parameters["env_a_dcy"] = 600.0f;
    vibraphone.parameters["env_a_sus"] = 0.0f;
    vibraphone.parameters["env_a_rel"] = 1000.0f;
    vibraphone.parameters["env_b_atk"] = 5.0f;
    vibraphone.parameters["env_b_dcy"] = 800.0f;
    vibraphone.parameters["env_b_sus"] = 0.0f;
    vibraphone.parameters["env_b_rel"] = 1000.0f;
    vibraphone.parameters["lfo_initial_amount"] = 0.04f;
    vibraphone.parameters["wheel_mod_dest_freq_a"] = 0.5f;
    vibraphone.parameters["wheel_mod_dest_freq_b"] = 0.5f;
    presets.push_back(vibraphone);

    // [40] Space Metal Bell
    Preset spaceBell = initPatch;
    spaceBell.name = "Space Metal Bell";
    spaceBell.parameters["osc_a_saw"] = 0.0f;
    spaceBell.parameters["osc_a_sqr"] = 1.0f;
    spaceBell.parameters["osc_a_pw"] = 50.0f;
    spaceBell.parameters["osc_b_freq"] = 2.8f; // Inharmonic overtone
    spaceBell.parameters["osc_b_sqr"] = 1.0f;
    spaceBell.parameters["osc_b_pw"] = 50.0f;
    spaceBell.parameters["mix_osc_a"] = 0.6f;
    spaceBell.parameters["mix_osc_b"] = 0.3f;
    spaceBell.parameters["vcf_cutoff"] = 400.0f;
    spaceBell.parameters["vcf_reson"] = 0.4f;
    spaceBell.parameters["vcf_env_amt"] = 0.6f;
    spaceBell.parameters["env_a_atk"] = 2.0f;
    spaceBell.parameters["env_a_dcy"] = 500.0f;
    spaceBell.parameters["env_a_sus"] = 0.0f;
    spaceBell.parameters["env_a_rel"] = 2000.0f;
    spaceBell.parameters["env_b_atk"] = 2.0f;
    spaceBell.parameters["env_b_dcy"] = 600.0f;
    spaceBell.parameters["env_b_sus"] = 0.0f;
    spaceBell.parameters["env_b_rel"] = 2000.0f;
    spaceBell.parameters["poly_mod_env_amt"] = 0.3f;
    spaceBell.parameters["poly_mod_osc_b_amt"] = 0.5f;
    spaceBell.parameters["poly_mod_dest_freq_a"] = 1.0f;
    presets.push_back(spaceBell);

    // [41] Marimba Analog
    Preset marimba = initPatch;
    marimba.name = "Marimba Analog";
    marimba.parameters["osc_a_saw"] = 0.0f;
    marimba.parameters["osc_a_sqr"] = 1.0f;
    marimba.parameters["osc_a_pw"] = 50.0f;
    marimba.parameters["osc_b_tri"] = 1.0f;
    marimba.parameters["mix_osc_a"] = 0.3f;
    marimba.parameters["mix_osc_b"] = 0.8f;
    marimba.parameters["vcf_cutoff"] = 380.0f; // Cutoff: 300 -> 380
    marimba.parameters["vcf_reson"] = 0.05f;
    marimba.parameters["vcf_env_amt"] = 0.2f;
    marimba.parameters["vcf_kb_track"] = 1.0f;
    marimba.parameters["env_a_atk"] = 2.0f;
    marimba.parameters["env_a_dcy"] = 150.0f;
    marimba.parameters["env_a_sus"] = 0.0f;
    marimba.parameters["env_a_rel"] = 200.0f;
    marimba.parameters["env_b_atk"] = 2.0f;
    marimba.parameters["env_b_dcy"] = 200.0f;
    marimba.parameters["env_b_sus"] = 0.0f;
    marimba.parameters["env_b_rel"] = 200.0f;
    presets.push_back(marimba);

    // [42] Tubular Tubular
    Preset tubularBell = initPatch;
    tubularBell.name = "Tubular Tubular";
    tubularBell.parameters["osc_a_sqr"] = 1.0f;
    tubularBell.parameters["osc_b_freq"] = 4.7f;
    tubularBell.parameters["osc_b_tri"] = 1.0f;
    tubularBell.parameters["mix_osc_a"] = 0.7f;
    tubularBell.parameters["mix_osc_b"] = 0.0f;
    tubularBell.parameters["vcf_cutoff"] = 1200.0f;
    tubularBell.parameters["vcf_reson"] = 0.2f;
    tubularBell.parameters["vcf_env_amt"] = 0.5f;
    tubularBell.parameters["env_a_atk"] = 2.0f;
    tubularBell.parameters["env_a_dcy"] = 800.0f;
    tubularBell.parameters["env_a_sus"] = 0.0f;
    tubularBell.parameters["env_a_rel"] = 2000.0f;
    tubularBell.parameters["env_b_atk"] = 1.0f;
    tubularBell.parameters["env_b_dcy"] = 1200.0f;
    tubularBell.parameters["env_b_sus"] = 0.0f;
    tubularBell.parameters["env_b_rel"] = 2000.0f;
    tubularBell.parameters["poly_mod_osc_b_amt"] = 0.55f;
    tubularBell.parameters["poly_mod_dest_freq_a"] = 1.0f;
    tubularBell.parameters["poly_mod_dest_pw_a"] = 1.0f;
    tubularBell.parameters["poly_mod_dest_vcf"] = 1.0f;
    presets.push_back(tubularBell);

    // =========================================================================
    // 8. SFX / Percussive (5 voices)
    // =========================================================================

    // [43] Space Sci-Fi FX
    Preset sciFiFx = initPatch;
    sciFiFx.name = "Space Sci-Fi FX";
    sciFiFx.parameters["osc_a_saw"] = 1.0f;
    sciFiFx.parameters["osc_b_saw"] = 1.0f;
    sciFiFx.parameters["mix_osc_a"] = 0.5f;
    sciFiFx.parameters["mix_osc_b"] = 0.5f;
    sciFiFx.parameters["mix_noise"] = 0.2f;
    sciFiFx.parameters["vcf_cutoff"] = 800.0f;
    sciFiFx.parameters["vcf_reson"] = 0.6f;
    sciFiFx.parameters["vcf_env_amt"] = 0.5f;
    sciFiFx.parameters["lfo_wave"] = 1.0f; // Square LFO
    sciFiFx.parameters["lfo_rate"] = 12.0f; // Modulate pitch at high speed
    sciFiFx.parameters["lfo_initial_amount"] = 0.8f;
    sciFiFx.parameters["wheel_mod_dest_freq_a"] = 1.0f;
    presets.push_back(sciFiFx);

    // [44] Analog Snare / Hat
    Preset percussiveNoise = initPatch;
    percussiveNoise.name = "Analog Snare / Hat";
    percussiveNoise.parameters["osc_a_saw"] = 0.0f;
    percussiveNoise.parameters["mix_osc_a"] = 0.0f;
    percussiveNoise.parameters["mix_noise"] = 1.0f; // Noise only
    percussiveNoise.parameters["vcf_cutoff"] = 1000.0f;
    percussiveNoise.parameters["vcf_reson"] = 0.0f;
    percussiveNoise.parameters["vcf_env_amt"] = 0.6f;
    percussiveNoise.parameters["env_a_atk"] = 1.0f;
    percussiveNoise.parameters["env_a_dcy"] = 80.0f;
    percussiveNoise.parameters["env_a_sus"] = 0.0f;
    percussiveNoise.parameters["env_a_rel"] = 80.0f;
    percussiveNoise.parameters["env_b_atk"] = 1.0f;
    percussiveNoise.parameters["env_b_dcy"] = 100.0f;
    percussiveNoise.parameters["env_b_sus"] = 0.0f;
    percussiveNoise.parameters["env_b_rel"] = 100.0f;
    presets.push_back(percussiveNoise);

    // [45] Windy Ocean
    Preset oceanFx = initPatch;
    oceanFx.name = "Windy Ocean";
    oceanFx.parameters["osc_a_saw"] = 0.0f;
    oceanFx.parameters["mix_osc_a"] = 0.0f;
    oceanFx.parameters["mix_noise"] = 0.9f;
    oceanFx.parameters["vcf_cutoff"] = 200.0f;
    oceanFx.parameters["vcf_reson"] = 0.2f;
    oceanFx.parameters["vcf_env_amt"] = 0.4f;
    oceanFx.parameters["env_a_atk"] = 3000.0f;
    oceanFx.parameters["env_a_dcy"] = 4000.0f;
    oceanFx.parameters["env_a_sus"] = 0.5f;
    oceanFx.parameters["env_a_rel"] = 3000.0f;
    oceanFx.parameters["env_b_atk"] = 2000.0f;
    oceanFx.parameters["env_b_dcy"] = 4000.0f;
    oceanFx.parameters["env_b_sus"] = 0.8f;
    oceanFx.parameters["env_b_rel"] = 3000.0f;
    oceanFx.parameters["lfo_wave"] = 0.0f;
    oceanFx.parameters["lfo_rate"] = 0.15f; // Wave movement at extremely low speed
    oceanFx.parameters["lfo_initial_amount"] = 0.3f;
    oceanFx.parameters["wheel_mod_dest_filter"] = 1.0f;
    presets.push_back(oceanFx);

    // [46] Pitch Sweeper
    Preset sweeperFx = initPatch;
    sweeperFx.name = "Pitch Sweeper";
    sweeperFx.parameters["osc_a_saw"] = 1.0f;
    sweeperFx.parameters["mix_osc_a"] = 0.8f;
    sweeperFx.parameters["vcf_cutoff"] = 5000.0f;
    sweeperFx.parameters["vcf_reson"] = 0.1f;
    sweeperFx.parameters["vcf_env_amt"] = 0.0f;
    sweeperFx.parameters["env_b_atk"] = 2.0f;
    sweeperFx.parameters["env_b_dcy"] = 800.0f;
    sweeperFx.parameters["env_b_sus"] = 0.0f;
    sweeperFx.parameters["env_b_rel"] = 500.0f;
    sweeperFx.parameters["poly_mod_env_amt"] = 0.7f; // Extreme modulation to pitch
    sweeperFx.parameters["poly_mod_dest_freq_a"] = 1.0f;
    presets.push_back(sweeperFx);

    // [47] Computer Chat
    Preset computerFx = initPatch;
    computerFx.name = "Computer Chat";
    computerFx.parameters["osc_a_saw"] = 0.0f;
    computerFx.parameters["osc_a_sqr"] = 1.0f;
    computerFx.parameters["osc_a_pw"] = 50.0f;
    computerFx.parameters["mix_osc_a"] = 0.8f;
    computerFx.parameters["vcf_cutoff"] = 2000.0f;
    computerFx.parameters["vcf_reson"] = 0.1f;
    computerFx.parameters["vcf_env_amt"] = 0.0f;
    computerFx.parameters["env_b_atk"] = 2.0f;
    computerFx.parameters["env_b_dcy"] = 1500.0f;
    computerFx.parameters["env_b_sus"] = 0.8f;
    computerFx.parameters["env_b_rel"] = 500.0f;
    computerFx.parameters["lfo_wave"] = 1.0f; // Square
    computerFx.parameters["lfo_rate"] = 15.0f; // Fast random modulation
    computerFx.parameters["lfo_initial_amount"] = 0.3f;
    computerFx.parameters["wheel_mod_dest_freq_a"] = 1.0f;
    presets.push_back(computerFx);

    // =========================================================================
    // 9. Init Patches (3 voices)
    // =========================================================================

    // [48] Init Voice (Standard start for sound creation. Saw+Square)
    Preset initVoice = initPatch;
    initVoice.name = "Init Voice";
    initVoice.parameters["osc_a_saw"] = 1.0f;
    initVoice.parameters["osc_a_sqr"] = 1.0f;
    initVoice.parameters["osc_b_saw"] = 0.0f;
    initVoice.parameters["osc_b_sqr"] = 1.0f;
    initVoice.parameters["mix_osc_a"] = 0.6f;
    initVoice.parameters["mix_osc_b"] = 0.4f;
    initVoice.parameters["env_a_atk"] = 1.0f;
    initVoice.parameters["env_a_dcy"] = 1.0f;
    initVoice.parameters["env_a_sus"] = 0.0f;
    initVoice.parameters["env_a_rel"] = 1.0f;
    initVoice.parameters["env_b_atk"] = 1.0f;
    initVoice.parameters["env_b_dcy"] = 15000.0f;
    initVoice.parameters["env_b_sus"] = 1.0f;
    initVoice.parameters["env_b_rel"] = 1.0f;
    presets.push_back(initVoice);

    // [49] Init Saw (OSC A Saw wave only)
    Preset initSaw = initPatch;
    initSaw.name = "Init Saw";
    initSaw.parameters["osc_a_saw"] = 1.0f;
    initSaw.parameters["osc_a_sqr"] = 0.0f;
    initSaw.parameters["mix_osc_a"] = 0.8f;
    initSaw.parameters["env_a_atk"] = 1.0f;
    initSaw.parameters["env_a_dcy"] = 1.0f;
    initSaw.parameters["env_a_sus"] = 0.0f;
    initSaw.parameters["env_a_rel"] = 1.0f;
    initSaw.parameters["env_b_atk"] = 1.0f;
    initSaw.parameters["env_b_dcy"] = 15000.0f;
    initSaw.parameters["env_b_sus"] = 1.0f;
    initSaw.parameters["env_b_rel"] = 1.0f;
    presets.push_back(initSaw);

    // [50] Init Square (OSC A Square wave only)
    Preset initSquare = initPatch;
    initSquare.name = "Init Square";
    initSquare.parameters["osc_a_saw"] = 0.0f;
    initSquare.parameters["osc_a_sqr"] = 1.0f;
    initSquare.parameters["mix_osc_a"] = 0.8f;
    initSquare.parameters["env_a_atk"] = 1.0f;
    initSquare.parameters["env_a_dcy"] = 1.0f;
    initSquare.parameters["env_a_sus"] = 0.0f;
    initSquare.parameters["env_a_rel"] = 1.0f;
    initSquare.parameters["env_b_atk"] = 1.0f;
    initSquare.parameters["env_b_dcy"] = 15000.0f;
    initSquare.parameters["env_b_sus"] = 1.0f;
    initSquare.parameters["env_b_rel"] = 1.0f;
    presets.push_back(initSquare);


    // Remapping based on the fixed 10-slot rule
    std::vector<Preset> finalPresets(100);
    Preset emptyPatch = initPatch;
    emptyPatch.name = "- Empty -";
    emptyPatch.category = "Reserved";

    for (int i = 0; i < 100; ++i)
    {
        finalPresets[i] = emptyPatch;
    }

    int tempIdx = 0;
    // Bass (6 voices: 0-5)
    for (int i = 0; i < 6; ++i) finalPresets[0 + i] = presets[tempIdx++];
    // Lead (6 voices: 10-15)
    for (int i = 0; i < 6; ++i) finalPresets[10 + i] = presets[tempIdx++];
    // Pad / Strings (6 voices: 20-25)
    for (int i = 0; i < 6; ++i) finalPresets[20 + i] = presets[tempIdx++];
    // Brass (6 voices: 30-35)
    for (int i = 0; i < 6; ++i) finalPresets[30 + i] = presets[tempIdx++];
    // Keyboards (6 voices: 40-45)
    for (int i = 0; i < 6; ++i) finalPresets[40 + i] = presets[tempIdx++];
    // Guitar / Pluck (6 voices: 50-55)
    for (int i = 0; i < 6; ++i) finalPresets[50 + i] = presets[tempIdx++];
    // Bell / Mallet (6 voices: 60-65)
    for (int i = 0; i < 6; ++i) finalPresets[60 + i] = presets[tempIdx++];
    // SFX / Percussive (5 voices: 70-74)
    for (int i = 0; i < 5; ++i) finalPresets[70 + i] = presets[tempIdx++];
    // Init Patches (3 voices: 80-82)
    for (int i = 0; i < 3; ++i) finalPresets[80 + i] = presets[tempIdx++];

    // Reset correct category and Reserved name
    for (int i = 0; i < 100; ++i)
    {
        if (i < 10)       finalPresets[i].category = "1. Bass";
        else if (i < 20) finalPresets[i].category = "2. Lead";
        else if (i < 30) finalPresets[i].category = "3. Pad / Strings";
        else if (i < 40) finalPresets[i].category = "4. Brass";
        else if (i < 50) finalPresets[i].category = "5. Keyboards";
        else if (i < 60) finalPresets[i].category = "6. Guitar / Pluck";
        else if (i < 70) finalPresets[i].category = "7. Bell / Mallet";
        else if (i < 80) finalPresets[i].category = "8. SFX / Percussive";
        else if (i < 90) finalPresets[i].category = "9. Init Patches";
        else             
        {
            finalPresets[i].category = "Reserved";
            finalPresets[i].name = "- Reserved -";
        }
    }

    return finalPresets;
}