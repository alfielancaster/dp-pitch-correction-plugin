#include "PitchDetector.h"
#include <cmath>

// to set the window size and frequency range for pitch detection. 
PitchDetector::PitchDetector(int windowSize, float minFrequencyIn, float maxFrequencyIn)
    : windowSize(windowSize), minFrequency(minFrequencyIn), maxFrequency(maxFrequencyIn)
{
}

int PitchDetector::getWindowSize() const
{
    return windowSize;
}

// use of: "https://dsp-workshop.nl/topics/pitch-detection/" & "https://dsp.gundy.dev/audio-coding/pitch-detection/"
// throughout to understand method and its uses in pitch detection.

// to detect pitch using autocorrelation by comparing audio samplyes at different lags.
float PitchDetector::detectPitch(const float *samples, double sampleRate) const
{
    // to check for invalid inputs.
    if (samples == nullptr || windowSize < 4 || sampleRate <= 0.0 || minFrequency <= 0.0f || maxFrequency <= minFrequency)
        return 0.0f;

    // to calculate signal energy.
    double energy = 0.0;
    for (int i = 0; i < windowSize; ++i)
    {
        double sample = samples[i];
        energy += sample * sample;
    }

    // to check if audio is too quiet to detect with 0.01 threshold used for testing.
    double rms = std::sqrt(energy / windowSize);

    if (rms < 0.01)
        return 0.0f;

    // to calcualte lag range from the frequncy limits.
    int minLag = static_cast<int>(sampleRate / maxFrequency);
    int maxLag = static_cast<int>(sampleRate / minFrequency);

    if (minLag < 1)
        minLag = 1;

    // to limit lag to allow for comparison.
    if (maxLag > windowSize / 2)
        maxLag = windowSize / 2;

    if (minLag + 2 > maxLag)
        return 0.0f;

    double twoLagsAgo = 0.0; // storing these values to help find a peak.
    double oneLagAgo = 0.0;

    for (int lag = minLag; lag <= maxLag; ++lag)
    {
        double correlation = 0.0;

        for (int i = 0; i < windowSize - lag; ++i)
        {
            double firstSample = samples[i];
            double delayedSample = samples[i + lag];

            correlation += firstSample * delayedSample;
        }

        // this finds the first strong peak and using 0.3 threshold for testing.
        if (lag >= minLag + 2 &&
            oneLagAgo > twoLagsAgo &&
            oneLagAgo >= correlation &&
            oneLagAgo > energy * 0.3)
        {
            int detectedLag = lag - 1;

            float frequency = static_cast<float>(sampleRate / detectedLag); // to convert detected lag into hz.

            if (frequency >= minFrequency && frequency <= maxFrequency)
                return frequency;

            return 0.0f;
        }

        twoLagsAgo = oneLagAgo;
        oneLagAgo = correlation;
    }

    return 0.0f;
}
