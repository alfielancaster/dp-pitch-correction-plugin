#include "../Source/PitchDetector.h"

#include <cmath>
#include <iostream>
#include <vector>

int main()
{
    // to set sample rate and the expected frequency. 
    const double sampleRate = 44100.0;
    const double frequency = 440.0;
    const double pi = 3.14159265358979323846;

    PitchDetector detector;

    std::vector<float> samples(detector.getWindowSize()); // to store generates samples for testing.

    // to generate a 440 Hz sine wave for testing the pitch detection.
    // use of: "https://juce.com/tutorials/tutorial_sine_synth/"
    // to understand sine wave generation using frequency and sample rate.
    for (int i = 0; i < detector.getWindowSize(); ++i)
    {
        samples[i] = static_cast<float>(
            0.5 * std::sin(2.0 * pi * frequency * i / sampleRate));
    }

    float detectedFrequency = detector.detectPitch(samples.data(), sampleRate); // to test pitch detector with generated sample.

    std::cout << "Expected Frequency: " << frequency << " Hz" << std::endl; // to output the expected and detected frequency for comparison.
    std::cout << "Detected Frequency: " << detectedFrequency << " Hz" << std::endl;

    return 0;
}
