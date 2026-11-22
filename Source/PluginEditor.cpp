#include "PluginProcessor.h"
#include "PluginEditor.h"

// use of: "https://docs.juce.com/master/classjuce_1_1Timer.html"
// used between files to understand using timers to update plugin display. 

//==============================================================================
PitchCorrectionPluginAudioProcessorEditor::PitchCorrectionPluginAudioProcessorEditor(PitchCorrectionPluginAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    frequencyLabel.setText("No pitch detected", juce::dontSendNotification); // to set up label display
    frequencyLabel.setJustificationType(juce::Justification::centred);
    frequencyLabel.setFont(juce::FontOptions(24.0f));

    addAndMakeVisible(frequencyLabel);

    startTimerHz(10); // used to understand updating the plugin display.
    setSize(400, 200);
}

PitchCorrectionPluginAudioProcessorEditor::~PitchCorrectionPluginAudioProcessorEditor()
{
}

void PitchCorrectionPluginAudioProcessorEditor::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(15.0f));
    g.drawFittedText("Pitch Detector Initial Prototype", 0, 20, getWidth(), 30, juce::Justification::centred, 1);
}

void PitchCorrectionPluginAudioProcessorEditor::resized()
{
    frequencyLabel.setBounds(20, 65, getWidth() - 40, 90); // to be displayed below the title text and centered. 
}

void PitchCorrectionPluginAudioProcessorEditor::timerCallback()
{
    float frequency = audioProcessor.getDetectedFrequency(); // to get the latest detected frequency.

    if (frequency > 0.0f)  
    {
        frequencyLabel.setText("Detected Frequency: " + juce::String(frequency, 1) + " Hz", juce::dontSendNotification);
    }
    else
    {
        frequencyLabel.setText("No pitch detected", juce::dontSendNotification);
    }
}
