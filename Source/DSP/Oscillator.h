#pragma once
#include <JuceHeader.h>

class Prop5Oscillator
{
public:
    Prop5Oscillator();
    ~Prop5Oscillator() = default;

    // 初期化（サンプルレートの設定）
    void prepare(double sampleRate);

    // パラメーターの設定
    void setFrequency(float freqInHz);
    void setPulseWidth(float pwPercentage); // 0.0 〜 100.0

    // 波形スイッチのオン/オフ
    void setSawEnabled(bool enabled)     { sawEnabled = enabled; }
    void setSquareEnabled(bool enabled)  { sqrEnabled = enabled; }
    void setTriangleEnabled(bool enabled){ triEnabled = enabled; } // OSC B用

    // OSC SYNC（ハードシンク）用：外部から位相を強制リセットする
    void resetPhase() { phase = 0.0f; }
    float getPhase() const { return phase; }

    // 1サンプル分の波形を生成して返す
    float processSample();

    // 部（Voiceクラス）から最新の出力値を覗き見るための関数
    float getSample() const { return lastSample; }

private:
    double currentSampleRate = 44100.0;
    float frequency = 440.0f;
    float phase = 0.0f;
    float phaseIncrement = 0.0f;
    float pulseWidth = 0.5f; // 0.0 〜 1.0に正規化して保持

    bool sawEnabled = true;
    bool sqrEnabled = false;
    bool triEnabled = false;

    float lastSample = 0.0f; // 最新の出力値を記憶しておくための変数

    void updateIncrement();
};

// ホワイトノイズジェネレーター
class Prop5Noise
{
public:
    void prepare(double sampleRate) { juce::ignoreUnused(sampleRate); }
    float processSample() 
    { 
        // -1.0 〜 1.0 のランダムな値を返す
        return random.nextFloat() * 2.0f - 1.0f; 
    }
private:
    juce::Random random;
};