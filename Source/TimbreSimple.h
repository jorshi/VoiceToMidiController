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
    
    // Returns the current centroid as a frequency
    float getCurrentCentroid() const;
    
    // Get a timbre ratio value as a midi value
    int getTimbreAsMidiValue(float f0) const;
    
    
private:
    
    // Update the centroid calculation with sampels from the input buffer
    void updateCentroid();
    
    // Current spectral centroid
    float spectralCentroid_;
    
    // Variables for FFT
    ScopedPointer<FFT> fft_;
    int fftSize_;
    
    // Buffer to store up samples and pointer to current position
    AudioBuffer<float> inputBuffer_;
    AudioBuffer<float> fftBuffer_;
    int readPos_;
    
    // Range of acceptable timbre ratios
    NormalisableRange<float> timbreRange_;
    
    // Sampling Rate
    static float rate_;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimbreSimple)
};



#endif  // TIMBRESIMPLE_H_INCLUDED
