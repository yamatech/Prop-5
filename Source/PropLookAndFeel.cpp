#include "PropLookAndFeel.h"

PropLookAndFeel::PropLookAndFeel()
{
    // Change default colors as needed
}

PropLookAndFeel::~PropLookAndFeel()
{
}

void PropLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                       float sliderPos, const float startAngle, const float endAngle,
                                       juce::Slider& slider)
{
    auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat();
    
    // Base the slider diameter on the smaller of width and height
    float diameter = std::min (bounds.getWidth(), bounds.getHeight());
    float radius = diameter * 0.5f - 2.0f;
    float cx = bounds.getCentreX();
    float cy = bounds.getCentreY();
    
    if (radius <= 0.0f)
        return;

    // --- 1. Shadow underneath (subtle shadow for premium look) ---
    g.setColour (juce::Colours::black.withAlpha (0.4f));
    g.fillEllipse (cx - radius + 1.0f, cy - radius + 2.0f, radius * 2.0f, radius * 2.0f);

    // --- 2. Draw jagged base (draw black lines from center outward) ---
    g.setColour (juce::Colour (0xff0d0d0d));
    int numTeeth = 18; // Number of teeth
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

    // --- 3. Main circle body of the knob (fill inside the teeth, hide boundaries) ---
    float bodyRadius = radius * 0.85f;
    juce::ColourGradient grad (juce::Colour (0xff242424), cx - bodyRadius, cy - bodyRadius,
                               juce::Colour (0xff0f0f0f), cx + bodyRadius, cy + bodyRadius, true);
    g.setGradientFill (grad);
    g.fillEllipse (cx - bodyRadius, cy - bodyRadius, bodyRadius * 2.0f, bodyRadius * 2.0f);

    // Contour line of knob body (slightly brighter edge to add 3D depth)
    g.setColour (juce::Colour (0xff3b3b3b));
    g.drawEllipse (cx - bodyRadius, cy - bodyRadius, bodyRadius * 2.0f, bodyRadius * 2.0f, 1.0f);

    // --- 4. Center silver insert (aluminum-brushed style plate) ---
    float silverRadius = radius * 0.45f;
    juce::ColourGradient silverGrad (juce::Colour (0xfffdfdfd), cx - silverRadius * 0.5f, cy - silverRadius * 0.5f,
                                     juce::Colour (0xffaeaeae), cx + silverRadius, cy + silverRadius, true);
    g.setGradientFill (silverGrad);
    g.fillEllipse (cx - silverRadius, cy - silverRadius, silverRadius * 2.0f, silverRadius * 2.0f);

    // Thin contour of the silver part
    g.setColour (juce::Colour (0xff5e5e5e));
    g.drawEllipse (cx - silverRadius, cy - silverRadius, silverRadius * 2.0f, silverRadius * 2.0f, 0.7f);

    // --- 5. Draw white pointer (indicator line) ---
    // Angle corresponding to the current slider value
    float angle = startAngle + sliderPos * (endAngle - startAngle);
    
    // Draw a white line from the edge of the silver insert to near the outer edge of the knob body
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
    
    // Dark plastic background of the switch body
    juce::Colour buttonColor = juce::Colour (0xff1c1c1e);
    if (shouldDrawButtonAsDown)
        buttonColor = juce::Colour (0xff0d0d0e);
    else if (shouldDrawButtonAsHighlighted)
        buttonColor = juce::Colour (0xff252528);
        
    g.setColour (buttonColor);
    g.fillRoundedRectangle (bounds.reduced (1.0f), 2.0f);
    
    // Thin edge for 3D depth
    g.setColour (juce::Colour (0xff303033));
    g.drawRoundedRectangle (bounds.reduced (1.0f), 2.0f, 1.0f);

    // LED window (near top center of the button)
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
        // LED ON (bright neon red)
        g.setColour (juce::Colour (0xffff2a1a));
        g.fillRect (ledRect);
        
        // Glow effect to the surroundings
        g.setColour (juce::Colour (0x44ff2a1a));
        g.drawRect (ledRect.expanded (1.5f), 1.0f);
    }
    else
    {
        // LED OFF (dark red)
        g.setColour (juce::Colour (0xff4a0d07));
        g.fillRect (ledRect);
    }

    // Text inside the switch (below LED window)
    float fontSize = std::clamp (bounds.getHeight() * 0.35f, 9.0f, 12.0f);
    g.setFont (fontSize);
    g.setColour (button.getToggleState() ? juce::Colours::white : juce::Colour (0xffa0a0a5));
    
    // Text area is below the LED window
    auto textRect = bounds.withTrimmedTop (ledRect.getBottom() + 1.0f).reduced (2.0f);
    g.drawFittedText (button.getButtonText(), textRect.toNearestInt(), juce::Justification::centred, 2);
}
