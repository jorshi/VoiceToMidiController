/*
  ==============================================================================

    TimbreSimple.cpp
    Created: 5 Apr 2017 6:21:32pm
    Author:  Jordie Shier 

  ==============================================================================
*/

#include "TimbreSimple.h"


TimbreSimple::TimbreSimple(int fftSize) : spectralCentroid_(0.0f), readPos_(0)
{
    // Make sure this is a power of 2
    fftSize_ = nextPowerOfTwo(fftSize);
    
    // Forward FFT
    fft_ = new FFT(std::log2(fftSize), false);
    
    // Update the input buffer size
    inputBuffer_.setSize(1, fftSize);
    fftBuffer_.setSize(1, fftSize);
    
    // Range of accepted timbre ratios -- this is just a guess right now
    timbreRange_ = NormalisableRange<float>(20.0f, 500.0f);
}


void TimbreSimple::run(const float *samples, int numSamples)
{
    int samplesRead = 0;
    float* input = inputBuffer_.getWritePointer(0);
    
    // Fill the input buffer. When enough samples are in the buffer, run
    // pitch detection
    while (samplesRead < numSamples)
    {
        // Accumulated enough samples to perform pitch
        if (readPos_ >= inputBuffer_.getNumSamples())
        {
            updateCentroid();
            readPos_ = 0;
        }
        else
        {
            // Copy samples into the input buffer
            input[readPos_] = samples[samplesRead];
            readPos_++;
            samplesRead++;
        }
    }
}


void TimbreSimple::updateCentroid()
{
    // Perform forward FFT on input samples -- performed in place
    fftBuffer_.copyFrom(0, 0, inputBuffer_, 0, 0, inputBuffer_.getNumSamples());
    fft_->performFrequencyOnlyForwardTransform(fftBuffer_.getWritePointer(0));
    
    float weightedSum = 0.0f;
    float sum = 0.0f;
    const float* sample = inputBuffer_.getReadPointer(0);
    
    // Calculate summations for spectral centroid
    for (int i = 0; i < inputBuffer_.getNumSamples(); i++)
    {
        weightedSum += (*sample) * i;
        sum += (*sample);
        ++sample;;
    }
    
    spectralCentroid_ = weightedSum / sum;
}


float TimbreSimple::getCurrentCentroid() const
{
    return spectralCentroid_ * (rate_ / fftSize_);
}


int TimbreSimple::getTimbreAsMidiValue(float f0) const
{
    if (f0 > 0)
    {
        float timbreRatio = timbreRange_.snapToLegalValue(getCurrentCentroid() / f0);
        return timbreRange_.convertTo0to1(timbreRatio) * 127;
    }

    return 0;
}


// Default rate -- but it will need to be set from the processor
float TimbreSimple::rate_ = 44100.0f;
