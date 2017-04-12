/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class VoiceToMidiControllerAudioProcessorEditor  :  public AudioProcessorEditor,
                                                    public ButtonListener,
                                                    private Timer
{
public:
    VoiceToMidiControllerAudioProcessorEditor (VoiceToMidiControllerAudioProcessor&);
    ~VoiceToMidiControllerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void buttonClicked(Button* button) override;

private:
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    
    void timerCallback() override;
    
    // Buttons
    TextButton timbreLearning;
    
    // Sliders
    Slider pitchSmoothingSlider;
    
    // Attachments
    ScopedPointer<SliderAttachment> pitchSmoothingAttachment;
    
    // Labels
    Label pitchSmoothingLabel;
    
    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VoiceToMidiControllerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoiceToMidiControllerAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
