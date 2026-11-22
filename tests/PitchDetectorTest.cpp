#include "../Source/PitchDetector.h"

#include <cmath>
#include <iostream>
#include <vector>

int main()
{
    const double sampleRate = 44100.0;
    const double frequency = 440.0;
    const double pi = 3.14159265358979323846;

    PitchDetector detector;

    std::vector<float> samples(detector.getWindowSize());

    for (int i = 0; i < detector.getWindowSize(); ++i)
    {
        samples[i] = static_cast<float>(
            0.5 * std::sin(2.0 * pi * frequency * i / sampleRate));
    }

    float detectedFrequency = detector.detectPitch(samples.data(), sampleRate);

    std::cout << "Expected Frequency: " << frequency << " Hz" << std::endl;
    std::cout << "Detected Frequency: " << detectedFrequency << " Hz" << std::endl;

    return 0;
}
