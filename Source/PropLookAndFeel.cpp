#include "PropLookAndFeel.h"

PropLookAndFeel::PropLookAndFeel()
{
    // 必要に応じてデフォルト色の変更など
}

PropLookAndFeel::~PropLookAndFeel()
{
}

void PropLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                       float sliderPos, const float startAngle, const float endAngle,
                                       juce::Slider& slider)
{
    auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat();
    
    // スライダーの直径は幅と高さの小さい方をベースにする
    float diameter = std::min (bounds.getWidth(), bounds.getHeight());
    float radius = diameter * 0.5f - 2.0f;
    float cx = bounds.getCentreX();
    float cy = bounds.getCentreY();
    
    if (radius <= 0.0f)
        return;

    // --- 1. 下地シャドウ（高級感用のうっすら影） ---
    g.setColour (juce::Colours::black.withAlpha (0.4f));
    g.fillEllipse (cx - radius + 1.0f, cy - radius + 2.0f, radius * 2.0f, radius * 2.0f);

    // --- 2. ギザギザのベースを描画（中心から外に向けて黒線を引く） ---
    g.setColour (juce::Colour (0xff0d0d0d));
    int numTeeth = 18; // ギザギザの数
    float teethThickness = 2.0f;
    
    for (int i = 0; i < numTeeth; ++i)
    {
        float angle = (float)i * (juce::MathConstants<float>::twoPi / (float)numTeeth);
        float startX = cx + std::sin(angle) * (radius * 0.75f);
        float startY = cy - std::cos(angle) * (radius * 0.75f);
        float endX = cx + std::sin(angle) * radius;
        float endY = cy - std::cos(angle) * radius;
        g.drawLine (startX, startY, endX, endY, teethThickness);
    }

    // --- 3. ノブのメイン円体（ギザギザの内側を塗りつぶし、境界を隠す） ---
    float bodyRadius = radius * 0.85f;
    juce::ColourGradient grad (juce::Colour (0xff242424), cx - bodyRadius, cy - bodyRadius,
                               juce::Colour (0xff0f0f0f), cx + bodyRadius, cy + bodyRadius, true);
    g.setGradientFill (grad);
    g.fillEllipse (cx - bodyRadius, cy - bodyRadius, bodyRadius * 2.0f, bodyRadius * 2.0f);

    // ノブ本体の輪郭線（すこし明るいエッジで立体感を出す）
    g.setColour (juce::Colour (0xff3b3b3b));
    g.drawEllipse (cx - bodyRadius, cy - bodyRadius, bodyRadius * 2.0f, bodyRadius * 2.0f, 1.0f);

    // --- 4. 中央のシルバーインサート（アルミ削り出し風プレート） ---
    float silverRadius = radius * 0.45f;
    juce::ColourGradient silverGrad (juce::Colour (0xfffdfdfd), cx - silverRadius * 0.5f, cy - silverRadius * 0.5f,
                                     juce::Colour (0xffaeaeae), cx + silverRadius, cy + silverRadius, true);
    g.setGradientFill (silverGrad);
    g.fillEllipse (cx - silverRadius, cy - silverRadius, silverRadius * 2.0f, silverRadius * 2.0f);

    // シルバー部分の細い輪郭
    g.setColour (juce::Colour (0xff5e5e5e));
    g.drawEllipse (cx - silverRadius, cy - silverRadius, silverRadius * 2.0f, silverRadius * 2.0f, 0.7f);

    // --- 5. ホワイトポインター（指示線）の描画 ---
    // 現在のスライダー値に対応する角度
    float angle = startAngle + sliderPos * (endAngle - startAngle);
    
    // シルバーインサートの端から、ノブ本体の外周付近まで白線を引く
    float pointerStart = silverRadius;
    float pointerEnd = bodyRadius;
    
    float pX1 = cx + std::sin (angle) * pointerStart;
    float pY1 = cy - std::cos (angle) * pointerStart;
    float pX2 = cx + std::sin (angle) * pointerEnd;
    float pY2 = cy - std::cos (angle) * pointerEnd;

    g.setColour (juce::Colours::white);
    g.drawLine (pX1, pY1, pX2, pY2, 1.8f);
}

void PropLookAndFeel::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
                                       bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat();
    
    // スイッチ本体の暗いプラスチック背景
    juce::Colour buttonColor = juce::Colour (0xff1c1c1e);
    if (shouldDrawButtonAsDown)
        buttonColor = juce::Colour (0xff0d0d0e);
    else if (shouldDrawButtonAsHighlighted)
        buttonColor = juce::Colour (0xff252528);
        
    g.setColour (buttonColor);
    g.fillRoundedRectangle (bounds.reduced (1.0f), 2.0f);
    
    // 立体感のための薄いエッジ
    g.setColour (juce::Colour (0xff303033));
    g.drawRoundedRectangle (bounds.reduced (1.0f), 2.0f, 1.0f);

    // LED窓（ボタンの上部中央付近）
    float ledW = std::clamp (bounds.getWidth() * 0.22f, 7.0f, 16.0f);
    float ledH = std::clamp (bounds.getHeight() * 0.16f, 4.0f, 8.0f);
    
    auto ledRect = juce::Rectangle<float> (
        bounds.getCentreX() - ledW * 0.5f,
        bounds.getY() + bounds.getHeight() * 0.15f,
        ledW,
        ledH
    );

    if (button.getToggleState())
    {
        // LED点灯（明るいネオン赤）
        g.setColour (juce::Colour (0xffff2a1a));
        g.fillRect (ledRect);
        
        // 周囲へのグロー（光彩）効果
        g.setColour (juce::Colour (0x44ff2a1a));
        g.drawRect (ledRect.expanded (1.5f), 1.0f);
    }
    else
    {
        // LED消灯（暗いダークレッド）
        g.setColour (juce::Colour (0xff4a0d07));
        g.fillRect (ledRect);
    }

    // スイッチ内のテキスト（LED窓の下）
    float fontSize = std::clamp (bounds.getHeight() * 0.35f, 9.0f, 12.0f);
    g.setFont (fontSize);
    g.setColour (button.getToggleState() ? juce::Colours::white : juce::Colour (0xffa0a0a5));
    
    // テキスト領域はLED窓のさらに下
    auto textRect = bounds.withTrimmedTop (ledRect.getBottom() + 1.0f).reduced (2.0f);
    g.drawFittedText (button.getButtonText(), textRect.toNearestInt(), juce::Justification::centred, 2);
}
