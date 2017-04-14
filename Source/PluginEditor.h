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
#include "LoomLookAndFeel.h"


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
    
    String midiNoteToString(int note);
    
    // Strings of pitches
    const String noteMap[12] = {
        "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"
    };
    
    // Recatngle Areas
    Rectangle<int> header;
    Rectangle<int> pitchDisplay;
    Rectangle<int> timbreArea;
    
    // Buttons
    TextButton timbreLearning;
    
    // Sliders
    Slider pitchSmoothingSlider;
    Slider timbreAttackSlider;
    Slider timbreReleaseSlider;
    
    // Attachments
    ScopedPointer<SliderAttachment> pitchSmoothingAttachment;
    ScopedPointer<SliderAttachment> timbreAttackAttachment;
    ScopedPointer<SliderAttachment> timbreReleaseAttachment;
    
    // Labels
    Label pitchSmoothingLabel;
    Label timbreAttackLabel;
    Label timbreReleaseLabel;
    
    LoomLookAndFeel lookAndFeel;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VoiceToMidiControllerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VoiceToMidiControllerAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
