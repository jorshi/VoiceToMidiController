/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PitchDetection.h"

#include <string>
#include <sstream>


//==============================================================================
/**
*/
class VoiceToMidiControllerAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    VoiceToMidiControllerAudioProcessor();
    ~VoiceToMidiControllerAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    float getDetectedF0();
    int getDetectedMidiNote();
    
    AudioProcessorValueTreeState* getParameters() const { return parameters_; };
    int dbToVelocity(float db);

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoiceToMidiControllerAudioProcessor)
    
    ScopedPointer<MidiOutput> midiOutput_;
    ScopedPointer<PitchDetection> pitchDetection_;
    ScopedPointer<AudioProcessorValueTreeState> parameters_;
    
    // Midi messages to facilitate switching between notes
    MidiMessage playingNote;
    MidiMessage nextNote;
    MidiMessage noteOff;
    
    bool isPlaying;
    
    const double startTime;
    double noteTime;
    
    NormalisableRange<float> dbRange_;
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
