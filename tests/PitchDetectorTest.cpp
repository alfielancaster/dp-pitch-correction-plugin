#include "../Source/PitchDetector.h"

#include <cmath>
#include <iostream>
#include <vector>

int main()
{
    // to set sample rate and pi value for generating sine waves. 
    const double sampleRate = 44100.0;
    const double pi = 3.14159265358979323846;

    PitchDetector detector;
    std::vector<float> samples(detector.getWindowSize()); // to store generates samples for testing.

    
    const double frequencies[] = {82.41, 110.0, 146.83, 
        196.0, 246.94, 329.63, 440.0, 
        587.33, 783.99, 880.0, 987.77}; // now using multiple frequencies for multiple musical notes.

    // use of: "https://juce.com/tutorials/tutorial_sine_synth/"
    // to understand sine wave generation using frequency and sample rate.

    // use of: "https://github.com/sevagh/pitch-detection/tree/master/test"
    // for the idea of sine wave testing for my very simple test implementation. 

    for (double frequency : frequencies)
    {
        // to calculate angle change for each sample. 
        double angleDelta = (frequency / sampleRate) * 2.0 * pi;
        double currentAngle = 0.0;

        // to generate sine wave using new angledelta and current angle for each sample.
        for (int i = 0; i < detector.getWindowSize(); ++i)
        {
            samples[i] = static_cast<float>(
                0.5 * std::sin(currentAngle));

            currentAngle += angleDelta;
        }

        // to detect the frequency using autocorrelation. 
        float detectedFrequency = detector.detectPitch(samples.data(), sampleRate);

        // use of: "https://cplusplus.com/reference/cmath/abs/" to return the positive value of the error.
        double error = std::abs(detectedFrequency - frequency);
        double errorPercentage = (error / frequency) * 100.0;

        std::cout << "Expected Frequency: " << frequency << " Hz" << std::endl;
        std::cout << "Detected Frequency: " << detectedFrequency << " Hz" << std::endl;
        std::cout << "Error: " << error << " Hz" << std::endl;
        std::cout << "Error Percentage: " << errorPercentage << "%" << std::endl;
        std::cout << std::endl;
    }

    // to fill sample buffer with silence to test for silence. 
    for (int i = 0; i < detector.getWindowSize(); ++i)
    {
        samples[i] = 0.0f;
    }

    float silenceResult = detector.detectPitch(samples.data(), sampleRate); // to check if the silence is detected. 

    std::cout << "Silence Test: " << silenceResult << " Hz" << std::endl;

    return 0;
}
