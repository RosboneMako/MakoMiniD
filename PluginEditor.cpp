/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Windows.h"

//==============================================================================
MakoDist01AudioProcessorEditor::MakoDist01AudioProcessorEditor (MakoDist01AudioProcessor& p)
    : AudioProcessorEditor (&p), makoProcessor (p)
{
    addMouseListener(this, true);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(680, 249);

    //R4.00 Start a Timer that lets us update the screen periodically.
    startTimerHz(10);
    
    //R4.00 Background Image of Mini D.
    imgBack = juce::ImageCache::getFromMemory(BinaryData::mmd_back_01_png, BinaryData::mmd_back_01_pngSize);

    //****************************************************************************************
    //R4.00 INITIALIZE OUR CONTROLS.
    //****************************************************************************************
    //R4.00 Define JUCE Sliders
    GUI_Init_Switch_Slider(&sldKnob[e_AmpOn], makoProcessor.Setting[e_AmpOn], .0f, 1.0f, 1.0f, "");
    GUI_Init_Large_Slider(&sldKnob[e_AmpQual], makoProcessor.Setting[e_AmpQual], .0f, 3.0f, 1.0f, "");     
    GUI_Init_Large_Slider(&sldKnob[e_AmpGain], makoProcessor.Setting[e_AmpGain], .0f, 1.0f, .01f, "");     
    GUI_Init_Large_Slider(&sldKnob[e_AmpLP], makoProcessor.Setting[e_AmpLP], 2000, 12000, 100, "");        
    GUI_Init_Large_Slider(&sldKnob[e_AmpComp], makoProcessor.Setting[e_AmpComp], .0f, 1.0f, .01f, "");     
    GUI_Init_Large_Slider(&sldKnob[e_AmpCRatio], makoProcessor.Setting[e_AmpCRatio], .0f, 1.0f, .01f, ""); 
    GUI_Init_Large_Slider(&sldKnob[e_AmpThin], makoProcessor.Setting[e_AmpThin], .0f, 1.0f, .01f, "");     
    GUI_Init_Large_Slider(&sldKnob[e_AmpSlope], makoProcessor.Setting[e_AmpSlope], .0f, 1.0f, .01f, "");   
    GUI_Init_Large_Slider(&sldKnob[e_AmpThump], makoProcessor.Setting[e_AmpThump], .0f, 1.0f, .01f, "");   
    GUI_Init_Large_Slider(&sldKnob[e_AmpAir], makoProcessor.Setting[e_AmpAir], .0f, 1.0f, .01f, "");       
    GUI_Init_Large_Slider(&sldKnob[e_AmpBoom], makoProcessor.Setting[e_AmpBoom], .0f, 1.0f, .01f, "");     
    GUI_Init_Large_Slider(&sldKnob[e_AmpCrisp], makoProcessor.Setting[e_AmpCrisp], .0f, 1.0f, .01f, "");   
    GUI_Init_Large_Slider(&sldKnob[e_AmpPower], makoProcessor.Setting[e_AmpPower], .0f, 1.0f, .01f, "");   
    GUI_Init_Large_Slider(&sldKnob[e_AmpClean], makoProcessor.Setting[e_AmpClean], .0f, 1.0f, .01f, "");   
    GUI_Init_Large_Slider(&sldKnob[e_AmpVol], makoProcessor.Setting[e_AmpVol], .0f, 1.0f, .01f, "");       
    GUI_Init_Large_Slider(&sldKnob[e_AmpEQMode], makoProcessor.Setting[e_AmpEQMode], 0, 19, 1, "");        
    GUI_Init_Large_Slider(&sldKnob[e_AmpEQ1], makoProcessor.Setting[e_AmpEQ1], -1.0f, 1.0f, .02f, "");       
    GUI_Init_Large_Slider(&sldKnob[e_AmpEQ2], makoProcessor.Setting[e_AmpEQ2], -1.0f, 1.0f, .02f, "");       
    GUI_Init_Large_Slider(&sldKnob[e_AmpEQ3], makoProcessor.Setting[e_AmpEQ3], -1.0f, 1.0f, .02f, "");
    GUI_Init_Large_Slider(&sldKnob[e_AmpEQ4], makoProcessor.Setting[e_AmpEQ4], -1.0f, 1.0f, .02f, "");
    GUI_Init_Large_Slider(&sldKnob[e_AmpEQ5], makoProcessor.Setting[e_AmpEQ5], -1.0f, 1.0f, .02f, "");
    GUI_Init_Large_Slider(&sldKnob[e_AmpEQ6], makoProcessor.Setting[e_AmpEQ6], -1.0f, 1.0f, .02f, "");

    GUI_Init_Switch_Slider(&sldKnob[e_IROn], makoProcessor.Setting[e_IROn], .0f, 1.0f, 1.0f, "");
    GUI_Init_Small_Slider(&sldKnob[e_IR], makoProcessor.Setting[e_IR], .0f, 199.0f, 1.0f, "");             
    GUI_Init_Small_Slider(&sldKnob[e_IRSize], makoProcessor.Setting[e_IRSize], -1.0f, 1.0f, .0f, "");      
    GUI_Init_Small_Slider(&sldKnob[e_IRVol], makoProcessor.Setting[e_IRVol], .0f, 1.0f, .01f, "");         
    
    GUI_Init_Switch_Slider(&sldKnob[e_Mod1On], makoProcessor.Setting[e_Mod1On], .0f, 1.0f, 1.0f, ""); 
    GUI_Init_Large_Slider(&sldKnob[e_Mod1Rate], makoProcessor.Setting[e_Mod1Rate], .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod1Depth], makoProcessor.Setting[e_Mod1Depth], .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod1Mix], makoProcessor.Setting[e_Mod1Mix], .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod1Synth], makoProcessor.Setting[e_Mod1Synth], .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod1Bronz], makoProcessor.Setting[e_Mod1Bronz], .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod1Vol], makoProcessor.Setting[e_Mod1Vol], .0f, 1.0f, .01f, "");      
        
    GUI_Init_Switch_Slider(&sldKnob[e_Mod2On], makoProcessor.Setting[e_Mod2On], .0f, 1.0f, 1.0f, ""); 
    GUI_Init_Large_Slider(&sldKnob[e_Mod2Rate], makoProcessor.Setting[e_Mod2Rate], .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod2Range], makoProcessor.Setting[e_Mod2Range], .0f, 2200.0f, 20.0f, " Hz");
    GUI_Init_Large_Slider(&sldKnob[e_Mod2FStart], makoProcessor.Setting[e_Mod2FStart], 100.0f, 500.0f, 5.0f, " Hz");
    GUI_Init_Large_Slider(&sldKnob[e_Mod2Mix], makoProcessor.Setting[e_Mod2Mix], .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod2Vol], makoProcessor.Setting[e_Mod2Vol], .0f, 1.0f, .01f, "");  

    GUI_Init_Switch_Slider(&sldKnob[e_Mod3On], makoProcessor.Setting[e_Mod3On], .0f, 1.0f, 1.0f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod3Rate], makoProcessor.Setting[e_Mod3Rate], .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod3Depth], makoProcessor.Setting[e_Mod3Depth], .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod3Tone], makoProcessor.Setting[e_Mod3Tone], .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod3Mix], makoProcessor.Setting[e_Mod3Mix], .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod3Bal], makoProcessor.Setting[e_Mod3Bal], .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod3Vol], makoProcessor.Setting[e_Mod3Vol], .0f, 1.0f, .01f, "");

    GUI_Init_Switch_Slider(&sldKnob[e_Mod4On], makoProcessor.Setting[e_Mod4On], .0f, 1.0f, 1.0f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod4Foff], makoProcessor.Setting[e_Mod4Foff], .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod4Doff], makoProcessor.Setting[e_Mod4Doff], .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod4Tone], makoProcessor.Setting[e_Mod4Tone], .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod4Mix], makoProcessor.Setting[e_Mod4Mix], .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod4Bal], makoProcessor.Setting[e_Mod4Bal], .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Mod4Vol], makoProcessor.Setting[e_Mod4Vol], .0f, 1.0f, .01f, "");

    GUI_Init_Switch_Slider(&sldKnob[e_DelOn], makoProcessor.Setting[e_DelOn], .0f, 1.0f, 1.0f, "");
    GUI_Init_Small_Slider(&sldKnob[e_DelTime], makoProcessor.Setting[e_DelTime], .0f, 1.0f, .01f, "");      
    GUI_Init_Small_Slider(&sldKnob[e_DelMix], makoProcessor.Setting[e_DelMix], .0f, 1.0f, .01f, "");
    GUI_Init_Small_Slider(&sldKnob[e_DelOffset], makoProcessor.Setting[e_DelOffset], .0f, 1.0f, .01f, "");
    GUI_Init_Small_Slider(&sldKnob[e_DelRepeat], makoProcessor.Setting[e_DelRepeat], .0f, 1.0f, .01f, "");
    GUI_Init_Small_Slider(&sldKnob[e_DelBal], makoProcessor.Setting[e_DelBal], .0f, 1.0f, .01f, "");
    GUI_Init_Small_Slider(&sldKnob[e_DelDuck], makoProcessor.Setting[e_DelDuck], .0f, 1.0f, .01f, "");
    
    GUI_Init_Switch_Slider(&sldKnob[e_RevOn], makoProcessor.Setting[e_RevOn], .0f, 1.0f, 1.0f, "");
    GUI_Init_Small_Slider(&sldKnob[e_RevMode], makoProcessor.Setting[e_RevMode], 0, 20, 1, "");             
    GUI_Init_Small_Slider(&sldKnob[e_RevMix], makoProcessor.Setting[e_RevMix], .0f, 1.0f, .01f, "");
    GUI_Init_Small_Slider(&sldKnob[e_RevTime], makoProcessor.Setting[e_RevTime], .0f, 1.0f, .01f, "");
    GUI_Init_Small_Slider(&sldKnob[e_RevBal], makoProcessor.Setting[e_RevBal], .0f, 1.0f, .01f, "");
    GUI_Init_Small_Slider(&sldKnob[e_RevChor], makoProcessor.Setting[e_RevChor], .0f, 1.0f, .01f, "");
    GUI_Init_Small_Slider(&sldKnob[e_RevSize], makoProcessor.Setting[e_RevSize], .0f, 1.0f, .01f, "");
    GUI_Init_Small_Slider(&sldKnob[e_RevLP], makoProcessor.Setting[e_RevLP], 500.0f, 5500.0f, 100.0f, " Hz");
    GUI_Init_Small_Slider(&sldKnob[e_RevHP], makoProcessor.Setting[e_RevHP], 50.0f, 550.0f, 10.0f, " Hz");
    GUI_Init_Small_Slider(&sldKnob[e_RevPreD], makoProcessor.Setting[e_RevPreD], .0f, 200.0f, 1.0f, " mS");
    GUI_Init_Small_Slider(&sldKnob[e_RevDuck], makoProcessor.Setting[e_RevDuck], .0f, 1.0f, .01f, "");
    GUI_Init_Small_Slider(&sldKnob[e_RevPar], makoProcessor.Setting[e_RevPar], .0f, 1.0f, 1.0f, "");
    
    GUI_Init_Switch_Slider(&sldKnob[e_GateOn], makoProcessor.Setting[e_GateOn], 0, 1, 1, "");
    GUI_Init_Large_Slider(&sldKnob[e_GateVol], .2f, .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_GateExp], .0f, .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_GateAvg], .0f, .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_GateFreq], .0f, .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_GateWah], .0f, .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_GateWahQ], 2.0f, .7f, 5.0f, .1f, "");
    GUI_Init_Large_Slider(&sldKnob[e_GateWahRange], .5f, .0f, 1.0f, .01f, "");
    
    GUI_Init_Switch_Slider(&sldKnob[e_CompOn], makoProcessor.Setting[e_CompOn], 0, 1, 1, "");
    GUI_Init_Large_Slider(&sldKnob[e_CompVol], .2f, .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_CompAtt], .2f, .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_CompRel], .2f, .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_CompThr], .5f, .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_CompRat], .5f, .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_CompDrv], .0f, .0f, 1.0f, .01f, "");

    GUI_Init_Switch_Slider(&sldKnob[e_Dist1On], makoProcessor.Setting[e_Dist1On], 0, 1, 1, "");
    GUI_Init_Large_Slider(&sldKnob[e_Dist1Vol], .2f, .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Dist1MidF], 800.0f, 400.0f, 1400.0f, 10.0f, " Hz");
    GUI_Init_Large_Slider(&sldKnob[e_Dist1Mid], .5f, .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Dist1MidQ], .35f, .1f, 3.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Dist1Cut], 3500.0f, 2000.0f, 7000.0f, 50.0f, " Hz");
    GUI_Init_Large_Slider(&sldKnob[e_Dist1Drv], .2f, .0f, 1.0f, .01f, "");

    GUI_Init_Switch_Slider(&sldKnob[e_Dist2On], makoProcessor.Setting[e_Dist2On], 0, 1, 1, "");
    GUI_Init_Large_Slider(&sldKnob[e_Dist2Vol], .2f, .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Dist2MidF], 800.0f, 400.0f, 1400.0f, 10.0f, " Hz");
    GUI_Init_Large_Slider(&sldKnob[e_Dist2Mid], .5f, .0f, 1.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Dist2MidQ], .35f, .1f, 3.0f, .01f, "");
    GUI_Init_Large_Slider(&sldKnob[e_Dist2Cut], 3500.0f, 2000.0f, 7000.0f, 50.0f, " Hz");
    GUI_Init_Large_Slider(&sldKnob[e_Dist2Drv], .2f, .0f, 1.0f, .01f, "");

    GUI_Init_Switch_Slider(&sldKnob[e_EQOn], makoProcessor.Setting[e_EQOn], 0, 1, 1, "");
    GUI_Init_Large_Slider(&sldKnob[e_EQVol], 1.0f, .0f, 2.0f, .02f, "");
    GUI_Init_Large_Slider(&sldKnob[e_EQF1], .0f, -12.0f, 12.0f, .01f, " dB");
    GUI_Init_Large_Slider(&sldKnob[e_EQF2], .0f, -12.0f, 12.0f, .01f, " dB");
    GUI_Init_Large_Slider(&sldKnob[e_EQF3], .0f, -12.0f, 12.0f, .01f, " dB");
    GUI_Init_Large_Slider(&sldKnob[e_EQF4], .0f, -12.0f, 12.0f, .01f, " dB");
    GUI_Init_Large_Slider(&sldKnob[e_EQF5], .0f, -12.0f, 12.0f, .01f, " dB");
    GUI_Init_Large_Slider(&sldKnob[e_EQF6], .0f, -12.0f, 12.0f, .01f, " dB");
    

    //R4.00 Define the AMPLIFIER top row knob (slider) positions on the screen/UI.
    KNOB_DefinePosition(e_AmpThin, 12, 100, 50, 50, "Thin", true);
    KNOB_DefinePosition(e_AmpQual, 62, 100, 50, 50, "Quality", true);
    KNOB_DefinePosition(e_AmpSlope, 112, 100, 50, 50, "Slope", true);
    KNOB_DefinePosition(e_AmpThump, 187, 100, 50, 50, "Thump", true);
    KNOB_DefinePosition(e_AmpAir, 237, 100, 50, 50, "Air", true);
    KNOB_DefinePosition(e_AmpPower, 287, 100, 50, 50, "Power", true);
    KNOB_DefinePosition(e_AmpComp, 362, 100, 50, 50, "Comp", true); 
    KNOB_DefinePosition(e_AmpCRatio, 412, 100, 50, 50, "Ratio", true);
    KNOB_DefinePosition(e_AmpClean, 462, 100, 50, 50, "Mix", true);
    KNOB_DefinePosition(e_AmpVol, 512, 100, 50, 50, "Volume", true);

    //4.00 Using the Outline color to define the KNOB style.
    sldKnob[e_AmpThin].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000002));
    sldKnob[e_AmpQual].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000002));
    sldKnob[e_AmpSlope].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000002));
    sldKnob[e_AmpThump].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000002));
    sldKnob[e_AmpAir].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000002));
    sldKnob[e_AmpPower].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000002));
    sldKnob[e_AmpComp].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000002));
    sldKnob[e_AmpCRatio].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000002));
    sldKnob[e_AmpClean].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000002));
    sldKnob[e_AmpVol].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000002));

    //R4.00 Define the AMPLIFIER bottom row knob (slider) positions on the screen/UI.
    KNOB_DefinePosition(e_AmpGain, 12, 168, 50, 50, "Gain", true);
    KNOB_DefinePosition(e_AmpBoom, 62, 168, 50, 50, "Boom", true);
    KNOB_DefinePosition(e_AmpCrisp, 112, 168, 50, 50, "Crisp", true);
    KNOB_DefinePosition(e_AmpEQMode, 162, 168, 50, 50, "EQ Mode", true);
    KNOB_DefinePosition(e_AmpEQ1, 212, 168, 50, 50, "", true);
    KNOB_DefinePosition(e_AmpEQ2, 262, 168, 50, 50, "", true);
    KNOB_DefinePosition(e_AmpEQ3, 312, 168, 50, 50, "", true);
    KNOB_DefinePosition(e_AmpEQ4, 362, 168, 50, 50, "", true);
    KNOB_DefinePosition(e_AmpEQ5, 412, 168, 50, 50, "", true);
    KNOB_DefinePosition(e_AmpEQ6, 462, 168, 50, 50, "", true);
    KNOB_DefinePosition(e_AmpLP, 512, 168, 50, 50, "", true);

    //4.00 Using the Outline color to define the KNOB style.
    sldKnob[e_AmpGain].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000002));
    sldKnob[e_AmpBoom].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000004));
    sldKnob[e_AmpCrisp].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000004));
    sldKnob[e_AmpThump].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000005));
    sldKnob[e_AmpAir].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000005));
    sldKnob[e_AmpPower].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000005));
    sldKnob[e_AmpEQMode].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000003));
    sldKnob[e_AmpEQ1].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000003));
    sldKnob[e_AmpEQ2].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000003));
    sldKnob[e_AmpEQ3].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000003));
    sldKnob[e_AmpEQ4].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000003));
    sldKnob[e_AmpEQ5].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000003));
    sldKnob[e_AmpEQ6].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000003));
    sldKnob[e_AmpLP].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000003));

    KNOB_DefinePosition(e_IRSize, 0, 140, 180, 20, "Size", true);
    KNOB_DefinePosition(e_IRVol, 0, 180, 180, 20, "Vol", true);
        
    sldKnob[e_Mod1Rate].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001)); 
    sldKnob[e_Mod1Depth].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_Mod1Mix].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_Mod1Synth].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_Mod1Bronz].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_Mod1Vol].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));

    sldKnob[e_Mod2Rate].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_Mod2Range].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_Mod2FStart].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_Mod2Mix].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_Mod2Vol].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));

    sldKnob[e_Mod3Rate].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_Mod3Depth].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_Mod3Mix].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_Mod3Tone].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_Mod3Bal].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_Mod3Vol].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));

    sldKnob[e_Mod4Foff].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_Mod4Doff].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_Mod4Mix].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_Mod4Tone].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));    
    sldKnob[e_Mod4Vol].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));
    sldKnob[e_Mod4Bal].setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000001));


    KNOB_DefinePosition(e_DelMix,    0, 100, 180, 20, "Mix", true);
    KNOB_DefinePosition(e_DelTime,   0, 130, 180, 20, "Time", true);
    KNOB_DefinePosition(e_DelOffset, 0, 160, 180, 20, "Offset", true);
    KNOB_DefinePosition(e_DelRepeat, 0, 190, 180, 20, "Repeat", true);
    KNOB_DefinePosition(e_DelBal,  190, 100, 180, 20, "Balance", true);
    KNOB_DefinePosition(e_DelDuck, 190, 130, 180, 20, "Ducking", true);
        
    KNOB_DefinePosition(e_RevMix,    0, 100, 180, 20, "Mix", true);
    KNOB_DefinePosition(e_RevTime,   0, 130, 180, 20, "Time/Decay", true);
    KNOB_DefinePosition(e_RevBal,    0, 160, 180, 20, "Balance", true);
    KNOB_DefinePosition(e_RevChor,   0, 190, 180, 20, "Chorus", true);
    KNOB_DefinePosition(e_RevSize, 190, 100, 180, 20, "Room Size", true);
    KNOB_DefinePosition(e_RevLP,   190, 130, 180, 20, "High Cut", true);
    KNOB_DefinePosition(e_RevHP,   190, 160, 180, 20, "Lo Cut", true);
    KNOB_DefinePosition(e_RevPreD, 380, 100, 180, 20, "PreDelay", true);
    KNOB_DefinePosition(e_RevDuck, 380, 130, 180, 20, "Ducking", true);
    KNOB_DefinePosition(e_RevPar,  380, 160, 180, 20, "Parallel", true);

    KNOB_DefinePosition(e_GateOn, 485, 70, 90, 18, "Gate On", true);
    KNOB_DefinePosition(e_CompOn, 485, 70, 90, 18, "Comp On", true);
    KNOB_DefinePosition(e_Dist1On, 485, 70, 90, 18, "Dist On", true);
    KNOB_DefinePosition(e_Mod1On, 485, 70, 90, 18, "Flange", true);
    KNOB_DefinePosition(e_Mod2On, 485, 70, 90, 18, "Mod Filter", true);
    KNOB_DefinePosition(e_Dist2On, 485, 70, 90, 18, "Dist On", true);
    KNOB_DefinePosition(e_AmpOn, 485, 70, 90, 18, "Amp", true);
    KNOB_DefinePosition(e_IROn, 485, 70, 90, 18, "IR", true);
    KNOB_DefinePosition(e_EQOn, 485, 70, 90, 18, "EQ", true);
    KNOB_DefinePosition(e_Mod3On, 485, 70, 90, 18, "Chorus", true);
    KNOB_DefinePosition(e_Mod4On, 485, 70, 90, 18, "Wide", true);
    KNOB_DefinePosition(e_DelOn, 485, 70, 90, 18, "Delay", true);
    KNOB_DefinePosition(e_RevOn, 485, 70, 90, 18, "Reverb", true);

    //R4.00 Initialize the on/off SWITCHES to invisible.
    sldKnob[e_GateOn].setVisible(false);
    sldKnob[e_CompOn].setVisible(false);
    sldKnob[e_Dist1On].setVisible(false);
    sldKnob[e_Mod1On].setVisible(false);
    sldKnob[e_Mod2On].setVisible(false);
    sldKnob[e_Dist2On].setVisible(false);
    sldKnob[e_AmpOn].setVisible(false);
    sldKnob[e_IROn].setVisible(false);
    sldKnob[e_EQOn].setVisible(false);
    sldKnob[e_Mod3On].setVisible(false);
    sldKnob[e_Mod4On].setVisible(false);
    sldKnob[e_DelOn].setVisible(false);
    sldKnob[e_RevOn].setVisible(false);


    //R3.00 AMPLIFIER selection dropdown.
    cbAmpList.setColour(juce::ComboBox::textColourId, juce::Colour(0xFFFFFFFF));
    cbAmpList.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xFF000000));
    cbAmpList.setColour(juce::ComboBox::arrowColourId, juce::Colour(192, 192, 192));
    cbAmpList.setColour(juce::ComboBox::outlineColourId, juce::Colour(0xFFFF8000));
    addAndMakeVisible(cbAmpList);
    cbAmpList.onChange = [this] { cbAmpListChanged(); };
    cbAmpList.setSelectedId(1);
    DefAmp_UpdateList();
    cbAmpList.setVisible(false);

    //R3.00 IR selection dropdown.
    cbIRList.setColour(juce::ComboBox::textColourId, juce::Colour(0xFFFFFFFF));
    cbIRList.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xFF000000));
    cbIRList.setColour(juce::ComboBox::arrowColourId, juce::Colour(192, 192, 192));
    cbIRList.setColour(juce::ComboBox::outlineColourId, juce::Colour(0xFFFF8000));
    addAndMakeVisible(cbIRList);
    cbIRList.onChange = [this] { cbIRListChanged(); };
    cbIRList.setVisible(false);
    DefIR_UpdateList();

    //R3.10 Setup LOAD PRESET button.
    butPresetLoad.setButtonText("Load Preset");
    butPresetLoad.setClickingTogglesState(false);
    butPresetLoad.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPresetLoad.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPresetLoad.addListener(this);
    addAndMakeVisible(butPresetLoad);

    //R3.10 Setup SAVE PRESET button.
    butPresetSave.setButtonText("Save Preset");
    butPresetSave.setClickingTogglesState(false);
    butPresetSave.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPresetSave.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPresetSave.addListener(this);
    addAndMakeVisible(butPresetSave);

    //R2.10 Setup VU button.
    butVUHide.setButtonText("VU off");
    butVUHide.setClickingTogglesState(true);
    butVUHide.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butVUHide.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butVUHide.addListener(this);
    addAndMakeVisible(butVUHide);

    //R4.00 Toggle Help On/Off.
    butHelp.setButtonText("?");
    butHelp.setClickingTogglesState(true);
    butHelp.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butHelp.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butHelp.addListener(this);
    addAndMakeVisible(butHelp);

    //R4.00 Get Default Speaker IR.
    butGetDefIR.setButtonText("Load Spkr");
    butGetDefIR.setClickingTogglesState(false);
    butGetDefIR.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butGetDefIR.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butGetDefIR.addListener(this);
    addAndMakeVisible(butGetDefIR);
    butGetDefIR.setVisible(false);
 
    //R3.00 Setup Amplifier Definition button.
    butDefAmps.setButtonText("Define Amps");
    butDefAmps.setClickingTogglesState(false);
    butDefAmps.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butDefAmps.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butDefAmps.addListener(this);
    addAndMakeVisible(butDefAmps);
    
    //R3.00 Setup IR Definition button.
    butDefIRs.setButtonText("Define IRs");
    butDefIRs.setClickingTogglesState(false);
    butDefIRs.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butDefIRs.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butDefIRs.addListener(this);
    addAndMakeVisible(butDefIRs);
   
    //R4.00 Setup Amplifier Definition button.
    butDBase_Load.setButtonText("Restore Defs");
    butDBase_Load.setClickingTogglesState(false);
    butDBase_Load.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butDBase_Load.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butDBase_Load.addListener(this);
    addAndMakeVisible(butDBase_Load);
    butDBase_Load.setVisible(false);

    butDBase_Save.setButtonText("Backup Defs");
    butDBase_Save.setClickingTogglesState(false);
    butDBase_Save.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butDBase_Save.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butDBase_Save.addListener(this);
    addAndMakeVisible(butDBase_Save);
    butDBase_Save.setVisible(false);

    //R2.10 Setup AMP & IR paging.
    butPageUp.setButtonText("Page Up");
    butPageUp.setClickingTogglesState(false);
    butPageUp.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPageUp.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPageUp.addListener(this);
    addAndMakeVisible(butPageUp);
    butPageUp.setVisible(false);

    butPageDn.setButtonText("Page Down");
    butPageDn.setClickingTogglesState(false);
    butPageDn.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPageDn.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPageDn.addListener(this);
    addAndMakeVisible(butPageDn);
    butPageDn.setVisible(false);

    labHelp.setFont(juce::Font(11.0f, juce::Font::bold));
    labHelp.setText("---", juce::dontSendNotification);
    labHelp.setJustificationType(juce::Justification::left);
    labHelp.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labHelp.setColour(juce::Label::textColourId, juce::Colour(0xFFC0C0C0));
    labHelp.setColour(juce::Label::outlineColourId, juce::Colour(0x00808080));
    addAndMakeVisible(labHelp);
    
    labVersion.setFont(juce::Font(10.0f, juce::Font::bold));
    labVersion.setText("v4.00", juce::dontSendNotification);
    labVersion.setJustificationType(juce::Justification::left);
    labVersion.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labVersion.setColour(juce::Label::textColourId, juce::Colour(0xFFC0C0C0));
    labVersion.setColour(juce::Label::outlineColourId, juce::Colour(0x00808080));
    addAndMakeVisible(labVersion);

    //R4.00 Pedal Compressor Trigger Light.
    labPedal2_Comp.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    addAndMakeVisible(labPedal2_Comp);
    labPedal2_Comp.setVisible(false);

    //R4.00 Amp Compressor Trigger Light.
    labAmp_Comp.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    addAndMakeVisible(labAmp_Comp);
    labAmp_Comp.setVisible(false);

    //R1.00 VU Meters.
    labVU_Out1.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    addAndMakeVisible(labVU_Out1);
    labVU_Out2.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    addAndMakeVisible(labVU_Out2);
    labVU_Out3.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    addAndMakeVisible(labVU_Out3);
    labVU_Out4.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    addAndMakeVisible(labVU_Out4);
    labVU_Out5.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    addAndMakeVisible(labVU_Out5);
    labVU_Out6.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    addAndMakeVisible(labVU_Out6);

    labVU_In1.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    addAndMakeVisible(labVU_In1);
    labVU_In2.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    addAndMakeVisible(labVU_In2);
    labVU_In3.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    addAndMakeVisible(labVU_In3);
    labVU_In4.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    addAndMakeVisible(labVU_In4);
    labVU_In5.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    addAndMakeVisible(labVU_In5);
    labVU_In6.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    addAndMakeVisible(labVU_In6);
   
    //R3.10 REVERB mode. 
    cbRevMode.setColour(juce::ComboBox::textColourId, juce::Colour(0xFFFFFFFF));
    cbRevMode.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xFF000000));
    cbRevMode.setColour(juce::ComboBox::arrowColourId, juce::Colour(192, 192, 192));
    cbRevMode.setColour(juce::ComboBox::outlineColourId, juce::Colour(0xFFFF8000));
    addAndMakeVisible(cbRevMode);
    cbRevMode.addItem("O - Basic", 1);
    cbRevMode.addItem("1 - Slap Smear", 2);
    cbRevMode.addItem("2 - Haunted", 3);
    cbRevMode.addItem("3 - Dreamy", 4);
    cbRevMode.addItem("4 - Club", 5);
    cbRevMode.addItem("5 - Darkness", 6);
    cbRevMode.addItem("6 - Medium Room", 7);
    cbRevMode.addItem("7 - Small Room", 8);
    cbRevMode.addItem("8 - Box", 9);
    cbRevMode.addItem("9 - DigiVerb", 10);
    cbRevMode.addItem("10 - Space", 11);
    cbRevMode.addItem("11 - Garage", 12);
    cbRevMode.addItem("12 - Fused Echo", 13);
    cbRevMode.addItem("13 - Medium Tile", 14);
    cbRevMode.addItem("14 - Arena", 15);
    cbRevMode.addItem("15 - Garage 2", 16);
    cbRevMode.addItem("16 - Inverted", 17);
    cbRevMode.addItem("17 - Hangar", 18);
    cbRevMode.addItem("18 - Inverted 2", 19);
    cbRevMode.addItem("19 - Regent", 20);
    cbRevMode.onChange = [this] { cbRevModeChanged(); };
    cbRevMode.setVisible(false);

    //R4.00 Attach our PARAMETERS to the actual controls used.
    //R4.00 Parameter ID is the connection. Must match the processor def.
    ParAtt[e_AmpOn] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_on", sldKnob[e_AmpOn]);
    ParAmpChan = std::make_unique <juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.parameters, "amp_chan", cbAmpList);
    ParAtt[e_AmpThin] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_thin", sldKnob[e_AmpThin]);
    ParAtt[e_AmpQual] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_qual", sldKnob[e_AmpQual]);
    ParAtt[e_AmpSlope] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_slope", sldKnob[e_AmpSlope]);
    ParAtt[e_AmpThump] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_thump", sldKnob[e_AmpThump]);
    ParAtt[e_AmpAir] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_air", sldKnob[e_AmpAir]);
    ParAtt[e_AmpPower] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_power", sldKnob[e_AmpPower]);
    ParAtt[e_AmpComp] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_comp", sldKnob[e_AmpComp]);
    ParAtt[e_AmpCRatio] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_cratio", sldKnob[e_AmpCRatio]);
    ParAtt[e_AmpClean] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_clean", sldKnob[e_AmpClean]);
    ParAtt[e_AmpVol] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_vol", sldKnob[e_AmpVol]);

    ParAtt[e_AmpGain] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_gain", sldKnob[e_AmpGain]);
    ParAtt[e_AmpBoom] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_boom", sldKnob[e_AmpBoom]);
    ParAtt[e_AmpCrisp] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_crisp", sldKnob[e_AmpCrisp]);
    ParAtt[e_AmpEQMode] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_eqmode", sldKnob[e_AmpEQMode]);
    ParAtt[e_AmpEQ1] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_eq1", sldKnob[e_AmpEQ1]);
    ParAtt[e_AmpEQ2] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_eq2", sldKnob[e_AmpEQ2]);
    ParAtt[e_AmpEQ3] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_eq3", sldKnob[e_AmpEQ3]);
    ParAtt[e_AmpEQ4] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_eq4", sldKnob[e_AmpEQ4]);
    ParAtt[e_AmpEQ5] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_eq5", sldKnob[e_AmpEQ5]);
    ParAtt[e_AmpEQ6] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_eq6", sldKnob[e_AmpEQ6]);
    ParAtt[e_AmpLP] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "amp_lp", sldKnob[e_AmpLP]);

    ParAtt[e_IROn] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "ir_on", sldKnob[e_IROn]);
    ParIRL = std::make_unique <juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.parameters, "ir", cbIRList);
    ParAtt[e_IRSize] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "ir_size", sldKnob[e_IRSize]);
    ParAtt[e_IRVol] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "ir_vol", sldKnob[e_IRVol]);
    
    ParAtt[e_Mod1On] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod1_on", sldKnob[e_Mod1On]);
    ParAtt[e_Mod1Vol] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod1_vol", sldKnob[e_Mod1Vol]);
    ParAtt[e_Mod1Rate] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod1_rate", sldKnob[e_Mod1Rate]);
    ParAtt[e_Mod1Depth] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod1_depth", sldKnob[e_Mod1Depth]);
    ParAtt[e_Mod1Mix] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod1_mix", sldKnob[e_Mod1Mix]);
    ParAtt[e_Mod1Synth] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod1_synth", sldKnob[e_Mod1Synth]);
    ParAtt[e_Mod1Bronz] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod1_bronz", sldKnob[e_Mod1Bronz]);
                    
    ParAtt[e_Mod2On] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod2_on", sldKnob[e_Mod2On]);
    ParAtt[e_Mod2Vol] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod2_vol", sldKnob[e_Mod2Vol]);
    ParAtt[e_Mod2Rate] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod2_rate", sldKnob[e_Mod2Rate]);
    ParAtt[e_Mod2Range] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod2_range", sldKnob[e_Mod2Range]);
    ParAtt[e_Mod2FStart] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod2_fstart", sldKnob[e_Mod2FStart]);
    ParAtt[e_Mod2Mix] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod2_mix", sldKnob[e_Mod2Mix]);
        
    ParAtt[e_Mod3On] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod3_on", sldKnob[e_Mod3On]);
    ParAtt[e_Mod3Vol] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod3_vol", sldKnob[e_Mod3Vol]);
    ParAtt[e_Mod3Rate] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod3_rate", sldKnob[e_Mod3Rate]);
    ParAtt[e_Mod3Depth] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod3_depth", sldKnob[e_Mod3Depth]);
    ParAtt[e_Mod3Mix] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod3_mix", sldKnob[e_Mod3Mix]);
    ParAtt[e_Mod3Tone] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod3_tone", sldKnob[e_Mod3Tone]);
    ParAtt[e_Mod3Bal] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod3_bal", sldKnob[e_Mod3Bal]);

    ParAtt[e_Mod4On] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod4_on", sldKnob[e_Mod4On]);
    ParAtt[e_Mod4Vol] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod4_vol", sldKnob[e_Mod4Vol]);
    ParAtt[e_Mod4Foff] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod4_foff", sldKnob[e_Mod4Foff]);
    ParAtt[e_Mod4Doff] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod4_doff", sldKnob[e_Mod4Doff]);
    ParAtt[e_Mod4Mix] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod4_mix", sldKnob[e_Mod4Mix]);
    ParAtt[e_Mod4Tone] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod4_tone", sldKnob[e_Mod4Tone]);
    ParAtt[e_Mod4Bal] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mod4_bal", sldKnob[e_Mod4Bal]);

    ParAtt[e_DelOn] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "delay_on", sldKnob[e_DelOn]);
    ParAtt[e_DelTime] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "delay_time", sldKnob[e_DelTime]);
    ParAtt[e_DelMix] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "delay_mix", sldKnob[e_DelMix]);
    ParAtt[e_DelOffset] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "delay_offset", sldKnob[e_DelOffset]);
    ParAtt[e_DelRepeat] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "delay_repeat", sldKnob[e_DelRepeat]);
    ParAtt[e_DelBal] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "delay_bal", sldKnob[e_DelBal]);
    ParAtt[e_DelDuck] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "delay_duck", sldKnob[e_DelDuck]);

    ParAtt[e_RevOn] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "reverb_on", sldKnob[e_RevOn]);
    ParAtt[e_RevMix] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "reverb_mix", sldKnob[e_RevMix]);
    ParAtt[e_RevTime] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "reverb_time", sldKnob[e_RevTime]);
    ParAtt[e_RevBal] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "reverb_bal", sldKnob[e_RevBal]);
    ParAtt[e_RevChor] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "reverb_chor", sldKnob[e_RevChor]);
    ParAtt[e_RevSize] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "reverb_size", sldKnob[e_RevSize]);
    ParAtt[e_RevLP] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "reverb_lp", sldKnob[e_RevLP]);
    ParAtt[e_RevHP] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "reverb_hp", sldKnob[e_RevHP]);
    ParAtt[e_RevPreD] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "reverb_pred", sldKnob[e_RevPreD]);
    ParAtt[e_RevDuck] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "reverb_duck", sldKnob[e_RevDuck]);
    ParAtt[e_RevPar] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "reverb_parallel", sldKnob[e_RevPar]);
    ParRevMode = std::make_unique <juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.parameters, "reverb_mode", cbRevMode);

    ParAtt[e_GateOn] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "gate_on", sldKnob[e_GateOn]);
    ParAtt[e_GateVol] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "gate_vol", sldKnob[e_GateVol]);
    ParAtt[e_GateExp] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "gate_exp", sldKnob[e_GateExp]);
    ParAtt[e_GateAvg] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "gate_avg", sldKnob[e_GateAvg]);
    ParAtt[e_GateFreq] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "gate_freq", sldKnob[e_GateFreq]);
    ParAtt[e_GateWah] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "gate_wah", sldKnob[e_GateWah]);
    ParAtt[e_GateWahQ] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "gate_wahq", sldKnob[e_GateWahQ]);
    ParAtt[e_GateWahRange] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "gate_wahrange", sldKnob[e_GateWahRange]);

    ParAtt[e_CompOn] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "comp_on", sldKnob[e_CompOn]);
    ParAtt[e_CompVol] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "comp_vol", sldKnob[e_CompVol]);
    ParAtt[e_CompAtt] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "comp_att", sldKnob[e_CompAtt]);
    ParAtt[e_CompRel] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "comp_rel", sldKnob[e_CompRel]);
    ParAtt[e_CompThr] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "comp_thr", sldKnob[e_CompThr]);
    ParAtt[e_CompRat] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "comp_rat", sldKnob[e_CompRat]);
    ParAtt[e_CompDrv] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "comp_drv", sldKnob[e_CompDrv]);

    ParAtt[e_Dist1On] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "dist1_on", sldKnob[e_Dist1On]);
    ParAtt[e_Dist1Vol] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "dist1_vol", sldKnob[e_Dist1Vol]);
    ParAtt[e_Dist1MidF] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "dist1_midf", sldKnob[e_Dist1MidF]);
    ParAtt[e_Dist1Mid] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "dist1_mid", sldKnob[e_Dist1Mid]);
    ParAtt[e_Dist1MidQ] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "dist1_midq", sldKnob[e_Dist1MidQ]);
    ParAtt[e_Dist1Cut] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "dist1_cut", sldKnob[e_Dist1Cut]);
    ParAtt[e_Dist1Drv] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "dist1_drv", sldKnob[e_Dist1Drv]);

    ParAtt[e_Dist2On] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "dist2_on", sldKnob[e_Dist2On]);
    ParAtt[e_Dist2Vol] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "dist2_vol", sldKnob[e_Dist2Vol]);
    ParAtt[e_Dist2MidF] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "dist2_midf", sldKnob[e_Dist2MidF]);
    ParAtt[e_Dist2Mid] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "dist2_mid", sldKnob[e_Dist2Mid]);
    ParAtt[e_Dist2MidQ] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "dist2_midq", sldKnob[e_Dist2MidQ]);
    ParAtt[e_Dist2Cut] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "dist2_cut", sldKnob[e_Dist2Cut]);
    ParAtt[e_Dist2Drv] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "dist2_drv", sldKnob[e_Dist2Drv]);

    ParAtt[e_EQOn] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "eq_on", sldKnob[e_EQOn]);
    ParAtt[e_EQVol] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "eq_vol", sldKnob[e_EQVol]);
    ParAtt[e_EQF1] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "eq_f1", sldKnob[e_EQF1]);
    ParAtt[e_EQF2] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "eq_f2", sldKnob[e_EQF2]);
    ParAtt[e_EQF3] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "eq_f3", sldKnob[e_EQF3]);
    ParAtt[e_EQF4] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "eq_f4", sldKnob[e_EQF4]);
    ParAtt[e_EQF5] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "eq_f5", sldKnob[e_EQF5]);
    ParAtt[e_EQF6] = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "eq_f6", sldKnob[e_EQF6]);

        
    //R1.00 Update the Look and Feel (Global colors) so drop down menu is the correct color. 
    getLookAndFeel().setColour(juce::DocumentWindow::backgroundColourId, juce::Colour(32, 32, 32));
    getLookAndFeel().setColour(juce::DocumentWindow::textColourId, juce::Colour(255, 255, 255));    
    getLookAndFeel().setColour(juce::DialogWindow::backgroundColourId, juce::Colour(32, 32, 32));
    getLookAndFeel().setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0, 0, 0));
    getLookAndFeel().setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xFF003060));
    getLookAndFeel().setColour(juce::TextButton::buttonOnColourId, juce::Colour(200, 96, 0));
    getLookAndFeel().setColour(juce::TextButton::buttonColourId, juce::Colour(0, 0, 0));
    getLookAndFeel().setColour(juce::ComboBox::backgroundColourId, juce::Colour(0, 0, 0));
    getLookAndFeel().setColour(juce::ListBox::backgroundColourId, juce::Colour(32, 32, 32));
    getLookAndFeel().setColour(juce::Label::backgroundColourId, juce::Colour(32, 32, 32));
   
    //R2.18 Calulate the GUI control Vals from makoProcessor variables. 
    Mako_UpdateVals(false);  
    
    //R4.00 Initialize the HELP string array indexes. Future use.
    //for (int t = 0; t < 120; t++) sldKnob_Help[t] = t;

    //R4.00 Initialize the Amplifier EQ Frequency labels.
    for (int t = 0; t < 6; t++) str_EQ[0][t] = str_EQ_mode0[t];
    for (int t = 0; t < 6; t++) str_EQ[1][t] = str_EQ_mode1[t];
    for (int t = 0; t < 6; t++) str_EQ[2][t] = str_EQ_mode2[t];
    for (int t = 0; t < 6; t++) str_EQ[3][t] = str_EQ_mode3[t];
    for (int t = 0; t < 6; t++) str_EQ[4][t] = str_EQ_mode4[t];
    for (int t = 0; t < 6; t++) str_EQ[5][t] = str_EQ_mode5[t];
    for (int t = 0; t < 6; t++) str_EQ[6][t] = str_EQ_mode6[t];
    for (int t = 0; t < 6; t++) str_EQ[7][t] = str_EQ_mode7[t];
    for (int t = 0; t < 6; t++) str_EQ[8][t] = str_EQ_mode8[t];
    for (int t = 0; t < 6; t++) str_EQ[9][t] = str_EQ_mode9[t];
    for (int t = 0; t < 6; t++) str_EQ[10][t] = str_EQ_mode10[t];
    for (int t = 0; t < 6; t++) str_EQ[11][t] = str_EQ_mode11[t];
    for (int t = 0; t < 6; t++) str_EQ[12][t] = str_EQ_mode12[t];
    for (int t = 0; t < 6; t++) str_EQ[13][t] = str_EQ_mode13[t];
    for (int t = 0; t < 6; t++) str_EQ[14][t] = str_EQ_mode14[t];
    for (int t = 0; t < 6; t++) str_EQ[15][t] = str_EQ_mode15[t];
    for (int t = 0; t < 6; t++) str_EQ[16][t] = str_EQ_mode16[t];
    for (int t = 0; t < 6; t++) str_EQ[17][t] = str_EQ_mode17[t];
    for (int t = 0; t < 6; t++) str_EQ[18][t] = str_EQ_mode18[t];
    for (int t = 0; t < 6; t++) str_EQ[19][t] = str_EQ_mode19[t];
        
}


MakoDist01AudioProcessorEditor::~MakoDist01AudioProcessorEditor()
{
    stopTimer();
}

//R4.00 Figure out which SLIDER was adjusted and handle the change.
//R4.00 Probably a better way to do this than search thru them all.
//R4.00 Could create custom Slider properties and trigger off those?
void MakoDist01AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{    
    //R3.10 Check which slider has been adjusted.
    for (int t = 0; t < e_PLast; t++)
    {
        if (slider == &sldKnob[t])
        {
            //R3.10 Update HELP bar.            
            labHelp.setText(LangStringHelp[t], juce::dontSendNotification);
                        
            //R3.10 Dont adjust settings for knobs we are not using.
            if (t == e_RevMode) return;

            //R3.10 Update the actual processor variable being edited.
            makoProcessor.Setting[t] = float(sldKnob[t].getValue());

            //R3.10 Flag that this needs updated.
            makoProcessor.SettingRqd[t] = true;

            //R3.10 Update our Amp EQs.
            if (t == e_AmpEQMode) 
            {
                Mako_EQ_Update(false);
                repaint();
            }            

            //R3.10 We need to update settings in processor.
            //R3.10 Increment changed var to be sure every change gets made, changed var is decremented in processor.
            makoProcessor.makoSettingsChanged += 1;

            //R3.10 Refresh screen if selecting ENABLED.
            if (t == e_GateOn) repaint();
            if (t == e_CompOn) repaint();
            if (t == e_Dist1On) repaint();
            if (t == e_Dist2On) repaint();
            if (t == e_AmpOn) repaint();
            if (t == e_IROn) repaint();
            if (t == e_EQOn) repaint();
            if (t == e_Mod1On) repaint();
            if (t == e_Mod2On) repaint();
            if (t == e_Mod3On) repaint();
            if (t == e_Mod4On) repaint();
            if (t == e_DelOn) repaint();
            if (t == e_RevOn) repaint();
            
            //R3.10 We have captured the correct slider change, exit this function.
            return;
        }
    }

}

//==============================================================================
void MakoDist01AudioProcessorEditor::paint (juce::Graphics& g)
{    
      
    //R4.00 If we changed edit mode, hide ALL controls. 
    if (editMode != editModeLast)
    {
        for (int t = 0; t < e_PLast; t++) sldKnob[t].setVisible(false);        
        cbRevMode.setVisible(false);
    }

    //R1.00 Paint the MAKO background only when needed.
    g.drawImageAt(imgBack, 0, 0);                
       
    //R4.00 Draw Pedal, Mod, Delay, and Reverb SIGNAL PATH status. 
    Pedals_Signal_Draw(g);    
    
       
    //R4.00 Check AMP/IR database editing.
    if ((editMode != EM_DefIRs) && (editModeLast == EM_DefIRs) && (editMode != EM_DefAmps))
    { 
        butPageUp.setVisible(false); 
        butPageDn.setVisible(false);
        butDBase_Load.setVisible(false);
        butDBase_Save.setVisible(false);
        
        //R4.00 If we have unsaved edits, force a save in processor.
        if (DefAmp_Editing == 2)
        {
            DefAmp_Editing = 0;                //R3.00 reset our Database editing var.
            makoProcessor.DefAmp_Save = true;  //R3.00 Tell Proc to save the database.
        }
    }
    if ((editMode != EM_DefAmps) && (editModeLast == EM_DefAmps) && (editMode != EM_DefIRs))
    {
        butPageUp.setVisible(false);
        butPageDn.setVisible(false);
        butDBase_Load.setVisible(false);
        butDBase_Save.setVisible(false);

        //R4.00 If we have unsaved edits, force a save in processor.
        if (DefAmp_Editing == 2)
        {
            DefAmp_Editing = 0;                //R3.00 reset our Database editing var.
            makoProcessor.DefAmp_Save = true;  //R3.00 Tell Proc to save the database.
        }
    }
    
    //R4.00 Turn OFF any AMPLIFIER controls when done editing amps.
    if ((editMode != EM_Amp) && (editModeLast == EM_Amp))
    {
        cbAmpList.setVisible(false);
        butGetDefIR.setVisible(false); 
        labAmp_Comp.setVisible(false);
    }
    
    //R4.00 IMPULSE RESPONSES
    if ((editMode != EM_IR) && (editModeLast == EM_IR))
    {
        cbIRList.setVisible(false);        
    }
    
    if ((editMode != EM_Ped2) && (editModeLast == EM_Ped2))
    {
        labPedal2_Comp.setVisible(false);
    }

    //R4.00 Paint the current objects being edited.
    switch (editMode)
    {
        case EM_DefAmps: DefAmp_Draw(g); break;
        case EM_DefIRs:  DefIR_Draw(g); break;
        case EM_Ped1:    Paint_Ped1(g); break;
        case EM_Ped2:    Paint_Ped2(g); break;
        case EM_Ped3:    Paint_Ped3(g); break;
        case EM_Ped4:    Paint_Ped4(g); break;
        case EM_Ped5:    Paint_Ped5(g); break;
        case EM_Mod1:    Paint_Mod1(g); break;
        case EM_Mod2:    Paint_Mod2(g); break;
        case EM_Mod3:    Paint_Mod3(g); break;
        case EM_Mod4:    Paint_Mod4(g); break;
        case EM_Amp:     Paint_Amp(g); break;
        case EM_IR:      Paint_IR(g); break;
        case EM_Delay:   Paint_Delay(g); break;
        case EM_Reverb:  Paint_Reverb(g); break;        
    }
            
    //R4.00 Update last Edit Mode.
    editModeLast = editMode;        
}

//R4.00 Paint the NOISE GATE ui in the settings panel.
void MakoDist01AudioProcessorEditor::Paint_Ped1(juce::Graphics& g)
{   
    //R4.00 Make our Slider controls visible.
    sldKnob[e_GateOn].setVisible(true);
    sldKnob[e_GateVol].setVisible(true);
    sldKnob[e_GateExp].setVisible(true);
    sldKnob[e_GateAvg].setVisible(true);
    sldKnob[e_GateFreq].setVisible(true);
    sldKnob[e_GateWah].setVisible(true);
    sldKnob[e_GateWahQ].setVisible(true);
    sldKnob[e_GateWahRange].setVisible(true);
 
    //R4.00 PURPLE - Noise Gate.
    Paint_Back(g, juce::Colour(0xFF8972B2));
    Paint_VertDiv(g, 105, 100, 180);
    Paint_VertDiv(g, 325, 100, 180);

    //R4.00 HEADERS
    g.setColour(juce::Colour(0xFFFF8000));
    g.setFont(12.0f);
    g.drawText("NOISE GATE (mono)", 10, 70, 580, 15, juce::Justification::centredLeft, 0);

    g.setColour(juce::Colour(0xFFFFFFFF));
    g.drawText("Volume",     30, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Exp Gate",  110, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Avg Gate",  180, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Freq Gate", 250, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Auto Wah",  330, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Wah Q",     400, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Wah Range", 470, 95, 70, 15, juce::Justification::centred, 0);

    g.setColour(juce::Colour(0xFFC0C0C0));    
    g.drawText("Multiple Gate with Envelope Filter Wah.", 10, 195, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Use Avg and Freq first. Fine tune with Exp gate.", 10, 210, 580, 15, juce::Justification::centredLeft, 0);
}

//R4.00 Paint the COMPRESSOR ui in the settings panel.
void MakoDist01AudioProcessorEditor::Paint_Ped2(juce::Graphics& g)
{
    //R4.00 Make our Slider controls visible.
    sldKnob[e_CompOn].setVisible(true);
    sldKnob[e_CompVol].setVisible(true);
    sldKnob[e_CompAtt].setVisible(true);
    sldKnob[e_CompRel].setVisible(true);
    sldKnob[e_CompThr].setVisible(true);
    sldKnob[e_CompRat].setVisible(true);
    sldKnob[e_CompDrv].setVisible(true);
    labPedal2_Comp.setVisible(true);

    //R4.00 PINK - Compressor.
    Paint_Back(g, juce::Colour(0xFFC13F70));
    Paint_VertDiv(g, 120, 100, 180);
    Paint_VertDiv(g, 450, 100, 180);

    //R4.00 HEADERS
    g.setColour(juce::Colour(0xFFFF8000));
    g.setFont(12.0f);
    g.drawText("COMPRESSOR (mono)", 10, 70, 580, 15, juce::Justification::centredLeft, 0);

    g.setColour(juce::Colour(0xFFFFFFFF));
    g.drawText("Volume", 50, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Attack", 130, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Release", 210, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Threshold", 290, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Ratio", 370, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Drive", 450, 95, 70, 15, juce::Justification::centred, 0);
    
    g.setColour(juce::Colour(0xFFC0C0C0));
    g.drawText("Standard Compressor with Drive.", 10, 195, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Add drive to smooth things over and get more gain.", 10, 210, 580, 15, juce::Justification::centredLeft, 0);
}

//R4.00 Paint the DISTORTION 1 ui in the settings panel.
void MakoDist01AudioProcessorEditor::Paint_Ped3(juce::Graphics& g)
{
    //R4.00 Make our slider controls visible.
    sldKnob[e_Dist1On].setVisible(true);
    sldKnob[e_Dist1Vol].setVisible(true);
    sldKnob[e_Dist1MidF].setVisible(true);
    sldKnob[e_Dist1Mid].setVisible(true);
    sldKnob[e_Dist1MidQ].setVisible(true);
    sldKnob[e_Dist1Cut].setVisible(true);
    sldKnob[e_Dist1Drv].setVisible(true);

    //R4.00 ORANGE - Distortion.
    Paint_Back(g, juce::Colour(0xFFF3690D));

    //R4.00 HEADERS
    g.setColour(juce::Colour(0xFFFF8000));
    g.setFont(12.0f);
    g.drawText("DISTORTION ONE (mono)", 10, 70, 580, 15, juce::Justification::centredLeft, 0);

    g.setColour(juce::Colour(0xFFFFFFFF));
    g.drawText("Volume", 50, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Mid Freq", 130, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Mid", 210, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Mid Q", 290, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Hi Cut", 370, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Drive", 450, 95, 70, 15, juce::Justification::centred, 0);

    g.setColour(juce::Colour(0xFFC0C0C0));
    g.drawText("Mid boost distortion effect.", 10, 195, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Adjust Q and drive to tune the sound.", 10, 210, 580, 15, juce::Justification::centredLeft, 0);
}

//R4.00 Paint the DISTORTION 2 ui in the settings panel.
void MakoDist01AudioProcessorEditor::Paint_Ped4(juce::Graphics& g)
{
    //R4.00 Make our Slider controls visible.
    sldKnob[e_Dist2On].setVisible(true);
    sldKnob[e_Dist2Vol].setVisible(true);
    sldKnob[e_Dist2MidF].setVisible(true);
    sldKnob[e_Dist2Mid].setVisible(true);
    sldKnob[e_Dist2MidQ].setVisible(true);
    sldKnob[e_Dist2Cut].setVisible(true);
    sldKnob[e_Dist2Drv].setVisible(true);

    //R4.00 ORANGE - Distortion.
    Paint_Back(g, juce::Colour(0xFFF3690D));

    //R4.00 HEADERS
    g.setColour(juce::Colour(0xFFFF8000));
    g.setFont(12.0f);
    g.drawText("DISTORTION TWO (mono)", 10, 70, 580, 15, juce::Justification::centredLeft, 0);

    g.setColour(juce::Colour(0xFFFFFFFF));
    g.drawText("Volume", 50, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Mid Freq", 130, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Mid", 210, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Mid Q", 290, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Hi Cut", 370, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Drive", 450, 95, 70, 15, juce::Justification::centred, 0);

    g.setColour(juce::Colour(0xFFC0C0C0));
    g.drawText("Mid boost distortion effect.", 10, 195, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Adjust Q and drive to tune the sound.", 10, 210, 580, 15, juce::Justification::centredLeft, 0);
}

//R4.00 Paint the 6 BAND EQUALIZER ui in the settings panel.
void MakoDist01AudioProcessorEditor::Paint_Ped5(juce::Graphics& g)
{
    //R4.00 Make our Slider controls visible.
    sldKnob[e_EQOn].setVisible(true);
    sldKnob[e_EQVol].setVisible(true);
    sldKnob[e_EQF1].setVisible(true);
    sldKnob[e_EQF2].setVisible(true);
    sldKnob[e_EQF3].setVisible(true);
    sldKnob[e_EQF4].setVisible(true);
    sldKnob[e_EQF5].setVisible(true);
    sldKnob[e_EQF6].setVisible(true);
    
    //R4.00 YELLOW - EQ.
    Paint_Back(g, juce::Colour(0xFFE1AD1F));

    //R4.00 HEADERS
    g.setColour(juce::Colour(0xFFFF8000));
    g.setFont(12.0f);
    g.drawText("EQUALIZER (mono)", 10, 70, 580, 15, juce::Justification::centredLeft, 0);

    g.setColour(juce::Colour(0xFFFFFFFF));
    g.drawText("Volume", 40, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("100 Hz", 110, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("350 Hz", 180, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("700 Hz", 250, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("1400 Hz", 320, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("2100 Hz", 390, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("5000 Hz", 460, 95, 70, 15, juce::Justification::centred, 0);

    g.setColour(juce::Colour(0xFFC0C0C0));
    g.drawText("6 Band Equalizer.", 10, 195, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Use to fine tune the sound after the amp/speaker.", 10, 210, 580, 15, juce::Justification::centredLeft, 0);
}

//R4.00 Paint the FLANGER ui in the settings panel.
void MakoDist01AudioProcessorEditor::Paint_Mod1(juce::Graphics& g)
{    
    //R4.00 Make our Slider controls visible.
    sldKnob[e_Mod1Vol].setVisible(true);
    sldKnob[e_Mod1Rate].setVisible(true);
    sldKnob[e_Mod1Depth].setVisible(true);
    sldKnob[e_Mod1Mix].setVisible(true);
    sldKnob[e_Mod1Synth].setVisible(true);
    sldKnob[e_Mod1Bronz].setVisible(true);
        
    //R4.00 Draw color Background.
    Paint_Back(g, juce::Colour(0xFF8080FF));
    Paint_VertDiv(g, 370, 100, 180);

    //R2.00 HEADERS
    g.setColour(juce::Colour(0xFFFF8000));
    g.setFont(12.0f);
    g.drawText("FLANGER | SYNTH | BRONZ (mono)", 10, 70, 580, 15, juce::Justification::centredLeft, 0);
    
    g.setColour(juce::Colour(0xFFFFFFFF));
    g.drawText("Volume", 45, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Rate", 125, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Depth", 205, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Mix", 285, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Synth", 380, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Bronz", 460, 95, 70, 15, juce::Justification::centred, 0);

    g.setColour(juce::Colour(0xFFC0C0C0));
    g.drawText("Short delay Chorus effect that creates a doppler/airplane swoosh.", 10, 210, 580, 15, juce::Justification::centredLeft, 0);
    
    sldKnob[e_Mod1On].setVisible(true);
   
}

//R4.00 Paint the MODULATED FILTER ui in the settings panel.
void MakoDist01AudioProcessorEditor::Paint_Mod2(juce::Graphics& g)
{
    //R4.00 Make our Slider controls visible.
    sldKnob[e_Mod2On].setVisible(true);
    sldKnob[e_Mod2Vol].setVisible(true);
    sldKnob[e_Mod2Rate].setVisible(true);
    sldKnob[e_Mod2Range].setVisible(true);
    sldKnob[e_Mod2FStart].setVisible(true);
    sldKnob[e_Mod2Mix].setVisible(true);
    
    //R4.00 Draw color Background.
    Paint_Back(g, juce::Colour(0xFF40FFFF));

    //R2.00 HEADERS
    g.setColour(juce::Colour(0xFFFF8000));
    g.setFont(12.0f);
    g.drawText("MODULATED FILTER (mono)", 10, 70, 580, 15, juce::Justification::centredLeft, 0);
    
    g.setColour(juce::Colour(0xFFFFFFFF));
    g.drawText("Volume", 90, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Rate", 170, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Range", 250, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("F Start", 330, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Mix", 410, 95, 70, 15, juce::Justification::centred, 0);
    
    g.setColour(juce::Colour(0xFFC0C0C0));
    g.drawText("Modulated bandpass filter.", 10, 210, 580, 15, juce::Justification::centredLeft, 0);
   
}

//R4.00 Paint the CHORUS ui in the settings panel.
void MakoDist01AudioProcessorEditor::Paint_Mod3(juce::Graphics& g)
{
    //R4.00 Make our Slider controls visible.
    sldKnob[e_Mod3Vol].setVisible(true);
    sldKnob[e_Mod3Rate].setVisible(true);
    sldKnob[e_Mod3Depth].setVisible(true);
    sldKnob[e_Mod3Mix].setVisible(true);
    sldKnob[e_Mod3Tone].setVisible(true);
    sldKnob[e_Mod3Bal].setVisible(true);

    //R4.00 Draw color Background.
    Paint_Back(g, juce::Colour(0xFF8080FF));

    //R2.00 HEADERS
    g.setColour(juce::Colour(0xFFFF8000));
    g.setFont(12.0f);
    g.drawText("CHORUS (stereo)", 10, 70, 580, 15, juce::Justification::centredLeft, 0);

    g.setColour(juce::Colour(0xFFFFFFFF));
    g.drawText("Volume", 50, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Rate", 130, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Depth", 210, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Mix", 290, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Tone", 370, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Bal", 450, 95, 70, 15, juce::Justification::centred, 0);

    g.setColour(juce::Colour(0xFFC0C0C0));
    g.drawText("Modulated delay.", 10, 210, 580, 15, juce::Justification::centredLeft, 0);

    sldKnob[e_Mod3On].setVisible(true);
   
}

//R4.00 Paint the WIDE LOAD ui in the settings panel.
void MakoDist01AudioProcessorEditor::Paint_Mod4(juce::Graphics& g)
{
    //R4.00 Make our Slider controls visible.
    sldKnob[e_Mod4Vol].setVisible(true);
    sldKnob[e_Mod4Foff].setVisible(true);
    sldKnob[e_Mod4Doff].setVisible(true);
    sldKnob[e_Mod4Mix].setVisible(true);
    sldKnob[e_Mod4Tone].setVisible(true);
    sldKnob[e_Mod4Bal].setVisible(true);

    //R4.00 Draw color Background.
    Paint_Back(g, juce::Colour(0xFF40FFFF));

    //R2.00 HEADERS
    g.setColour(juce::Colour(0xFFFF8000));
    g.setFont(12.0f);
    g.drawText("WIDE LOAD (stereo)", 10, 70, 580, 15, juce::Justification::centredLeft, 0);

    g.setColour(juce::Colour(0xFFFFFFFF));
    g.drawText("Volume", 50, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Freq Off", 130, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Del Off", 210, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Mix", 290, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Tone", 370, 95, 70, 15, juce::Justification::centred, 0);
    g.drawText("Bal", 450, 95, 70, 15, juce::Justification::centred, 0);

    g.setColour(juce::Colour(0xFFC0C0C0));
    g.drawText("Stereo effect used to make the sound wider.", 10, 210, 580, 15, juce::Justification::centredLeft, 0);

    sldKnob[e_Mod4On].setVisible(true);
    
}

//R4.00 Paint the AMPLIFIER ui in the settings panel.
void MakoDist01AudioProcessorEditor::Paint_Amp(juce::Graphics& g)
{
    if (DefAmp_Update) DefAmp_UpdateList();

    Paint_Back2(g, juce::Colour(0xFF303030), juce::Colour(0xFF000000));

    //R2.00 HEADERS
    g.setColour(juce::Colour(0xFFFF8000));
    g.setFont(12.0f);
    g.drawText("AMPLIFIER:", 10, 70, 550, 15, juce::Justification::centredLeft, 0);
    if (!makoProcessor.InputEQ_PreEQ) g.setColour(juce::Colour(0xFF303030));
    g.drawText("Pre EQ", 388, 70, 50, 15, juce::Justification::centredLeft, 0);
    
    g.setFont(10.0f);
    g.setColour(juce::Colour(0xFFFFFFFF));
    g.drawText("Thin", 10, 88, 50, 15, juce::Justification::centred, 0);
    g.drawText("Qual", 60, 88, 50, 15, juce::Justification::centred, 0);
    g.drawText("Slope", 110, 88, 50, 15, juce::Justification::centred, 0);
    g.drawText("Thump", 185, 88, 50, 15, juce::Justification::centred, 0);
    g.drawText("Air", 235, 88, 50, 15, juce::Justification::centred, 0);
    g.drawText("Power", 285, 88, 50, 15, juce::Justification::centred, 0);
    g.drawText("Comp", 360, 88, 50, 15, juce::Justification::centred, 0);
    g.drawText("C Ratio", 410, 88, 50, 15, juce::Justification::centred, 0);
    g.drawText("Clean Vol", 460, 88, 50, 15, juce::Justification::centred, 0);
    g.drawText("Vol", 510, 88, 50, 15, juce::Justification::centred, 0);

    g.drawText("Gain", 10, 157, 50, 15, juce::Justification::centred, 0);
    g.drawText("Boom", 60, 157, 50, 15, juce::Justification::centred, 0);
    g.drawText("Crisp", 110, 157, 50, 15, juce::Justification::centred, 0);
    g.drawText("EQ Style", 160, 157, 50, 15, juce::Justification::centred, 0);
    g.drawText("Hi Cut", 510, 157, 50, 15, juce::Justification::centred, 0);

    //R4.00 Draw the EQ freqs.
    int t_EQmode = makoProcessor.Setting[e_AmpEQMode];
    g.drawText(str_EQ[t_EQmode][0], 210, 157, 50, 15, juce::Justification::centred, 0);
    g.drawText(str_EQ[t_EQmode][1], 260, 157, 50, 15, juce::Justification::centred, 0);
    g.drawText(str_EQ[t_EQmode][2], 310, 157, 50, 15, juce::Justification::centred, 0);
    g.drawText(str_EQ[t_EQmode][3], 360, 157, 50, 15, juce::Justification::centred, 0);
    g.drawText(str_EQ[t_EQmode][4], 410, 157, 50, 15, juce::Justification::centred, 0);
    g.drawText(str_EQ[t_EQmode][5], 460, 157, 50, 15, juce::Justification::centred, 0);

    Paint_VertDiv(g, 173, 90, 148);
    Paint_VertDiv(g, 348, 90, 148);

    //R4.00 Make our controls visible.
    cbAmpList.setVisible(true);
    butGetDefIR.setVisible(true);
    labAmp_Comp.setVisible(true);
    sldKnob[e_AmpOn].setVisible(true);

    for (int t = e_AmpQual; t < (e_AmpEQMode + 1); t++) sldKnob[t].setVisible(true);
    
    Mako_EQ_Update(true);
}

//R4.00 Paint the SPEAKER IR ui in the settings panel.
void MakoDist01AudioProcessorEditor::Paint_IR(juce::Graphics& g)
{
    juce::Path pathTrace;

    if (DefIR_Update) DefIR_UpdateList();
    cbIRList.setVisible(true);
        
    Paint_Back2(g, juce::Colour(0xFF303030), juce::Colour(0xFF000000));

    //R2.00 HEADERS
    g.setColour(juce::Colour(0xFFFF8000));
    g.setFont(12.0f);
    g.drawText("SPEAKER IMPULSE RESPONSE", 10, 70, 580, 15, juce::Justification::centredLeft, 0);
    
    g.setColour(juce::Colour(0xFFFFFFFF));
    g.drawText("IR", 10, 95, 80, 15, juce::Justification::centredLeft, 0);
    g.drawText("Size", 10, 130, 80, 15, juce::Justification::centredLeft, 0);
    g.drawText("Volume", 10, 170, 80, 15, juce::Justification::centredLeft, 0);
            
    //R4.00 Draw some of the currently loaded IR.
    g.setColour(juce::Colour(0xFF000000));
    g.fillRect(200, 140, 350, 62);
    g.setColour(juce::Colour(0xFFFF8000));
    g.drawRect(200, 140, 350, 62);
    g.drawRect(200, 170, 350, 1);    
    g.setColour(juce::Colour(0xFFFFFFFF));
    pathTrace.startNewSubPath(200, 171); for (int t = 0; t < 349; t++)  pathTrace.lineTo(200 + t, int(171 - (makoProcessor.IR_Speaker[t] * 30.0f))); g.strokePath(pathTrace, juce::PathStrokeType(1.0f));
    //pathTrace.clear(); //R4.00 Need to clear for color change.
    
    //R4.00 Make our Slider controls visible.
    sldKnob[e_IRSize].setVisible(true);
    sldKnob[e_IRVol].setVisible(true); 
    sldKnob[e_IROn].setVisible(true);
}

//R4.00 Paint the DELAY ui in the settings panel.
void MakoDist01AudioProcessorEditor::Paint_Delay(juce::Graphics& g)
{
    //R4.00 Make our Slider controls visible.
    sldKnob[e_DelOn].setVisible(true);
    sldKnob[e_DelMix].setVisible(true);
    sldKnob[e_DelTime].setVisible(true);
    sldKnob[e_DelOffset].setVisible(true);
    sldKnob[e_DelRepeat].setVisible(true);
    sldKnob[e_DelBal].setVisible(true);
    sldKnob[e_DelDuck].setVisible(true);

    Paint_Back(g, juce::Colour(0xFF6C7C8A));

    //R4.00 HEADERS
    g.setColour(juce::Colour(0xFFFF8000));
    g.setFont(12.0f);
    g.drawText("DELAY (stereo)", 10, 70, 580, 15, juce::Justification::centredLeft, 0);

    g.setFont(10.0f);
    g.setColour(juce::Colour(0xFFFFFFFF));
    g.drawText("Mix", 10, 90, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Time", 10, 120, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Offset", 10, 150, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Repeat", 10, 180, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Balance", 200, 90, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Ducking", 200, 120, 580, 15, juce::Justification::centredLeft, 0);
    
}

//R4.00 Paint the REVERB ui in the settings panel.
void MakoDist01AudioProcessorEditor::Paint_Reverb(juce::Graphics& g)
{
    //R4.00 Make our controls visible.
    cbRevMode.setVisible(true);
    sldKnob[e_RevOn].setVisible(true);
    sldKnob[e_RevMix].setVisible(true);
    sldKnob[e_RevTime].setVisible(true);
    sldKnob[e_RevBal].setVisible(true);
    sldKnob[e_RevChor].setVisible(true);
    sldKnob[e_RevSize].setVisible(true);
    sldKnob[e_RevLP].setVisible(true);
    sldKnob[e_RevHP].setVisible(true);
    sldKnob[e_RevPreD].setVisible(true);
    sldKnob[e_RevDuck].setVisible(true);
    sldKnob[e_RevPar].setVisible(true);

    Paint_Back(g, juce::Colour(0xFF808080));

    //R4.00 HEADERS
    g.setColour(juce::Colour(0xFFFF8000));
    g.setFont(12.0f);
    g.drawText("REVERB (stereo)", 10, 70, 580, 15, juce::Justification::centredLeft, 0);

    g.setFont(10.0f);
    g.setColour(juce::Colour(0xFFFFFFFF));
    g.drawText("Effect Type", 130,  65, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Mix",          10,  90, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Time/Decay",   10, 120, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Balance",      10, 150, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Chorus",       10, 180, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Room Size",   200, 90, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("High Cut",   200, 120, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Low Cut",    200, 150, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("PreDelay",   390, 90, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Ducking",    390, 120, 580, 15, juce::Justification::centredLeft, 0);
    g.drawText("Rev-Delay Parallel", 390, 150, 580, 15, juce::Justification::centredLeft, 0);

}

//R4.00 Fill the settings panel with a gradient shaded color. 
void MakoDist01AudioProcessorEditor::Paint_Back(juce::Graphics& g, juce::Colour col)
{
    //R4.00 Draw color Background, Dark Grey to color to Dark Grey. 148-199
    juce::ColourGradient ColGrad = juce::ColourGradient(juce::Colour(0xFF202020), 0.0f, 89.0f, juce::Colour(col), 0.0f, 140.0f, false);
    g.setGradientFill(ColGrad);
    g.fillRect(4, 89, 565, 51);
    ColGrad = juce::ColourGradient(juce::Colour(col), 0.0f, 140.0f, juce::Colour(0xFF101010), 0.0f, 247.0f, false);//199-306
    g.setGradientFill(ColGrad);
    g.fillRect(4, 140, 565, 107);
}

//R4.00 Fill the settings panel top section with a gradient shaded color. 
void MakoDist01AudioProcessorEditor::Paint_Back2(juce::Graphics& g, juce::Colour colTop, juce::Colour colBot)
{
    //R4.00 Draw color Background gradient top to bottom.
    g.setGradientFill(juce::ColourGradient(juce::Colour(colTop), 0, 88, juce::Colour(colBot), 0, 190, false));
    g.fillRect(4, 88, 565, 102);
}

//R4.00 Draw shaded verticle line separators.
void MakoDist01AudioProcessorEditor::Paint_VertDiv(juce::Graphics& g, int x, int yStart, int yStop)
{
    //R4.00 Draw section divider lines.
    g.setColour(juce::Colour(0xFFF000000));
    g.drawVerticalLine(x, yStart, yStop);
    g.setColour(juce::Colour(0xFFF606060));
    g.drawVerticalLine(x+1, yStart, yStop);
}

//R4.00 Resize our window and set all control positions.
void MakoDist01AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
   
    //R1.00 Draw all of the defined KNOBS.
    for (int t = 0; t < e_PLast; t++)
    {
        sldKnob[t].setBounds(Knob_Pos[t].x, Knob_Pos[t].y, Knob_Pos[t].sizex, Knob_Pos[t].sizey);
    }

    //R4.00 HELP
    labHelp.setBounds (26, 229, 560, 15);
    labVersion.setBounds(265, 45, 75, 15);
    butHelp.setBounds(6, 229, 20, 15);

    //R4.00 Main Menu Bar Buttons
    butPresetLoad.setBounds(579, 60, 95, 18); 
    butPresetSave.setBounds(579, 85, 95, 18); 
    butVUHide.setBounds(579, 120, 95, 18);    
    butDefAmps.setBounds(579, 155, 95, 18);
    butDefIRs.setBounds(579, 180, 95, 18);

    //R4.00 AMP section buttons.
    butGetDefIR.setBounds(430, 70, 55, 15);
    
    //R4.00 Database editing buttons.
    butPageUp.setBounds(345, 65, 65, 18);
    butPageDn.setBounds(345, 85, 65, 18);
    butDBase_Load.setBounds(345, 175, 65, 18);
    butDBase_Save.setBounds(345, 195, 65, 18);

    //R4.00 Amp, IR, and Reverb selection combo boxes.
    cbAmpList.setBounds(72, 67, 310, 18);
    cbIRList.setBounds(30, 95, 320, 18);
    cbRevMode.setBounds(200, 68, 140, 18);

    //R4.00 GATE knobs.
    sldKnob[e_GateVol].setBounds(30, 110, 70, 70);
    sldKnob[e_GateExp].setBounds(110, 110, 70, 70);
    sldKnob[e_GateAvg].setBounds(180, 110, 70, 70);
    sldKnob[e_GateFreq].setBounds(250, 110, 70, 70);
    sldKnob[e_GateWah].setBounds(330, 110, 70, 70);
    sldKnob[e_GateWahQ].setBounds(400, 110, 70, 70);
    sldKnob[e_GateWahRange].setBounds(470, 110, 70, 70);

    //R4.00 COMPRESSOR knobs.
    sldKnob[e_CompVol].setBounds(40, 110, 70, 70);
    sldKnob[e_CompAtt].setBounds(130, 110, 70, 70);
    sldKnob[e_CompRel].setBounds(210, 110, 70, 70);
    sldKnob[e_CompThr].setBounds(290, 110, 70, 70);
    sldKnob[e_CompRat].setBounds(370, 110, 70, 70);
    sldKnob[e_CompDrv].setBounds(460, 110, 70, 70);

    //R4.00 DISTORTION 1 knobs.
    sldKnob[e_Dist1Vol].setBounds(50, 110, 70, 70);
    sldKnob[e_Dist1MidF].setBounds(130, 110, 70, 70);
    sldKnob[e_Dist1Mid].setBounds(210, 110, 70, 70);
    sldKnob[e_Dist1MidQ].setBounds(290, 110, 70, 70);
    sldKnob[e_Dist1Cut].setBounds(370, 110, 70, 70);
    sldKnob[e_Dist1Drv].setBounds(450, 110, 70, 70);

    //R4.00 FLANGER knobs.
    sldKnob[e_Mod1Vol].setBounds(45, 110, 70, 70);
    sldKnob[e_Mod1Rate].setBounds(125, 110, 70, 70);
    sldKnob[e_Mod1Depth].setBounds(205, 110, 70, 70);
    sldKnob[e_Mod1Mix].setBounds(285, 110, 70, 70);
    sldKnob[e_Mod1Synth].setBounds(380, 110, 70, 70);
    sldKnob[e_Mod1Bronz].setBounds(460, 110, 70, 70);
    
    //R4.00 MOD FILTER knobs.
    sldKnob[e_Mod2Vol].setBounds(90, 110, 70, 70);
    sldKnob[e_Mod2Rate].setBounds(170, 110, 70, 70);
    sldKnob[e_Mod2Range].setBounds(250, 110, 70, 70);
    sldKnob[e_Mod2FStart].setBounds(330, 110, 70, 70);
    sldKnob[e_Mod2Mix].setBounds(410, 110, 70, 70);
    
    //R4.00 DISTORTION 2 knobs.
    sldKnob[e_Dist2Vol].setBounds(50, 110, 70, 70);
    sldKnob[e_Dist2MidF].setBounds(130, 110, 70, 70);
    sldKnob[e_Dist2Mid].setBounds(210, 110, 70, 70);
    sldKnob[e_Dist2MidQ].setBounds(290, 110, 70, 70);
    sldKnob[e_Dist2Cut].setBounds(370, 110, 70, 70);
    sldKnob[e_Dist2Drv].setBounds(450, 110, 70, 70);

    //R4.00 EQUALIZER knobs.
    sldKnob[e_EQVol].setBounds(40, 110, 70, 70);
    sldKnob[e_EQF1].setBounds(110, 110, 70, 70);
    sldKnob[e_EQF2].setBounds(180, 110, 70, 70);
    sldKnob[e_EQF3].setBounds(250, 110, 70, 70);
    sldKnob[e_EQF4].setBounds(320, 110, 70, 70);
    sldKnob[e_EQF5].setBounds(390, 110, 70, 70);
    sldKnob[e_EQF6].setBounds(460, 110, 70, 70);

    //R4.00 CHORUS knobs.
    sldKnob[e_Mod3Vol].setBounds(50, 110, 70, 70);
    sldKnob[e_Mod3Rate].setBounds(130, 110, 70, 70);
    sldKnob[e_Mod3Depth].setBounds(210, 110, 70, 70);
    sldKnob[e_Mod3Mix].setBounds(290, 110, 70, 70);
    sldKnob[e_Mod3Tone].setBounds(370, 110, 70, 70);
    sldKnob[e_Mod3Bal].setBounds(450, 110, 70, 70);

    //R4.00 WIDE LOAD knobs.
    sldKnob[e_Mod4Vol].setBounds(50, 110, 70, 70);
    sldKnob[e_Mod4Foff].setBounds(130, 110, 70, 70);
    sldKnob[e_Mod4Doff].setBounds(210, 110, 70, 70);
    sldKnob[e_Mod4Mix].setBounds(290, 110, 70, 70);
    sldKnob[e_Mod4Tone].setBounds(370, 110, 70, 70);
    sldKnob[e_Mod4Bal].setBounds(450, 110, 70, 70);

    labAmp_Comp.setBounds(363, 92, 4, 6);
    labPedal2_Comp.setBounds(294, 100, 4, 6);

    //R1.00 Input VU
    labVU_In1.setBounds(35, 53, 8, 2);
    labVU_In2.setBounds(45, 53, 8, 2);
    labVU_In3.setBounds(55, 53, 8, 2);
    labVU_In4.setBounds(65, 53, 8, 2);
    labVU_In5.setBounds(75, 53, 8, 2);
    labVU_In6.setBounds(85, 53, 8, 2);

    //R1.00 Output VU
    labVU_Out1.setBounds(470, 53, 8, 2);
    labVU_Out2.setBounds(480, 53, 8, 2);
    labVU_Out3.setBounds(490, 53, 8, 2);
    labVU_Out4.setBounds(500, 53, 8, 2);
    labVU_Out5.setBounds(510, 53, 8, 2);
    labVU_Out6.setBounds(520, 53, 8, 2);    
}


int MakoDist01AudioProcessorEditor::Num_Normalize(float V)
{
    //R1.00 Convert float 0.0-1.0 to 0 to 100 for display on editor. 
    int tV = V * 100;
    return tV;
}


void MakoDist01AudioProcessorEditor::buttonClicked (juce::Button* butt)
{       
    //R4.00 We need to update settings in processor.
    makoProcessor.makoSettingsChanged += 1;
    makoProcessor.makoSettingsClearbuffers = 1;
   
    auto buttstate = butt->getState();
    
    //R4.00 Toggle VU meter operation.
    if (butt == &butVUHide)
    {
        labHelp.setText("VU METER: Dont update VU meters to Reduce CPU usage.", juce::dontSendNotification);
        VU_Off();
        if (makoProcessor.makoVUHide == 0)
        {
            makoProcessor.makoVUHide = 1;
            
        }
        else
            makoProcessor.makoVUHide = 0;
    }

    //R4.00 Added AMP database definition.
    if (butt == &butDefAmps)
    {
        //R4.00 If we are 1st editing, calculate the current page of edits.
        if (DefAmp_Page == 0)
        {
            int tI = cbAmpList.getSelectedId() - 1;
            if (tI < 0) tI = 0;
            DefAmp_Page = tI / 10;
        }
        editMode = EM_DefAmps;
        DefAmp_Editing = 1;    //R4.00 Flag we are editing the database.
        repaint();
    }
    if (butt == &butDefIRs)
    {
        //R4.00 If we are 1st editing, calculate the current page of edits.
        if (DefIR_Page == 0)
        {
            int tI = cbIRList.getSelectedId() - 1;
            if (tI < 0) tI = 0;
            DefIR_Page = tI / 10;
        }
        editMode = EM_DefIRs;
        DefAmp_Editing = 1;    //R4.00 Flag we are editing the database.
        repaint();
    }
    
    if (butt == &butPageUp)
    {
        if (editMode == EM_DefAmps)
        {
            if (0 < DefAmp_Page) DefAmp_Page--;
        }
        if (editMode == EM_DefIRs)
        {
            if (0 < DefIR_Page) DefIR_Page--;
        }
        repaint();
    }
    if (butt == &butPageDn)
    {
        if (editMode == EM_DefAmps)
        {
            if (DefAmp_Page < 19) DefAmp_Page++;
        }
        if (editMode == EM_DefIRs)
        {
            if (DefIR_Page < 19) DefIR_Page++;
        }
        repaint();
    }

    if (butt == &butGetDefIR)
    {
        int idx = cbAmpList.getSelectedId();
        cbIRList.setSelectedId(idx);
    }
    
    if (butt == &butPresetLoad)
    {
        editMode = EM_Load; repaint();

        dlgFile = std::make_unique<juce::FileChooser>("Load Mako Mini D preset file", juce::File{}, "*.mnd");
        auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        dlgFile->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file != juce::File{}) FILE_Mako_Load(file.getFullPathName());
                editMode = EM_None; repaint();
            });
    }

    if (butt == &butPresetSave)
    {
        editMode = EM_Save; repaint();

        dlgFile = std::make_unique<juce::FileChooser>("Save Mako Mini D preset file", SetupPath_Current, "*.mnd");
        auto chooserFlags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;

        dlgFile->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file != juce::File{}) FILE_Mako_Save(file.getFullPathName());
                editMode = EM_None; repaint();
            });
    }

    //R4.00 Database Backup - Load.
    if (butt == &butDBase_Load)
    {
        dlgFile = std::make_unique<juce::FileChooser>("MAKO DISTORTION Database Backup", juce::File{}, "*.M2B");
        auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        //R4.00 Store the selected file.
        dlgFile->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file != juce::File{})
                {
                    FILE_DBase_Load(file.getFullPathName());
                    DefAmp_Update = true;                    
                    DefAmp_Editing = 2;       //R3.00 Flag that we have unsaved database entries.
                }
            });
    }

    //R4.00 Database Backup - Save.
    if (butt == &butDBase_Save)
    {
        dlgFile = std::make_unique<juce::FileChooser>("MAKO DISTORTION Database Backup", juce::File{}, "*.M2B");
        auto chooserFlags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;

        //R4.00 Store the selected file.
        dlgFile->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file != juce::File{}) FILE_DBase_Save(file.getFullPathName());
            });
    }

    //R4.00 Toggle Help visibility.
    if (butt == &butHelp)
    {
        if (Help_Show)
        {
            Help_Show = false;
            labHelp.setVisible(false);
        }
        else
        {
            Help_Show = true;
            labHelp.setVisible(true);
        }
    }


}

//R4.00 Read in Amp/IR database. Mako Distortion 2 compatible.
void MakoDist01AudioProcessorEditor::FILE_DBase_Load(juce::String tFile)
{
    int Frev = 0;
    bool FileValid = false;
    juce::FileInputStream Input(tFile);
    {
        Input.setPosition(0);

        //R4.00 Get the file revision.
        auto b = Input.readNextLine();
        if (b == "MD2_SAVE_400") Frev = 400;

        //R4.00 If we have a valid file, read the data.
        if (Frev != 400)
            FileValid = false;
        else
        {
            b = Input.readNextLine();    //R4.00 Do not edit this file notice.

            //R4.00 Read in the Samples used for Synths. Not used in MiniD.
            b = Input.readNextLine(); 
            b = Input.readNextLine(); 
            b = Input.readNextLine(); 
            b = Input.readNextLine(); 
            b = Input.readNextLine(); 
            b = Input.readNextLine(); 
            b = Input.readNextLine(); 
            b = Input.readNextLine(); 

            //R4.00 Read in the AMPLIFIER DATABASE.
            if (399 < Frev)
            {
                b = Input.readNextLine();      //R4.00 AMP header.
                for (int t = 0; t < 200; t++)
                {
                    b = Input.readNextLine();
                    if (19 < t)
                    {
                        makoProcessor.DefAmp_Path[t] = b;
                        makoProcessor.DefAmp_File[t] = makoProcessor.DefAmp_Path[t].fromLastOccurrenceOf("\\", false, true);
                    }

                    b = Input.readNextLine();
                    if (19 < t) makoProcessor.DefAmp_GainSetting[t] = FILE_ClipInt(b.getFloatValue(), 0, 2);

                    b = Input.readNextLine();
                    if (19 < t)
                    {
                        makoProcessor.DefAmp_PreGain[t] = false;
                        if (FILE_ClipInt(b.getFloatValue(), 0, 1)) makoProcessor.DefAmp_PreGain[t] = true;
                    }
                }
                b = Input.readNextLine();
            }

            //R4.00 Read in the IR DATABASE.
            if (399 < Frev)
            {
                b = Input.readNextLine(); //R3.00 IR header.
                for (int t = 0; t < 200; t++)
                {
                    b = Input.readNextLine();
                    if (19 < t)
                    {
                        makoProcessor.DefIR_Path[t] = b;
                        makoProcessor.DefIR_File[t] = makoProcessor.DefIR_Path[t].fromLastOccurrenceOf("\\", false, true);
                    }
                }
                b = Input.readNextLine();
            }
        }
    }

}

//R4.00 Save the Amp/IR database. Mako Distortion 2 compatible.
void MakoDist01AudioProcessorEditor::FILE_DBase_Save(juce::String tFile)
{
    juce::FileOutputStream output(tFile);
    {
        //R4.00 Over write any data.
        output.setPosition(0);
        output.truncate();

        //R4.00 Mako IR PATHS file, version 0.
        output.writeText("MD2_SAVE_400\r\n", false, false, nullptr);
        output.writeText("DO NOT EDIT THIS FILE - Initial IR and WAVE file data that is not stored in VST data.\r\n", false, false, nullptr);

        //R4.00 Store the Samples used if any. Not used in MiniD.
        output.writeText("\r\n", false, false, nullptr);
        output.writeText("\r\n", false, false, nullptr);
        output.writeText("\r\n", false, false, nullptr);
        output.writeText("\r\n", false, false, nullptr);
        output.writeText("\r\n", false, false, nullptr);
        output.writeText("\r\n", false, false, nullptr);
        output.writeText("\r\n", false, false, nullptr);
        output.writeText("\r\n", false, false, nullptr);

        //R4.00 Store Amplifier List.
        output.writeText(std::string("AMPLIFIER DATABASE") + "\r\n", false, false, nullptr);
        for (int t = 0; t < 200; t++)
        {
            output.writeText(makoProcessor.DefAmp_Path[t] + "\r\n", false, false, nullptr);
            output.writeText(std::to_string(makoProcessor.DefAmp_GainSetting[t]) + "\r\n", false, false, nullptr);
            output.writeText(std::to_string(makoProcessor.DefAmp_PreGain[t]) + "\r\n", false, false, nullptr);
        }
        output.writeText("\r\n", false, false, nullptr);

        //R4.00 Store Amplifier List.
        output.writeText(std::string("IR DATABASE") + "\r\n", false, false, nullptr);
        for (int t = 0; t < 200; t++)
        {
            output.writeText(makoProcessor.DefIR_Path[t] + "\r\n", false, false, nullptr);
        }
        output.writeText("\r\n", false, false, nullptr);
    }
}

//R4.00 
void MakoDist01AudioProcessorEditor::timerCallback()
{           
    //R4.00 Clear the Help text when we start up. 
    if (FLAG_ClearHelp)
    {
        FLAG_ClearHelp = false;
        labHelp.setText("Mako Mini D - v4.00", juce::sendNotification);
    }

    //R2.00 PROC has updated the VALUE TREE, update our GUI vars.
    if (makoProcessor.makoEditorNeedsUpdated == 1)
    {
        makoProcessor.makoEditorNeedsUpdated = 0;
        Mako_UpdateVals(false);
        repaint(); 
    }
    
    //R2.22 Our HELP string has changed, update screen.
    if (ctrlHelp != ctrlHelpLast)
    {
        ctrlHelpLast = ctrlHelp;
        labHelp.setText(LangStringHelp[ctrlHelp], juce::dontSendNotification);
    }

    if (!makoProcessor.makoVUHide) VU_Meter_Update_Juce();

    if (makoProcessor.makoAmpCompLight != makoProcessor.makoAmpCompLightLast)
    {
        makoProcessor.makoAmpCompLightLast = makoProcessor.makoAmpCompLight;
        if (makoProcessor.makoAmpCompLight)
            labAmp_Comp.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF00FF00));
        else
            labAmp_Comp.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    }

    if (makoProcessor.makoPedal2CompLight != makoProcessor.makoPedal2CompLightLast)
    {
        makoProcessor.makoPedal2CompLightLast = makoProcessor.makoPedal2CompLight;
        if (makoProcessor.makoPedal2CompLight)
            labPedal2_Comp.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF00FF00));
        else
            labPedal2_Comp.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    }

    //R1.00 Reset our CLIP flags. 
    makoProcessor.makoClipStage = 0;    
}


void MakoDist01AudioProcessorEditor::Mako_UpdateVals(bool FlagProcessor)
{
    //R2.18 The editor is completely destroyed and rebuilt everytime you change VST selections. 
    //R2.18 Need to pull every variable from the processor since it stays running at all times.

    //R3.10 Rebuild lists after USER requested database reload has completed.
    if (makoProcessor.DefAmp_Reload == 2)
    {
        labHelp.setText("", juce::dontSendNotification); //R3.00 Clear any previous errors.
        DefAmp_UpdateList();
        DefIR_UpdateList();
        makoProcessor.DefAmp_Reload = 0;                 //R3.00 Reset the request variable.
    }
}

//R4.00 Render the VU Meters using Juce Label objects.
void MakoDist01AudioProcessorEditor::VU_Meter_Update_Juce()
{
    //R4.00 VU METERS
    //R4.00 They are not really needed for VST since DAW will have them.
    //R4.00 Good for EXE and debugging code changes since you can flag where clipping occurs.

    //R4.00 INPUT VU METER.
    //R4.00 Update our VU meter Label objects.
    if (1 < makoProcessor.makoVU_In)
        labVU_In1.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF40FF40));
    else
        labVU_In1.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    if (20 < makoProcessor.makoVU_In)
        labVU_In2.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF40FF40));
    else
        labVU_In2.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    if (40 < makoProcessor.makoVU_In)
        labVU_In3.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF40FF40));
    else
        labVU_In3.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    if (60 < makoProcessor.makoVU_In)
        labVU_In4.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF40FF40));
    else
        labVU_In4.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    if (80 < makoProcessor.makoVU_In)
        labVU_In5.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF40FF40));
    else
        labVU_In5.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    if (95 < makoProcessor.makoVU_In)
        labVU_In6.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF40FF40));
    else
        labVU_In6.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    

    //R4.00 OUTPUT VU METER
    //R4.00 Update our VU meter Label objects.
    if (1 < makoProcessor.makoVU_Out)
        labVU_Out1.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF40FF40));
    else
        labVU_Out1.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    if (20 < makoProcessor.makoVU_Out)
        labVU_Out2.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF40FF40));
    else
        labVU_Out2.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    if (40 < makoProcessor.makoVU_Out)
        labVU_Out3.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF40FF40));
    else
        labVU_Out3.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    if (60 < makoProcessor.makoVU_Out)
        labVU_Out4.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF40FF40));
    else
        labVU_Out4.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    if (80 < makoProcessor.makoVU_Out)
        labVU_Out5.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF40FF40));
    else
        labVU_Out5.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    if ((95 < makoProcessor.makoVU_Out) || (0 < makoProcessor.makoClipStage))
    {
        switch (makoProcessor.makoClipStage)
        {
        case 0: //R1.00 No clipping. 
            labVU_Out6.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF40FF40)); break;
        case 1://R1.00 PREAMP clipping. 
            labVU_Out6.setColour(juce::Label::backgroundColourId, juce::Colour(255, 0, 255)); break;
        case 2://R1.00 AMP clipping. 
            labVU_Out6.setColour(juce::Label::backgroundColourId, juce::Colour(255, 255, 0)); break;
        case 3: //R4.00 Final 
            labVU_Out6.setColour(juce::Label::backgroundColourId, juce::Colour(255, 0, 0)); break;
        case 4: //R4.00 Reverb Clip.
            labVU_Out6.setColour(juce::Label::backgroundColourId, juce::Colour(192, 255, 255)); break;
        default:
            labVU_Out6.setColour(juce::Label::backgroundColourId, juce::Colour(255, 0, 0)); break;
        }
    }
    else
        labVU_Out6.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));

}

//R4.00 Added VU clear function.
void MakoDist01AudioProcessorEditor::VU_Off()
{
    labVU_In1.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    labVU_In2.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    labVU_In3.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    labVU_In4.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    labVU_In5.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    labVU_In6.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    labVU_Out1.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    labVU_Out2.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    labVU_Out3.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    labVU_Out4.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    labVU_Out5.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
    labVU_Out6.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF000000));
}


//R4.00 Mouse Override functions not in use.
/*
void MakoDist01AudioProcessorEditor::mouseUp(const juce::MouseEvent& e)
{
    return;
}

void MakoDist01AudioProcessorEditor::mouseDrag(const juce::MouseEvent& e)
{
    return;   
}

void MakoDist01AudioProcessorEditor::mouseMove(const juce::MouseEvent& e)
{
    return;
}
*/

//R4.00 Handle user clicks that are not on Juce controls.
void MakoDist01AudioProcessorEditor::mouseDown(const juce::MouseEvent& e)
{
    //R2.00 This gets called twice for every click. 
    //R2.00 Do not use the 2nd call or toggles will not work.
    if (MOUSEHACK_IsDown)
    {
        MOUSEHACK_IsDown = false;
        return;
    }
    MOUSEHACK_IsDown = true;

    std::string tMess = "";
    
    int mButton = 2; 
    if (e.mods.isLeftButtonDown()) mButton = 1;
    if (e.mods.isMiddleButtonDown()) mButton = 3;
    int posX = e.getPosition().x;
    int posY = e.getPosition().y;

    MOUSE_Down = mButton;

    //R3.10 Check valid click.
    if (e.eventComponent->getBoundsInParent().getWidth() < 600) return;

    //R2.00 Check for presses in SIGNAL PATH area.
    if ((rcPedal1.y1 < posY) && (posY < rcPedal1.y2))
    {
        DBG_Int1 = 0;
        if ((rcPedal1.x1 < posX) && (posX < rcPedal1.x2))  Mouse_Check_Pedal(mButton, 1, EM_Ped1);
        if ((rcPedal2.x1 < posX) && (posX < rcPedal2.x2))  Mouse_Check_Pedal(mButton, 2, EM_Ped2);
        if ((rcPedal3.x1 < posX) && (posX < rcPedal3.x2))  Mouse_Check_Pedal(mButton, 3, EM_Ped3);
        if ((rcPedal4.x1 < posX) && (posX < rcPedal4.x2))  Mouse_Check_Pedal(mButton, 4, EM_Ped4);
        if ((rcPedal5.x1 < posX) && (posX < rcPedal5.x2))  Mouse_Check_Pedal(mButton, 5, EM_Ped5);

        if ((rcMod1.x1 < posX) && (posX < rcMod1.x2)) Mouse_Check_Mod1(mButton);
        if ((rcMod2.x1 < posX) && (posX < rcMod2.x2)) Mouse_Check_Mod2(mButton);
        if ((rcMod3.x1 < posX) && (posX < rcMod3.x2)) Mouse_Check_Mod3(mButton);
        if ((rcMod4.x1 < posX) && (posX < rcMod4.x2)) Mouse_Check_Mod4(mButton);

        if ((rcAmp.x1 < posX) && (posX < rcAmp.x2))        Mouse_Check_Amp(mButton);
        if ((rcIR.x1 < posX) && (posX < rcIR.x2))          Mouse_Check_IR(mButton);
        
        if ((rcDelay.x1 < posX) && (posX < rcDelay.x2))    Mouse_Check_Del(mButton);
        if ((rcReverb.x1 < posX) && (posX < rcReverb.x2))  Mouse_Check_Rev(mButton);
        
        repaint();
    }
            
    //R4.00 Check for presses in SETTINGS area.
    if ((63 < posY) && (posY < 245))
    {
        switch (editMode)
        {
        case (EM_DefAmps): Mouse_Check_DefAmp(mButton, posX, posY); break;
        case (EM_DefIRs): Mouse_Check_DefIR(mButton, posX, posY); break;
        }        
    }
    
}

//R4.00 Check mouse clicks in the SIGNAL PATH area.
void MakoDist01AudioProcessorEditor::Mouse_Check_Pedal(int button, int idx, int editmode)
{
    float tF;
    int tP = 0;
    int tMode = 0;

    //R4.00 Translate PEDAL number to actual pedal and get possible editMode.
    switch (idx)
    {
    case 1: tP = e_GateOn;  tMode = EM_Ped1; break;
    case 2: tP = e_CompOn;  tMode = EM_Ped2; break;
    case 3: tP = e_Dist1On; tMode = EM_Ped3; break;
    case 4: tP = e_Dist2On; tMode = EM_Ped4; break;
    case 5: tP = e_EQOn;    tMode = EM_Ped5; break;
    }

    //R4.00 Check for pedal selection.
    if (button == 1) editMode = tMode;

    //R4.00 Check for right mouse click.
    if ((tP) && (button == 2))
    {
        //R4.00 Toggle the units On/Off status.
        tF = sldKnob[tP].getValue();
        if (.5f < tF)
            sldKnob[tP].setValue(0.0);
        else
            sldKnob[tP].setValue(1.0);
    }

    repaint();
}


//R4.00 Check mouse clicks in the SIGNAL PATH area.
void MakoDist01AudioProcessorEditor::Mouse_Check_Amp(int button)
{
    if (button == 1)
        editMode = EM_Amp;
    else
        (makoProcessor.Setting[e_AmpOn]) ? sldKnob[e_AmpOn].setValue(0.0) : sldKnob[e_AmpOn].setValue(1.0);
}

//R4.00 Check mouse clicks in the SIGNAL PATH area.
void MakoDist01AudioProcessorEditor::Mouse_Check_IR(int button)
{
    if (button == 1)
        editMode = EM_IR;
    else
        (makoProcessor.Setting[e_IROn]) ? sldKnob[e_IROn].setValue(0.0) : sldKnob[e_IROn].setValue(1.0);
}

//R4.00 Check mouse clicks in the SIGNAL PATH area.
void MakoDist01AudioProcessorEditor::Mouse_Check_Mod1(int button)
{
    if (button == 1)
        editMode = EM_Mod1;
    else
        (makoProcessor.Setting[e_Mod1On]) ? sldKnob[e_Mod1On].setValue(0.0) : sldKnob[e_Mod1On].setValue(1.0);
}

//R4.00 Check mouse clicks in the SIGNAL PATH area.
void MakoDist01AudioProcessorEditor::Mouse_Check_Mod2(int button)
{
    if (button == 1)
        editMode = EM_Mod2;
    else
        (makoProcessor.Setting[e_Mod2On]) ? sldKnob[e_Mod2On].setValue(0.0) : sldKnob[e_Mod2On].setValue(1.0);
}

//R4.00 Check mouse clicks in the SIGNAL PATH area.
void MakoDist01AudioProcessorEditor::Mouse_Check_Mod3(int button)
{
    if (button == 1)
        editMode = EM_Mod3;
    else
        (makoProcessor.Setting[e_Mod3On]) ? sldKnob[e_Mod3On].setValue(0.0) : sldKnob[e_Mod3On].setValue(1.0);
}

//R4.00 Check mouse clicks in the SIGNAL PATH area.
void MakoDist01AudioProcessorEditor::Mouse_Check_Mod4(int button)
{
    if (button == 1)
        editMode = EM_Mod4;
    else
        (makoProcessor.Setting[e_Mod4On]) ? sldKnob[e_Mod4On].setValue(0.0) : sldKnob[e_Mod4On].setValue(1.0);
}

//R4.00 Check mouse clicks in the SIGNAL PATH area.
void MakoDist01AudioProcessorEditor::Mouse_Check_Del(int button)
{
    if (button == 1)
        editMode = EM_Delay;
    else
        (makoProcessor.Setting[e_DelOn]) ? sldKnob[e_DelOn].setValue(0.0) : sldKnob[e_DelOn].setValue(1.0);

}

//R4.00 Check mouse clicks in the SIGNAL PATH area.
void MakoDist01AudioProcessorEditor::Mouse_Check_Rev(int button)
{
    if (button == 1)
        editMode = EM_Reverb;
    else
        (makoProcessor.Setting[e_RevOn]) ? sldKnob[e_RevOn].setValue(0.0) : sldKnob[e_RevOn].setValue(1.0);
}

//R4.00 Check mouse clicks while editing the Amp Database.
void MakoDist01AudioProcessorEditor::Mouse_Check_DefAmp(int mButton, int posX, int posY)
{
    //R3.00 Calc index from the 10 (0-9) rows on screen.
    int Idx = (213 - posY) / 15;
    if (Idx < 0) Idx = 0;
    if (9 < Idx) Idx = 9;

    //R3.00 Offset row selection by PAGE value.
    Idx = (9 - Idx) + (DefAmp_Page * 10);

    //R3.00 Dont edit Amp 0 - 19.
    if (Idx < 20) return;

    //R4.00 We have clicked somewhere, so flag that we need to save edits.
    if ((27 < posX) && (posX < 330))
    {
        DefAmp_CurrentIdx = Idx;
        DefAmp_Update = true;
        DefAmp_Editing = 2;       //R3.00 Flag that we have unsaved database entries.
    }

    //R3.00 Set PreEQ and Amplifier Gain Range.
    if ((27 < posX) && (posX < 52)) makoProcessor.DefAmp_PreGain[Idx] = !makoProcessor.DefAmp_PreGain[Idx];
    if ((54 < posX) && (posX < 74)) makoProcessor.DefAmp_GainSetting[Idx] = 0;
    if ((76 < posX) && (posX < 96)) makoProcessor.DefAmp_GainSetting[Idx] = 1;
    if ((98 < posX) && (posX < 118)) makoProcessor.DefAmp_GainSetting[Idx] = 2;

    if ((120 < posX) && (posX < 330))
    {        
        //R3.00 ADD a file to the database. This calls an async func, list is updated there when it completes.
        if (mButton == 1) MAKO_Amp_UpdateDef();
        
        //R3.00 Delete a file from the database.
        if (mButton == 2)
        {
            makoProcessor.DefAmp_File[Idx] = "";
            makoProcessor.DefAmp_Path[Idx] = "";
            makoProcessor.DefAmp_GainSetting[Idx] = 0;            
        }        
    }    
    repaint();
}

//R4.00 Check mouse clicks while editing the Speaker IR Database.
void MakoDist01AudioProcessorEditor::Mouse_Check_DefIR(int mButton, int posX, int posY)
{
    //R3.00 Calc index from the 10 (0-9) rows on screen.
    int Idx = (213 - posY) / 15;
    if (Idx < 0) Idx = 0;
    if (9 < Idx) Idx = 9;

    //R3.00 Offset row selection by PAGE value.
    Idx = (9 - Idx) + (DefIR_Page * 10);

    //R3.00 Dont edit IR 0 - 19.
    if (Idx < 20) return;

    if ((27 < posX) && (posX < 321))
    {
        DefIR_CurrentIdx = Idx;        
        DefIR_Update = true;
        DefAmp_Editing = 2;     //R3.00 Flag that we have unsaved database entries.
        
        //R3.00 ADD a file to the database. This calls an async func, list is updated there when it completes.
        if (mButton == 1) MAKO_IR_UpdateDef();

        //R3.00 Delete a file from the database.
        if (mButton == 2)
        {
            makoProcessor.DefIR_File[Idx] = "";
            makoProcessor.DefIR_Path[Idx] = "";            
            DefIR_UpdateList();
        }        
    }
    repaint();
}

//R4.00 Let user select an amplifier IR file name and path.
void MakoDist01AudioProcessorEditor::MAKO_Amp_UpdateDef()
{
    juce::File Fail;
    Fail = juce::File(makoProcessor.Amp_Path);

    if (Fail.existsAsFile())
        dlgFile = std::make_unique<juce::FileChooser>("Load Amp Input EQ (1024 sample)", makoProcessor.Amp_Path, "*.wav");
    else
        dlgFile = std::make_unique<juce::FileChooser>("Load Amp Input EQ (1024 sample)", juce::File{}, "*.wav");
    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    //R2.23 Amp file.
    dlgFile->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != juce::File{})
            {
                FILE_Mako_Amp_Load_Def(file.getFullPathName());                
            }
        });
}

//R4.00 Store the selected File and Path in the database.
void MakoDist01AudioProcessorEditor::FILE_Mako_Amp_Load_Def(juce::String tFile)
{    
    //R4.00 Store the selected File and Path in the database.
    makoProcessor.DefAmp_Path[DefAmp_CurrentIdx] = tFile;
    makoProcessor.DefAmp_File[DefAmp_CurrentIdx] = makoProcessor.DefAmp_Path[DefAmp_CurrentIdx].fromLastOccurrenceOf("\\", false, true);
    
    //R3.00 Update the Start directory when searching IRs. 
    makoProcessor.Amp_Path = tFile;
    makoProcessor.Amp_File = makoProcessor.Amp_Path.fromLastOccurrenceOf("\\", false, true);

    repaint();
}

//R4.00 Load an 1024 sample IR wave file and store in InputEQ_FromFile.
void MakoDist01AudioProcessorEditor::FILE_Mako_Amp_Load_Wave(juce::String tFile)
{
    float max = 0.0f;
    int maxIdx = 0;
    int LIS = 0;

    //R2.23 Track the filename of this IR.
    makoProcessor.Amp_Path = tFile;
    makoProcessor.Amp_File = makoProcessor.Amp_Path.fromLastOccurrenceOf("\\", false, true);
    repaint();

    //R2.23 Setup JUCE variables to read the WAVE file.
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::File jFile = juce::File::File(tFile);
    formatManager.registerBasicFormats();
    auto fileBuffer = juce::AudioBuffer < float >::AudioBuffer();
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(jFile));

    //R2.23 Read the WAVE file.
    if (reader.get() != nullptr)
    {
        auto SampCount = (float)reader->lengthInSamples;
        if ((SampCount < 3000) && (reader->sampleRate < 50000))
        {
            fileBuffer.setSize((int)reader->numChannels, (int)reader->lengthInSamples);
            reader->read(&fileBuffer, 0, (int)reader->lengthInSamples, 0, true, false);

            LIS = (int)reader->lengthInSamples;
            if (1024 < LIS) LIS = 1024;           //R2.23 Limit to 1024 samples.

            //R2.23 Load data up to our end point, fill 0s after that.
            for (int t = 0; t < LIS; t++) makoProcessor.InputEQ_FromFile[t] = fileBuffer.getSample(0, t);
            for (int t = LIS; t < 1024; t++) makoProcessor.InputEQ_FromFile[t] = 0.0f;

            float Avg = 0.0f;
            for (int t = 0; t < 1024; t++) Avg += abs(makoProcessor.InputEQ_FromFile[t]);
            labHelp.setText("Amp IR loaded (avg " + std::to_string(int(Avg)) + ").", juce::dontSendNotification);
            makoProcessor.InputEQ_VOffset = Avg;

            //R3.00 Tell proc we have a new amp.
            makoProcessor.makoSettingsChanged += 1;  //R2.23 Flag processor we need to update values
        }
        else
        {
            //R3.00 Handle File size issues.
            labHelp.setText(std::string("ERROR:Invalid Amp file (") + std::to_string(int(makoProcessor.Setting[e_AmpChan])) + std::string(") (1024 samples~22mS@48kHz)."), juce::dontSendNotification);
            repaint();
        }
    }
    else
    {
        //R3.00 Handle File errors.
        labHelp.setText(std::string("ERROR: Selected amp (") + std::to_string(int(makoProcessor.Setting[e_AmpChan])) + std::string(") file missing or invalid."), juce::dontSendNotification);
        repaint();
    }
}


//R4.00 Clear and reload fresh data to the AMP selection list box.
void MakoDist01AudioProcessorEditor::DefAmp_UpdateList()
{    
    std::string d = "";

    int Idx = cbAmpList.getSelectedId();

    cbAmpList.clear();

    for (int t = 0; t < 200; t++)
    {
        d = std::to_string(t) + " - " + makoProcessor.DefAmp_File[t].toStdString();
        cbAmpList.addItem(d, t + 1);
    }    

    cbAmpList.setSelectedId(Idx);

    DefAmp_Update = false;
}

//R4.00 User selected a new ampllfier to use.
void MakoDist01AudioProcessorEditor::cbAmpListChanged()
{
    int Idx = cbAmpList.getSelectedId() - 1;
    makoProcessor.Setting[e_AmpChan] = Idx;
    
    //R3.00 Amp is not built-In (0-19), so check for external amp from database.
    if (19 < Idx)
    {
        if (makoProcessor.DefAmp_Path[Idx] == "")
        {
            //R3.00 no amplifier defined for this position so set to clean full range amp.
            makoProcessor.InputEQ_GainRange = 10; makoProcessor.InputEQ_IRGain = .8f;
            for (int t = 0; t < 1024; t++) makoProcessor.InputEQ_FromFile[t] = 0.0f;
            makoProcessor.InputEQ_FromFile[0] = 1.0f;
        }
        else
        {
            labHelp.setText("", juce::dontSendNotification); //R3.00 Clear any previous errors.

            //R3.00 Valid external database Amplifier selected.
            FILE_Mako_Amp_Load_Wave(makoProcessor.DefAmp_Path[Idx]);

            //R4.00 Setup some vars based on PRE AMP or POST AMP option.
            makoProcessor.InputEQ_PreEQ = makoProcessor.DefAmp_PreGain[Idx];
            if (makoProcessor.InputEQ_PreEQ)
            {
                switch (int(makoProcessor.DefAmp_GainSetting[Idx]))
                {
                case 0: makoProcessor.InputEQ_GainRange = 200; makoProcessor.InputEQ_IRGain = .8f; break;
                case 1: makoProcessor.InputEQ_GainRange = 40000; makoProcessor.InputEQ_IRGain = .04f; break;
                case 2: makoProcessor.InputEQ_GainRange = 400000; makoProcessor.InputEQ_IRGain = .02f; break;
                }
                makoProcessor.InputEQ_IRGain = makoProcessor.InputEQ_IRGain * (1.0f - (makoProcessor.InputEQ_VOffset * .01f));
            }
            else
            {
                switch (int(makoProcessor.DefAmp_GainSetting[Idx]))
                {
                case 0: makoProcessor.InputEQ_GainRange = 20; makoProcessor.InputEQ_IRGain = .7f; break;
                case 1: makoProcessor.InputEQ_GainRange = 500; makoProcessor.InputEQ_IRGain = .3f; break;
                case 2: makoProcessor.InputEQ_GainRange = 5000; makoProcessor.InputEQ_IRGain = .2f; break;
                }
            }


            //R3.00 FORCE amp update in processor.
            makoProcessor.Setting_Last[e_AmpChan] = 0.0f;
        }
    }    

    //R3.00 Flag processor we need to update values
    makoProcessor.SettingRqd[e_AmpChan] = true;
    makoProcessor.makoSettingsChanged += 1;      
}

//R4.00 Draw the Amplifier database editor.
void MakoDist01AudioProcessorEditor::DefAmp_Draw(juce::Graphics& g)
{
    int tX;
    int P = DefAmp_Page * 10;

    butPageUp.setVisible(true);
    butPageDn.setVisible(true);
    butDBase_Load.setVisible(true);
    butDBase_Save.setVisible(true);

    g.setColour(juce::Colour(0xFF000000));
    g.fillRect(5, 62, 563, 157);

    g.setColour(juce::Colour(0xFFFF8000));
    g.setFont(12.0f);
    g.drawText("Amplifier Database", 415, 65, 200, 15, juce::Justification::topLeft, 0);
    g.setColour(juce::Colour(0xFFFFFFFF));
    g.setFont(10.0f);
    g.drawText("Left Click cell to update.", 415, 80, 200, 15, juce::Justification::topLeft, 0);
    g.drawText("Right Click cell to delete.", 415, 95, 200, 15, juce::Justification::topLeft, 0);
    g.drawText("(Number, Gain, Amp File)", 415, 110, 200, 15, juce::Justification::topLeft, 0);
    g.drawText("* Amps are fixed", 415, 125, 200, 15, juce::Justification::topLeft, 0);
    g.drawText("It is recommended to periodically", 415, 175, 200, 15, juce::Justification::topLeft, 0);
    g.drawText("backup the Amp/IR database.", 415, 190, 200, 15, juce::Justification::topLeft, 0);
    g.drawText("Backup Defs does all Amp and IRs.", 355, 265, 200, 15, juce::Justification::topLeft, 0);

    for (int t = 0; t < 10; t++)
    {
        tX = t * 15;

        g.setColour(juce::Colour(0xFFFF8000));
        g.fillRect(5, 65 + tX, 20, 13);

        g.setColour(juce::Colour(0xFF404040));
        g.fillRect(27, 65 + tX, 25, 13);
        g.fillRect(54, 65 + tX, 20, 13);
        g.fillRect(76, 65 + tX, 20, 13);
        g.fillRect(98, 65 + tX, 20, 13);
        g.fillRect(120, 65 + tX, 215, 13);

        //R4.00 Number
        g.setColour(juce::Colour(0xFFFFFFFF));
        g.drawText(std::to_string(P + t), 7, 67 + tX, 16, 15, juce::Justification::topRight, 0);

        if (makoProcessor.DefAmp_PreGain[P + t])
        {
            g.setColour(juce::Colour(0xFFFFFFFF));
        }
        else
        {
            g.setColour(juce::Colour(0xFF808080));
        }
        g.drawText("PreQ", 27, 65 + tX, 25, 15, juce::Justification::centred, 0);

        //R4.00 GAIN
        if (makoProcessor.DefAmp_GainSetting[P + t] == 0)
        {
            g.setColour(juce::Colour(0xFFFFFFFF));            
        }
        else
        {
            g.setColour(juce::Colour(0xFF808080));            
        }
        g.drawText("Lo", 54, 65 + tX, 20, 15, juce::Justification::centred, 0);

        if (makoProcessor.DefAmp_GainSetting[P + t] == 1)
        {
            g.setColour(juce::Colour(0xFFFFFFFF));
        }
        else
        {
             g.setColour(juce::Colour(0xFF808080));             
        }        
        g.drawText("Med", 76, 65 + tX, 20, 15, juce::Justification::centred, 0);

        if (makoProcessor.DefAmp_GainSetting[P + t] == 2)
        {
            g.setColour(juce::Colour(0xFFFFFFFF));
        }
        else
        {
            g.setColour(juce::Colour(0xFF808080));            
        }        
        g.drawText("Hi", 98, 65 + tX, 20, 15, juce::Justification::centred, 0);
                

        //R4.00 IR FILE
        g.setColour(juce::Colour(0xFFFFFFFF));
        g.drawText(makoProcessor.DefAmp_File[P + t], 120, 67 + tX, 211, 15, juce::Justification::topLeft, 0);

    }
}

//R4.00 Draw the Speaker IR database editor.
void MakoDist01AudioProcessorEditor::DefIR_Draw(juce::Graphics& g)
{
    int tX;
    int P = DefIR_Page * 10;

    butPageUp.setVisible(true);
    butPageDn.setVisible(true);
    butDBase_Load.setVisible(true);
    butDBase_Save.setVisible(true);

    g.setColour(juce::Colour(0xFF000000));
    g.fillRect(5, 162, 563, 157);
    
    g.setColour(juce::Colour(0xFFFF8000));
    g.setFont(12.0f);
    g.drawText("IR/Speaker Database", 415, 65, 200, 15, juce::Justification::topLeft, 0);
    g.setColour(juce::Colour(0xFFFFFFFF));
    g.setFont(10.0f);
    g.drawText("Left Click cell to update.", 415, 80, 200, 15, juce::Justification::topLeft, 0);
    g.drawText("Right Click cell to delete.", 415, 95, 200, 15, juce::Justification::topLeft, 0);
    g.drawText("(Number, IR File)", 415, 110, 200, 15, juce::Justification::topLeft, 0);
    g.drawText("* IRs are fixed", 415, 125, 200, 15, juce::Justification::topLeft, 0);
    g.drawText("It is recommended to periodically", 415, 175, 200, 15, juce::Justification::topLeft, 0);
    g.drawText("backup the Amp/IR database.", 415, 190, 200, 15, juce::Justification::topLeft, 0);
    g.drawText("Backup Defs does all Amp and IRs.", 415, 205, 200, 15, juce::Justification::topLeft, 0);


    for (int t = 0; t < 10; t++)
    {
        tX = t * 15;

        g.setColour(juce::Colour(0xFFFF8000));
        g.fillRect(5, 65 + tX, 20, 13);

        g.setColour(juce::Colour(0xFF404040));
        g.fillRect(27, 65 + tX, 200, 13);

        g.setColour(juce::Colour(0xFFFFFFFF));

        //R4.00 Number
        g.drawText(std::to_string(P + t), 7, 67 + tX, 16, 15, juce::Justification::topRight, 0);

        //R4.00 IR FILE
        g.drawText(makoProcessor.DefIR_File[P + t], 29, 67 + tX, 196, 15, juce::Justification::topLeft, 0);
    }

}

//R4.00 Rebuild the speaker IR list box.
void MakoDist01AudioProcessorEditor::DefIR_UpdateList()
{
    std::string d = " ";
    
    cbIRList.clear();
    for (int t = 0; t < 200; t++)
    {
        d = std::to_string(t) + " - " + makoProcessor.DefIR_File[t].toStdString();
        cbIRList.addItem(d, t + 1);
    }
    cbIRList.setSelectedId(makoProcessor.makoIR + 1);

    DefIR_Update = false;
}

//R4.00 User selected a speaker IR to use.
void MakoDist01AudioProcessorEditor::cbIRListChanged()
{
    //R3.00 Handle user selections from IR LEFT dropdown list.
    int Idx = cbIRList.getSelectedId() - 1;
    makoProcessor.Setting[e_IR] = Idx;
    
    //R3.00 Is this a built-in amp or an external amp.
    if (19 < Idx)
    {
        if (makoProcessor.DefIR_Path[Idx] == "")
        {
            //R4.00 No IR defined for this position so set to clean full range amp.
            for (int t = 0; t < 1024; t++) makoProcessor.IR_FromFile[t] = 0.0f;
            makoProcessor.IR_FromFile[0] = 1.0f;
        }
        else
        {
            labHelp.setText("", juce::dontSendNotification); //R3.00 Clear any previous errors.

            //R3.00 Valid external database IR File selected.
            FILE_Mako_IR_Load_Wave(makoProcessor.DefIR_Path[Idx]);            
        }

        sldKnob[e_IRSize].setValue(.0f);
        sldKnob[e_IRVol].setValue(1.0f);
    }
    else
    {
        //R3.00 Force default IR Voice when coming from back from a file.
        if ((Idx < 20) && (19 < makoProcessor.Setting_Last[e_IR]))
        {            
            sldKnob[e_IRSize].setValue(.0f);
            sldKnob[e_IRVol].setValue(1.0f);
        }       
    }

    //R3.00 Flag processor we need to update values
    makoProcessor.SettingRqd[e_IR] = true;
    makoProcessor.makoSettingsChanged += 1;
}

//R4.00 User has sleected a new REVERB style.
void MakoDist01AudioProcessorEditor::cbRevModeChanged()
{
    int Sel = cbRevMode.getSelectedId();

    makoProcessor.Setting[e_RevMode] = Sel;

    //R3.10 Flag processor we need to update values
    makoProcessor.SettingRqd[e_RevMode] = true;
    makoProcessor.makoSettingsChanged += 1;
}

//R4.00 Draw the Pedal, Mod, Amp, etc On/Off light in the Signal Path area.
void MakoDist01AudioProcessorEditor::Pedals_Signal_Draw(juce::Graphics& g)
{   
    g.setColour(juce::Colour(0xFF00FF00));
    
    if (0.5f < makoProcessor.Setting[e_GateOn])  g.fillRect(21, 20, 10, 4);   //g.drawImageAt(img_pedal_on[1], rcPedal1.x1, rcPedal1.y1);
    if (0.5f < makoProcessor.Setting[e_CompOn])  g.fillRect(61, 20, 10, 4);   //g.drawImageAt(img_pedal_on[2], rcPedal2.x1, rcPedal2.y1);
    if (0.5f < makoProcessor.Setting[e_Dist1On]) g.fillRect(101, 20, 10, 4);   //g.drawImageAt(img_pedal_on[3], rcPedal3.x1, rcPedal3.y1);
    if (0.5f < makoProcessor.Setting[e_Dist2On]) g.fillRect(221, 20, 10, 4);   //g.drawImageAt(img_pedal_on[4], rcPedal4.x1, rcPedal4.y1);
    if (0.5f < makoProcessor.Setting[e_EQOn])    g.fillRect(381, 20, 10, 4);   //g.drawImageAt(img_pedal_on[5], rcPedal5.x1, rcPedal5.y1);
    
    if (makoProcessor.Setting[e_Mod1On]) g.fillRect(139, 20, 10, 4);  //g.drawImageAt(imgMod1_On, rcMod1.x1, rcMod1.y1);
    if (makoProcessor.Setting[e_Mod2On]) g.fillRect(179, 20, 10, 4);  //g.drawImageAt(imgMod2_On, rcMod2.x1, rcMod2.y1);
    if (makoProcessor.Setting[e_Mod3On]) g.fillRect(419, 20, 10, 4);  //g.drawImageAt(imgMod3_On, rcMod3.x1, rcMod3.y1);
    if (makoProcessor.Setting[e_Mod4On]) g.fillRect(459, 20, 10, 4);  //g.drawImageAt(imgMod4_On, rcMod4.x1, rcMod4.y1);    

    if (makoProcessor.Setting[e_AmpOn]) g.fillRect(281, 24, 10, 4);
    if (makoProcessor.Setting[e_IROn])  g.fillEllipse(335, 24, 10, 10);
   
    if (makoProcessor.Setting[e_DelOn]) g.fillRect(499, 20, 10, 4);  
    if (makoProcessor.Setting[e_RevOn]) g.fillRect(539, 20, 10, 4);  
}




//R4.00 Setup the Amp EQ based on selected style.
void MakoDist01AudioProcessorEditor::Mako_EQ_Update(bool VisOnly)
{
    int tV = int(makoProcessor.Setting[e_AmpEQMode]);

    if (!VisOnly)
    {
        makoProcessor.Setting[e_AmpEQMode] = tV;
        Mako_EQ_VolReset();
    }

    //R2.16 Fill in our EQ vars with the selected EQ setup.
    switch (tV)
    {
      case 0: Mako_EQ_Set_0(VisOnly); break;
      case 1: Mako_EQ_Set_1(VisOnly); break;
      case 2: Mako_EQ_Set_2(VisOnly); break;
      case 3: Mako_EQ_Set_3(VisOnly); break;
      case 4: Mako_EQ_Set_4(VisOnly); break;
      case 5: Mako_EQ_Set_5(VisOnly); break;
      case 6: Mako_EQ_Set_6(VisOnly); break;
      case 7: Mako_EQ_Set_7(VisOnly); break;
      case 8: Mako_EQ_Set_8(VisOnly); break;
      case 9: Mako_EQ_Set_9(VisOnly); break;
      case 10: Mako_EQ_Set_10(VisOnly); break;
      case 11: Mako_EQ_Set_11(VisOnly); break;
      case 12: Mako_EQ_Set_12(VisOnly); break;
      case 13: Mako_EQ_Set_13(VisOnly); break;
      case 14: Mako_EQ_Set_14(VisOnly); break;
      case 15: Mako_EQ_Set_15(VisOnly); break;
      case 16: Mako_EQ_Set_16(VisOnly); break;
      case 17: Mako_EQ_Set_17(VisOnly); break;
      case 18: Mako_EQ_Set_18(VisOnly); break;
      case 19: Mako_EQ_Set_19(VisOnly); break;
    }

    if (!VisOnly)
    {
        makoProcessor.SettingRqd[e_AmpEQ1] = true;
        makoProcessor.SettingRqd[e_AmpEQ2] = true;
        makoProcessor.SettingRqd[e_AmpEQ3] = true;
        makoProcessor.SettingRqd[e_AmpEQ4] = true;
        makoProcessor.SettingRqd[e_AmpEQ5] = true;
        makoProcessor.SettingRqd[e_AmpEQ6] = true;
        makoProcessor.makoSettingsChanged += 6;     //R2.00 Flag processor we need to update values
    }
    
}

//R4.00 Reset EQ Volumes when a new style is selected.
void MakoDist01AudioProcessorEditor::Mako_EQ_VolReset()
{
    sldKnob[e_AmpEQ1].setValue(.0f);
    sldKnob[e_AmpEQ2].setValue(.0f);
    sldKnob[e_AmpEQ3].setValue(.0f);
    sldKnob[e_AmpEQ4].setValue(.0f);
    sldKnob[e_AmpEQ5].setValue(.0f);
    sldKnob[e_AmpEQ6].setValue(.0f);
}

//R4.00 Setup EQ 0 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_0(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_0();
    
    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(false);
    }
}

//R4.00 Setup EQ 1 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_1(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_1();
    
    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(false);
    }
}

//R4.00 Setup EQ 2 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_2(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_2();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(false);
    }
}

//R4.00 Setup EQ 3 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_3(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_3();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(true);
    }
}

//R4.00 Setup EQ 4 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_4(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_4();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(false);
    }
}

//R4.00 Setup EQ 5 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_5(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_5();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(true);
    }
}

//R4.00 Setup EQ 6 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_6(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_6();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(true);
    }
}

//R4.00 Setup EQ 7 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_7(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_7();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(true);
    }
}

//R4.00 Setup EQ 8 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_8(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_8();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(true);
    }
}

//R4.00 Setup EQ 9 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_9(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_9();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(true);
    }
}

//R4.00 Setup EQ 10 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_10(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_10();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(true);
    }
}

//R4.00 Setup EQ 11 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_11(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_11();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(true);
    }
}

//R4.00 Setup EQ 12 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_12(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_12();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(true);
    }
}

//R4.00 Setup EQ 13 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_13(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_13();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(true);
    }
}

//R4.00 Setup EQ 14 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_14(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_14();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(true);
    }
}

//R4.00 Setup EQ 15 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_15(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_15();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(true);
    }
}

//R4.00 Setup EQ 16 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_16(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_16();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(true);
    }
}

//R4.00 Setup EQ 17 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_17(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_17();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(false);
        sldKnob[e_AmpEQ5].setVisible(false);
        sldKnob[e_AmpEQ6].setVisible(false);
    }
}

//R4.00 Setup EQ 18 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_18(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_18();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(true);
    }
}

//R4.00 Setup EQ 19 and draw EQ knobs.
void MakoDist01AudioProcessorEditor::Mako_EQ_Set_19(bool VisOnly)
{
    if (!VisOnly) makoProcessor.Mako_EQ_Set_19();

    if (editMode == EM_Amp)
    {
        sldKnob[e_AmpEQ1].setVisible(true);
        sldKnob[e_AmpEQ2].setVisible(true);
        sldKnob[e_AmpEQ3].setVisible(true);
        sldKnob[e_AmpEQ4].setVisible(true);
        sldKnob[e_AmpEQ5].setVisible(true);
        sldKnob[e_AmpEQ6].setVisible(true);
    }
}


//R4.00 Adjusting some oddball delay settings. Should be in Processor?
void MakoDist01AudioProcessorEditor::Mako_Update_Delay()
{
    //R2.00 Adjust the DELAY mix. 
    if (makoProcessor.Setting[e_DelMix] < .5f)
    {
        makoProcessor.Delay_Dry = 1.0f;
        makoProcessor.Delay_Wet = makoProcessor.Setting[e_DelMix] * 2;
    }
    else
    {
        makoProcessor.Delay_Dry = 1.0f - ((makoProcessor.Setting[e_DelMix] - .5f) * 2.0f);
        makoProcessor.Delay_Wet = 1.0f;
    }

    //R2.00 Adjust the DELAY balance. 
    if (makoProcessor.Setting[e_DelBal] < .5f)
    {
        makoProcessor.Delay_BalLR[0] = 1.0f;
        makoProcessor.Delay_BalLR[1] = makoProcessor.Setting[e_DelBal] * 2;
    }
    else
    {
        makoProcessor.Delay_BalLR[0] = 1.0f - ((makoProcessor.Setting[e_DelBal] - .5f) * 2.0f);
        makoProcessor.Delay_BalLR[1] = 1.0f;
    }
}

//R4.00 Adjusting some oddball reverb settings. Should be in Processor?
void MakoDist01AudioProcessorEditor::Mako_Update_Reverb()
{
    //R2.00 Adjust the Reverb mix. 
    if (makoProcessor.Setting[e_RevMix] < .5f)
    {
        makoProcessor.Reverb_Dry = 1.0f;
        makoProcessor.Reverb_Wet = makoProcessor.Setting[e_RevMix] * 2;
    }
    else
    {
        makoProcessor.Reverb_Dry = 1.0f - ((makoProcessor.Setting[e_RevMix] - .5f) * 2.0f);
        makoProcessor.Reverb_Wet = 1.0f;
    }

    //R2.00 Adjust the Reverb balance. 
    if (makoProcessor.Setting[e_RevBal] < .5f)
    {
        makoProcessor.Reverb_BalLR[0] = 1.0f;
        makoProcessor.Reverb_BalLR[1] = makoProcessor.Setting[e_RevBal] * 2;
    }
    else
    {
        makoProcessor.Reverb_BalLR[0] = 1.0f - ((makoProcessor.Setting[e_RevBal] - .5f) * 2.0f);
        makoProcessor.Reverb_BalLR[1] = 1.0f;
    }
}

//R4.00 Let the user select a speaker IR file for the database.
void MakoDist01AudioProcessorEditor::MAKO_IR_UpdateDef()
{
    juce::File Fail;
    Fail = juce::File(makoProcessor.IR_Path);

    //R4.00 If we have a valid path already, start the dialog there.
    if (Fail.existsAsFile())
        dlgFile = std::make_unique<juce::FileChooser>("Load speaker IR (1024 sample)", makoProcessor.IR_Path, "*.wav");
    else
        dlgFile = std::make_unique<juce::FileChooser>("Load speaker IR (1024 sample)", juce::File{}, "*.wav");
    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    //R4.00 Create a dialog box to get the filename. 
    dlgFile->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file != juce::File{})
            {
                FILE_Mako_IR_Load_Def(file.getFullPathName());
                DefIR_UpdateList();
            }
        });
}

//R4.00 Store the full speaker IR path and directory into the database.
void MakoDist01AudioProcessorEditor::FILE_Mako_IR_Load_Def(juce::String tFile)
{
    //R4.00 Store the full path and directory into the database.
    makoProcessor.DefIR_Path[DefIR_CurrentIdx] = tFile;
    makoProcessor.DefIR_File[DefIR_CurrentIdx] = makoProcessor.DefIR_Path[DefIR_CurrentIdx].fromLastOccurrenceOf("\\", false, true);

    repaint();
}

//R4.00 Load a proprietary Mako MiniD PRESET file.
//R4.00 This should be an XML file. Presented here as a text file for simplicity.
//R4.00 Adding new parameters will break existing files unless the new parm is added last.
void MakoDist01AudioProcessorEditor::FILE_Mako_Load(juce::String tFile)
{
    //R4.00 Try to start the dialog in the correct directory.
    SetupName_Current = tFile.fromLastOccurrenceOf("\\", false, true);
    SetupPath_Current = tFile;

    //R4.00 Load a MAKO mini D setup file.
    juce::FileInputStream Input(tFile);
    {
        Input.setPosition(0);

        //R4.00 read the file version.
        auto b = Input.readNextLine(); 
        
        //R4.00 Verify the FILE REVISION.
        int Frev = 200;
        if (b == "MiniD_0400") Frev = 400;
        if ((Frev < 400) || (400 < Frev))  //R4.00 Undefined file format.
        {
            labHelp.setText(std::string("ERROR: Unsupported/Invalid Preset file. MND rev 4 files only."), juce::dontSendNotification);
            return;          
        }
        
        //R4.00 Settings label.
        b = Input.readNextLine();      

        //R4.00 Read every parameter/slider value.
        for (int t = 0; t < e_PLast; t++)
        {
            b = Input.readNextLine(); makoProcessor.Setting[t] = b.getFloatValue();
        }
        b = Input.readNextLine();

        
        //R4.00 Apply the loaded settings to their LISTBOXES.
        cbAmpList.setSelectedId(makoProcessor.Setting[e_AmpChan] + 1);
        cbIRList.setSelectedId(makoProcessor.Setting[e_IR] + 1);
        
        //R4.00 Apply settings.
        for (int t = 0; t < e_PLast; t++) sldKnob[t].setValue(makoProcessor.Setting[t]);        
    }
}

//R4.00 Save a proprietary Mako MiniD PRESET file.
//R4.00 This should be an XML file. Presented here as a text file for simplicity.
//R4.00 Adding new parameters will break existing files unless the new parm is added last.
void MakoDist01AudioProcessorEditor::FILE_Mako_Save(juce::String tFile)
{   
    //R4.00 Try to start the dialog in the correct directory.
    SetupName_Current = tFile.fromLastOccurrenceOf("\\", false, true);
    SetupPath_Current = tFile;

    //R4.00 Save a MAKO mini D setup file.    
    juce::FileOutputStream output(tFile);
    {
        //R4.00 Overwrite any data.
        output.setPosition(0);
        output.truncate();

        //R4.00 Write Mako Mini D header.
        output.writeText("MiniD_0400\r\n", false, false, nullptr); 
        
        //R4.00 Save every parameter/slider value.
        output.writeText("SETTINGS\r\n", false, false, nullptr);
        for (int t = 0; t < e_PLast; t++)
        {
            output.writeText(std::to_string(makoProcessor.Setting[t]) + "\r\n", false, false, nullptr);
        }
        output.writeText("\r\n", false, false, nullptr);        
        
    }
}

//R4.00 Load an IR wave file and put the data in makoProcessor.IR_FromFile[] array.
void MakoDist01AudioProcessorEditor::FILE_Mako_IR_Load_Wave(juce::String tFile)
{
    float Vabs;
    float max = 0.0f;
    int maxIdx = 0;
    int maxSign = 1;
    int LIS = 0;

    //R2.15 Track the filename of this IR.
    makoProcessor.makoIR_Path = tFile;
    makoProcessor.makoIR_Name = makoProcessor.makoIR_Path.fromLastOccurrenceOf("\\", false, true);
    repaint();

    //R2.15 Setup JUCE variables to read the WAVE file.
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::File jFile = juce::File::File(tFile);
    formatManager.registerBasicFormats();
    auto fileBuffer = juce::AudioBuffer < float >::AudioBuffer();
    std::unique_ptr<juce::AudioFormatReader> reader (formatManager.createReaderFor(jFile));

    //R2.15 Read the WAVE file.
    if (reader.get() != nullptr)
    {
        //R4.0 0left here in case we want to check duration.
        //auto duration = (float)reader->lengthInSamples / reader->sampleRate;              
        //if (duration < 1)

        auto SampCount = (float)reader->lengthInSamples;
        if ((SampCount < 3000) && (reader->sampleRate < 50000))
        {
            fileBuffer.setSize((int)reader->numChannels, (int)reader->lengthInSamples);  
            reader->read(&fileBuffer, 0, (int)reader->lengthInSamples, 0, true, false); 

            LIS = (int)reader->lengthInSamples;
            if (1024 < LIS) LIS = 1024;           //R2.18 Limit to 1024 samples.

            //R3.00 Get the max val in case we need to normalize the IR down to 1.0 max.
            float tV;
            float VScale = 1.0f;
            for (int t = 0; t < LIS; t++)
            {
                tV = fileBuffer.getSample(0, t);
                Vabs = abs(tV);
                if (max < Vabs)
                {
                    max = Vabs;
                    maxIdx = t;

                    if (0.0f < tV)
                        maxSign = 1;
                    else
                        maxSign = -1;
                }
            }

            //R2.15 Check for IR scale requirements.
            if (1.0f < max)  VScale = 1.0f / max;

            //R2.15 Store the WAVE file data into our Temporary IR buffer.
            for (int t = 0; t < LIS; t++) makoProcessor.IR_FromFile[t] = VScale * fileBuffer.getSample(0, t);
            for (int t = LIS; t < 1024; t++) makoProcessor.IR_FromFile[t] = 0.0f;

            //R2.15 Force IR to update.
            //makoProcessor.makoIR_Last = 100;
            makoProcessor.makoSettingsChanged += 1;        //R2.00 Flag processor we need to update values

        }
        else
        {
            //R3.00 Handle File size issues.
            labHelp.setText(std::string("ERROR:Invalid IR (") + std::to_string(int(makoProcessor.makoIR)) + std::string(") (1024 samples~22mS@48kHz)."), juce::dontSendNotification);
            repaint();
        }

    }
    else
    {
        //R3.00 Handle File size issues.
        labHelp.setText(std::string("ERROR: Selected IR (") + std::to_string(int(makoProcessor.makoIR)) + std::string(") file missing or invalid."), juce::dontSendNotification);
        repaint();
    }

}

int MakoDist01AudioProcessorEditor::FILE_ClipInt(int val, int min, int max)
{
    int val2 = val;
    if (val < min) val2 = min;
    if (max < val) val2 = max;
    return val2;
}

float MakoDist01AudioProcessorEditor::FILE_ClipFloat(float val, float min, float max)
{
    float val2 = val;
    if (val < min) val2 = min;
    if (max < val) val2 = max;
    return val2;
}

//R4.00 This func lets us setup the ROTARY SLIDER objects a little easier.
void MakoDist01AudioProcessorEditor::GUI_Init_Large_Slider(juce::Slider* slider, float Val, float Vmin, float Vmax, float Vinterval, juce::String Suffix)
{
    //R1.00 Setup the slider edit parameters.
    slider->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    slider->setTextValueSuffix(Suffix);
    slider->setRange(Vmin, Vmax, Vinterval);
    slider->setValue(Val);
    slider->addListener(this);
    addAndMakeVisible(slider);
    slider->setVisible(false);

    //R1.00 Override the default Juce drawing routines and use ours.
    slider->setLookAndFeel(&makoLookAndFeel);

    //R1.00 Setup the type and colors for the sliders.
    slider->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider->setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xFFA0A0A0));
    slider->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xFF202020));
    slider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::textBoxHighlightColourId, juce::Colour(10, 100, 150));
    slider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0x00000000));    //R1.00 Make this SEE THRU. Alpha=0.
    slider->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0x00000000)); //R1.00 Using this to set different knob types. 0 = default.
    slider->setColour(juce::Slider::thumbColourId, juce::Colour(0xFF000000));
}

//R4.00 This func lets us setup the HORIZONTAL SLIDER objects a little easier.
void MakoDist01AudioProcessorEditor::GUI_Init_Small_Slider(juce::Slider* slider, float Val, float Vmin, float Vmax, float Vinterval, juce::String Suffix)
{
    //R1.00 Setup the slider edit parameters.
    slider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 70, 20);
    slider->setRange(Vmin, Vmax, Vinterval);
    slider->setTextValueSuffix(Suffix);
    slider->setValue(Val);
    slider->addListener(this);
    addAndMakeVisible(slider);
    slider->setVisible(false);

    //R1.00 Setup the type and colors for the sliders.
    slider->setSliderStyle(juce::Slider::LinearHorizontal);
    slider->setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xFFA0A0A0));
    slider->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xFF202020));
    slider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::textBoxHighlightColourId, juce::Colour(0xFF003860));
    slider->setColour(juce::Slider::backgroundColourId, juce::Colour(0xFF003860));
    slider->setColour(juce::Slider::thumbColourId, juce::Colour(0xFFF0F0F0));

    //R1.00 Override the default Juce drawing routines and use ours to draw switch images.
    slider->setLookAndFeel(&makoLookAndFeel);
    slider->setColour(juce::Slider::trackColourId, juce::Colour(0x00000000));
}

//R4.00 This func lets us setup the SWITCH objects a little easier.
void MakoDist01AudioProcessorEditor::GUI_Init_Switch_Slider(juce::Slider* slider, float Val, float Vmin, float Vmax, float Vinterval, juce::String Suffix)
{
    //R1.00 Setup the slider edit parameters.
    slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    slider->setRange(Vmin, Vmax, Vinterval);
    slider->setTextValueSuffix(Suffix);
    slider->setValue(Val);
    slider->addListener(this);
    addAndMakeVisible(slider);

    slider->setColour(juce::Slider::trackColourId, juce::Colour(0x00000001));

    //R1.00 Override the default Juce drawing routines and use ours to draw switch images.
    slider->setLookAndFeel(&makoLookAndFeel);
}

//R4.00 Define our knob positions so we can just loop thru and draw them all easily.
void MakoDist01AudioProcessorEditor::KNOB_DefinePosition(int idx, float x, float y, float sizex, float sizey, juce::String name, bool ForceBounds)
{    
    Knob_Pos[idx].x = x;
    Knob_Pos[idx].y = y;
    Knob_Pos[idx].sizex = sizex;
    Knob_Pos[idx].sizey = sizey;
    Knob_Name[idx] = name;

    sldKnob[idx].setTitle(name);

    if (ForceBounds) sldKnob[idx].setBounds(Knob_Pos[idx].x, Knob_Pos[idx].y, Knob_Pos[idx].sizex, Knob_Pos[idx].sizey);
}
