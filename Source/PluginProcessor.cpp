/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PitchCorrectionPluginAudioProcessor::PitchCorrectionPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
#endif
{
}

PitchCorrectionPluginAudioProcessor::~PitchCorrectionPluginAudioProcessor()
{
}

//==============================================================================
const juce::String PitchCorrectionPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PitchCorrectionPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PitchCorrectionPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PitchCorrectionPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PitchCorrectionPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PitchCorrectionPluginAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int PitchCorrectionPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PitchCorrectionPluginAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String PitchCorrectionPluginAudioProcessor::getProgramName(int index)
{
    return {};
}

void PitchCorrectionPluginAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
}

//==============================================================================
void PitchCorrectionPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // use of: "https://docs.juce.com/master/classjuce_1_1AudioProcessor.html#a440ef16a5c86486ec0fa11dafd42d117"
    // to understand initalising resources before audio processing. 
    
    pitchSamples.resize(detector.getWindowSize()); // to prepare the samples for detection.

    samplePosition = 0;
    detectedFrequency.store(0.0f);

    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void PitchCorrectionPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PitchCorrectionPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void PitchCorrectionPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    // use of previous code structure from original code to understand how to process audio blocks in JUCE.
    // "https://github.com/juce-framework/JUCE/blob/master/examples/CMake/AudioPlugin/PluginProcessor.cpp"

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // use of: "https://docs.juce.com/master/classjuce_1_1AudioBuffer.html"
    // to understand how to use getReadPointer() to access audio samples.

    if (totalNumInputChannels > 0 && buffer.getNumChannels() > 0 && !pitchSamples.empty())
    {
        const float *inputSamples = buffer.getReadPointer(0); // to get the input samples from the first channel.

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            pitchSamples[samplePosition] = inputSamples[i]; // to store the samples in the pitchSamples vector.
            samplePosition++;

            if (samplePosition >= detector.getWindowSize())
            {
                float frequency = detector.detectPitch(pitchSamples.data(), getSampleRate()); // to detect the pitch using autocorrelation.
                detectedFrequency.store(frequency);                                           // to store detected frequency in atomic variable.

                samplePosition = 0; // to reset the sample position for the next block of samples.
            }
        }
    }
}

//==============================================================================
bool PitchCorrectionPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *PitchCorrectionPluginAudioProcessor::createEditor()
{
    return new PitchCorrectionPluginAudioProcessorEditor(*this);
}

//==============================================================================
void PitchCorrectionPluginAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PitchCorrectionPluginAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new PitchCorrectionPluginAudioProcessor();
}
