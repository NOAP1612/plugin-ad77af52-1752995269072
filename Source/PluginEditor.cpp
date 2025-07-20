#include "PluginProcessor.h"
#include "PluginEditor.h"

StereoReverbAudioProcessorEditor::StereoReverbAudioProcessorEditor (StereoReverbAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState(vts)
{
    setSize (400, 220);

    // Room Size Slider
    roomSizeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    roomSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    roomSizeSlider.setRange(0.0, 1.0, 0.01);
    roomSizeSlider.setSkewFactor(0.7f);
    roomSizeSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::deepskyblue);
    roomSizeSlider.setColour(juce::Slider::thumbColourId, juce::Colours::blueviolet);
    addAndMakeVisible(roomSizeSlider);
    roomSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState, "roomSize", roomSizeSlider);

    roomSizeLabel.setText("Room Size", juce::dontSendNotification);
    roomSizeLabel.attachToComponent(&roomSizeSlider, false);
    roomSizeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(roomSizeLabel);

    // Decay Time Slider
    decayTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    decayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    decayTimeSlider.setRange(0.1, 10.0, 0.01);
    decayTimeSlider.setSkewFactorFromMidPoint(1.0);
    decayTimeSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    decayTimeSlider.setColour(juce::Slider::thumbColourId, juce::Colours::darkorange);
    addAndMakeVisible(decayTimeSlider);
    decayTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(valueTreeState, "decayTime", decayTimeSlider);

    decayTimeLabel.setText("Decay Time (s)", juce::dontSendNotification);
    decayTimeLabel.attachToComponent(&decayTimeSlider, false);
    decayTimeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(decayTimeLabel);
}

StereoReverbAudioProcessorEditor::~StereoReverbAudioProcessorEditor()
{
}

void StereoReverbAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff181c1f));
    g.setColour(juce::Colours::white);
    g.setFont(22.0f);
    g.drawFittedText("Stereo Reverb", 0, 10, getWidth(), 30, juce::Justification::centred, 1);

    // Draw subtle panel
    g.setColour(juce::Colour(0xff22282c));
    g.fillRoundedRectangle(20, 50, getWidth()-40, getHeight()-70, 16.0f);
}

void StereoReverbAudioProcessorEditor::resized()
{
    int sliderW = 120;
    int sliderH = 120;
    int margin = 40;
    int top = 70;

    roomSizeSlider.setBounds(margin, top, sliderW, sliderH);
    decayTimeSlider.setBounds(getWidth() - sliderW - margin, top, sliderW, sliderH);
}
```

```cmake
#