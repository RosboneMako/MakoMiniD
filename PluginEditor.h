/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct t_KnobCoors {
    float x;
    float y;
    float sizex;
    float sizey;
};

//R4.00 Create a new "Look and Feel" class based on Juces LnF class.
//R4.00 This is how to Draw your own controls.
class MakoLookAndFeel : public juce::LookAndFeel_V4
{
private:
    juce::Image imgSwitchOn;
    juce::Image imgSwitchOff;

    //R4.00 Ten tick mark angles around a slider.
    float TICK_Angle[11] = { 8.79645920, 8.29380417, 7.79114914, 7.28849411, 6.78583908, 6.28318405, 5.78052902, 5.27787399, 4.77521896, 4.27256393, 3.76 };
    float TICK_Cos[11] = {};
    float TICK_Sin[11] = {};

public:
    //R1.00 Let the user select a knob style.
    int MakoSliderKnobStyle = 3;
    float Kpts[32];
    juce::Path pathKnob;

    MakoLookAndFeel()
    {
        imgSwitchOn = juce::ImageCache::getFromMemory(BinaryData::tog_enable_on_png, BinaryData::tog_enable_on_pngSize);
        imgSwitchOff = juce::ImageCache::getFromMemory(BinaryData::tog_enable_off_png, BinaryData::tog_enable_off_pngSize);

        //R1.00 Do some PRECALC on Sin/Cos since they are expensive on CPU.
        for (int t = 0; t < 11; t++)
        {
            TICK_Cos[t] = std::cosf(TICK_Angle[t]);
            TICK_Sin[t] = std::sinf(TICK_Angle[t]);
        }

        //R1.00 Define the Path points to make a knob (Style 3).
        Kpts[0] = -2.65325243300477f;
        Kpts[1] = 8.60001462363607f;
        Kpts[2] = 0.0f;
        Kpts[3] = 10.0f;
        Kpts[4] = 2.65277678639377f;
        Kpts[5] = 8.60016135439157f;
        Kpts[6] = 7.81826556234706f;
        Kpts[7] = 6.23495979109873f;
        Kpts[8] = 8.3778301945593f;
        Kpts[9] = 3.28815468479365f;
        Kpts[10] = 9.74931428347318f;
        Kpts[11] = -2.22505528067641f;
        Kpts[12] = 7.79431009355225f;
        Kpts[13] = -4.4998589050713f;
        Kpts[14] = 4.3390509473009f;
        Kpts[15] = -9.00958583269659f;
        Kpts[16] = 1.34161181197136f;
        Kpts[17] = -8.89944255254108f;
        Kpts[18] = -4.33855264588318f;
        Kpts[19] = -9.00982579958681f;
        Kpts[20] = -6.12133095297134f;
        Kpts[21] = -6.59767439058605f;
        Kpts[22] = -9.74919120703023f;
        Kpts[23] = -2.22559448434896f;
        Kpts[24] = -8.97486228392824f;
        Kpts[25] = .672195644527914f;
        Kpts[26] = -7.81861038843018f;
        Kpts[27] = 6.23452737534543f;
        Kpts[28] = -5.07025014121689f;
        Kpts[29] = 7.4358969536627f;
        Kpts[30] = -2.65325243300477f;
        Kpts[31] = 8.60001462363607f;

        //R1.00 Create the actual PATH for our KNOB style 3.
        pathKnob.startNewSubPath(Kpts[0], Kpts[1]);
        for (int t = 0; t < 32; t += 2)
        {
            pathKnob.lineTo(Kpts[t], Kpts[t + 1]);
        }
        pathKnob.closeSubPath();

        //R1.00 Recreate our points with smoothed corners.
        pathKnob = pathKnob.createPathWithRoundedCorners(4.0f);
    }

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle, juce::Slider& sld) override
    {
        int tY = y + 4;
        int tY2;
        int tHeight = height - 8;
        int xPos = sliderPos;

        juce::ColourGradient ColGrad;
        juce::Colour C1 = sld.findColour(juce::Slider::trackColourId);

        if (C1 == juce::Colour(0x1))
        {
            //R1.00 Are using this func to draw Switches (On (1) / Off(0) only).
            if (sld.getValue() < .5f)
                g.drawImageAt(imgSwitchOff, x, y);
            else
                g.drawImageAt(imgSwitchOn, x, y);
        }
        else
        {
            int xOff = (xPos - x);

            //R4.00 Fill in the bar to our setting (Xoff).
            ColGrad = juce::ColourGradient(juce::Colour(0xFFFF8000), 0.0f, tY, juce::Colour(0xFF603000), 0.0f, tY + tHeight, false); 
            g.setGradientFill(ColGrad);
            g.fillRect(x, tY + 1, xOff, tHeight - 2);
            
            //R4.00 Fill in the remaining area.
            g.setColour(juce::Colour(0xFF000000));
            g.fillRect(x + xOff, tY + 1, width - xOff, tHeight - 2);

            //R4.00 Outline Option 2.
            g.setColour(juce::Colour(0xFF404040));
            g.drawLine(x - 1, tY, x + width, tY, 1.0f); //+1
            g.setColour(juce::Colour(0xFF606060));
            g.drawLine(x - 1, tY, x - 1, tY + tHeight, 1.0f);
            g.drawLine(x + width, tY, x + width, tY + tHeight, 1.0f);
            g.setColour(juce::Colour(0xFF808080));
            g.drawLine(x - 1, tY + tHeight, x + width, tY + tHeight, 1.0f);
            
            //R4.00 Tick Marks.
            g.setColour(juce::Colour(0xFFFFFFFF));
            tY2 = tY + tHeight + 1;
            g.drawLine(x, tY2, x, tY2 + 3);
            g.drawLine(x + (width * .25f), tY2, x + (width * .25f), tY2 + 1);
            g.drawLine(x + (width * .50f), tY2, x + (width * .50f), tY2 + 3);
            g.drawLine(x + (width * .75f), tY2, x + (width * .75f), tY2 + 1);            
            g.drawLine(x + width, tY2, x + width, tY2 + 3);

        }
    }

    //R1.00 Override the Juce SLIDER drawing function so our code gets called instead of Juces code.
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& sld) override
    {
        //R1.00 Most of these are from JUCE demo code. Could be reduced if not used.
        auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        //R1.00 Mako Var defs.
        float sinA;
        float cosA;
        juce::ColourGradient ColGrad;
        int col;

        //R1.00 Defs here for future use and to make the code easier to read.
        bool DrawKnob = true;
        bool DrawKnobEdge = true;
        bool DrawKnobNotches = false;
        bool DrawGuitarStyle = true;
        
        //R4.00 Using the outline color as knob style selector.
        juce::Colour C1 = sld.findColour(juce::Slider::rotarySliderOutlineColourId);
        int Cx = C1.getBlue();
        switch (Cx)
        {
        case 0:
            ColGrad = juce::ColourGradient(juce::Colour(0xFF808080), 0.0f, y, juce::Colour(0xFF202020), 0.0f, y + height, false);
            break;
        case 1:
        {
            ColGrad = juce::ColourGradient(juce::Colour(0xFF808080), 0.0f, y, juce::Colour(0xFF202020), 0.0f, y + height, false);
            DrawKnobEdge = true;
            DrawKnobNotches = true;
            DrawGuitarStyle = false;
            break;
        }
        case 2:
        {
            ColGrad = juce::ColourGradient(juce::Colour(0xFF808080), 0.0f, y, juce::Colour(0xFF202020), 0.0f, y + height, false);
            DrawKnobEdge = false;
            DrawKnobNotches = false;
            break;
        }
        case 3:
        {
            ColGrad = juce::ColourGradient(juce::Colour(0xFF0077CA), 0.0f, y, juce::Colour(0xFF003860), 0.0f, y + height, false);
            DrawKnobEdge = false;
            DrawKnobNotches = false;
            break;
        }
        case 4:
        {
            ColGrad = juce::ColourGradient(juce::Colour(0xFF3F8156), 0.0f, y, juce::Colour(0xFF2A563A), 0.0f, y + height, false);
            DrawKnobEdge = false;
            DrawKnobNotches = false;
            break;
        }
        case 5:
        {
            ColGrad = juce::ColourGradient(juce::Colour(0xFF600060), 0.0f, y, juce::Colour(0xFF400020), 0.0f, y + height, false);
            DrawKnobEdge = false;
            DrawKnobNotches = false;
            break;
        }
        }

        if (DrawKnob)
        {
            //1.00 Draw the KNOB face.
            g.setGradientFill(ColGrad);
            g.fillEllipse(rx, ry, rw, rw);
        }

        if (DrawKnobEdge)
        {
            //R1.00 Draw shading around knob face.
            ColGrad = juce::ColourGradient(juce::Colour(0xC0FFFFFF), 0.0f, y, juce::Colour(0xFF606060), 0.0f, y + height, false);
            g.setGradientFill(ColGrad);
            g.drawEllipse(rx - 1.0f, ry - 1.0f, rw + 2.0f, rw + 2.0f, 2.0f);

            ColGrad = juce::ColourGradient(juce::Colour(0xFF000000), 0.0f, y, juce::Colour(0xFF404040), 0.0f, y + height, false);
            g.setGradientFill(ColGrad);
            g.drawEllipse(rx, ry, rw, rw, 2.0f);
        }

        //R1.00 Draw guitar style knob.
        if (DrawGuitarStyle)
        {
            //R1.00 Copy our predefined KNOB PATH, scale it, and then transform it to the centre position.
            juce::Path p = pathKnob;
            p.applyTransform(juce::AffineTransform::scale(radius / 11.0f));
            p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

            ColGrad = juce::ColourGradient(juce::Colour(0xFFC0C0C0), 0.0f, y, juce::Colour(0xFF000000), 0.0f, y + height, false);
            g.setGradientFill(ColGrad);
            g.strokePath(p, juce::PathStrokeType(2.0f));
        }

        //R1.00 Dont draw anymore objects if the control is disabled.
        if (sld.isEnabled() == false) return;
                
        if (DrawKnobNotches)
        {
            //R4.00 Knob notches.
            ColGrad = juce::ColourGradient(juce::Colour(0xFFE0E0E0), 0.0f, y, juce::Colour(0xFF404040), 0.0f, y + height, false);
            g.setGradientFill(ColGrad);
            for (float ang = .7854f; ang < 6.0f; ang += .7854f)
            {
                sinA = std::sinf(ang + angle);
                cosA = std::cosf(ang + angle);
                g.drawLine(centreX + (sinA * radius * .9f), centreY - (cosA * radius * .9f), centreX + sinA * radius, centreY - cosA * radius, 2.0f);
            }
        }

        //R4.00 Draw setting marks.
        g.setColour(juce::Colour(0xFFFFFFFF));
        sinA = TICK_Sin[0] * radius; cosA = TICK_Cos[0] * radius; g.drawLine(centreX + (sinA * 1.3f), centreY - (cosA * 1.3f), centreX + sinA * 1.15f, centreY - cosA * 1.15f, 1.0f);
        //sinA = TICK_Sin[5] * radius; cosA = TICK_Cos[5] * radius; g.drawLine(centreX + (sinA * 1.3f), centreY - (cosA * 1.3f), centreX + sinA * 1.15f, centreY - cosA * 1.15f, 1.0f);
        sinA = TICK_Sin[10] * radius; cosA = TICK_Cos[10] * radius; g.drawLine(centreX + (sinA * 1.3f), centreY - (cosA * 1.3f), centreX + sinA * 1.15f, centreY - cosA * 1.15f, 1.0f);
        
        //R1.00 JUCE version of KNOB position indicator (pointer).
        //R1.00 This method may be faster and is better for drawing weird shapes.
        juce::Path p;
        auto pointerLength = radius * 0.23f;
        auto pointerThickness = 6.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

        // pointer
        g.setColour(juce::Colour(0xFFFF8000));
        g.fillPath(p);

        
    }
};


//==============================================================================
/**
*/
class MakoDist01AudioProcessorEditor  : public juce::AudioProcessorEditor,  public juce::Button::Listener, public juce::Slider::Listener, public juce::Timer //, public juce::MouseListener
                                        
{
public:
    MakoDist01AudioProcessorEditor (MakoDist01AudioProcessor&);
    ~MakoDist01AudioProcessorEditor() override;
    
    //R1.00 OUR override functions.
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* butt) override;

    //R3.10 Define the Look and Feel class we created.
    MakoLookAndFeel makoLookAndFeel;

    void timerCallback() override;
    bool MOUSEHACK_IsDown = false;
    int MOUSE_Down = 0;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    
private:
    juce::uint32 colorButton = 0xFFE0E0E0;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MakoDist01AudioProcessor& makoProcessor;
    
    bool FLAG_ClearHelp = true;
    void VU_Meter_Update_Juce();

    void mouseDown(const juce::MouseEvent&) override;
    //void mouseDrag(const juce::MouseEvent&) override;
    //void mouseMove(const juce::MouseEvent&) override;
    //void mouseUp(const juce::MouseEvent&) override;
    
    void Paint_Ped1(juce::Graphics& g);
    void Paint_Ped2(juce::Graphics& g);
    void Paint_Ped3(juce::Graphics& g);
    void Paint_Ped4(juce::Graphics& g);
    void Paint_Ped5(juce::Graphics& g);
    void Paint_Mod1(juce::Graphics& g);
    void Paint_Mod2(juce::Graphics& g);
    void Paint_Mod3(juce::Graphics& g);
    void Paint_Mod4(juce::Graphics& g);
    void Paint_Amp(juce::Graphics& g);
    void Paint_IR(juce::Graphics& g);
    void Paint_Delay(juce::Graphics& g);
    void Paint_Reverb(juce::Graphics& g);
    void Paint_Back(juce::Graphics& g, juce::Colour col);
    void Paint_Back2(juce::Graphics& g, juce::Colour colTop, juce::Colour colBot);
    void Paint_VertDiv(juce::Graphics& g, int x, int yStart, int yStop);

    int ctrlHelp = 0;
    int ctrlHelpLast = 0;
        
    juce::Label labHelp;
    juce::Label labValue;  //R4.00 Added for Pedal/Mod values.
    juce::Label labSuffix; //R4.00 Added for Pedal/Mod values.

    juce::Label labVersion;
    
    juce::Image imgBack;
    juce::Image imgLogGraph;
    
    //R1.00 Define our UI Juce Slider controls.
    juce::Slider sldKnob[120];
    
    //R4.00 Future use for different languages.
    //int sldKnob_Help[120];      //R4.00 index into the Help String array. Indexes changed for pedals and Mods.

    //R4.00 This enum makes it easier to code Knobs and Settings.
    //R4.00 This Enum is also defined in the Processor and must match.
    //R4.00 Any new indexes should be added at the end or it will break any local files saved.
    //R4.00 New indexes will break the Help Strings below.
    enum enKnobIndexes {
        e_AmpOn, e_AmpMode, e_AmpQual, e_AmpGain, e_AmpChan, e_AmpLP, e_AmpComp, e_AmpCRatio, e_AmpThin, e_AmpSlope, e_AmpThump, e_AmpAir, e_AmpBoom, e_AmpCrisp, e_AmpPower, e_AmpClean, e_AmpVol, e_AmpEQMode, e_AmpEQ1, e_AmpEQ2, e_AmpEQ3, e_AmpEQ4, e_AmpEQ5, e_AmpEQ6,
        e_IROn, e_IR, e_IRSize, e_IRVol,
        e_DelOn, e_DelTime, e_DelMix, e_DelOffset, e_DelRepeat, e_DelBal, e_DelDuck,
        e_RevOn, e_RevMode, e_RevMix, e_RevTime, e_RevBal, e_RevChor, e_RevSize, e_RevLP, e_RevHP, e_RevPreD, e_RevDuck, e_RevPar,

        e_GateOn, e_GateVol, e_GateExp, e_GateAvg, e_GateFreq, e_GateWah, e_GateWahQ, e_GateWahRange,
        e_CompOn, e_CompVol, e_CompAtt, e_CompRel, e_CompThr, e_CompRat, e_CompDrv,
        e_Dist1On, e_Dist1Vol, e_Dist1MidF, e_Dist1Mid, e_Dist1MidQ, e_Dist1Cut, e_Dist1Drv,
        e_Mod1On, e_Mod1Vol, e_Mod1Rate, e_Mod1Depth, e_Mod1Mix, e_Mod1Tone, e_Mod1Synth, e_Mod1Bronz,
        e_Mod2On, e_Mod2Vol, e_Mod2Rate, e_Mod2Range, e_Mod2FStart, e_Mod2Mix, 
        e_Dist2On, e_Dist2Vol, e_Dist2MidF, e_Dist2Mid, e_Dist2MidQ, e_Dist2Cut, e_Dist2Drv,
        e_EQOn, e_EQVol, e_EQF1, e_EQF2, e_EQF3, e_EQF4, e_EQF5, e_EQF6,
        e_Mod3On, e_Mod3Vol, e_Mod3Rate, e_Mod3Depth, e_Mod3Mix, e_Mod3Tone, e_Mod3Bal,
        e_Mod4On, e_Mod4Vol, e_Mod4Foff, e_Mod4Doff, e_Mod4Mix, e_Mod4Tone, e_Mod4Bal,

        e_PLast, 
    };

    void GUI_Init_Large_Slider(juce::Slider* slider, float Val, float Vmin, float Vmax, float Vinterval, juce::String Suffix);
    void GUI_Init_Small_Slider(juce::Slider* slider, float Val, float Vmin, float Vmax, float Vinterval, juce::String Suffix);
    void GUI_Init_Switch_Slider(juce::Slider* slider, float Val, float Vmin, float Vmax, float Vinterval, juce::String Suffix);

    //R1.00 Define the coords and text for our knobs. Not JUCE related. 
    t_KnobCoors Knob_Pos[120] = {};
    juce::String Knob_Name[120] = {};
    void KNOB_DefinePosition(int idx, float x, float y, float sizex, float sizey, juce::String name, bool ForceBounds);

    //R2.00 EDIT MODE enums. FreqTests must be last.
    enum enEditMode   : int { EM_None, EM_DefAmps, EM_DefIRs, EM_Ped1, EM_Ped2, EM_Ped3, EM_Ped4, EM_Ped5, EM_Mod1, EM_Mod2, EM_Mod3, EM_Mod4, EM_Amp, EM_IR, EM_Delay, EM_Reverb, EM_Load, EM_Save   };

    //R4.00 Struct and position defs for mouse click checks in signal path.
    struct makoRect
    {
        int x1;
        int y1;
        int x2;
        int y2;
    };

    //R4.00 Set some screen positions on the SIGNAL PATH.
    makoRect rcPedal1 = {   10, 6,  35, 38 };
    makoRect rcPedal2 = {   50, 6,  75, 38 };
    makoRect rcPedal3 = {   90, 6, 115, 38 };
    makoRect rcPedal4 = {  210, 6, 235, 38 };
    makoRect rcPedal5 = {  370, 6, 398, 38 };
    makoRect rcMod1   = {  128, 6, 160, 38 };
    makoRect rcMod2   = {  168, 6, 200, 38 };
    makoRect rcAmp    = {  250, 6, 320, 38 };
    makoRect rcIR     = {  330, 6, 360, 38 };
    makoRect rcMod3   = {  408, 6, 440, 38 };
    makoRect rcMod4   = {  450, 6, 482, 38 };
    makoRect rcDelay  = {  490, 6, 520, 38 };
    makoRect rcReverb = {  530, 6, 560, 38 };
     

    //R4.00 EDIT MODE
    //R4.00 Track which block we are editing.
    int editMode = 0;
    int editModeLast = 0;

    //R4.00 Check for mouse clicks in Sig Path and Setting areas that do not use Juce controls. 
    void Mouse_Check_Pedal(int button, int idx, int editmode);
    void Mouse_Check_Amp(int button);
    void Mouse_Check_IR(int button);
    void Mouse_Check_Mod1(int button);
    void Mouse_Check_Mod2(int button);
    void Mouse_Check_Mod3(int button);
    void Mouse_Check_Mod4(int button);
    void Mouse_Check_Del(int button);
    void Mouse_Check_Rev(int button);
    void Mouse_Check_DefAmp(int mButton, int posX, int posY);  //R3.00 Added.
    void Mouse_Check_DefIR(int mButton, int posX, int posY);   //R3.00 Added.


    //R4.00 AMPLIFIER Database Editing.
    juce::ComboBox cbAmpList;
    bool DefAmp_Update = true;
    void DefAmp_UpdateList();
    void cbAmpListChanged();
    int DefAmp_Page = 0;

    void FILE_DBase_Load(juce::String tFile);
    void FILE_DBase_Save(juce::String tFile);

    //R4.00 IMPULSE RESPONSE Database Editing.
    juce::ComboBox cbIRList;
    int DefIR_Page = 0;
    void DefAmp_Draw(juce::Graphics& g);
    void DefIR_Draw(juce::Graphics& g);
    bool DefIR_Update = true;
    void DefIR_UpdateList();
    void cbIRListChanged();
    

    //R4.00 Reverb functions.
    juce::ComboBox cbRevMode;
    void cbRevModeChanged();

    int DBG_Int1 = 0;
    
    void Pedals_Signal_Draw(juce::Graphics& g);
    void Mako_UpdateVals(bool FlagProcessor);
        
    //*************************************************************************************
    //R4.00 AMP EQ MODES
    //*************************************************************************************
    //R4.00 These string defs get put into str_EQ. These are just here to make defining them easier.
    std::string str_EQ_mode0[8] = { "150 Hz", "500 Hz", "1.2 kHz", "2.5 kHz", "5.5 kHz", "", "", "" };
    std::string str_EQ_mode1[8] = { "100 Hz", "330 Hz", "700 Hz", "1.8 kHz", "5 kHz", "", "", "" };
    std::string str_EQ_mode2[8] = { "80 Hz", "220 Hz", "750 Hz", "2.2 kHz", "6.6 kHz", "", "", "" };
    std::string str_EQ_mode3[8] = { "120 Hz", "600 Hz", "2 kHz", "10 kHz", "Deep", "Harsh", "", "" };
    std::string str_EQ_mode4[8] = { "100 Hz", "400 Hz", "1 kHz", "2.5 kHz", "6.6 kHz", "", "", "" };
    std::string str_EQ_mode5[8] = { "50 Hz", "120 Hz", "240 Hz", "500 Hz", "1 kHz", "2.2 kHz", "", "" };
    std::string str_EQ_mode6[8] = { "80 Hz", "300 Hz", "900 Hz", "1.5 kHz", "3 kHz", "6 kHz", "", "" };
    std::string str_EQ_mode7[8] = { "100 Hz", "400 Hz", "1 kHz", "2 kHz", "4 kHz", "8 kHz", "", "" };
    std::string str_EQ_mode8[8] = { "80 Hz", "400 Hz", "750 Hz", "1.5 kHz", "3 kHz", "6 kHz", "", "" };
    std::string str_EQ_mode9[8] = { "80 Hz", "200 Hz", "400 Hz", "800 Hz", "1.6 kHz", "3.2 kHz", "", "" };
    std::string str_EQ_mode10[8] = { "100 Hz", "450 Hz", "700 Hz", "900 Hz", "1.5 kHz", "3 kHz", "", "" };
    std::string str_EQ_mode11[8] = { "80 Hz", "180 Hz", "600 Hz", "1.2 kHz", "2 kHz", "4 kHz", "", "" };
    std::string str_EQ_mode12[8] = { "120 Hz", "450 Hz", "800 Hz", "1 kHz", "Harsh", "2.2 kHz", "", "" };
    std::string str_EQ_mode13[8] = { "100 Hz", "200 Hz", "400 Hz", "800 Hz", "1.6 kHz", "3.2 kHz", "", "" };
    std::string str_EQ_mode14[8] = { "80 Hz", "500 Hz", "900 Hz", "1.4 kHz", "2.5 kHz", "5 kHz", "", "" };
    std::string str_EQ_mode15[8] = { "200 Hz", "700 Hz", "1.2 kHz", "1.9 kHz", "3 kHz", "8 kHz", "", "" };
    std::string str_EQ_mode16[8] = { "50 Hz", "120 Hz", "360 Hz", "600 Hz", "900 Hz", "4 kHz", "", "" };
    std::string str_EQ_mode17[8] = { "Fen Bass", "Fen Mid", "Fen Treb", "", "", "", "", "" };
    std::string str_EQ_mode18[8] = { "Bass", "240 Hz", "660 Hz", "1.2 kHz", "2.4 kHz", "Treble", "", "" };
    std::string str_EQ_mode19[8] = { "100 Hz", "350 Hz", "700 Hz", "1.4 kHz", "2.8 kHz", "10 kHz", "", "" };
    std::string str_EQ[21][6] = {};  //R4.00 Holds the actual string printed on screen.
    
    void Mako_EQ_Update(bool VisOnly);
    void Mako_EQ_VolReset();
    void Mako_EQ_Set_0(bool VisOnly);
    void Mako_EQ_Set_1(bool VisOnly);
    void Mako_EQ_Set_2(bool VisOnly);
    void Mako_EQ_Set_3(bool VisOnly);
    void Mako_EQ_Set_4(bool VisOnly);
    void Mako_EQ_Set_5(bool VisOnly);
    void Mako_EQ_Set_6(bool VisOnly);
    void Mako_EQ_Set_7(bool VisOnly);
    void Mako_EQ_Set_8(bool VisOnly);
    void Mako_EQ_Set_9(bool VisOnly);
    void Mako_EQ_Set_10(bool VisOnly);
    void Mako_EQ_Set_11(bool VisOnly);
    void Mako_EQ_Set_12(bool VisOnly);
    void Mako_EQ_Set_13(bool VisOnly);
    void Mako_EQ_Set_14(bool VisOnly);
    void Mako_EQ_Set_15(bool VisOnly);
    void Mako_EQ_Set_16(bool VisOnly);
    void Mako_EQ_Set_17(bool VisOnly);
    void Mako_EQ_Set_18(bool VisOnly);
    void Mako_EQ_Set_19(bool VisOnly);


    //R4.00 Final effects.
    void Mako_Update_Delay();
    void Mako_Update_Reverb();
        
    //REVERB COUNT
    juce::String Reverb_Title[20] = {"0 Basic","1 Slap Smear","2 Haunted","3 Dreamy","4 Club","5 Darkness","6 Medium Room", "7 Small Room", "8 Box", "9 DigiVerb", "10 Space", "11 Garage","12 Fused Echo","13 Medium Tile" ,"14 Arena" ,"15 Garage 2" ,"16 Inverted" ,"17 Hangar" ,"18 Inverted 2" ,"19 Regent" }; 

    //R4.00 Compressor Trigger Lights.
    juce::Label labAmp_Comp;
    juce::Label labPedal2_Comp;

    
    //R4.00 VU Meter segment tracking.
    juce::Label labVU_Out1;
    juce::Label labVU_Out2;
    juce::Label labVU_Out3;
    juce::Label labVU_Out4;
    juce::Label labVU_Out5;
    juce::Label labVU_Out6;
    
    void VU_Off();
    juce::Label labVU_In1;
    juce::Label labVU_In2;
    juce::Label labVU_In3;
    juce::Label labVU_In4;
    juce::Label labVU_In5;
    juce::Label labVU_In6;
    
    //*********************************************************
    //R1.00 Setup buttons and variables.
    //*********************************************************
    juce::TextButton butPresetLoad;  //R4.00 load a local preset.
    juce::TextButton butPresetSave;  //R4.00 Save a local preset.
    juce::TextButton butVUHide;      //R4.00 Stop updating VU meters to save CPU.
    juce::TextButton butDefAmps;     //R4.00 Load the Amp database editor.
    juce::TextButton butDefIRs;      //R4.00 Load the IR database editor.
    juce::TextButton butPageUp;      //R4.00 Paging button for Amp/Ir editor.
    juce::TextButton butPageDn;      //R4.00 Paging button for Amp/Ir editor.
    juce::TextButton butGetDefIR;    //R4.00 Load the Speaker IR slot that matches the current Amp slot.
    juce::TextButton butAmp4x;       //R4.20 Toggle 4x oversampling.
    juce::TextButton butHelp;        //R4.00 Toggle the help text on/off. Text gets crazy when multiple knobs are automated.
    bool Help_Show = true;

    //R4.00 Database backup and restore.
    juce::TextButton butDBase_Load;
    juce::TextButton butDBase_Save;

    int Num_Normalize(float V);

    
    //R2.23 Added amp Loading.
    void FILE_Mako_Amp_Load_Wave(juce::String tFile);
    int DefAmp_CurrentIdx = 0;
    int DefAmp_Editing = 0;                             //R3.00 Edit mode. 0-Not editing, 1-Editing, 2-Data needs saved.
    void MAKO_Amp_UpdateDef();
    void FILE_Mako_Amp_Load_Def(juce::String tFile);

    int DefIR_CurrentIdx = 0;
    void MAKO_IR_UpdateDef();
    void FILE_Mako_IR_Load_Def(juce::String tFile);

    void FILE_Mako_Load(juce::String tFile);
    void FILE_Mako_Save(juce::String tFile);
    void FILE_Mako_IR_Load_Wave(juce::String tFile);
    float FILE_ClipFloat(float val, float min, float max);
    int FILE_ClipInt(int val, int min, int max);
    

    //R4.00 This is a start at making it possible to use various languages.
    //R4.00 Need strings for each control and its help also.
    std::string LangStringHelp[200] =
    {   
        //R4.00 AMPLIFIER
        "", //On/Off
        "AMP MODE: Mono/Stereo Operation setting.",                                    //1
        "AMP QUALITY: Adjust amp IR resolution. 0-Less CPU (128s), 3-more CPU (1024s).",
        "AMP GAIN: Adjust the volume/distortion into the gain stage. Use Power for cleaner amps.",        
        "AMP CHANNEL: Select an amplifier model to play.",                                         
        "AMP LOW PASS: Reduce high freqs after the gain stages. Clean up fizz and thicken.",                                  
        "AMP COMP: Add post amp compression. Comp sets the threshold level. (1=Off).",
        "AMP COMP RATIO: How much to compress signal when triggered. Lower is more.",
        "AMP THIN: Apply compression based on how hard amp clipping occurs. (0=off)",
        "AMP SLOPE: Mix between hard and soft (HT) clipping. 0-Hard 1-Soft",                       
        "AMP THUMP: Add a distorted bass boost.",                                                  
        "AMP AIR: Add a distorted high freq boost.",                                               
        "AMP BOOM: Reduce lows entering the amp gain section. Lower only.",                        
        "CRISP: Reduce highs entering the amp gain section. Lower only.",                          
        "AMP POWER: Post gain distortion. Acts as power amp. Great for cleans.",
        "AMP CLEAN: Adjust the clean (unamplified) signal volume.", 
        "AMP VOLUME: Adjust the final volume of the signal.",
        "AMP EQ STYLE: Select from several different EQ styles.",
        "AMP EQ1: Adjust the amplifiers frequency response.",
        "AMP EQ2: Adjust the amplifiers frequency response.",
        "AMP EQ3: Adjust the amplifiers frequency response.",
        "AMP EQ4: Adjust the amplifiers frequency response.",
        "AMP EQ5: Adjust the amplifiers frequency response.",
        "AMP EQ6: Adjust the amplifiers frequency response.",
                
        //R4.00 IMPULSE RESPONSE
        "", //IR On
        "IR: Select the IR to use for the left channel.",         //25     
        "IR Size: Resample the IR to alter the apparent speaker size.",
        "IR VOL: Adjust the volume for this channel. Negative values invert R phase.",
     
        //R4.00 DELAY
        "", //On
        "DELAY TIME : Adjust the amount of time between echoes.",                          //29
        "DELAY MIX: Adjust the blend amount between the clean and delayed signal." ,
        "DELAY OFFSET: Adjust the time diff between L and R delays.",
        "DELAY REPEATS: Adjust how many echoes will be recreated.",
        "DELAY BAL: Adjust the Left/Right balance of the effect.",
        "DELAY DUCK: Reduce the effect volume when playing.",         

        //R4.00 REVERB
        "", 
        "REVERB MODE: Adjust the mode/style of reverb to apply.",                         //36
        "REVERB MIX: Adjust the blend amount between the clean and delayed signal." ,    
        "REVERB TIME/DECAY: Adjust how long the reverb echoes will persist.",            
        "REVERB BAL: Adjust the Left/Right balance of the effect.",                      
        "REVERB CHORUS: Adjust the amount of chorus effect to apply.",                   
        "REVERB SIZE: Adjust the reverb chamber room size.",
        "REVERB HIGH CUT: Reduce the reverb high frequency response.",
        "REVERB LOW CUT: Reduce the reverb low frequency response.",
        "REVERB PREDELAY: Time before the fist sound reflections occur.",
        "REVERB DUCK: Reduce the effect volume when playing.",
        "REV-DLY PARALLEL: When set Del and Rev are put in parallel instead of series.",

        //R4.00 Gate
        "", //On
        "VOLUME: Add or remove volume.", //47
        "EXPAND: Adjust this to force quiet notes off. Use AVG and FREQ first.",
        "AVERAGE: Adjust volume based on average signal level.",
        "FREQUENCY: Adjust a high cut filter based on average signal level.",
        "AUTO WAH: Adjust how much WAH sound should be heard. Setting of 0 equals off.",
        "WAH Q: Set how much WAH sound there will be.",
        "WAH RANGE: Adjusts the ratio of guitar signal and high frequency sweep. Lower is better.",

        //R4.00 Comp
        "", //On
        "VOLUME: Add or remove volume",  //54
        "ATTACK: Set how fast the compressor turns on.",
        "RELEASE: Set how long it takes to turn off the compressor.",
        "THRESHOLD: Signal that exceeds this level will be compressed. Indicator lights when triggered.",
        "RATIO: How much to compress. Lower is more. Values between .2 (5:1) and .5 (2:1) are a good start.",
        "DRIVE: Add some drive and volume to the compressed signal.",

        //R4.00 Dist 1
        "", //On
        "VOLUME: Add or remove volume",   //61
        "MID FREQ: Set the frequency to base our sound on.",
        "MID GAIN: Set how much of that frequency to add/remove.",     //R4.20 Changed.
        "MID Q: Set the amount a wah sound. More helps flubby bass.",
        "HIGH CUT: Remove some of the harsh distortion harmonics.",
        "DRIVE: Add some drive and volume.",

        //R4.00 FLANGER
        "", //On
        "VOLUME: Add or remove volume",  //68
        "RATE: Set how fast to cycle the flange effect.",
        "DEPTH: Set how long to delay the flange echoes.",
        "MIX: Set the mix of clean and effect. Best at 50%.",
        "TONE: Remove some high freqs from the effected signal.",
        "SYNTH: Synth adjusts how much to synthesize the signal. Touch sensitive effect.",
        "BRONZ: Bronz adjusts the mix of signal and bronz (rectified) effect. Gain before effect is good.",
        
        //R4.00 MOD FILTER
        "", //On
        "VOLUME: Add or remove volume",  //74
        "RATE: Set how fast to cycle the effect.",
        "RANGE: How many freqs to sweep thru for each cycle.",
        "FREQ START: What is the lowest freq to sweep.",
        "MIX: Set the mix of clean and effect. Best at 100%.",
        
        //R4.00 Dist 2
        "", //On
        "VOLUME: Add or remove volume",     //80
        "MID FREQ: Set the frequency to base our sound on.",
        "MID GAIN: Set how much of that frequency to  add/remove.",     //R4.20 Changed.
        "MID Q: Set the amount a wah sound. More helps flubby bass.",
        "HIGH CUT: Remove some of the harsh distortion harmonics.",
        "DRIVE: Add some drive and volume.",

        //R4.00 Equalizer
        "", //On
        "VOLUME: Add or remove volume",     //87
        "FREQ 1: Add or remove this frequency.",
        "FREQ 2: Add or remove this frequency.",
        "FREQ 3: Add or remove this frequency.",
        "FREQ 4: Add or remove this frequency.",
        "FREQ 5: Add or remove this frequency.",
        "FREQ 6: Add or remove this frequency.",

        //R4.00 CHORUS
        "", //On
        "VOLUME: Add or remove volume",  //95
        "RATE: Set how fast to cycle the effect.",
        "DEPTH: Set how long to delay the echoes.",
        "MIX: Set the mix of clean and effect. Best at 50%.",
        "TONE: Remove some high freqs from the effected signal.",
        "BALANCE: Push the effect signal to the left or right channels.",

        //R4.00 WIDE LOAD
        "", //On
        "VOLUME: Add or remove volume",  //102
        "FREQ OFFSET: This effect pushes mids in one chan and removes in the other.",
        "DELAY OFFSET: This effect sets the delay time for the L/R delays.",
        "MIX: Set the mix of clean and effect. Set to 100% for Haas Pan.",
        "TONE: Remove some high freqs from the effected signal.",
        "BALANCE: Push the effect signal to the left or right channels.",

    };

    //R4.00 Some file work vars.
    juce::String SetupName_Current = "";
    juce::String SetupPath_Current = "";
    std::unique_ptr<juce::FileChooser> dlgFile;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MakoDist01AudioProcessorEditor)


public:
    
    //R1.00 Define our SLIDER attachment variables.
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt[120]; 

    //R4.00 COMBO BOX Attachments.
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> ParAmpChan;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> ParIRL;    
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> ParRevMode;


};

