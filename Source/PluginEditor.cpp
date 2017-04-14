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
    
    addAndMakeVisible(&timbreLearning);
    timbreLearning.setButtonText("Learn Timbre");
    timbreLearning.setClickingTogglesState(true);
    timbreLearning.setToggleState(false, dontSendNotification);
    timbreLearning.addListener(this);

    AudioProcessorValueTreeState* parameters = processor.getParameters();
    
    // Create the pitch smoothing slider and connect to the parameters
    addAndMakeVisible(&pitchSmoothingSlider);
    addAndMakeVisible(&pitchSmoothingLabel);
    pitchSmoothingLabel.setText("Pitch Smoothing", dontSendNotification);
    pitchSmoothingLabel.attachToComponent(&pitchSmoothingSlider, false);
    pitchSmoothingAttachment = new SliderAttachment(*parameters,
                                                    "pitch_smoothing",
                                                    pitchSmoothingSlider);
    
    // Timbre Attack Smoothing Envelope
    addAndMakeVisible(&timbreAttackSlider);
    addAndMakeVisible(&timbreAttackLabel);
    timbreAttackLabel.setText("Timbre Attack", dontSendNotification);
    timbreAttackLabel.attachToComponent(&timbreAttackSlider, false);
    timbreAttackAttachment = new SliderAttachment(*parameters,
                                                  "timbre_attack",
                                                  timbreAttackSlider);
    
    // Timbre Attack Smoothing Envelope
    addAndMakeVisible(&timbreReleaseSlider);
    addAndMakeVisible(&timbreReleaseLabel);
    timbreReleaseLabel.setText("Timbre Release", dontSendNotification);
    timbreReleaseLabel.attachToComponent(&timbreReleaseSlider, false);
    timbreReleaseAttachment = new SliderAttachment(*parameters,
                                                  "timbre_release",
                                                  timbreReleaseSlider);
    
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
    g.drawFittedText(pitch, 0, 50, getWidth(), 50.0f, Justification::centred, 1);
}

void VoiceToMidiControllerAudioProcessorEditor::resized()
{
    pitchSmoothingSlider.setBounds(10, 125, 250, 20);
    timbreAttackSlider.setBounds(10, 165, 250, 20);
    timbreReleaseSlider.setBounds(10, 205, 250, 20);
    timbreLearning.setBounds(10, 250, 200, 25);
}


void VoiceToMidiControllerAudioProcessorEditor::timerCallback()
{
    repaint();
}

void VoiceToMidiControllerAudioProcessorEditor::buttonClicked(juce::Button *button)
{
    if (button == &timbreLearning)
    {
        if (button->getToggleState())
        {
            processor.startTimbreLearning();
        }
        else
        {
            processor.stopTimbreLearning();
        }
    }
}
