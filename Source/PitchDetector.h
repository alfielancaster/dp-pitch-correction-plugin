#pragma once

class PitchDetector
{
public:
    PitchDetector(int windowSize = 2048, float minFrequency = 80.0f, float maxFrequency = 1000.0f);

    float detectPitch(const float *samples, double sampleRate) const;
    int getWindowSize() const;

private:
    int windowSize;
    float minFrequency;
    float maxFrequency;
};
