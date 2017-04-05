/*
  ==============================================================================

    PitchDetection.h
    Created: 22 Mar 2017 11:41:06am
    Author:  Jordie Shier 

  ==============================================================================
*/

#ifndef PITCHDETECTION_H_INCLUDED
#define PITCHDETECTION_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <numeric>
#include <algorithm>

class PitchDetection
{
public:
    
    //Constuctor
    PitchDetection(int bufferSize);
    
    // Deconstructor
    ~PitchDetection();
    
    // Pass new samples into detection function
    void runDetection(const float* samples, int numSamples);
    
    // Get the current estimation for pitch
    float getCurrentPitch() const { return detectedF0_[f0Pointer_]; };
    
    // Get an averaged pitch
    float getSmoothedPitch(int smoothingFactor) const;

    // Some defined parameters for pitch detection
    enum
    {
        maxFreqSmoothing = 100
    };
    
private:
    
    // Updates the pitch calculation using samples in the input buffer
    void updatePitch();
    
    // Quadratic interpolation on the Yin buffer
    float quadIntMin(int position);
    
    // Read pointer for input buffer
    int readPos_;

    // Pitch tolerance
    float tolerance_;

    // Buffer used in calculation of pitch
    AudioBuffer<float> yinData_;
    AudioBuffer<float> inputBuffer_;
    
    // Buffer for moving average of detected pitches
    Array<float> detectedF0_;
    int f0Pointer_;
};



#endif  // PITCHDETECTION_H_INCLUDED
