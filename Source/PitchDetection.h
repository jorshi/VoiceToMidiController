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
    float getPitch() const { return pitch_; };
    
private:
    
    // Updates the pitch calculation using samples in the input buffer
    void updatePitch();
    
    // Quadratic interpolation on the Yin buffer
    float quadIntMin(int position);
    
    // Read pointer for input buffer
    int readPos_;

    // Pitch tolerance and the current detected pitch estimation
    float tolerance_;
    float pitch_;

    // Buffer used in calculation of pitch
    AudioBuffer<float> yinData_;
    AudioBuffer<float> inputBuffer_;
};



#endif  // PITCHDETECTION_H_INCLUDED
