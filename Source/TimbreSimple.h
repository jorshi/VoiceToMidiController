/*
  ==============================================================================

    TimbreSimple.h
    Created: 5 Apr 2017 6:21:32pm
    Author:  Jordie Shier 

  ==============================================================================
*/

#ifndef TIMBRESIMPLE_H_INCLUDED
#define TIMBRESIMPLE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class TimbreSimple
{
public:
    
    // Constructor
    TimbreSimple(int fftSize);
    
    // Deconstructor
    ~TimbreSimple() {};
    
    // Run on new buffer
    void run(const float* samples, int numSamples);
    
    // Static method to set sampling rate
    static void setRate(float rate) { rate_ = rate; };
    
    
private:
    
    void updateCentroid();
    
    // Current spectral centroid
    float spectralCentroid_;
    
    // Variables for FFT
    ScopedPointer<FFT> fft_;
    int fftSize_;
    
    // Buffer to store up samples and pointer to current position
    AudioBuffer<float> inputBuffer_;
    int readPos_;
    
    // Sampling Rate
    static float rate_;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimbreSimple)
};



#endif  // TIMBRESIMPLE_H_INCLUDED
