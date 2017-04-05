/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
VoiceToMidiControllerAudioProcessorEditor::VoiceToMidiControllerAudioProcessorEditor (VoiceToMidiControllerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{

    startTimer(50);
    setSize (400, 300);
    
    // Create the pitch smoothing slider and connect to the parameters
    addAndMakeVisible(&pitchSmoothingSlider);
    addAndMakeVisible(&pitchSmoothingLabel);
    pitchSmoothingLabel.setText("Pitch Smoothing", dontSendNotification);
    pitchSmoothingLabel.attachToComponent(&pitchSmoothingSlider, false);
    AudioProcessorValueTreeState* parameters = processor.getParameters();
    pitchSmoothingAttachment = new SliderAttachment(*parameters,
                                                    "pitch_smoothing",
                                                    pitchSmoothingSlider);
}

VoiceToMidiControllerAudioProcessorEditor::~VoiceToMidiControllerAudioProcessorEditor()
{
}

//==============================================================================
void VoiceToMidiControllerAudioProcessorEditor::paint (Graphics& g)
{
    String pitch = std::to_string(processor.getDetectedMidiNote());
    
    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.setFont (50.0f);
    g.drawFittedText (pitch, getLocalBounds(), Justification::centred, 1);
}

void VoiceToMidiControllerAudioProcessorEditor::resized()
{
    pitchSmoothingSlider.setBounds(10, 200, 250, 20);
}


void VoiceToMidiControllerAudioProcessorEditor::timerCallback()
{
    repaint();
}
