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
#include <vector>

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

    // Run timbre analysis on input and return a midi value corresponding to that value
    int filteredTimbre(float f0);
    
    // Enter and exit the learning mode
    void startLearning();
    void stopLearning();    
    
private:
    
    // Update the centroid calculation with sampels from the input buffer
    void updateSpectrum();
    
    // Current spectral centroid
    float spectralCentroid_;
    float timbreRatio_;
    
    // Variables for FFT
    ScopedPointer<FFT> fft_;
    int fftSize_;
    
    // Buffer to store up samples and pointer to current position
    AudioBuffer<float> inputBuffer_;
    AudioBuffer<float> hann_;
    std::vector<FFT::Complex> fftInput_;
    std::vector<FFT::Complex> fftOutput_;
    std::vector<float> magnitude_;
    int readPos_;
    
    // Range of acceptable timbre ratios
    NormalisableRange<float> timbreRange_;
    
    // Sampling Rate
    static float rate_;
    
    float smoothingConstant_;
    
    // To indicate whether we are learning the range. When in the learning state
    // output of midi values is ceased and the learning min and max values are updated.
    // When learning mode is exited the timbreRange variable is updated which is used
    // in further conversions of the timbre ratio to midi values.
    bool isLearningRange_;
    float learningMin_;
    float learningMax_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimbreSimple)
};



#endif  // TIMBRESIMPLE_H_INCLUDED
