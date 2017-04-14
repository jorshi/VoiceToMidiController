/*
  ==============================================================================

    LoomLookAndFeel.h
    Created: 18 Mar 2017 1:48:42am
    Author:  Jordie Shier 

    This is a look and feel template from an existing project by the author.
    Using this here for the colour scheme and button drawing.
 
  ==============================================================================
*/

#ifndef LOOMLOOKANDFEEL_H_INCLUDED
#define LOOMLOOKANDFEEL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class LoomLookAndFeel : public LookAndFeel_V3
{
public:
    
    // Default Constructor
    LoomLookAndFeel();
    
    // Custom Buttons
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override;
    
    // Custom Button Text
    Font getTextButtonFont (TextButton&, int buttonHeight) override;
    
    // Custom Label Font
    Font getLabelFont (Label& label) override;
    
    // Colours    
    static const Colour buttonEdgeColour;
    static const Colour buttonTextColour;
    static const Colour loomTextColour;
    static const Colour boundaryColour;
    static const Colour spectraColour;
    static const ColourGradient gradient;
    
    void setButtonRadius(float radius) { buttonRadius = radius; };

private:
    
    float buttonRadius;
};

#endif  // LOOMLOOKANDFEEL_H_INCLUDED
