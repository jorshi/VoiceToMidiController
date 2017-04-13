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
    setLookAndFeel(&lookAndFeel);

    startTimer(50);
    setSize (500, 350);
    
    // Areas
    header.setBounds(0, 0, getWidth(), 48);
    pitchDisplay.setBounds((getWidth()/2) - 70, header.getBottom() + 20, 140, 75);
    timbreArea.setBounds(25, pitchDisplay.getBottom() + 35, getWidth() - 50, 100);
    
    addAndMakeVisible(&timbreLearning);
    timbreLearning.setButtonText("Learn Timbre");
    timbreLearning.setClickingTogglesState(true);
    timbreLearning.setToggleState(false, dontSendNotification);
    timbreLearning.addListener(this);

    AudioProcessorValueTreeState* parameters = processor.getParameters();
    
    // Create the pitch smoothing slider and connect to the parameters
    addAndMakeVisible(&pitchSmoothingSlider);
    pitchSmoothingSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    pitchSmoothingSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(&pitchSmoothingLabel);
    pitchSmoothingLabel.setText("Pitch Smoothing", dontSendNotification);
    pitchSmoothingLabel.attachToComponent(&pitchSmoothingSlider, false);
    pitchSmoothingAttachment = new SliderAttachment(*parameters,
                                                    "pitch_smoothing",
                                                    pitchSmoothingSlider);
    
    // Timbre Attack Smoothing Envelope
    addAndMakeVisible(&timbreAttackSlider);
    timbreAttackSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    timbreAttackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    timbreAttackSlider.setTextValueSuffix(" ms");
    
    addAndMakeVisible(&timbreAttackLabel);
    timbreAttackLabel.setText("Attack", dontSendNotification);
    timbreAttackLabel.attachToComponent(&timbreAttackSlider, false);
    timbreAttackLabel.setJustificationType(Justification::centred);
    timbreAttackAttachment = new SliderAttachment(*parameters,
                                                  "timbre_attack",
                                                  timbreAttackSlider);
    
    // Timbre Attack Smoothing Envelope
    addAndMakeVisible(&timbreReleaseSlider);
    timbreReleaseSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    timbreReleaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    timbreReleaseSlider.setTextValueSuffix(" ms");
    
    addAndMakeVisible(&timbreReleaseLabel);
    timbreReleaseLabel.setText("Release", dontSendNotification);
    timbreReleaseLabel.attachToComponent(&timbreReleaseSlider, false);
    timbreReleaseLabel.setJustificationType(Justification::centred);
    timbreReleaseAttachment = new SliderAttachment(*parameters,
                                                  "timbre_release",
                                                  timbreReleaseSlider);
    // Resize everything
    resized();
    
}

VoiceToMidiControllerAudioProcessorEditor::~VoiceToMidiControllerAudioProcessorEditor()
{
}

//==============================================================================
void VoiceToMidiControllerAudioProcessorEditor::paint (Graphics& g)
{
    g.setGradientFill(LoomLookAndFeel::gradient);
    g.fillAll();
    
    // Header
    g.setColour(Colour::fromRGB(47, 47, 47));
    g.fillRect(header);
    
    g.setColour(Colour::fromRGB(40, 40, 40));
    g.drawLine(0, timbreArea.getY(), getWidth(), timbreArea.getY(), 2.0f);
    
    g.setColour(LoomLookAndFeel::buttonTextColour);
    g.setFont(header.getHeight() -10);
    g.drawText("V2M", header.getX(), header.getY(), 100, header.getHeight(), Justification::centred);
    
    g.setColour(LoomLookAndFeel::boundaryColour);
    g.drawLine(0, header.getBottom(), header.getWidth(), header.getBottom());
    
    g.setColour(LoomLookAndFeel::buttonEdgeColour);
    g.drawRoundedRectangle(pitchDisplay.toFloat(), 2.0f, 1.0f);
   
    // Draw detected pitch
    String pitch = midiNoteToString(processor.getDetectedMidiNote());
    g.setColour (LoomLookAndFeel::buttonTextColour);
    g.setFont (50.0f);
    g.drawText(pitch, pitchDisplay, Justification::centred);
    
    g.setColour(LoomLookAndFeel::loomTextColour);
    g.setFont(20);
    g.drawText("Timbre", timbreArea.getX(), timbreArea.getY() + 15, timbreArea.getWidth(), 20, Justification::centred);
    
}

void VoiceToMidiControllerAudioProcessorEditor::resized()
{
    pitchSmoothingSlider.setBounds(pitchDisplay.getRight() + 50, pitchDisplay.getY() + 15, 90, 65);
    
    int topOffset = 60;
    timbreAttackSlider.setBounds(timbreArea.getX() + 10, timbreArea.getY() + topOffset, 90, 65);
    timbreReleaseSlider.setBounds(timbreArea.getX() + 120, timbreArea.getY() + topOffset, 90, 65);
    timbreLearning.setBounds(timbreArea.getX() + 255, timbreArea.getY() + topOffset + 10, 175, 35);
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

String VoiceToMidiControllerAudioProcessorEditor::midiNoteToString(int note)
{
    String noteString;
    if (note >= 0)
    {
        int octave = std::floor(note / 12.0f);
        noteString = noteMap[note % 12];
        noteString += std::to_string(octave);
    }
    else
    {
            noteString = "-";
    }
    return noteString;
}
