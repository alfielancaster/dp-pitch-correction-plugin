
#include "PitchDetector.h"
#include <cmath>

PitchDetector::PitchDetector(int windowSize, float minFrequencyIn, float maxFrequencyIn)
    : windowSize(windowSize), minFrequency(minFrequencyIn), maxFrequency(maxFrequencyIn)
{
}

int PitchDetector::getWindowSize() const
{
    return windowSize;
}

float PitchDetector::detectPitch(const float *samples, double sampleRate) const
{
    if (samples == nullptr || windowSize < 4 || sampleRate <= 0.0 || minFrequency <= 0.0f || maxFrequency <= minFrequency)
        return 0.0f;

    double energy = 0.0;
    for (int i = 0; i < windowSize; ++i)
    {
        double sample = samples[i];
        energy += sample * sample;
    }

    double rms = std::sqrt(energy / windowSize);

    if (rms < 0.01)
        return 0.0f;

    int minLag = static_cast<int>(sampleRate / maxFrequency);
    int maxLag = static_cast<int>(sampleRate / minFrequency);

    if (minLag < 1)
        minLag = 1;

    if (maxLag > windowSize / 2)
        maxLag = windowSize / 2;

    if (minLag + 2 > maxLag)
        return 0.0f;

    double twoLagsAgo = 0.0;
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

        if (lag >= minLag + 2 &&
            oneLagAgo > twoLagsAgo &&
            oneLagAgo >= correlation &&
            oneLagAgo > energy * 0.3)
        {
            int detectedLag = lag - 1;

            float frequency = static_cast<float>(sampleRate / detectedLag);

            if (frequency >= minFrequency && frequency <= maxFrequency)
                return frequency;

            return 0.0f;
        }

        twoLagsAgo = oneLagAgo;
        oneLagAgo = correlation;
    }

    return 0.0f;
}
