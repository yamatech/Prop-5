#pragma once
#include <JuceHeader.h>
#include <unordered_map>
#include <vector>

struct Preset
{
    juce::String name;
    std::unordered_map<juce::String, float> parameters;
};

inline std::vector<Preset> getFactoryPresets()
{
    std::vector<Preset> presets;

    // ---------------------------------------------------------
    // 1. Init Patch
    // ---------------------------------------------------------
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
        {"vcf_kb_track", 0.0f}, // 0: OFF
        {"env_a_atk", 20.0f},
        {"env_a_dcy", 200.0f},
        {"env_a_sus", 0.7f},
        {"env_a_rel", 300.0f},
        {"env_b_atk", 5.0f},
        {"env_b_dcy", 150.0f},
        {"env_b_sus", 1.0f},
        {"env_b_rel", 200.0f},
        {"vca_mode", 0.0f}, // Envelope Mode
        {"poly_mod_env_amt", 0.0f},
        {"poly_mod_osc_b_amt", 0.0f},
        {"poly_mod_dest_freq_a", 0.0f},
        {"poly_mod_dest_pw_a", 0.0f},
        {"poly_mod_dest_vcf", 0.0f},
        {"lfo_wave", 0.0f}, // Triangle
        {"lfo_rate", 5.0f},
        {"lfo_initial_amount", 0.0f},
        {"wheel_mod_source_mix", 0.0f},
        {"wheel_mod_dest_freq_a", 0.0f},
        {"wheel_mod_dest_freq_b", 0.0f},
        {"wheel_mod_dest_pw_a", 0.0f},
        {"wheel_mod_dest_pw_b", 0.0f},
        {"wheel_mod_dest_filter", 0.0f},
        {"pitchbend", 0.0f},
        {"modwheel", 0.0f}
    };
    presets.push_back(initPatch);

    // ---------------------------------------------------------
    // 2. Classic Saw Bass
    // ---------------------------------------------------------
    Preset sawBass;
    sawBass.name = "Classic Saw Bass";
    sawBass.parameters = initPatch.parameters;
    sawBass.parameters["osc_a_freq"] = 0.5f; // 1オクターブ下
    sawBass.parameters["osc_a_saw"] = 1.0f;
    sawBass.parameters["osc_b_freq"] = 0.5f;
    sawBass.parameters["osc_b_fine"] = 0.08f; // 少しデチューン
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

    // ---------------------------------------------------------
    // 3. Poly Pad
    // ---------------------------------------------------------
    Preset polyPad;
    polyPad.name = "Poly Pad";
    polyPad.parameters = initPatch.parameters;
    polyPad.parameters["osc_a_saw"] = 1.0f;
    polyPad.parameters["osc_b_saw"] = 1.0f;
    polyPad.parameters["osc_b_fine"] = 0.05f;
    polyPad.parameters["mix_osc_a"] = 0.6f;
    polyPad.parameters["mix_osc_b"] = 0.6f;
    polyPad.parameters["vcf_cutoff"] = 800.0f;
    polyPad.parameters["vcf_reson"] = 0.1f;
    polyPad.parameters["vcf_env_amt"] = 0.3f;
    polyPad.parameters["vcf_kb_track"] = 2.0f; // FULL
    polyPad.parameters["env_a_atk"] = 600.0f;
    polyPad.parameters["env_a_dcy"] = 1200.0f;
    polyPad.parameters["env_a_sus"] = 0.7f;
    polyPad.parameters["env_a_rel"] = 1000.0f;
    polyPad.parameters["env_b_atk"] = 300.0f;
    polyPad.parameters["env_b_dcy"] = 1000.0f;
    polyPad.parameters["env_b_sus"] = 0.9f;
    polyPad.parameters["env_b_rel"] = 800.0f;
    polyPad.parameters["wheel_mod_dest_freq_a"] = 1.0f;
    polyPad.parameters["lfo_initial_amount"] = 0.05f; // 緩やかなビブラート
    presets.push_back(polyPad);

    // ---------------------------------------------------------
    // 4. Synth Brass
    // ---------------------------------------------------------
    Preset synthBrass;
    synthBrass.name = "Synth Brass";
    synthBrass.parameters = initPatch.parameters;
    synthBrass.parameters["osc_a_saw"] = 1.0f;
    synthBrass.parameters["osc_b_saw"] = 1.0f;
    synthBrass.parameters["osc_b_fine"] = 0.06f;
    synthBrass.parameters["mix_osc_a"] = 0.65f;
    synthBrass.parameters["mix_osc_b"] = 0.65f;
    synthBrass.parameters["vcf_cutoff"] = 400.0f;
    synthBrass.parameters["vcf_reson"] = 0.15f;
    synthBrass.parameters["vcf_env_amt"] = 0.65f;
    synthBrass.parameters["vcf_kb_track"] = 2.0f; // FULL
    synthBrass.parameters["env_a_atk"] = 80.0f;   // ブラス独特の立ち上がり
    synthBrass.parameters["env_a_dcy"] = 600.0f;
    synthBrass.parameters["env_a_sus"] = 0.6f;
    synthBrass.parameters["env_a_rel"] = 300.0f;
    synthBrass.parameters["env_b_atk"] = 10.0f;
    synthBrass.parameters["env_b_dcy"] = 800.0f;
    synthBrass.parameters["env_b_sus"] = 0.9f;
    synthBrass.parameters["env_b_rel"] = 300.0f;
    presets.push_back(synthBrass);

    // ---------------------------------------------------------
    // 5. Crystal Bell
    // ---------------------------------------------------------
    Preset crystalBell;
    crystalBell.name = "Crystal Bell";
    crystalBell.parameters = initPatch.parameters;
    crystalBell.parameters["osc_a_freq"] = 2.0f; // 1オクターブ上
    crystalBell.parameters["osc_a_sqr"] = 1.0f;
    crystalBell.parameters["osc_a_saw"] = 0.0f;
    crystalBell.parameters["osc_b_freq"] = 3.0f; // 1オクターブ＋5度上
    crystalBell.parameters["osc_b_tri"] = 1.0f;
    crystalBell.parameters["mix_osc_a"] = 0.7f;
    crystalBell.parameters["mix_osc_b"] = 0.0f; // OSC B音量はカット（変調器として使用）
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
    crystalBell.parameters["poly_mod_env_amt"] = 0.15f;
    crystalBell.parameters["poly_mod_osc_b_amt"] = 0.65f; // 金属的なFM音を作るための深いモジュレーション
    crystalBell.parameters["poly_mod_dest_freq_a"] = 1.0f;
    crystalBell.parameters["poly_mod_dest_vcf"] = 1.0f;
    presets.push_back(crystalBell);

    // ---------------------------------------------------------
    // 6. Electric Piano
    // ---------------------------------------------------------
    Preset electricPiano;
    electricPiano.name = "Classic Electric Piano";
    electricPiano.parameters = initPatch.parameters;
    electricPiano.parameters["osc_a_freq"] = 1.0f;
    electricPiano.parameters["osc_a_sqr"] = 1.0f;
    electricPiano.parameters["osc_a_saw"] = 0.0f;
    electricPiano.parameters["osc_b_freq"] = 1.0f;
    electricPiano.parameters["osc_b_tri"] = 1.0f;
    electricPiano.parameters["osc_b_fine"] = 0.03f;
    electricPiano.parameters["mix_osc_a"] = 0.6f;
    electricPiano.parameters["mix_osc_b"] = 0.5f;
    electricPiano.parameters["vcf_cutoff"] = 1200.0f;
    electricPiano.parameters["vcf_reson"] = 0.1f;
    electricPiano.parameters["vcf_env_amt"] = 0.4f;
    electricPiano.parameters["vcf_kb_track"] = 1.0f; // HALF
    electricPiano.parameters["env_a_atk"] = 2.0f;
    electricPiano.parameters["env_a_dcy"] = 350.0f;
    electricPiano.parameters["env_a_sus"] = 0.0f;
    electricPiano.parameters["env_a_rel"] = 600.0f;
    electricPiano.parameters["env_b_atk"] = 2.0f;
    electricPiano.parameters["env_b_dcy"] = 500.0f;
    electricPiano.parameters["env_b_sus"] = 0.0f;
    electricPiano.parameters["env_b_rel"] = 600.0f;
    presets.push_back(electricPiano);

    // ---------------------------------------------------------
    // 7. Drawbar Organ
    // ---------------------------------------------------------
    Preset organ;
    organ.name = "Drawbar Organ";
    organ.parameters = initPatch.parameters;
    organ.parameters["osc_a_freq"] = 1.0f;
    organ.parameters["osc_a_sqr"] = 1.0f;
    organ.parameters["osc_a_saw"] = 0.0f;
    organ.parameters["osc_b_freq"] = 1.5f; // 3度上 (オルガンの倍音構成)
    organ.parameters["osc_b_tri"] = 1.0f;
    organ.parameters["mix_osc_a"] = 0.7f;
    organ.parameters["mix_osc_b"] = 0.5f;
    organ.parameters["vcf_cutoff"] = 20000.0f; // フィルター全開
    organ.parameters["vcf_reson"] = 0.0f;
    organ.parameters["vcf_env_amt"] = 0.0f;
    organ.parameters["vca_mode"] = 1.0f; // Gate動作 (即座にON/OFF)
    presets.push_back(organ);

    // ---------------------------------------------------------
    // 8. Nasty Feedback Lead
    // ---------------------------------------------------------
    Preset feedbackLead;
    feedbackLead.name = "Nasty Feedback Lead";
    feedbackLead.parameters = initPatch.parameters;
    feedbackLead.parameters["osc_a_saw"] = 1.0f;
    feedbackLead.parameters["osc_b_saw"] = 1.0f;
    feedbackLead.parameters["osc_b_fine"] = 0.08f;
    feedbackLead.parameters["mix_osc_a"] = 0.6f;
    feedbackLead.parameters["mix_osc_b"] = 0.6f;
    feedbackLead.parameters["vcf_cutoff"] = 350.0f;
    feedbackLead.parameters["vcf_reson"] = 0.75f; // レゾナンス高めで歪み感
    feedbackLead.parameters["vcf_env_amt"] = 0.7f;
    feedbackLead.parameters["env_a_atk"] = 2.0f;
    feedbackLead.parameters["env_a_dcy"] = 600.0f;
    feedbackLead.parameters["env_a_sus"] = 0.4f;
    feedbackLead.parameters["env_a_rel"] = 300.0f;
    feedbackLead.parameters["poly_mod_osc_b_amt"] = 0.4f;
    feedbackLead.parameters["poly_mod_dest_freq_a"] = 1.0f;
    feedbackLead.parameters["poly_mod_dest_vcf"] = 1.0f;
    presets.push_back(feedbackLead);

    // ---------------------------------------------------------
    // 9. Drone Pad
    // ---------------------------------------------------------
    Preset dronePad;
    dronePad.name = "Drone Pad";
    dronePad.parameters = initPatch.parameters;
    dronePad.parameters["osc_a_saw"] = 1.0f;
    dronePad.parameters["osc_b_tri"] = 1.0f;
    dronePad.parameters["osc_b_freq"] = 0.5f; // オクターブ下
    dronePad.parameters["osc_b_fine"] = 0.04f;
    dronePad.parameters["mix_osc_a"] = 0.5f;
    dronePad.parameters["mix_osc_b"] = 0.6f;
    dronePad.parameters["mix_noise"] = 0.15f; // ノイズを少し混ぜる
    dronePad.parameters["vcf_cutoff"] = 600.0f;
    dronePad.parameters["vcf_reson"] = 0.2f;
    dronePad.parameters["vcf_env_amt"] = 0.4f;
    dronePad.parameters["env_a_atk"] = 2000.0f; // 非常に長い立ち上がり
    dronePad.parameters["env_a_dcy"] = 4000.0f;
    dronePad.parameters["env_a_sus"] = 0.8f;
    dronePad.parameters["env_a_rel"] = 3000.0f;
    dronePad.parameters["env_b_atk"] = 1500.0f;
    dronePad.parameters["env_b_dcy"] = 3000.0f;
    dronePad.parameters["env_b_sus"] = 1.0f;
    dronePad.parameters["env_b_rel"] = 2500.0f;
    dronePad.parameters["wheel_mod_dest_filter"] = 1.0f;
    dronePad.parameters["lfo_rate"] = 0.8f; // ゆっくりフィルターを揺らす
    dronePad.parameters["lfo_initial_amount"] = 0.15f;
    presets.push_back(dronePad);

    // ---------------------------------------------------------
    // 10. Bright Synth Lead
    // ---------------------------------------------------------
    Preset brightLead;
    brightLead.name = "Bright Synth Lead";
    brightLead.parameters = initPatch.parameters;
    brightLead.parameters["osc_a_saw"] = 1.0f;
    brightLead.parameters["osc_b_saw"] = 1.0f;
    brightLead.parameters["osc_b_freq"] = 2.0f; // OSC Bを1オクターブ上に設定してシンク
    brightLead.parameters["osc_sync"] = 1.0f;    // シンクON
    brightLead.parameters["mix_osc_a"] = 0.7f;
    brightLead.parameters["mix_osc_b"] = 0.5f;
    brightLead.parameters["vcf_cutoff"] = 3000.0f;
    brightLead.parameters["vcf_reson"] = 0.2f;
    brightLead.parameters["vcf_env_amt"] = 0.4f;
    brightLead.parameters["env_a_atk"] = 2.0f;
    brightLead.parameters["env_a_dcy"] = 400.0f;
    brightLead.parameters["env_a_sus"] = 0.7f;
    brightLead.parameters["env_a_rel"] = 250.0f;
    brightLead.parameters["env_b_atk"] = 2.0f;
    brightLead.parameters["env_b_dcy"] = 400.0f;
    brightLead.parameters["env_b_sus"] = 0.9f;
    brightLead.parameters["env_b_rel"] = 250.0f;
    presets.push_back(brightLead);

    // ---------------------------------------------------------
    // 11. Space Sci-Fi FX
    // ---------------------------------------------------------
    Preset sciFiFx;
    sciFiFx.name = "Space Sci-Fi FX";
    sciFiFx.parameters = initPatch.parameters;
    sciFiFx.parameters["osc_a_saw"] = 1.0f;
    sciFiFx.parameters["osc_b_saw"] = 1.0f;
    sciFiFx.parameters["mix_osc_a"] = 0.5f;
    sciFiFx.parameters["mix_osc_b"] = 0.5f;
    sciFiFx.parameters["mix_noise"] = 0.2f;
    sciFiFx.parameters["vcf_cutoff"] = 800.0f;
    sciFiFx.parameters["vcf_reson"] = 0.6f;
    sciFiFx.parameters["vcf_env_amt"] = 0.5f;
    sciFiFx.parameters["lfo_wave"] = 1.0f; // Square LFO
    sciFiFx.parameters["lfo_rate"] = 12.0f; // 高速でピッチを変調
    sciFiFx.parameters["wheel_mod_dest_freq_a"] = 1.0f;
    sciFiFx.parameters["lfo_initial_amount"] = 0.8f;
    presets.push_back(sciFiFx);

    // ---------------------------------------------------------
    // 12. Pluck Arp/Seq
    // ---------------------------------------------------------
    Preset pluckArp;
    pluckArp.name = "Pluck Arp/Seq";
    pluckArp.parameters = initPatch.parameters;
    pluckArp.parameters["osc_a_saw"] = 1.0f;
    pluckArp.parameters["osc_b_sqr"] = 1.0f;
    pluckArp.parameters["osc_b_fine"] = 0.04f;
    pluckArp.parameters["mix_osc_a"] = 0.6f;
    pluckArp.parameters["mix_osc_b"] = 0.5f;
    pluckArp.parameters["vcf_cutoff"] = 200.0f;
    pluckArp.parameters["vcf_reson"] = 0.3f;
    pluckArp.parameters["vcf_env_amt"] = 0.8f;
    pluckArp.parameters["env_a_atk"] = 1.0f;
    pluckArp.parameters["env_a_dcy"] = 120.0f;
    pluckArp.parameters["env_a_sus"] = 0.0f;
    pluckArp.parameters["env_a_rel"] = 80.0f;
    pluckArp.parameters["env_b_atk"] = 1.0f;
    pluckArp.parameters["env_b_dcy"] = 200.0f;
    pluckArp.parameters["env_b_sus"] = 0.0f;
    pluckArp.parameters["env_b_rel"] = 80.0f;
    presets.push_back(pluckArp);

    // ---------------------------------------------------------
    // 13. Flute
    // ---------------------------------------------------------
    Preset flute;
    flute.name = "Flute";
    flute.parameters = initPatch.parameters;
    flute.parameters["osc_a_freq"] = 2.0f; // 高オクターブ
    flute.parameters["osc_a_sqr"] = 1.0f;  // 矩形波
    flute.parameters["osc_a_pw"] = 50.0f;  // ほぼクラリネット/フルートに近い純粋矩形波
    flute.parameters["osc_b_freq"] = 2.0f;
    flute.parameters["osc_b_tri"] = 1.0f;  // 三角波を重ねる
    flute.parameters["mix_osc_a"] = 0.5f;
    flute.parameters["mix_osc_b"] = 0.5f;
    flute.parameters["mix_noise"] = 0.12f; // ブレス音をシミュレートするノイズ
    flute.parameters["vcf_cutoff"] = 500.0f; // 暗めにして高域をカット
    flute.parameters["vcf_reson"] = 0.05f;
    flute.parameters["vcf_env_amt"] = 0.3f;
    flute.parameters["env_a_atk"] = 120.0f; // アタックを柔らかく
    flute.parameters["env_a_dcy"] = 300.0f;
    flute.parameters["env_a_sus"] = 0.7f;
    flute.parameters["env_a_rel"] = 200.0f;
    flute.parameters["env_b_atk"] = 80.0f;
    flute.parameters["env_b_dcy"] = 300.0f;
    flute.parameters["env_b_sus"] = 0.8f;
    flute.parameters["env_b_rel"] = 200.0f;
    presets.push_back(flute);

    return presets;
}