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
    
    // Update buffer sizes
    inputBuffer_.setSize(1, fftSize);
    fftInput_.resize(fftSize);
    fftOutput_.resize(fftSize);
    
    // Range of accepted timbre ratios -- this is just a guess right now
    timbreRange_ = NormalisableRange<float>(0.0f, 20.0f);
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
    const float* sample = inputBuffer_.getReadPointer(0);
    for (int i = 0; i < inputBuffer_.getNumSamples(); i++)
    {
        fftInput_[i].r = sample[i];
        fftInput_[i].i = 0;
    }
    
    fft_->perform(fftInput_.data(), fftOutput_.data());
    
    float weightedSum = 0.0f;
    float sum = 0.0f;
    float mag;
    
    // Calculate summations for spectral centroid
    for (int i = 0; i < fftOutput_.size() / 2; i++)
    {
        mag = sqrt(fftOutput_[i].r * fftOutput_[i].r + fftOutput_[i].i * fftOutput_[i].i);
        weightedSum += mag * i;
        sum += mag;
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
