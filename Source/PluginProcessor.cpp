/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
VoiceToMidiControllerAudioProcessor::VoiceToMidiControllerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    

    // Only output on virtual midi port if this is not windows
    #if JUCE_LINUX || JUCE_MAC || JUCE_IOS || DOXYGEN
    
    // Check for instances of the VoiceToMidi and increment the
    // instance count for identifying this instance.
    int instance = 1;
    StringArray activeDevices = MidiInput::getDevices();
    
    for (auto device = activeDevices.begin(); device != activeDevices.end(); ++device)
    {
        std::string deviceName = device->toStdString();
        std::size_t found = deviceName.find("VoiceToMidi ");
        if (found != std::string::npos)
        {
            // Extract the instance number and increment this instance
            int instanceCount = 0;
            std::string name = "";
            std::stringstream stream(deviceName);
            stream >> name >> instanceCount;
            instance = instanceCount + 1;
        }
    }
    
    // Create a new device
    midiOutput_ = MidiOutput::createNewDevice("VoiceToMidi " + std::to_string(instance));
    
    // Pitch detection object
    pitchDetection_ = new PitchDetection(1024);
    
    #endif
}

VoiceToMidiControllerAudioProcessor::~VoiceToMidiControllerAudioProcessor()
{
}

//==============================================================================
const String VoiceToMidiControllerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VoiceToMidiControllerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VoiceToMidiControllerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double VoiceToMidiControllerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VoiceToMidiControllerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VoiceToMidiControllerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VoiceToMidiControllerAudioProcessor::setCurrentProgram (int index)
{
}

const String VoiceToMidiControllerAudioProcessor::getProgramName (int index)
{
    return String();
}

void VoiceToMidiControllerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void VoiceToMidiControllerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void VoiceToMidiControllerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VoiceToMidiControllerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void VoiceToMidiControllerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Processing block
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
        
        // Pass samples in for pitch detection
        pitchDetection_->runDetection(channelData, buffer.getNumSamples());
    }
    
    // Create and output midi here. Send the message out on midiMessages as well
    // as on the virtual midi port if this is not Windows
    #if JUCE_LINUX || JUCE_MAC || JUCE_IOS || DOXYGEN
    
    // Send out on the virtual midi port here
    
    #endif
    
}

//==============================================================================
bool VoiceToMidiControllerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* VoiceToMidiControllerAudioProcessor::createEditor()
{
    return new VoiceToMidiControllerAudioProcessorEditor (*this);
}

//==============================================================================
void VoiceToMidiControllerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void VoiceToMidiControllerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VoiceToMidiControllerAudioProcessor();
}
