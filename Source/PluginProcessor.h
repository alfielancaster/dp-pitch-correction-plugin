/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PitchDetector.h"
#include <vector>
#include <atomic>

//==============================================================================
/**
 */
class PitchCorrectionPluginAudioProcessor : public juce::AudioProcessor
{
public:
  //==============================================================================
  PitchCorrectionPluginAudioProcessor();
  ~PitchCorrectionPluginAudioProcessor() override;

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

  //==============================================================================
  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String &newName) override;

  //==============================================================================
  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

  float getDetectedFrequency() const // to return the lastest frequency detected by the algorithm.
  {
    return detectedFrequency.load();
  }

private:
  //==============================================================================
  PitchDetector detector;
  std::vector<float> pitchSamples;
  int samplePosition = 0;

  // use of: "https://en.cppreference.com/cpp/atomic/atomic"
  // to understand how to use atomic variables in c++.
  std::atomic<float> detectedFrequency{0.0f}; // to store the frequency so the plugin can access it

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchCorrectionPluginAudioProcessor)
};
