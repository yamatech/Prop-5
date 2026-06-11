#pragma once
#include <JuceHeader.h>

class PropLookAndFeel : public juce::LookAndFeel_V4
{
public:
    PropLookAndFeel();
    ~PropLookAndFeel() override;

    // ロータリーノブのカスタム描画
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, const float startAngle, const float endAngle,
                           juce::Slider& slider) override;

    // 四角ボタンスイッチのカスタム描画 (ToggleButton用)
    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                           bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
};
