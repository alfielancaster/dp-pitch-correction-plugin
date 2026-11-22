#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class PitchCorrectionPluginAudioProcessorEditor  : public juce::AudioProcessorEditor , private juce::Timer
{
public:
    PitchCorrectionPluginAudioProcessorEditor (PitchCorrectionPluginAudioProcessor&);
    ~PitchCorrectionPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Label frequencyLabel;

    void timerCallback() override;

    PitchCorrectionPluginAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchCorrectionPluginAudioProcessorEditor)
};
