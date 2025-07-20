#include "PluginProcessor.h"
#include "PluginEditor.h"

StereoReverbAudioProcessor::StereoReverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       ),
#endif
    parameters(*this, nullptr, juce::Identifier("StereoReverbParams"),
    {
        std::make_unique<juce::AudioParameterFloat>("roomSize", "Room Size", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f),
        std::make_unique<juce::AudioParameterFloat>("decayTime", "Decay Time", juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f), 2.0f)
    })
{
    roomSizeParam = parameters.getRawParameterValue("roomSize");
    decayTimeParam = parameters.getRawParameterValue("decayTime");
}

StereoReverbAudioProcessor::~StereoReverbAudioProcessor()
{
}

const juce::String StereoReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StereoReverbAudioProcessor::acceptsMidi() const
{
   return false;
}

bool StereoReverbAudioProcessor::producesMidi() const
{
   return false;
}

bool StereoReverbAudioProcessor::isMidiEffect() const
{
    return false;
}

double StereoReverbAudioProcessor::getTailLengthSeconds() const
{
    return 10.0;
}

int StereoReverbAudioProcessor::getNumPrograms()
{
    return 1;
}

int StereoReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void StereoReverbAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String StereoReverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void StereoReverbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void StereoReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = 2;

    reverbL.reset();
    reverbR.reset();

    reverbL.prepare(spec);
    reverbR.prepare(spec);
}

void StereoReverbAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StereoReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // Only stereo in/out supported
    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo()
        || layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}
#endif

void StereoReverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Update reverb parameters
    juce::Reverb::Parameters params;
    params.roomSize = *roomSizeParam;
    // Decay time mapping: JUCE's Reverb uses damping and roomSize, not decay directly.
    // We'll map decayTime to damping inversely, and scale wetLevel for longer decays.
    float decay = *decayTimeParam;
    params.damping = juce::jlimit(0.0f, 1.0f, 1.0f - (decay - 0.1f) / 9.9f); // 0.1s -> 1.0 damping, 10s -> 0.0 damping
    params.wetLevel = 0.33f + 0.33f * juce::jlimit(0.0f, 1.0f, (decay - 0.1f) / 9.9f); // wetter for longer decays
    params.dryLevel = 1.0f - params.wetLevel * 0.5f;
    params.width = 1.0f;
    params.freezeMode = 0.0f;

    reverbL.setParameters(params);
    reverbR.setParameters(params);

    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getWritePointer(1);

    juce::AudioBuffer<float> tempBuffer(2, buffer.getNumSamples());
    tempBuffer.copyFrom(0, 0, left, buffer.getNumSamples());
    tempBuffer.copyFrom(1, 0, right, buffer.getNumSamples());

    reverbL.processMono(tempBuffer.getWritePointer(0), buffer.getNumSamples());
    reverbR.processMono(tempBuffer.getWritePointer(1), buffer.getNumSamples());

    buffer.copyFrom(0, 0, tempBuffer.getReadPointer(0), buffer.getNumSamples());
    buffer.copyFrom(1, 0, tempBuffer.getReadPointer(1), buffer.getNumSamples());
}

bool StereoReverbAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* StereoReverbAudioProcessor::createEditor()
{
    return new StereoReverbAudioProcessorEditor (*this, parameters);
}

void StereoReverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, true);
    parameters.state.writeToStream(stream);
}

void StereoReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
        parameters.state = tree;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StereoReverbAudioProcessor();
}
```

```cpp