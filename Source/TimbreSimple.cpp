/*
  ==============================================================================

    TimbreSimple.cpp
    Created: 5 Apr 2017 6:21:32pm
    Author:  Jordie Shier 

  ==============================================================================
*/

#include "TimbreSimple.h"


TimbreSimple::TimbreSimple(int fftSize) : spectralCentroid_(0.0f)
{
    // Make sure this is a power of 2
    fftSize_ = nextPowerOfTwo(fftSize);
    
    // Forward FFT
    fft_ = new FFT(std::log2(fftSize), false);
    
    inputBuffer_.setSize(1, fftSize);
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
    fft_->performFrequencyOnlyForwardTransform(inputBuffer_.getWritePointer(0));
    
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

// Default rate -- but it will need to be set from the processor
float TimbreSimple::rate_ = 44100.0f;
