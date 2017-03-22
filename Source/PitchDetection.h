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
    PitchDetection(int length);
    
    // Deconstructor
    ~PitchDetection();
    
    // Pass new samples into detection function
    void runDetection(const float* samples, int numSamples);
    
private:
    
    // Updates the pitch calculation using samples in the input buffer
    void updatePitch();
    
    int bufferSize_;
    int yinLengh_;
    int readPos_;

    float tolerance_;
    float pitch_;

    // Buffer used in calculation of pitch
    AudioBuffer<float> yinData_;
    AudioBuffer<float> inputBuffer_;
};



#endif  // PITCHDETECTION_H_INCLUDED
