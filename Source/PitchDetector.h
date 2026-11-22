#pragma once

class PitchDetector
{
public:
    // to set default values for window size and frequency limits for testing
    PitchDetector(int windowSize = 2048, float minFrequency = 80.0f, float maxFrequency = 1000.0f);

    float detectPitch(const float *samples, double sampleRate) const; // to detect pitch in Hz. 
    int getWindowSize() const;

private:
    int windowSize;
    float minFrequency;
    float maxFrequency;
};
