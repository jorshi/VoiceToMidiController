/*
  ==============================================================================

    PitchDetection.cpp
    Created: 22 Mar 2017 11:41:06am
    Author:  Jordie Shier 

  ==============================================================================
*/

#include "PitchDetection.h"


PitchDetection::PitchDetection(int bufferSize) : readPos_(0)
{
    // Assumming that the length passed in will be a multiple of 2
    // Allocate half the buffer size length for the yin data sample buffer
    yinData_.setSize(1, bufferSize/2);
    inputBuffer_.setSize(1, bufferSize);
    
    // YIN default tolerance was 0.15
    tolerance_ = 0.15;
    
    pitch_ = -1.0;
}


PitchDetection::~PitchDetection()
{
}

void PitchDetection::runDetection(const float *samples, int numSamples)
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
            readPos_ = 0;
            updatePitch();
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


float PitchDetection::quadIntMin(int position)
{
    float s0, s1, s2;
    const float* data = yinData_.getReadPointer(0);
    
    // TODO: remove this -- the origin algorithm did some more checks
    // on the posistion, but I think it is safe to assume that position
    // received here will be a valid index in yinData
    jassert(position >= 0 && position < yinData_.getNumSamples());
    
    if (position == 0 || position == yinData_.getNumSamples() - 1)
        return position;
    
    s0 = data[position - 1];
    s1 = data[position];
    s2 = data[position + 1];
    
    return position + 0.5 * (s0 - s2) / (s0 - 2 * s1 + s2);
}

// Implementation of the YIN algorithm
void PitchDetection::updatePitch()
{
    int j, tau;
    int period;
    float tmp1 = 0.0, tmp2 = 0.0;
    
    float* yin = yinData_.getWritePointer(0);
    const float* input = inputBuffer_.getReadPointer(0);
    
    float detectedPitch_ = -1.0;
    
    yin[0] = 1.0;
    for (tau = 1; tau < yinData_.getNumSamples(); tau++)
    {
        yin[tau] = 0.0;
        for (j = 0; j < yinData_.getNumSamples(); j++)
        {
            tmp1 = input[j] - input[j+tau];
            yin[tau] += tmp1 * tmp1;
        }
        tmp2 += yin[tau];
        
        if (tmp2 != 0)
        {
            yin[tau] *= tau/tmp2;
        }
        else
        {
            yin[tau] = 1.0;
        }
        
        period = tau - 3;
        if (tau > 4 && yin[period] < tolerance_ && yin[period] < yin[period+1])
        {
            detectedPitch_ = quadIntMin(period);
            break;
        }
    }
    
    if (detectedPitch_ < 0.0)
    {
        // Run Quadtatic Interpolation on minimum element
        float* minElement = std::min(yin, yin + yinData_.getNumSamples());
        detectedPitch_ = quadIntMin(minElement - yin);
    }
    
    pitch_ = detectedPitch_;
}