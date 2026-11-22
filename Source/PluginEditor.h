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
    juce::Label frequencyLabel; // to store label used to display detected frequency.

    void timerCallback() override; // to update the frequency display when the timer runs. 

    PitchCorrectionPluginAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchCorrectionPluginAudioProcessorEditor)
};
