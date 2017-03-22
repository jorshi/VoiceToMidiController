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
}

VoiceToMidiControllerAudioProcessorEditor::~VoiceToMidiControllerAudioProcessorEditor()
{
}

//==============================================================================
void VoiceToMidiControllerAudioProcessorEditor::paint (Graphics& g)
{
    String pitch = std::to_string(processor.getDetectedPitch());
    
    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.setFont (15.0f);
    g.drawFittedText (pitch, getLocalBounds(), Justification::centred, 1);
}

void VoiceToMidiControllerAudioProcessorEditor::resized()
{
}


void VoiceToMidiControllerAudioProcessorEditor::timerCallback()
{
    repaint();
}
