/*
  ==============================================================================

    TimbreSimple.cpp
    Created: 5 Apr 2017 6:21:32pm
    Author:  Jordie Shier 

  ==============================================================================
*/

#include "TimbreSimple.h"


TimbreSimple::TimbreSimple(int fftSize) : readPos_(0)
{
    // Make sure this is a power of 2
    fftSize_ = nextPowerOfTwo(fftSize);
    
    // Forward FFT
    fft_ = new FFT(std::log2(fftSize_), false);
    
    // Update buffer sizes
    inputBuffer_.setSize(1, fftSize_);
    hann_.setSize(1, fftSize_);
    fftInput_.resize(fftSize_);
    fftOutput_.resize(fftSize_);
    magnitude_.resize(fftSize_/2);
    
    // Range of accepted timbre ratios -- this is just a guess right now
    timbreRange_ = NormalisableRange<float>(1.0f, 7.0f);
    timbreRatio_ = timbreRange_.start;
 
    // Pre-calculate a Hann window
    float* sample = hann_.getWritePointer(0);
    int N = hann_.getNumSamples();
    for (int i = 0; i < N; i++)
    {
        sample[i] = 0.5 - 0.5 * cos((2 * double_Pi * i) / (N-1));
    }
    
    isLearningRange_ = false;
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
            updateSpectrum();
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


void TimbreSimple::updateSpectrum()
{
    // Perform forward FFT on input samples -- performed in place
    const float* sample = inputBuffer_.getReadPointer(0);
    const float* window = hann_.getReadPointer(0);
    for (int i = 0; i < inputBuffer_.getNumSamples(); i++)
    {
        fftInput_[i].r = sample[i] * window[i];
        fftInput_[i].i = 0;
    }
    
    fft_->perform(fftInput_.data(), fftOutput_.data());
    
    // Calculate summations for spectral centroid
    for (int i = 0; i < fftOutput_.size() / 2; i++)
    {
        magnitude_[i] = sqrt(fftOutput_[i].r * fftOutput_[i].r + fftOutput_[i].i * fftOutput_[i].i);
    }
}


int TimbreSimple::filteredTimbre(float f0)
{
    if (f0 > 0)
    {
        // Get the magnitude bins from current FFT based on the current detected pitch
        float binFreq = rate_ / fftSize_;
        int lowerBin = std::floor((f0*2) / binFreq);
        int upperBin = std::ceil((f0*8) / binFreq);
        
        // Sum and weighted some for spectral centroid calculation
        float sum = 0.0f;
        float weightedSum = 0.0f;
        
        // Spectral centroid on filtered portion of the magnitude spectrum
        for (int i = lowerBin; i <= upperBin; i++)
        {
            weightedSum += magnitude_[i] * i;
            sum += magnitude_[i];
        }
        
        // Spectral centroid and the ratio with respect to the detected F0
        float centroid = (weightedSum / sum) * binFreq;
        float ratio = centroid / f0;
        
        if (!isLearningRange_)
        {
            // Simple smoothing function for timbre
            if (ratio > timbreRatio_)
            {
                timbreRatio_ = (0.99 * ratio) + (0.01 * timbreRatio_);
            }
            else
            {
                timbreRatio_ = (0.11 * ratio) + (0.89 * timbreRatio_);
            }
            
            // Convert to MIDI value and return
            ratio = timbreRange_.snapToLegalValue(timbreRatio_);
            return timbreRange_.convertTo0to1(ratio) * 127;
        }
        else // Learning mode, continually update min and max values of detected ratios
        {
            // Update the range learning variables
            if (ratio < learningMin_)
            {
                learningMin_ = ratio;
            }
            
            if (ratio > learningMax_)
            {
                learningMax_ = ratio;
            }

            return 0;
        }

    }
    
    return 0;
}

void TimbreSimple::startLearning()
{
    isLearningRange_ = true;
    learningMin_ = MAXFLOAT;
    learningMax_ = 0.0f;
}

void TimbreSimple::stopLearning()
{
    timbreRange_ = NormalisableRange<float>(learningMin_, learningMax_);
    isLearningRange_ = false;
}


// Default rate -- but it will need to be set from the processor
float TimbreSimple::rate_ = 44100.0f;
