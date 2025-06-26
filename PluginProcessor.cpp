/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "cmath" 
#include "Windows.h"

//R1.00 DO NOT DECLARE ANY GLOBAL VARIABLES
//R1.00 This may cause major issues when multiple instances are used in DAWs.

//==============================================================================
MakoDist01AudioProcessor::MakoDist01AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    
    //R4.00 Added for VALUE TREE.
    //R4.00 Define the VST parameters. These are the values that get changed by the DAW (Parameter ID).
    //R4.00 These are connected to the GUI controls in the editor with the Parameter Attachment vars.
    parameters (*this, nullptr, "PARAMETERS",
    {       
        std::make_unique<juce::AudioParameterInt>("amp_on","Amp On", 0, 1, 0),
        std::make_unique<juce::AudioParameterInt>("amp_qual","Amp Qual", 0, 3, 3),
        std::make_unique<juce::AudioParameterFloat>("amp_gain","Amp Gain", 0, 1.0f, .3f),
        std::make_unique<juce::AudioParameterInt>("amp_chan","Amp Channel", 0, 199, 1),
        std::make_unique<juce::AudioParameterInt>("amp_lp","Amp Low Pass", 2000, 12000, 12000),
        std::make_unique<juce::AudioParameterFloat>("amp_comp","Amp Comp", 0, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("amp_cratio","Amp Comp Ratio", 0, 1.0f, .3f),
        std::make_unique<juce::AudioParameterFloat>("amp_thin","Amp Thin", .0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("amp_slope","Amp Slope", .0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("amp_thump","Amp Thump", .0f, 1.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("amp_air","Amp Air", .0f, 1.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("amp_boom","Amp Boom", .0f, 1.0f, .5f),
        std::make_unique<juce::AudioParameterFloat>("amp_crisp","Amp Crisp", .0f, 1.0f, .2f),
        std::make_unique<juce::AudioParameterFloat>("amp_power","Amp Power", .0f, 1.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("amp_clean","Amp Clean Mix", .0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("amp_vol","Amp Volume", .0f, 1.0f, .3f),
        std::make_unique<juce::AudioParameterInt>("amp_eqmode","Amp EQ Mode", 0, 19, 0),
        std::make_unique<juce::AudioParameterFloat>("amp_eq1","Amp EQ 1", -1.0f, 1.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("amp_eq2","Amp EQ 2", -1.0f, 1.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("amp_eq3","Amp EQ 3", -1.0f, 1.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("amp_eq4","Amp EQ 4", -1.0f, 1.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("amp_eq5","Amp EQ 5", -1.0f, 1.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("amp_eq6","Amp EQ 6", -1.0f, 1.0f, .0f),

        std::make_unique<juce::AudioParameterInt>("ir_on","IR On", 0, 1, 0),
        std::make_unique<juce::AudioParameterInt>("ir","IR", 0, 199, 2),
        std::make_unique<juce::AudioParameterFloat>("ir_size","IR Size", -1.0f, 1.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("ir_vol","IR Volume", 0.0f, 1.0f, 1.0f),
        
        std::make_unique<juce::AudioParameterInt>("mod1_on","Mod 1 On", 0, 1, 0),
        std::make_unique<juce::AudioParameterFloat>("mod1_rate","Mod 1 Rate", .0f, 1.0f, .1f),
        std::make_unique<juce::AudioParameterFloat>("mod1_depth","Mod 1 Depth", .0f, 1.0f, .1f),
        std::make_unique<juce::AudioParameterFloat>("mod1_mix","Mod 1 Mix", .0f, 1.0f, .5f),
        std::make_unique<juce::AudioParameterFloat>("mod1_synth","Mod 1 Synth", .0f, 1.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("mod1_bronz","Mod 1 Bronz", .0f, 1.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("mod1_vol","Mod 1 Vol", .0f, 1.0f, .5f),        

        std::make_unique<juce::AudioParameterInt>("mod2_on","Mod 2 On", 0, 1, 0),
        std::make_unique<juce::AudioParameterFloat>("mod2_rate","Mod 2 Rate", .0f, 1.0f, .3f),        
        std::make_unique<juce::AudioParameterInt>("mod2_range","Mod 2 Range", 0, 2200, 1100),
        std::make_unique<juce::AudioParameterInt>("mod2_fstart","Mod 2 F Start", 100, 500, 300),
        std::make_unique<juce::AudioParameterFloat>("mod2_mix","Mod 2 Mix", .0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("mod2_vol","Mod 2 Vol", .0f, 1.0f, .5f),

        std::make_unique<juce::AudioParameterInt>("mod3_on","Mod 3 On", 0, 1, 0),
        std::make_unique<juce::AudioParameterFloat>("mod3_rate","Mod 3 Rate", .0f, 1.0f, .3f),
        std::make_unique<juce::AudioParameterFloat>("mod3_depth","Mod 3 Depth", .0f, 1.0f, .2f),
        std::make_unique<juce::AudioParameterFloat>("mod3_mix","Mod 3 Mix", 0.0f, 1.0f, .5f),
        std::make_unique<juce::AudioParameterFloat>("mod3_tone","Mod 3 Tone", .0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("mod3_bal","Mod 3 Bal", .0f, 1.0f, .5f),
        std::make_unique<juce::AudioParameterFloat>("mod3_vol","Mod 3 Vol", .0f, 1.0f, .5f),

        std::make_unique<juce::AudioParameterInt>("mod4_on","Mod 4 On", 0, 1, 0),
        std::make_unique<juce::AudioParameterFloat>("mod4_foff","Mod 4 Foff", .0f, 1.0f, .1f),
        std::make_unique<juce::AudioParameterFloat>("mod4_doff","Mod 4 Doff", -1.0f, 1.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("mod4_mix","Mod 4 Mix", .0f, 1.0f, .5f),
        std::make_unique<juce::AudioParameterFloat>("mod4_tone","Mod 4 Tone", .0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("mod4_bal","Mod 4 Bal", .0f, 1.0f, .5f),
        std::make_unique<juce::AudioParameterFloat>("mod4_vol","Mod 4 Vol", .0f, 1.0f, .5f),

        std::make_unique<juce::AudioParameterInt>("delay_on","Delay On", 0, 1, 1),
        std::make_unique<juce::AudioParameterFloat>("delay_time","Delay Time", .0f, 1.0f, .3f),
        std::make_unique<juce::AudioParameterFloat>("delay_mix","Delay Mix", .0f, 1.0f, .1f),
        std::make_unique<juce::AudioParameterFloat>("delay_offset","Delay Offset", .0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("delay_repeat","Delay Repeat", .0f, 1.0f, .3f),
        std::make_unique<juce::AudioParameterFloat>("delay_bal","Delay Balance", .0f, 1.0f, .5f),
        std::make_unique<juce::AudioParameterFloat>("delay_duck","Delay Duck", .0f, 1.0f, .0f),

        std::make_unique<juce::AudioParameterInt>("reverb_on","Reverb On", 0, 1, 1),
        std::make_unique<juce::AudioParameterInt>("reverb_mode","Rev Mode", 0, 20, 0),
        std::make_unique<juce::AudioParameterFloat>("reverb_mix","Rev Mix", .0f, 1.0f, .1f),
        std::make_unique<juce::AudioParameterFloat>("reverb_time","Rev Time", .0f, 1.0f, .75f),
        std::make_unique<juce::AudioParameterFloat>("reverb_bal","Rev Bal", .0f, 1.0f, .5f),
        std::make_unique<juce::AudioParameterFloat>("reverb_chor","Rev Chor", .0f, 1.0f, .5f),
        std::make_unique<juce::AudioParameterFloat>("reverb_size","Rev Size", .0f, 1.0f, .4f),
        std::make_unique<juce::AudioParameterInt>("reverb_lp","Rev LP", 500, 5500, 1500),
        std::make_unique<juce::AudioParameterInt>("reverb_hp","Rev HP", 50, 550, 50),
        std::make_unique<juce::AudioParameterInt>("reverb_pred","Rev PreD", 0, 200, 0),
        std::make_unique<juce::AudioParameterFloat>("reverb_duck","Rev Duck", .0f, 1.0f, .0f),
        std::make_unique<juce::AudioParameterInt>("reverb_parallel","Rev Parallel", 0, 1, 0),

        std::make_unique<juce::AudioParameterInt>("gate_on","Gate On", 0, 1, 0),
        std::make_unique<juce::AudioParameterFloat>("gate_vol","Gate Vol", .0f, 1.0f, .2f),
        std::make_unique<juce::AudioParameterFloat>("gate_exp","Gate Exp", .0f, 1.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("gate_avg","Gate Avg", .0f, 1.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("gate_freq","Gate Freq", .0f, 1.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("gate_wah","Gate Wah", .0f, 1.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("gate_wahq","Gate WahQ", .7f, 5.0f, 1.4f),
        std::make_unique<juce::AudioParameterFloat>("gate_wahrange","Gate Range", .0f, 1.0f, .5f),

        std::make_unique<juce::AudioParameterInt>("comp_on","Comp On", 0, 1, 0),
        std::make_unique<juce::AudioParameterFloat>("comp_vol","Comp Vol", .0f, 1.0f, .2f),
        std::make_unique<juce::AudioParameterFloat>("comp_att","Comp Att", .0f, 1.0f, .2f),
        std::make_unique<juce::AudioParameterFloat>("comp_rel","Comp Rel", .0f, 1.0f, .2f),
        std::make_unique<juce::AudioParameterFloat>("comp_thr","Comp Thr", .0f, 1.0f, .5f),
        std::make_unique<juce::AudioParameterFloat>("comp_rat","Comp Rat", .0f, 1.0f, .5f),
        std::make_unique<juce::AudioParameterFloat>("comp_drv","Comp Drv", .0f, 1.0f, .0f),

        std::make_unique<juce::AudioParameterInt>("dist1_on","Dist 1 On", 0, 1, 0),
        std::make_unique<juce::AudioParameterFloat>("dist1_vol","Dist 1 Vol", .0f, 1.0f, .2f),
        std::make_unique<juce::AudioParameterInt>("dist1_midf","Dist 1 MidF", 400, 1400, 800),
        std::make_unique<juce::AudioParameterFloat>("dist1_mid","Dist 1 Mid", .0f, 1.0f, .5f),
        std::make_unique<juce::AudioParameterFloat>("dist1_midq","Dist 1 MidQ", .0f, 1.0f, .35f),
        std::make_unique<juce::AudioParameterInt>("dist1_cut","Dist 1 Cut", 2000, 7000, 3500),
        std::make_unique<juce::AudioParameterFloat>("dist1_drv","Dist1 Drv", .0f, 1.0f, .2f),

        std::make_unique<juce::AudioParameterInt>("dist2_on", "Dist 2 On", 0, 1, 0),
        std::make_unique<juce::AudioParameterFloat>("dist2_vol","Dist 2 Vol", .0f, 1.0f, .2f),
        std::make_unique<juce::AudioParameterInt>("dist2_midf","Dist 2 MidF", 400, 1400, 800),
        std::make_unique<juce::AudioParameterFloat>("dist2_mid","Dist 2 Mid", .0f, 1.0f, .5f),
        std::make_unique<juce::AudioParameterFloat>("dist2_midq","Dist 2 MidQ", .0f, 1.0f, .35f),
        std::make_unique<juce::AudioParameterInt>("dist2_cut","Dist 2 Cut", 2000, 7000, 3500),
        std::make_unique<juce::AudioParameterFloat>("dist2_drv","Dist 2 Drv", .0f, 1.0f, .2f),

        std::make_unique<juce::AudioParameterInt>("eq_on", "EQ On", 0, 1, 0),
        std::make_unique<juce::AudioParameterFloat>("eq_vol", "EQ Vol", .0f, 2.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("eq_f1", "EQ Freq 1", -12.0f, 12.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("eq_f2", "EQ Freq 2", -12.0f, 12.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("eq_f3", "EQ Freq 3", -12.0f, 12.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("eq_f4", "EQ Freq 4", -12.0f, 12.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("eq_f5", "EQ Freq 5", -12.0f, 12.0f, .0f),
        std::make_unique<juce::AudioParameterFloat>("eq_f6", "EQ Freq 6", -12.0f, 12.0f, .0f),

    }
        
    )

#endif
{     
    Mako_Program_Construct();
}

MakoDist01AudioProcessor::~MakoDist01AudioProcessor()
{
    //R3.00 This should never be called unless closing as the save button was pressed.
    if (DefAmp_Save) Mako_ExternalData_Save();   
}

//==============================================================================
const juce::String MakoDist01AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MakoDist01AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MakoDist01AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MakoDist01AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MakoDist01AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MakoDist01AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MakoDist01AudioProcessor::getCurrentProgram()
{
    return 0;
}

void MakoDist01AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MakoDist01AudioProcessor::getProgramName (int index)
{
    return {};
}

void MakoDist01AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MakoDist01AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    Mako_Program_Update(false);
}

void MakoDist01AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MakoDist01AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void MakoDist01AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    float tS = 0; float tS2 = 0; float tSD = 0; float tSR = 0;
    float tMixSample = 0.0f;
    int tAmpMode = int(Setting[e_AmpMode] + .1f);
    
    //R1.00 Update our output and input PEAK VOLUME variables.
    makoVU_Out = makoVU_PeakOut * 100;  makoVU_PeakOut *= .98f;
    makoVU_In = makoVU_PeakIn * 100;    makoVU_PeakIn *= .98f;

    //R1.00 Make a copy of sample rate. JUCE says this is the best place to read it.
    makoSampleRate = MakoDist01AudioProcessor::getSampleRate();

    //R1.00 Handle any settings changes made in the Editor. 
    if (0 < makoSettingsChanged)
    {  
        while (0 < makoSettingsChanged) Mako_SettingsUpdate(true);
    }

    // JUCE NOTES
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //JUCE NOTES
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    //R2.22 Create an array to hold the MONO audio data. Size will be unknown.
    int BScnt = buffer.getNumSamples();
    std::vector <float> channelMonoData(BScnt, 0.0f);  
    
    //R2.22 Loop thru all of the channels of audio and adjust the audio buffers for each channel.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
    
        //R2.11 Process the AUDIO buffer data.
        for (int samp = 0; samp < BScnt; samp++)
        {
            //R4.00 Process the left channel(0) to a point (Pedal 5 EQ), then process in stereo.
            if (channel == 0)
            {
                //R1.00 Get audio SAMPLE
                tS = buffer.getSample(channel, samp);    //R1.00 Get the RAW untouched sample. 

                //R2.00 Apply Effects PEDALS. 
                if (DoPed1Enabled) tS = Mako_FX_Audio_NoiseGate(tS, channel, 1);   //Mako_PEDAL_Audio(tS, channel, 1);
                if (DoPed2Enabled) tS = Mako_FX_Audio_Compressor(tS, channel, 2);  //Mako_PEDAL_Audio(tS, channel, 2);
                if (DoPed3Enabled) tS = Mako_FX_Audio_Distortion1(tS, channel, 3); //Mako_PEDAL_Audio(tS, channel, 3);

                //R1.00 Input VU Meter - Store the loudest Sample. Verify we are not clipping in the mod section!
                if (makoVU_PeakIn < abs(tS)) makoVU_PeakIn = abs(tS);
                tS = Mako_Clip(tS, vu_PreAmp);

                //R2.23 Apply MOD effects.
                if (DoMod1) tS = Mako_FX_Audio_Flange(tS, channel);
                if (DoMod2) tS = Mako_FX_Audio_ModWah(tS, channel);

                if (DoPed4Enabled) tS = Mako_FX_Audio_Distortion2(tS, channel, 4);

                //R4.00 Do FINAL EQ in PreAmp position.
                if (InputEQ_PreEQ) tS = Mako_Amp_EQ(tS * .01f, channel);

                //R4.00 Apply high/low pass filters to shape signal before gain stage.
                if (DoAmpBoom) tS = Mako_Amp_Boom(tS, channel);
                if (DoAmpCrisp) tS = Mako_Amp_Crisp(tS, channel);

                //R2.22 Do clean amp or IR based input EQ Amps.
                if (DoAmp)
                    tS = Mako_Amp_InputIR(tS, channel);
                else
                    tS *= (Setting[e_AmpGain] * 20.0f);

                //R2.21 Calc Speaker Thump and AIR.
                if (DoAmpThump) tS = Mako_Amp_Thump(tS, channel);
                if (DoAmpAir)  tS = Mako_Amp_Air(tS, channel);

                //R1.00 Apply Speaker Impulse Response.
                if (DoIR)  tS = Mako_CabSim(tS, channel);

                //R4.00 Do FINAL EQ if not in preAmp position.
                if (!InputEQ_PreEQ) tS = Mako_Amp_EQ(tS * .1f, channel);

                //R4.00 Compressor.
                if (DoAmpComp) tS = Mako_Amp_Comp(tS, channel);

                //******************************************************************
                // R1.00 POWER AMP STAGE for POST EQ Power Amp (default). 
                //******************************************************************
                if (DoAmpPower) tS = tanhf(tS * (1.0f + (Setting[e_AmpPower] * 10.0f)));

                //R4.00 Apply the amp Volume setting.
                tS = Mako_Volume_Final(tS, channel);
               
                //R4.00 LOW PASS - Removes harshness. Moved to position after POWER to remove crispyness.
                if (DoAmpLP) tS = Filter_Calc_BiQuad(tS, channel, &fil_Amp_LP);

                if (DoPed5Enabled) tS = Mako_FX_Audio_EQ(tS, channel, 5);

                tS = Mako_Clip(tS, vu_Amp);

                //R2.22 Store the mono signal(Chan 0) calculated up to this point to be used as Chan 1 data.
                if (tAmpMode == 0) channelMonoData[samp] = tS;
            }

            //R4.00 If working with channel 1, pull the stored data from Chan 0 to create a two channel mono up to this point.
            if (channel) tS = channelMonoData[samp];

            //R4.00 STEREO from here on out.

            //R1.00 Apply FX post amp code, Delay, and Reverb.
            if (DoMod3) tS = Mako_FX_Audio_Chorus(tS, channel);
            if (DoMod4) tS = Mako_FX_Audio_WideLoad(tS, channel);

            //R4.00 Do Delay and Reverb in Parallel or Series.
            switch (DoRevPar)
            {
            case rdm_Series: //Series.
                if (DoDelMix) tS = Mako_FX_Delay(tS, channel);
                if (DoRevMix) tS = Mako_FX_Reverb(tS, channel);
                break;
            case rdm_Parallel: //Parallel.
                tSD = Mako_FX_Delay(tS, channel);
                tSR = Mako_FX_Reverb(tS, channel);
                tS = (tSD + tSR) * .5f;
                break;
            case rdm_ParDel: //Delay Only.
                tS = Mako_FX_Delay(tS, channel);
                break;
            case rdm_ParRev: //Verb only.
                tS = Mako_FX_Reverb(tS, channel);
                break;
            case rdm_None: //none.
                break;
            }            
    
            //R4.00 Post gain - LOW CUTS
            //R4.00 This filter may be necessary if getting DC offset in code.
            //tS = Mako_EQ_Output_Filters(tS, channel);

            //R1.00 Track our highest peak volume. 
            if (makoVU_PeakOut < abs(tS)) makoVU_PeakOut = abs(tS);
            tS = Mako_Clip(tS, vu_Final);

            //R4.00 Write the modified audio into the sample buffer for playback.
            channelData[samp] = tS;
        }       
    }
    
}

//R3.00 Reduce lows entering the AMP IR gain calcs.
float MakoDist01AudioProcessor::Mako_Amp_Boom(float tSample, int channel)
{  
   return Filter_Calc_BiQuad(tSample, channel, &fil_Amp_Boom);
}

//R3.00 Reduce highs entering the AMP IR gain calcs.
float MakoDist01AudioProcessor::Mako_Amp_Crisp(float tSample, int channel)
{
    return Filter_Calc_BiQuad(tSample, channel, &fil_Amp_Crisp);
}

//R4.00 Apply gain to a Low Pass filter then mix in with the original audio signal.
float MakoDist01AudioProcessor::Mako_Amp_Thump(float tSample, int channel)
{
    float tS = Filter_Calc_BiQuad(tSample, channel, &fil_Amp_Thump);
    return tSample + tanhf(tS * Setting[e_AmpThump] * 4.0f);

}

//R4.00 Apply gain to a High Pass filter then mix in with the original audio signal.
float MakoDist01AudioProcessor::Mako_Amp_Air(float tSample, int channel)
{
    float tS = Filter_Calc_BiQuad(tSample, channel, &fil_Amp_Air);
    return tSample + tanhf(tS * Setting[e_AmpAir] * 4.0f);
}

//R1.00 FINAL Volume.
float MakoDist01AudioProcessor::Mako_Volume_Final(float tSample, int channel)
{
    float tS = tSample;
    
    //R1.00 Slowly fade volume in when making PRESET changes.
    if (Setting[e_AmpVol] < Amp_Vol)
        Amp_Vol = Setting[e_AmpVol];
    else
        Amp_Vol = (Amp_Vol * RATI1000ms) + (Setting[e_AmpVol] * RAT1000ms);
       
    //R4.00 Apply VOLUME. Square to get more range on knob.
    tS = tS * (Amp_Vol * Amp_Vol * 15.0f);
       
    //R1.00 Final CLIP check. Flag that we are clipping.
    if (tS < -.9999f)
    {
        tS = -.9999f;
        makoClipStage = 2;
    }
    else if (.9999f < tS)
    {
        tS = .9999f;
        makoClipStage = 2;
    }

    return tS;
}


//R2.00 EQ 0 
void MakoDist01AudioProcessor::Mako_EQ_Set_0()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = false;
    
    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;

    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;

    Amp_EQ1.Freq = 150.0f;
    Amp_EQ2.Freq = 500.0f;
    Amp_EQ3.Freq = 1200.0f;
    Amp_EQ4.Freq = 2500.0f;
    Amp_EQ5.Freq = 5500.0f;

    Amp_EQ1.Q = .707f;
    Amp_EQ2.Q = .707f;
    Amp_EQ3.Q = .35f;
    Amp_EQ4.Q = .707f;
    Amp_EQ5.Q = 1.414f;
}

//R2.00 EQ 1
void MakoDist01AudioProcessor::Mako_EQ_Set_1()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = false;
    
    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;

    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;

    Amp_EQ1.Freq = 100.0f;
    Amp_EQ2.Freq = 330.0f;
    Amp_EQ3.Freq = 700.0f;
    Amp_EQ4.Freq = 1800.0f;
    Amp_EQ5.Freq = 5000.0f;

    Amp_EQ1.Q = .707f;
    Amp_EQ2.Q = .707f;
    Amp_EQ3.Q = .350f;
    Amp_EQ4.Q = .150f;
    Amp_EQ5.Q = .350f;
}

//R2.00 EQ 2
void MakoDist01AudioProcessor::Mako_EQ_Set_2()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = false;
    
    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;

    Amp_EQ1.RangedB = 12.0f;
    Amp_EQ2.RangedB = 12.0f;
    Amp_EQ3.RangedB = 12.0f;
    Amp_EQ4.RangedB = 12.0f;
    Amp_EQ5.RangedB = 12.0f;

    Amp_EQ1.Freq = 80.0f;
    Amp_EQ2.Freq = 220.0f;
    Amp_EQ3.Freq = 750.0f;
    Amp_EQ4.Freq = 2200.0f;
    Amp_EQ5.Freq = 6600.0f;

    Amp_EQ1.Q = .707f;
    Amp_EQ2.Q = .35f;
    Amp_EQ3.Q = .35f;
    Amp_EQ4.Q = .35f;
    Amp_EQ5.Q = .35f;
}

//R2.00 EQ 3
void MakoDist01AudioProcessor::Mako_EQ_Set_3()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = true;
    
    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;
    Amp_EQ6.Gain = .50f;

    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 12.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 12.0f;
    Amp_EQ6.RangedB = 12.0f;

    Amp_EQ1.Freq = 120.0f;
    Amp_EQ2.Freq = 600.0f;
    Amp_EQ3.Freq = 2000.0f;
    Amp_EQ4.Freq = 10000.0f;
    Amp_EQ5.Freq = 80.0f;
    Amp_EQ6.Freq = 1500.0f;

    Amp_EQ1.Q = .35f;
    Amp_EQ2.Q = .35f;
    Amp_EQ3.Q = .15f;
    Amp_EQ4.Q = .15f;
    Amp_EQ5.Q = 1.414f;
    Amp_EQ6.Q = 1.414f;

}

//R2.00 EQ 4
void MakoDist01AudioProcessor::Mako_EQ_Set_4()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = false;
    
    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;

    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;

    Amp_EQ1.Freq = 100.0f;
    Amp_EQ2.Freq = 400.0f;
    Amp_EQ3.Freq = 1000.0f;
    Amp_EQ4.Freq = 2500.0f;
    Amp_EQ5.Freq = 6600.0f;

    Amp_EQ1.Q = .707f;
    Amp_EQ2.Q = .35f;
    Amp_EQ3.Q = .35f;
    Amp_EQ4.Q = .35f;
    Amp_EQ5.Q = .707f;

}

//R2.00 EQ 5
void MakoDist01AudioProcessor::Mako_EQ_Set_5()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = true;
    
    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;
    Amp_EQ6.Gain = .50f;

    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;
    Amp_EQ6.RangedB = 24.0f;

    Amp_EQ1.Freq = 50.0f;
    Amp_EQ2.Freq = 120.0f;
    Amp_EQ3.Freq = 240.0f;
    Amp_EQ4.Freq = 500.0f;
    Amp_EQ5.Freq = 1000.0f;
    Amp_EQ6.Freq = 2200.0f;

    Amp_EQ1.Q = .35f;
    Amp_EQ2.Q = .707f;
    Amp_EQ3.Q = .707f;
    Amp_EQ4.Q = .707f;
    Amp_EQ5.Q = .707f;
    Amp_EQ6.Q = .35f;
}

//R2.00 EQ 6
void MakoDist01AudioProcessor::Mako_EQ_Set_6()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = true;
    
    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;
    Amp_EQ6.Gain = .50f;

    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;
    Amp_EQ6.RangedB = 24.0f;

    Amp_EQ1.Freq = 80.0f;
    Amp_EQ2.Freq = 300.0f;
    Amp_EQ3.Freq = 900.0f;
    Amp_EQ4.Freq = 1500.0f;
    Amp_EQ5.Freq = 3000.0f;
    Amp_EQ6.Freq = 6000.0f;

    Amp_EQ1.Q = .707f;
    Amp_EQ2.Q = .35f;
    Amp_EQ3.Q = .35f;
    Amp_EQ4.Q = .35f;
    Amp_EQ5.Q = .35f;
    Amp_EQ6.Q = .707f;
}

//R2.00 EQ 7
void MakoDist01AudioProcessor::Mako_EQ_Set_7()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = true;
    
    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;
    Amp_EQ6.Gain = .50f;

    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;
    Amp_EQ6.RangedB = 24.0f;

    Amp_EQ1.Freq = 100.0f;
    Amp_EQ2.Freq = 400.0f;
    Amp_EQ3.Freq = 1000.0f;
    Amp_EQ4.Freq = 2000.0f;
    Amp_EQ5.Freq = 4000.0f;
    Amp_EQ6.Freq = 8000.0f;

    Amp_EQ1.Q = .1f;
    Amp_EQ2.Q = .5f;
    Amp_EQ3.Q = .35f;
    Amp_EQ4.Q = .35f;
    Amp_EQ5.Q = .35f;
    Amp_EQ6.Q = .1f;
}

//R2.00 EQ 8
void MakoDist01AudioProcessor::Mako_EQ_Set_8()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = true;
    
    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;
    Amp_EQ6.Gain = .50f;

    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;
    Amp_EQ6.RangedB = 24.0f;

    Amp_EQ1.Freq = 80.0f;
    Amp_EQ2.Freq = 400.0f;
    Amp_EQ3.Freq = 750.0f;
    Amp_EQ4.Freq = 1500.0f;
    Amp_EQ5.Freq = 3000.0f;
    Amp_EQ6.Freq = 6000.0f;

    Amp_EQ1.Q = .35f;
    Amp_EQ2.Q = .5f;
    Amp_EQ3.Q = .5f;
    Amp_EQ4.Q = .35f;
    Amp_EQ5.Q = .2f;
    Amp_EQ6.Q = .1f;
}

//R2.00 EQ 9
void MakoDist01AudioProcessor::Mako_EQ_Set_9()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = true;
    
    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;
    Amp_EQ6.Gain = .50f;
    
    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;
    Amp_EQ6.RangedB = 24.0f;
    
    Amp_EQ1.Freq = 80.0f;
    Amp_EQ2.Freq = 200.0f;
    Amp_EQ3.Freq = 400.0f;
    Amp_EQ4.Freq = 800.0f;
    Amp_EQ5.Freq = 1600.0f;
    Amp_EQ6.Freq = 3200.0f;
    
    Amp_EQ1.Q = .707f;
    Amp_EQ2.Q = .707f;
    Amp_EQ3.Q = .707f;
    Amp_EQ4.Q = .35f;
    Amp_EQ5.Q = .707f;
    Amp_EQ6.Q = .707f;    
}

void MakoDist01AudioProcessor::Mako_EQ_Set_10()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = true;
    
    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;
    Amp_EQ6.Gain = .50f;
    
    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;
    Amp_EQ6.RangedB = 24.0f;
   
    Amp_EQ1.Freq = 100.0f;
    Amp_EQ2.Freq = 450.0f;
    Amp_EQ3.Freq = 700.0f;
    Amp_EQ4.Freq = 900.0f;
    Amp_EQ5.Freq = 1500.0f;
    Amp_EQ6.Freq = 3000.0f;
    
    Amp_EQ1.Q = .707f;
    Amp_EQ2.Q = .707f;
    Amp_EQ3.Q = .35f;
    Amp_EQ4.Q = .35f;
    Amp_EQ5.Q = .707f;
    Amp_EQ6.Q = .35f;    
}

void MakoDist01AudioProcessor::Mako_EQ_Set_11()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = true;

    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;
    Amp_EQ6.Gain = .50f;

    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;
    Amp_EQ6.RangedB = 24.0f;

    Amp_EQ1.Freq = 80.0f;
    Amp_EQ2.Freq = 180.0f;
    Amp_EQ3.Freq = 600.0f;
    Amp_EQ4.Freq = 1200.0f;
    Amp_EQ5.Freq = 2000.0f;
    Amp_EQ6.Freq = 4000.0f;

    Amp_EQ1.Q = .707f;
    Amp_EQ2.Q = .707f;
    Amp_EQ3.Q = .35f;
    Amp_EQ4.Q = .35f;
    Amp_EQ5.Q = .35f;
    Amp_EQ6.Q = .35f;    
}

void MakoDist01AudioProcessor::Mako_EQ_Set_12()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = true;

    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;
    Amp_EQ6.Gain = .50f;

    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;
    Amp_EQ6.RangedB = 24.0f;

    Amp_EQ1.Freq = 120.0f;
    Amp_EQ2.Freq = 450.0f;
    Amp_EQ3.Freq = 800.0f;
    Amp_EQ4.Freq = 1000.0f;
    Amp_EQ5.Freq = 1500.0f;
    Amp_EQ6.Freq = 2200.0f;

    Amp_EQ1.Q = .707f;
    Amp_EQ2.Q = .707f;
    Amp_EQ3.Q = .707f;
    Amp_EQ4.Q = .707f;
    Amp_EQ5.Q = 1.414f;
    Amp_EQ6.Q = .35f;
}

void MakoDist01AudioProcessor::Mako_EQ_Set_13()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = true;

    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;
    Amp_EQ6.Gain = .50f;

    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;
    Amp_EQ6.RangedB = 24.0f;

    Amp_EQ1.Freq = 100.0f;
    Amp_EQ2.Freq = 200.0f;
    Amp_EQ3.Freq = 400.0f;
    Amp_EQ4.Freq = 800.0f;
    Amp_EQ5.Freq = 1600.0f;
    Amp_EQ6.Freq = 3200.0f;

    Amp_EQ1.Q = .707f;
    Amp_EQ2.Q = .707f;
    Amp_EQ3.Q = .707f;
    Amp_EQ4.Q = .707f;
    Amp_EQ5.Q = .707f;
    Amp_EQ6.Q = .707f;
}

void MakoDist01AudioProcessor::Mako_EQ_Set_14()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = true;

    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;
    Amp_EQ6.Gain = .50f;

    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;
    Amp_EQ6.RangedB = 24.0f;

    Amp_EQ1.Freq = 80.0f;
    Amp_EQ2.Freq = 500.0f;
    Amp_EQ3.Freq = 900.0f;
    Amp_EQ4.Freq = 1400.0f;
    Amp_EQ5.Freq = 2500.0f;
    Amp_EQ6.Freq = 5000.0f;

    Amp_EQ1.Q = .707f;
    Amp_EQ2.Q = .707f;
    Amp_EQ3.Q = .707f;
    Amp_EQ4.Q = .35f;
    Amp_EQ5.Q = .707;
    Amp_EQ6.Q = .35f;
}

void MakoDist01AudioProcessor::Mako_EQ_Set_15()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = true;

    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;
    Amp_EQ6.Gain = .50f;

    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;
    Amp_EQ6.RangedB = 24.0f;

    Amp_EQ1.Freq = 200.0f;
    Amp_EQ2.Freq = 700.0f;
    Amp_EQ3.Freq = 1200.0f;
    Amp_EQ4.Freq = 1900.0f;
    Amp_EQ5.Freq = 3000.0f;
    Amp_EQ6.Freq = 8000.0f;

    Amp_EQ1.Q = .35f;
    Amp_EQ2.Q = .707f;
    Amp_EQ3.Q = .707f;
    Amp_EQ4.Q = .35f;
    Amp_EQ5.Q = .707;
    Amp_EQ6.Q = .35f;
}

void MakoDist01AudioProcessor::Mako_EQ_Set_16()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = true;

    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;
    Amp_EQ6.Gain = .50f;

    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;
    Amp_EQ6.RangedB = 24.0f;

    Amp_EQ1.Freq = 50.0f;
    Amp_EQ2.Freq = 120.0f;
    Amp_EQ3.Freq = 300.0f;
    Amp_EQ4.Freq = 600.0f;
    Amp_EQ5.Freq = 900.0f;
    Amp_EQ6.Freq = 4000.0f;

    Amp_EQ1.Q = .707f;
    Amp_EQ2.Q = .707f;
    Amp_EQ3.Q = .707f;
    Amp_EQ4.Q = .707f;
    Amp_EQ5.Q = .707;
    Amp_EQ6.Q = .35f;
}

void MakoDist01AudioProcessor::Mako_EQ_Set_17()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = false;
    Amp_EQ5.Enabled = false;
    Amp_EQ6.Enabled = false;

    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;
    Amp_EQ6.Gain = .50f;

    Amp_EQ1.RangedB = 30.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 30.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;
    Amp_EQ6.RangedB = 24.0f;

    Amp_EQ1.Freq = 20.0f;
    Amp_EQ2.Freq = 700.0f;
    Amp_EQ3.Freq = 10000.0f;
    //Amp_EQ4.Freq = 600.0f;
    //Amp_EQ5.Freq = 900.0f;
    //Amp_EQ6.Freq = 4000.0f;

    Amp_EQ1.Q = .1f;
    Amp_EQ2.Q = .35f;
    Amp_EQ3.Q = .1f;
    //Amp_EQ4.Q = .707f;
    //Amp_EQ5.Q = .707;
    //Amp_EQ6.Q = .35f;
}

void MakoDist01AudioProcessor::Mako_EQ_Set_18()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = true;

    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;
    Amp_EQ6.Gain = .50f;

    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;
    Amp_EQ6.RangedB = 24.0f;

    Amp_EQ1.Freq = 60.0f;
    Amp_EQ2.Freq = 240.0f;
    Amp_EQ3.Freq = 660.0f;
    Amp_EQ4.Freq = 1200.0f;
    Amp_EQ5.Freq = 2400.0f;
    Amp_EQ6.Freq = 10000.0f;

    Amp_EQ1.Q = .1f;
    Amp_EQ2.Q = .35f;
    Amp_EQ3.Q = .5f;
    Amp_EQ4.Q = .5f;
    Amp_EQ5.Q = .5f;
    Amp_EQ6.Q = .1f;
}

void MakoDist01AudioProcessor::Mako_EQ_Set_19()
{
    Amp_EQ1.Enabled = true;
    Amp_EQ2.Enabled = true;
    Amp_EQ3.Enabled = true;
    Amp_EQ4.Enabled = true;
    Amp_EQ5.Enabled = true;
    Amp_EQ6.Enabled = true;

    Amp_EQ1.Gain = .50f;
    Amp_EQ2.Gain = .50f;
    Amp_EQ3.Gain = .50f;
    Amp_EQ4.Gain = .50f;
    Amp_EQ5.Gain = .50f;
    Amp_EQ6.Gain = .50f;

    Amp_EQ1.RangedB = 24.0f;
    Amp_EQ2.RangedB = 24.0f;
    Amp_EQ3.RangedB = 24.0f;
    Amp_EQ4.RangedB = 24.0f;
    Amp_EQ5.RangedB = 24.0f;
    Amp_EQ6.RangedB = 24.0f;

    Amp_EQ1.Freq = 100.0f;
    Amp_EQ2.Freq = 350.0f;
    Amp_EQ3.Freq = 700.0f;
    Amp_EQ4.Freq = 1400.0f;
    Amp_EQ5.Freq = 2800.0f;
    Amp_EQ6.Freq = 10000.0f;

    Amp_EQ1.Q = .1f;
    Amp_EQ2.Q = .35f;
    Amp_EQ3.Q = .5f;
    Amp_EQ4.Q = .5f;
    Amp_EQ5.Q = .5f;
    Amp_EQ6.Q = .1f;
}



void MakoDist01AudioProcessor::Mako_EQ_Set_Filters()
{
    //R2.00 AMPLIFIER EQ
    if ((Amp_EQ1.Enabled) && (SettingRqd[e_AmpEQ1]))
    {
        Mako_UpdateLastVar(e_AmpEQ1);
        Filter_BP_Coeffs(Setting[e_AmpEQ1] * Amp_EQ1.RangedB, Amp_EQ1.Freq, Amp_EQ1.Q, &fil_Amp_EQ1);
    }
    if ((Amp_EQ2.Enabled) && (SettingRqd[e_AmpEQ2]))
    {
        Mako_UpdateLastVar(e_AmpEQ2);
        Filter_BP_Coeffs(Setting[e_AmpEQ2] * Amp_EQ2.RangedB, Amp_EQ2.Freq, Amp_EQ2.Q, &fil_Amp_EQ2);
    }
    if ((Amp_EQ3.Enabled) && (SettingRqd[e_AmpEQ3]))
    {
        Mako_UpdateLastVar(e_AmpEQ3);
        Filter_BP_Coeffs(Setting[e_AmpEQ3] * Amp_EQ3.RangedB, Amp_EQ3.Freq, Amp_EQ3.Q, &fil_Amp_EQ3);
    }
    if ((Amp_EQ4.Enabled) && (SettingRqd[e_AmpEQ4]))
    {
        Mako_UpdateLastVar(e_AmpEQ4);
        Filter_BP_Coeffs(Setting[e_AmpEQ4] * Amp_EQ4.RangedB, Amp_EQ4.Freq, Amp_EQ4.Q, &fil_Amp_EQ4);
    }
    if ((Amp_EQ5.Enabled) && (SettingRqd[e_AmpEQ5]))
    {
        Mako_UpdateLastVar(e_AmpEQ5);
        Filter_BP_Coeffs(Setting[e_AmpEQ5] * Amp_EQ5.RangedB, Amp_EQ5.Freq, Amp_EQ5.Q, &fil_Amp_EQ5);
    }
    if ((Amp_EQ6.Enabled) && (SettingRqd[e_AmpEQ6]))
    {
        Mako_UpdateLastVar(e_AmpEQ6);
        Filter_BP_Coeffs(Setting[e_AmpEQ6] * Amp_EQ6.RangedB, Amp_EQ6.Freq, Amp_EQ6.Q, &fil_Amp_EQ6);
    }
    
}

//R1.00 Some settings need to be calculated here instead of editor. 
void MakoDist01AudioProcessor::Mako_SettingsUpdate(bool UpdateParms)
{   
    int StartCount = makoSettingsChanged;

    //R3.00 User has requested a database reload.
    if (DefAmp_Reload == 1)
    {
        Mako_ExternalData_Load(true);
        DefAmp_Reload = 2;           //R3.00 Reload request complete.
        makoEditorNeedsUpdated = 1;  //R3.00 Flag editor to rebuild its amp lists.
    }

    //R3.00 We have unsaved database edits, save.
    if (DefAmp_Save)
    {
        DefAmp_Save = false;
        Mako_ExternalData_Save();
    }

    //**********************************************************
    //R4.00 Amp Settings
    //**********************************************************
    if (SettingRqd[e_AmpLP])
    {
        Mako_UpdateLastVar(e_AmpLP);
        Filter_LP_Coeffs(Setting[e_AmpLP], &fil_Amp_LP);
    }       
        
    if (SettingRqd[e_AmpChan])
    {
        Mako_UpdateLastVar(e_AmpChan);
        Mako_AMP_Setup();
    }   

    if (SettingRqd[e_AmpQual])
    {
        Mako_UpdateLastVar(e_AmpQual);

        switch (int(Setting[e_AmpQual]))
        {
        case 0: Amp_Qual_Max = 128; break;
        case 1: Amp_Qual_Max = 256; break;
        case 2: Amp_Qual_Max = 512; break;
        case 3: Amp_Qual_Max = 1024; break;
        }
    }

    Mako_EQ_Set_Filters();

    //R3.00 Added Boom and Crisp.
    if (SettingRqd[e_AmpBoom])
    {
        Mako_UpdateLastVar(e_AmpBoom);
        Filter_FO_HP_Coeffs(20.0f + ((1.0f - Setting[e_AmpBoom]) * 1000.0f), &fil_Amp_Boom);
    }
    if (SettingRqd[e_AmpCrisp])
    {
        Mako_UpdateLastVar(e_AmpCrisp);
        Filter_FO_LP_Coeffs(500.0f + Setting[e_AmpCrisp] * 5000.0f, &fil_Amp_Crisp);
    }

    //**********************************************************
    //R4.00 PEDAL SETTINGS
    //**********************************************************
    if (SettingRqd[e_GateAvg])  { Mako_UpdateLastVar(e_GateAvg);  Mako_FX_Setup_NoiseGate(1); }
    if (SettingRqd[e_GateFreq]) { Mako_UpdateLastVar(e_GateFreq); Mako_FX_Setup_NoiseGate(1); }
    if (SettingRqd[e_GateWahRange]) { Mako_UpdateLastVar(e_GateWahRange); Mako_FX_Setup_NoiseGate(1); }

    if (SettingRqd[e_CompThr]) { Mako_UpdateLastVar(e_CompThr);   Mako_FX_Setup_Compressor(2); }

    if (SettingRqd[e_Dist1Mid]) { Mako_UpdateLastVar(e_Dist1Mid); Mako_FX_Setup_Distortion1(3); }
    if (SettingRqd[e_Dist1MidF]) { Mako_UpdateLastVar(e_Dist1MidF); Mako_FX_Setup_Distortion1(3); }
    if (SettingRqd[e_Dist1MidQ]) { Mako_UpdateLastVar(e_Dist1MidQ); Mako_FX_Setup_Distortion1(3); }
    if (SettingRqd[e_Dist1Cut]) { Mako_UpdateLastVar(e_Dist1Cut); Mako_FX_Setup_Distortion1(3); }
    
    if (SettingRqd[e_Dist2Mid])  { Mako_UpdateLastVar(e_Dist2Mid);  Mako_FX_Setup_Distortion2(4); }
    if (SettingRqd[e_Dist2MidF]) { Mako_UpdateLastVar(e_Dist2MidF); Mako_FX_Setup_Distortion2(4); }
    if (SettingRqd[e_Dist2MidQ]) { Mako_UpdateLastVar(e_Dist2MidQ); Mako_FX_Setup_Distortion2(4); }
    if (SettingRqd[e_Dist2Cut]) { Mako_UpdateLastVar(e_Dist2Cut);   Mako_FX_Setup_Distortion2(4); }
    
    //R4.00 EQ Settings.
    if (SettingRqd[e_EQF1]) { Mako_UpdateLastVar(e_EQF1); Mako_FX_Setup_EQ(5); }
    if (SettingRqd[e_EQF2]) { Mako_UpdateLastVar(e_EQF2); Mako_FX_Setup_EQ(5); }
    if (SettingRqd[e_EQF3]) { Mako_UpdateLastVar(e_EQF3); Mako_FX_Setup_EQ(5); }
    if (SettingRqd[e_EQF4]) { Mako_UpdateLastVar(e_EQF4); Mako_FX_Setup_EQ(5); }
    if (SettingRqd[e_EQF5]) { Mako_UpdateLastVar(e_EQF5); Mako_FX_Setup_EQ(5); }
    if (SettingRqd[e_EQF6]) { Mako_UpdateLastVar(e_EQF6); Mako_FX_Setup_EQ(5); }

    //**********************************************************
    //R4.00 MODULATION SETTINGS
    //**********************************************************
    //R4.00 DEPTH settings.
    if (SettingRqd[e_Mod1Depth]) { Mako_UpdateLastVar(e_Mod1Depth); Mako_FX_Setup_Mod1(); }
    if (SettingRqd[e_Mod2Range]) { Mako_UpdateLastVar(e_Mod2Range); Mako_FX_Setup_Mod2(); }
    if (SettingRqd[e_Mod2Rate])  { Mako_UpdateLastVar(e_Mod2Rate);  Mako_FX_Setup_Mod2(); }
    if (SettingRqd[e_Mod3Depth]) { Mako_UpdateLastVar(e_Mod3Depth); Mako_FX_Setup_Mod3(); }
    if (SettingRqd[e_Mod4Foff])  { Mako_UpdateLastVar(e_Mod4Foff);  Mako_FX_Setup_Mod4(); }
 
    //R4.00 BALANCE Settings.
    if (SettingRqd[e_Mod3Bal]) { Mako_UpdateLastVar(e_Mod3Bal); Mako_FX_Setup_Mod3(); }
    if (SettingRqd[e_Mod4Bal]) { Mako_UpdateLastVar(e_Mod4Bal); Mako_FX_Setup_Mod4(); }

    //**********************************************************
    //R1.00 DELAY Settings.
    //**********************************************************
    if (SettingRqd[e_DelTime] || SettingRqd[e_DelOffset])
    {
        Mako_UpdateLastVar(e_DelTime);
        Mako_UpdateLastVar(e_DelOffset);
        Delay_B_Ring1[0] = int(2.0f * Setting[e_DelTime] * (.5f + ((1.0f - Setting[e_DelOffset]) * .5f)) * makoSampleRate);
        Delay_B_Ring1_Max[0] = int(2.0f * Setting[e_DelTime] * (.5f + ((1.0f - Setting[e_DelOffset]) * .5f)) * makoSampleRate) + 1;
        Delay_B_Ring1[1] = int(2.0f * Setting[e_DelTime] * makoSampleRate);
        Delay_B_Ring1_Max[1] = int(2.0f * Setting[e_DelTime] * makoSampleRate) + 1;
    }
    if (SettingRqd[e_DelMix])
    {
        Mako_UpdateLastVar(e_DelMix);
        if (Setting[e_DelMix] < .5f)
        {
            Delay_Dry = 1.0f;
            Delay_Wet = Setting[e_DelMix] * 2;
        }
        else
        {
            Delay_Dry = 1.0f - ((Setting[e_DelMix] - .5f) * 2.0f);
            Delay_Wet = 1.0f;
        }
    }
    if (SettingRqd[e_DelBal])
    {
        Mako_UpdateLastVar(e_DelBal);
        if (Setting[e_DelBal] < .5f)
        {
            Delay_BalLR[0] = 1.0f;
            Delay_BalLR[1] = Setting[e_DelBal] * 2;
        }
        else
        {
            Delay_BalLR[0] = 1.0f - ((Setting[e_DelBal] - .5f) * 2.0f);
            Delay_BalLR[1] = 1.0f;
        }
    }

    //**********************************************************
    //R2.00 REVERB Settings
    //**********************************************************
    if (SettingRqd[e_RevTime])
    {
        Mako_UpdateLastVar(e_RevTime);
        Reverb_TimeInv = 1.0f - Setting[e_RevTime];
    }    
    if (SettingRqd[e_RevSize])
    {   
        Mako_UpdateLastVar(e_RevSize);
        Mako_FX_Reverb_CalcSize();        
    }
    if (SettingRqd[e_RevMode])
    {
        Mako_UpdateLastVar(e_RevMode);
        Mako_FX_Reverb_CalcSize();
    }
    //R2.22 Calc our PreDelay. Max 200mS or 38400 samples. 
    if (SettingRqd[e_RevPreD])
    {   
        Mako_UpdateLastVar(e_RevPreD);
        PreDEnd = Setting[e_RevPreD] * .001f * makoSampleRate;
        if (PreDMax < PreDEnd) PreDEnd = PreDMax;
    }
    if (SettingRqd[e_RevMix])
    {
        Mako_UpdateLastVar(e_RevMix);
        if (Setting[e_RevMix] < .5f)
        {
            Reverb_Dry = 1.0f;
            Reverb_Wet = Setting[e_RevMix] * 2;
        }
        else
        {
            Reverb_Dry = 1.0f - ((Setting[e_RevMix] - .5f) * 2.0f);
            Reverb_Wet = 1.0f;
        }
    }
    if (SettingRqd[e_RevBal])
    {
        Mako_UpdateLastVar(e_RevBal);
        if (Setting[e_RevBal] < .5f)
        {
            Reverb_BalLR[0] = 1.0f;
            Reverb_BalLR[1] = Setting[e_RevBal] * 2;
        }
        else
        {
            Reverb_BalLR[0] = 1.0f - ((Setting[e_RevBal] - .5f) * 2.0f);
            Reverb_BalLR[1] = 1.0f;
        }
    }
    if (SettingRqd[e_RevLP])
    {
        Mako_UpdateLastVar(e_RevLP);
        Filter_LP_Coeffs(Setting[e_RevLP], &fil_Rev_LP);   //R2.00 NEEDS to use real freqs. 500-20k
    }
    if (SettingRqd[e_RevHP])
    {
        Mako_UpdateLastVar(e_RevHP);
        Filter_HP_Coeffs(Setting[e_RevHP], &fil_Rev_HP);    //R2.00 50-10k
    }

    //**********************************************************
    //R4.00 SPEAKER Impulse Response Settings
    //**********************************************************
    if (SettingRqd[e_IR])
    {
        Mako_UpdateLastVar(e_IR);
        Mako_SetIR();        
    }    
    if (SettingRqd[e_IRSize])
    {
        Mako_UpdateLastVar(e_IRSize);
        Mako_SetIR();
    }    


    //R4.00 Hack for smoother transitions.
    if (makoSettingsClearbuffers == 1)
    {
        //R1.00 Clear Delay/Reverb buffers so we dont get loud bangs when updating. 
        //R1.00 Still happens anyway.
        for (int t = 0; t <= 192000; t++)
        {
            Delay_B[0][t] = 0.0f;
            Delay_B[1][t] = 0.0f;
            Reverb_B[0][t] = 0.0f;
            Reverb_B[1][t] = 0.0f;
        }

        makoSettingsClearbuffers = 0;
    }

    //R4.00 Set DO TEST vars.
    //R4.00 These vars are used to speed up the Audio processing decision making.
    (0.001f < Setting[e_GateOn]) ? DoPed1Enabled = true : DoPed1Enabled = false;
    (0.001f < Setting[e_CompOn]) ? DoPed2Enabled = true : DoPed2Enabled = false;
    (0.001f < Setting[e_Dist1On]) ? DoPed3Enabled = true : DoPed3Enabled = false;
    (0.001f < Setting[e_Dist2On]) ? DoPed4Enabled = true : DoPed4Enabled = false;
    (0.001f < Setting[e_EQOn]) ? DoPed5Enabled = true : DoPed5Enabled = false;
    
    (0.001f < Setting[e_Mod1On]) ? DoMod1 = true : DoMod1 = false;
    (0.001f < Setting[e_Mod2On]) ? DoMod2 = true : DoMod2 = false;
    (0.001f < Setting[e_Mod3On]) ? DoMod3 = true : DoMod3 = false;
    (0.001f < Setting[e_Mod4On]) ? DoMod4 = true : DoMod4 = false;

    (0.001f < Setting[e_AmpOn]) ? DoAmp = true : DoAmp = false;
    (Setting[e_AmpBoom] < .999f) ? DoAmpBoom = true : DoAmpBoom = false;
    (Setting[e_AmpCrisp] < .999f) ? DoAmpCrisp = true : DoAmpCrisp = false;
    (0.001f < Setting[e_AmpThump]) ? DoAmpThump = true : DoAmpThump = false;
    (0.001f < Setting[e_AmpAir]) ? DoAmpAir = true : DoAmpAir = false;
    (0.001f < Setting[e_AmpPower]) ? DoAmpPower = true : DoAmpPower = false;
    (Setting[e_AmpLP] < 11999.0f) ? DoAmpLP = true : DoAmpLP = false;
    (Setting[e_AmpComp] < .999f) ? DoAmpComp = true : DoAmpComp = false;
    (0.001f < Setting[e_IROn]) ? DoIR = true : DoIR = false;
    (0.001f < Setting[e_DelMix]) ? DoDelMix = true : DoDelMix = false;
    (0.001f < Setting[e_DelOn]) ? DoDelMix = true : DoDelMix = false;
    (0.001f < Setting[e_RevMix]) ? DoRevMix = true : DoRevMix = false;
    (0.001f < Setting[e_RevOn]) ? DoRevMix = true : DoRevMix = false;
    
    //R4.00 Configure the Reverb-Delay parallel mode if using.
    DoRevPar = rdm_Series;
    if ((0.001f < Setting[e_RevPar]) && DoRevMix && DoDelMix) DoRevPar = rdm_Parallel;
    if ((0.001f < Setting[e_RevPar]) && (!DoRevMix) && (DoDelMix)) DoRevPar = rdm_ParDel;
    if ((0.001f < Setting[e_RevPar]) && (DoRevMix) && (!DoDelMix)) DoRevPar = rdm_ParRev;
    if ((Setting[e_RevPar] < .001f) && (!DoRevMix) && (!DoDelMix)) DoRevPar = rdm_None;

    //R4.00 Setup checks for Amplifier EQ.
    ((Amp_EQ1.Enabled) && (Setting[e_AmpEQ1] != .0f)) ? DoAmpEQ1 = true: DoAmpEQ1 = false;
    ((Amp_EQ2.Enabled) && (Setting[e_AmpEQ2] != .0f)) ? DoAmpEQ2 = true : DoAmpEQ2 = false;
    ((Amp_EQ3.Enabled) && (Setting[e_AmpEQ3] != .0f)) ? DoAmpEQ3 = true : DoAmpEQ3 = false;
    ((Amp_EQ4.Enabled) && (Setting[e_AmpEQ4] != .0f)) ? DoAmpEQ4 = true : DoAmpEQ4 = false;
    ((Amp_EQ5.Enabled) && (Setting[e_AmpEQ5] != .0f)) ? DoAmpEQ5 = true : DoAmpEQ5 = false;
    ((Amp_EQ6.Enabled) && (Setting[e_AmpEQ6] != .0f)) ? DoAmpEQ6 = true : DoAmpEQ6 = false;

    //R4.00 Setup checks for Six Band EQ.
    (0.0f != Setting[e_EQF1]) ? DoEQ1 = true: DoEQ1 = false;
    (0.0f != Setting[e_EQF2]) ? DoEQ2 = true : DoEQ2 = false;
    (0.0f != Setting[e_EQF3]) ? DoEQ3 = true : DoEQ3 = false;
    (0.0f != Setting[e_EQF4]) ? DoEQ4 = true : DoEQ4 = false;
    (0.0f != Setting[e_EQF5]) ? DoEQ5 = true : DoEQ5 = false;
    (0.0f != Setting[e_EQF6]) ? DoEQ6 = true : DoEQ6 = false;

    //R1.00 RESET our settings changed flag. 
    if (makoSettingsChanged == StartCount) makoSettingsChanged -= 1; //R4.00 A change is not being found, decrement so we dont get stuck in a loop.
    if (makoSettingsChanged < 0) makoSettingsChanged = 0;
}

//R4.00 Update the LAST setting and decrement our setting counter.
void MakoDist01AudioProcessor::Mako_UpdateLastVar(int idx)
{    
    Setting_Last[idx] = Setting[idx]; 

    //R4.00 Reset flag and update counter.
    if (SettingRqd[idx])
    {
        SettingRqd[idx] = false;
        makoSettingsChanged -= 1;
    }
}


//==============================================================================
bool MakoDist01AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MakoDist01AudioProcessor::createEditor()
{
    return new MakoDist01AudioProcessorEditor (*this);
}

//==============================================================================

//R4.00 DAW parameter writer.
void MakoDist01AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    //R1.00 Save our parameters to file/DAW.
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);    
}

//R4.00 DAW parameter reader.
void MakoDist01AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    //R1.00 Read our parameters from file/DAW.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));

    return;
}

//R4.00 Initialize some program variables.
void MakoDist01AudioProcessor::Mako_Program_Init()
{
    //*******************************************************************
    // MAKO VARIABLES
    //*******************************************************************
    //R2.00 Only call this once. It gets called everytime you "render" a track in a DAW. 
    if ((PrePlay) && (makoSampleRate == MakoDist01AudioProcessor::getSampleRate())) return;
    PrePlay = true;

    Mako_FX_Reverb_CalcSize();

    //R1.00 Get the SAMPLE RATE.
    //char SR[100]; 
    //sprintf(SR, "RATE: %f \n", MakoDist01AudioProcessor::getSampleRate());
    //MessageBox(0, SR, "Error", MB_ICONSTOP);
    makoSampleRate = MakoDist01AudioProcessor::getSampleRate();
    if (makoSampleRate < 21000) makoSampleRate = 48000;
    if (192000 < makoSampleRate) makoSampleRate = 48000;

    //R1.00 Calculate some rough decay subtraction values for peak tracking (compress,autowah,etc). 
    Release_100mS = (1.0f / .100f) * (1.0f / makoSampleRate);
    Release_200mS = (1.0f / .200f) * (1.0f / makoSampleRate);
    Release_300mS = (1.0f / .300f) * (1.0f / makoSampleRate);
    Release_400mS = (1.0f / .400f) * (1.0f / makoSampleRate);
    Release_500mS = (1.0f / .500f) * (1.0f / makoSampleRate);

    //R1.00 REVERB MODULATE Vars.
    RevMod_B_Rate_Up = 1.0f / makoSampleRate;       //R1.00 RATE How fast we traverse thru the modulation indexes.
    RevMod_B_Rate_Down = -1.0f / makoSampleRate;
    RevMod_B_Rate = RevMod_B_Rate_Up;               //R1.00 Starting rate.
    RevMod_B_Ring1[0] = 0;         //R1.00 index into buffer.
    RevMod_B_Ring1_Max[0] = 8000;  //R1.00 Size of buffer.
    RevMod_B_Offset[0] = 1190;     //R1.00 Where our mod index starts
    RevMod_B_Offset_Min[0] = 880;  //R1.00 580 - Min distance our index can get from the buffer index.
    RevMod_B_Offset_Max[0] = 1460; //R1.00 1860 - Max distance our index can get from the buffer index.
    RevMod_B_Ring1[1] = 0;
    RevMod_B_Ring1_Max[1] = 8000;
    RevMod_B_Offset[1] = 900;       //R1.00 We will modulate from 1mS (48) onward. 
    RevMod_B_Offset_Min[1] = 880;
    RevMod_B_Offset_Max[1] = 1460;

    //R1.00 DELAY Vars.
    Delay_B_Ring1[0] = 15000;
    Delay_B_Ring1_Max[0] = 15001;
    Delay_B_Ring1[1] = 30000;
    Delay_B_Ring1_Max[1] = 30001;
    
    //R1.00 MODULATE Vars - FLANGER.
    Mod1_B_Rate_Up = 1.0f / makoSampleRate;    //R4.00 RATE How fast we traverse thru the modulation indexes.
    Mod1_B_Rate_Down = -1.0f / makoSampleRate;
    Mod1_B_Rate[0] = Mod1_B_Rate_Up;           //R4.00 Starting rate.
    Mod1_B_Rate[1] = Mod1_B_Rate_Up;           //R4.00 Starting rate.
    Mod1_B_Ring1[0] = 0;                       //R4.00 index into buffer.
    Mod1_B_Ring1_Max[0] = 8000;                //R4.00 Size of buffer.
    Mod1_B_Offset[0] = 1600;                   //R4.00 Where our mod index starts
    Mod1_B_Offset_Min[0] = 280;                //R4.00 Min distance our index can get from the buffer index.
    Mod1_B_Offset_Max[0] = 1860;               //R4.00 Max distance our index can get from the buffer index.
    Mod1_B_Ring1[1] = 0;
    Mod1_B_Ring1_Max[1] = 8000;
    Mod1_B_Offset[1] = 800;       
    Mod1_B_Offset_Min[1] = 280;   
    Mod1_B_Offset_Max[1] = 1860;  
    
    //R4.00 Pedal 1 DC Removal.
    Filter_HP_Coeffs(50.0f, &fil_Mod1_DCRemoval);

    //R1.00 MODULATE 2 Vars - MOD FILTER.
    Mod2_B_Rate_Up = 1.0f / makoSampleRate;    //R4.00 RATE How fast we traverse thru the modulation indexes.
    Mod2_B_Rate_Down = -1.0f / makoSampleRate;
    Mod2_B_Rate[0] = Mod2_B_Rate_Up;           //R4.00 Starting rate left.
    Mod2_B_Rate[1] = Mod2_B_Rate_Up;           //R4.00 Starting rate right.
    
    //R4.00 MODULATE 3 Vars - CHORUS.
    Mod3_B_Rate_Up = 1.0f / makoSampleRate;    //R4.00 RATE How fast we traverse thru the modulation indexes.
    Mod3_B_Rate_Down = -1.0f / makoSampleRate;
    Mod3_B_Rate[0] = Mod3_B_Rate_Up;           //R4.00 Starting rate left.
    Mod3_B_Rate[1] = Mod3_B_Rate_Up;           //R4.00 Starting rate right.
    Mod3_B_Ring1[0] = 0;                       //R4.00 index into buffer.
    Mod3_B_Ring1_Max[0] = 8000;                //R4.00 Size of buffer.
    Mod3_B_Offset[0] = 1600;                   //R4.00 Where our mod index starts
    Mod3_B_Offset_Min[0] = 280;                //R4.00 Min distance our index can get from the buffer index.
    Mod3_B_Offset_Max[0] = 1860;               //R4.00 Max distance our index can get from the buffer index.
    Mod3_B_Ring1[1] = 0;
    Mod3_B_Ring1_Max[1] = 8000;
    Mod3_B_Offset[1] = 800;       
    Mod3_B_Offset_Min[1] = 280;   
    Mod3_B_Offset_Max[1] = 1860;  

    //R4.00 MODULATE 4 Vars - WIDE LOAD.
    Mod4_B_Ring1[0] = 0;                       //R4.00 index into buffer.
    Mod4_B_Ring1_Max[0] = 8000;                //R4.00 Size of buffer.
    Mod4_B_Ring1[1] = 0;
    Mod4_B_Ring1_Max[1] = 8000;

    //R4.00 Initial IR Cab.
    Mako_SetIR();    

    //R2.00 Calculate the default filter coefficients.
    Filter_HP_Coeffs(20.0f, &fil_Out_HP);          //R4.00 Hi Pass filter to block any DC we add in code.
    Filter_LP_Coeffs(1500.0f, &fil_Rev_LP);        //R4.00 Reverb Filters.
    Filter_HP_Coeffs(50.0f, &fil_Rev_HP);                  
        
    //R2.00 Pre Calc our AMPLIFIER EQs.
    Filter_LP_Coeffs(Setting[e_AmpLP], &fil_Amp_LP);
    Filter_FO_LP_Coeffs(150.0f, &fil_Amp_Thump);   //R4.00 THUMP
    Filter_HP_Coeffs(1500.0f, &fil_Amp_Air);       //R4.00 AIR
    Mako_EQ_Set_0();
   
}

//R4.00 Initialize variables, etc.
void MakoDist01AudioProcessor::Mako_Program_Construct()
{   
    Mako_Program_Init();

    //R4.00 Init some problem variables.
    Setting[e_AmpLP] = 2000.0f;
    Setting[e_Dist1Cut] = 2000.0f;
    Setting[e_Dist2Cut] = 2000.0f;
    Setting[e_DelMix] = 0.0f;
    Setting[e_RevMix] = 0.0f;

    //R3.00 Force strings to be "" since we trigger off of this in the LOAD routine.
    for (int t = 0; t < 200; t++) DefAmp_File[t] = "";
    Mako_ExternalData_Load(true);

    switch (int(Setting[e_AmpEQMode]))
    {
    case 0: Mako_EQ_Set_0(); break;
    case 1: Mako_EQ_Set_1(); break;
    case 2: Mako_EQ_Set_2(); break;
    case 3: Mako_EQ_Set_3(); break;
    case 4: Mako_EQ_Set_4(); break;
    case 5: Mako_EQ_Set_5(); break;
    case 6: Mako_EQ_Set_6(); break;
    case 7: Mako_EQ_Set_7(); break;
    case 8: Mako_EQ_Set_8(); break;
    case 9: Mako_EQ_Set_9(); break;
    case 10: Mako_EQ_Set_10(); break;
    case 11: Mako_EQ_Set_11(); break;
    case 12: Mako_EQ_Set_12(); break;
    case 13: Mako_EQ_Set_13(); break;
    case 14: Mako_EQ_Set_14(); break;
    case 15: Mako_EQ_Set_15(); break;
    case 16: Mako_EQ_Set_16(); break;
    case 17: Mako_EQ_Set_17(); break;
    case 18: Mako_EQ_Set_18(); break;
    case 19: Mako_EQ_Set_19(); break;
    }
    Mako_EQ_Set_Filters();

    makoEditorNeedsUpdated = 1;

    Mako_Program_Update(true);

    Mako_SettingsUpdate(false);    
}

//R4.00 Initialize some variables. 
void MakoDist01AudioProcessor::Mako_Program_Update(bool ForceUpdate)
{
    //R1.00 Get the SAMPLE RATE. Exit if unchanged since last run.
    if ((makoSampleRate == MakoDist01AudioProcessor::getSampleRate()) && (!ForceUpdate)) return;

    //R1.00 Get Sample Rate.
    makoSampleRate = MakoDist01AudioProcessor::getSampleRate();
    if (makoSampleRate < 21000) makoSampleRate = 48000;
    if (192000 < makoSampleRate) makoSampleRate = 48000;
    
    //R4.00 PreCalc some averaging ratios.
    //S = (5 * (1 / Ratio)) / SampRate
    RAT20ms = 1.0f / ((.02f * makoSampleRate) / 5.0f);  
    RATI20ms = 1.0f - RAT20ms;                          //.995 @ 48kHz 
    RAT50ms = 1.0f / ((.05f * makoSampleRate) / 5.0f);  
    RATI50ms = 1.0f - RAT50ms;                          //.997 @ 48kHz
    RAT100ms = 1.0f / ((.1f * makoSampleRate) / 5.0f);  
    RATI100ms = 1.0f - RAT100ms;                        //.9989 @ 48kHz
    RAT1000ms = 1.0f / ((1.0f * makoSampleRate) / 5.0f);
    RATI1000ms = 1.0f - RAT1000ms;                      //.99989 @ 48kHz

    //R1.00 REVERB MODULATE Vars.
    RevMod_B_Rate_Up = 1.0f / makoSampleRate;       //R1.00 RATE How fast we traverse thru the modulation indexes.
    RevMod_B_Rate_Down = -1.0f / makoSampleRate;
    RevMod_B_Rate = RevMod_B_Rate_Up;               //R1.00 Starting rate.
    
    //R1.00 MODULATE Vars.
    Mod1_B_Rate_Up = 1.0f / makoSampleRate;    //R1.00 RATE How fast we traverse thru the modulation indexes.
    Mod1_B_Rate_Down = -1.0f / makoSampleRate;
    Mod1_B_Rate[0] = Mod1_B_Rate_Up;           //R1.00 Starting rate.
    Mod1_B_Rate[1] = Mod1_B_Rate_Up;           //R1.00 Starting rate.
    
    //R1.00 MODULATE 2 Vars.
    Mod2_B_Rate_Up = 1.0f / makoSampleRate;    //R1.00 RATE How fast we traverse thru the modulation indexes.
    Mod2_B_Rate_Down = -1.0f / makoSampleRate;
    Mod2_B_Rate[0] = Mod2_B_Rate_Up;           //R1.00 Starting rate.
    Mod2_B_Rate[1] = Mod2_B_Rate_Up;           //R1.00 Starting rate.

    //R4.00 MODULATE 3 Vars.
    Mod3_B_Rate_Up = 1.0f / makoSampleRate;    //R1.00 RATE How fast we traverse thru the modulation indexes.
    Mod3_B_Rate_Down = -1.0f / makoSampleRate;
    Mod3_B_Rate[0] = Mod3_B_Rate_Up;           //R1.00 Starting rate.
    Mod3_B_Rate[1] = Mod3_B_Rate_Up;           //R1.00 Starting rate.
    
    //R4.00 MODULATE 4 Vars are not needed.
    
    //R2.00 Calculate the default filter coefficients.
    Filter_HP_Coeffs(20.0f, &fil_Out_HP);
    Filter_LP_Coeffs(1500.0f, &fil_Rev_LP);      //R2.00 NEEDS to use real freqs. 500-20k
    Filter_HP_Coeffs(50.0f, &fil_Rev_HP);        //R2.00 50-10k
        
    //R2.00 Pre Calc our AMPLIFIER EQs.
    Filter_LP_Coeffs(Setting[e_AmpLP], &fil_Amp_LP);
    Filter_FO_LP_Coeffs(150.0f, &fil_Amp_Thump);   
    Filter_HP_Coeffs(1500.0f, &fil_Amp_Air);       
    Filter_FO_HP_Coeffs(20.0f + ((1.0f - Setting_Last[e_AmpBoom]) * 1000.0f), &fil_Amp_Boom);
    Filter_FO_LP_Coeffs(500.0f + Setting_Last[e_AmpCrisp] * 5000.0f, &fil_Amp_Crisp);
        
    if (Amp_EQ1.Enabled) Filter_BP_Coeffs(Amp_EQ1.Gain * Amp_EQ1.RangedB, Amp_EQ1.Freq, Amp_EQ1.Q, &fil_Amp_EQ1);
    if (Amp_EQ2.Enabled) Filter_BP_Coeffs(Amp_EQ2.Gain * Amp_EQ2.RangedB, Amp_EQ2.Freq, Amp_EQ2.Q, &fil_Amp_EQ2);
    if (Amp_EQ3.Enabled) Filter_BP_Coeffs(Amp_EQ3.Gain * Amp_EQ3.RangedB, Amp_EQ3.Freq, Amp_EQ3.Q, &fil_Amp_EQ3);
    if (Amp_EQ4.Enabled) Filter_BP_Coeffs(Amp_EQ4.Gain * Amp_EQ4.RangedB, Amp_EQ4.Freq, Amp_EQ4.Q, &fil_Amp_EQ4);
    if (Amp_EQ5.Enabled) Filter_BP_Coeffs(Amp_EQ5.Gain * Amp_EQ5.RangedB, Amp_EQ5.Freq, Amp_EQ5.Q, &fil_Amp_EQ5);
    if (Amp_EQ6.Enabled) Filter_BP_Coeffs(Amp_EQ6.Gain * Amp_EQ6.RangedB, Amp_EQ6.Freq, Amp_EQ6.Q, &fil_Amp_EQ6);
   
    //R4.00 Setup EQ Pedal 
    Mako_FX_Setup_EQ(5);
}

//R4.00 Load our last used In/Out IRs from the Windows USER data dir for this app.
void MakoDist01AudioProcessor::Mako_ExternalData_Load(bool ForceUpdate)
{
    //R4.00 Have issues in DAW where this file is not loaded when instantiated.
    if (DefAmp_File[0] == "") MakoExternalData_Loaded = false;

    //R4.00 Only load at the start of the application.
    if ((!ForceUpdate) && (MakoExternalData_Loaded)) return;
    MakoExternalData_Loaded = true;

    //R4.00 AMPLIFIER database.
    DefAmp_File[0] = "*No amplifier";
    DefAmp_File[1] = "*Assman"; DefAmp_GainSetting[1] = 0;      DefAmp_PreGain[1] = true;
    DefAmp_File[2] = "*Train"; DefAmp_GainSetting[2] = 0;       DefAmp_PreGain[2] = true;
    DefAmp_File[3] = "*Tweedle"; DefAmp_GainSetting[3] = 0;     DefAmp_PreGain[3] = true;
    DefAmp_File[4] = "*Divine 13"; DefAmp_GainSetting[4] = 1;   DefAmp_PreGain[4] = true;
    DefAmp_File[5] = "*CL30"; DefAmp_GainSetting[5] = 1;        DefAmp_PreGain[5] = true;
    DefAmp_File[6] = "*Matches"; DefAmp_GainSetting[6] = 1;     DefAmp_PreGain[6] = true;
    DefAmp_File[7] = "*Dr Zzzs"; DefAmp_GainSetting[7] = 1;     DefAmp_PreGain[7] = true;
    DefAmp_File[8] = "*Dumbo"; DefAmp_GainSetting[8] = 2;       DefAmp_PreGain[8] = true;
    DefAmp_File[9] = "*AC30c"; DefAmp_GainSetting[9] = 2;       DefAmp_PreGain[9] = true;
    DefAmp_File[10] = "*Brit J45"; DefAmp_GainSetting[10] = 2;  DefAmp_PreGain[10] = true;
    DefAmp_File[11] = "*England 15"; DefAmp_GainSetting[11] = 2;
    DefAmp_File[12] = "*JM War 10"; DefAmp_GainSetting[12] = 2;
    DefAmp_File[13] = "*Bogo Sheba"; DefAmp_GainSetting[13] = 2;
    DefAmp_File[14] = "*Orangutan"; DefAmp_GainSetting[14] = 2;
    DefAmp_File[15] = "*JoSox"; DefAmp_GainSetting[15] = 2;
    DefAmp_File[16] = "*Reverend Red "; DefAmp_GainSetting[16] = 2;
    DefAmp_File[17] = "*Crank It"; DefAmp_GainSetting[17] = 2;
    DefAmp_File[18] = "*515 III"; DefAmp_GainSetting[18] = 2;
    DefAmp_File[19] = "*Bugs 6260"; DefAmp_GainSetting[19] = 2;
    for (int t = 20; t < 200; t++)
    {
        DefAmp_GainSetting[t] = 2;
    }

    DefIR_File[0] = "*None";
    DefIR_File[1] = "*AssMan";
    DefIR_File[2] = "*Train";
    DefIR_File[3] = "*Tweedle";
    DefIR_File[4] = "*Divine 13";
    DefIR_File[5] = "*CL30";
    DefIR_File[6] = "*Matches";
    DefIR_File[7] = "*Dr Zzzs";
    DefIR_File[8] = "*DumbO";
    DefIR_File[9] = "*AC30c";
    DefIR_File[10] = "*Brit J45";
    DefIR_File[11] = "*England 15";
    DefIR_File[12] = "*JM War 10";
    DefIR_File[13] = "*BoGo Sheba";
    DefIR_File[14] = "*Orangutan";
    DefIR_File[15] = "*JoSoX";
    DefIR_File[16] = "*Reverend Red";
    DefIR_File[17] = "*Crank It";
    DefIR_File[18] = "*515 III";
    DefIR_File[19] = "*bug 6260";
    for (int t = 20; t < 200; t++) DefIR_File[20] = "";

    //R4.00 Read in the Amp and Speaker IR database.
    juce::File f = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getFullPathName() + "\\MakoMiniD\\ExternalData.txt";

    if (f.existsAsFile())
    {
        int Frev = 0;
        bool FileValid = false;
        juce::FileInputStream Input(f);
        {
            Input.setPosition(0);

            //R4.00 Get the file revision.
            auto b = Input.readNextLine();        
            if (b == "MINID_SAVE_400") Frev = 400;

            //R4.00 If we have a valid file, read the data.
            if (Frev < 400)
                FileValid = false;
            else
            {                
                b = Input.readNextLine();    //R4.00 Do not edit this file notice.

                //R4.00 Read in the AMPLIFIER DATABASE.
                if (399 < Frev)
                {                    
                    b = Input.readNextLine();      //R4.00 AMP header.
                    for (int t = 0; t < 200; t++)
                    {
                        b = Input.readNextLine();
                        if (19 < t)
                        {
                            DefAmp_Path[t] = b;
                            DefAmp_File[t] = DefAmp_Path[t].fromLastOccurrenceOf("\\", false, true);
                        }
                        
                        b = Input.readNextLine();
                        if (19 < t) DefAmp_GainSetting[t] = Clip_Int(b.getFloatValue(), 0, 2);
                        
                        b = Input.readNextLine();
                        if (19 < t)
                        {
                            DefAmp_PreGain[t] = false;
                            if (Clip_Int(b.getFloatValue(), 0, 1))DefAmp_PreGain[t] = true;
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
                            DefIR_Path[t] = b;
                            DefIR_File[t] = DefIR_Path[t].fromLastOccurrenceOf("\\", false, true);
                        }
                    }
                    b = Input.readNextLine();

                }

                //R4.00 We have successfully loaded the file.
                FileValid = true;
            }
        }
                
    }

    return;
}

//R4.00 Save the Amp and IR database file.
void MakoDist01AudioProcessor::Mako_ExternalData_Save()
{   
    //R2.23 We need to create a data directory to save our large STRING data. Strings are NOT suitable for host parameters (bool,float).
    juce::File fDir = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getFullPathName() + "\\MakoMiniD";
    juce::Result tRes(fDir.createDirectory());

    //R2.23 Did our directory get created?
    if (!fDir.isDirectory()) return;

    //R2.23 We have a working data directory to store data, save data.
    juce::File f = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getFullPathName() + "\\MakoMiniD\\ExternalData.txt";
    juce::FileOutputStream output(f);
    {
        //R4.00 Over write any data.
        output.setPosition(0);
        output.truncate();

        //R4.00 Mako IR PATHS file, version 0.
        output.writeText("MINID_SAVE_400\r\n", false, false, nullptr);
        output.writeText("DO NOT EDIT THIS FILE - Initial IR and WAVE file data that is not stored in VST data.\r\n", false, false, nullptr);

        //R4.00 Store Amplifier List.
        output.writeText(std::string("AMPLIFIER DATABASE") + "\r\n", false, false, nullptr);
        for (int t = 0; t < 200; t++)
        {
            output.writeText(DefAmp_Path[t] + "\r\n", false, false, nullptr);
            output.writeText(std::to_string(DefAmp_GainSetting[t]) + "\r\n", false, false, nullptr);
            output.writeText(std::to_string(DefAmp_PreGain[t]) + "\r\n", false, false, nullptr);
        }
        output.writeText("\r\n", false, false, nullptr);

        //R4.00 Store Speaker List.
        output.writeText(std::string("IR DATABASE") + "\r\n", false, false, nullptr);
        for (int t = 0; t < 200; t++)
        {
            output.writeText(DefIR_Path[t] + "\r\n", false, false, nullptr);
        }
        output.writeText("\r\n", false, false, nullptr);
       
    }
}

//R4.00 Tool to limit a float value to a certain range of numbers.
float MakoDist01AudioProcessor::Clip_Float(float val, float min, float max)
{
    float val2 = val;
    if (val < min)
        val2 = min;
    else if (max < val)
        val2 = max;

    return val2;
}

//R4.00 Tool to limit an integer value to a certain range of numbers.
int MakoDist01AudioProcessor::Clip_Int(int val, int min, int max)
{
    int val2 = val;
    if (val < min)
        val2 = min;
    else if (max < val)
        val2 = max;

    return val2;
}

//R4.00 Set our current Amp IR to a preprogrammed Amp IR or a user defined IR from InputEQ_FromFile.
void MakoDist01AudioProcessor::Mako_AMP_Setup()
{
    //R3.00 Load the IR array with the selected Bult-in amp or file data.
    switch (int(Setting[e_AmpChan]))
    {
    case 0: //R2.22 Not used. Code here just in case to catch a bug.        
        for (int t = 0; t < 1024; t++) InputEQ[t] = 0.0f;
        InputEQ[0] = .9f;
        InputEQ_IRGain = .9f;
        InputEQ_GainRange = 20.0f;
        InputEQ_PreEQ = false;
        break;
    case 1://R4.00 Assman
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_Bassman[t];
        InputEQ_IRGain = .3f;
        InputEQ_GainRange = 400.0f;
        InputEQ_PreEQ = true;
        break;
    case 2://R4.00 Trained
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_Train[t];
        InputEQ_IRGain = .05f;
        InputEQ_GainRange = 2000.0f;
        InputEQ_PreEQ = true;
        break;
    case 3://R4.00 Tweedle
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_Tweed[t];
        InputEQ_IRGain = .05f;
        InputEQ_GainRange = 2000.0f;
        InputEQ_PreEQ = true;
        break;
    case 4://R4.00 Divine
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_Div13[t];
        InputEQ_IRGain = .05f;
        InputEQ_GainRange = 10000.0f;
        InputEQ_PreEQ = true;
        break;
    case 5://R4.00 CL30
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_CL30[t];
        InputEQ_IRGain = .05f;
        InputEQ_GainRange = 5000.0f;
        InputEQ_PreEQ = true;
        break;
    case 6://R4.00 Matches Night
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_MatNight[t];
        InputEQ_IRGain = .05f;
        InputEQ_GainRange = 10000.0f;
        InputEQ_PreEQ = true;
        break;    
    case 7://R4.00 Dr Zzzs
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_DrZ[t];
        InputEQ_IRGain = .04f;
        InputEQ_GainRange = 20000.0f;
        InputEQ_PreEQ = true;
        break;
    case 8://R4.00 Dumbo
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_Dumbo[t];
        InputEQ_IRGain = .04f;
        InputEQ_GainRange = 50000.0f;
        InputEQ_PreEQ = true;
        break; 
    case 9://R4.00 AC30c
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_AC30c[t];
        InputEQ_IRGain = .04f;
        InputEQ_GainRange = 20000.0f;
        InputEQ_PreEQ = true;
        break;
    case 10://R4.00 BritJ45
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_AC30c[t];
        InputEQ_IRGain = .04f;
        InputEQ_GainRange = 50000.0f;
        InputEQ_PreEQ = true;
        break;
    case 11://R4.00 England 15
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_EnglGM15[t];
        InputEQ_IRGain = .04f;
        InputEQ_GainRange = 20000.0f;
        InputEQ_PreEQ = true;
        break;
    case 12://R4.00 JM War 10
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_VM410[t];
        InputEQ_IRGain = .3f;
        InputEQ_GainRange = 1000.0f;
        InputEQ_PreEQ = false;
        break;
    case 13://R4.00 Bogo Sheba
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_BogoSheba[t];
        InputEQ_IRGain = .3f;
        InputEQ_GainRange = 1000.0f;
        InputEQ_PreEQ = false;
        break;        
    case 14://R4.00 Bogo Orangutan
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_Orangutan[t];
        InputEQ_IRGain = .3f;
        InputEQ_GainRange = 1000.0f;
        InputEQ_PreEQ = false;
        break;
    case 15: //R4.00 JoSox
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_JSX[t];
        InputEQ_IRGain = .3f;
        InputEQ_GainRange = 1000.0f;
        InputEQ_PreEQ = false;
        break;     
    case 16: //R4.00 Reverend Red
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_RevRed[t];
        InputEQ_IRGain = .3f;
        InputEQ_GainRange = 1000.0f;
        InputEQ_PreEQ = false;
        break;
    case 17: //R4.00 CrankIt
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_Krank[t];
        InputEQ_IRGain = .3f;
        InputEQ_GainRange = 1000.0f;
        InputEQ_PreEQ = false;
        break;    
    case 18: //R4.00 5153
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_5153[t];
        InputEQ_IRGain = .3f;
        InputEQ_GainRange = 1000.0f;
        InputEQ_PreEQ = false;
        break;
    case 19: //R4.00 Bug 6260
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_Bug6260[t];
        InputEQ_IRGain = .3f;
        InputEQ_GainRange = 5000.0f;
        InputEQ_PreEQ = false;
        break;    
    default:
        for (int t = 0; t < 1024; t++) InputEQ[t] = InputEQ_FromFile[t];        
        break;
    }

    //R4.00 Clear the IR buffers or we get clicks.
    for (int t = 0; t < 1024; t++)
    {
        InEQ_AudioBuffer[0][t] = 0.0f;
        InEQ_AudioBuffer[1][t] = 0.0f;
    }

    //R4.00 Flag the editor it needs to update the PRE EQ indicator.
    makoEditorNeedsUpdated = 1;
}


//R4.00 Initialize the Mod1 Flanger effect.
void MakoDist01AudioProcessor::Mako_FX_Setup_Mod1()
{
    Mod1_B_Rate[0] = Mod1_B_Rate_Up; Mod1_B_Rate[1] = Mod1_B_Rate_Up;
    Mod1_B_Offset[0] = 800.0f; Mod1_B_Offset[1] = 1000.0f;                                    //R4.00 Spread out offsets for Stereo effect.
    Mod1_B_Offset_Min[0] = 280.0f + (Setting[e_Mod1Depth] * 200.0f);                              //R4.00 Chorus buffer start position 280 - 480.
    Mod1_B_Offset_Min[1] = 280.0f + (Setting[e_Mod1Depth] * 200.0f);
    Mod1_B_Offset_Max[0] = Mod1_B_Offset_Min[0] + 200.0f + (Setting[e_Mod1Depth] * 2200.0f);  //R4.00 Chorus buffer End position 480 - 2880.
    Mod1_B_Offset_Max[1] = Mod1_B_Offset_Min[1] + 200.0f + (Setting[e_Mod1Depth] * 2200.0f);       
}

//R4.00 Initialize the Mod2 Mod Filter effect.
void MakoDist01AudioProcessor::Mako_FX_Setup_Mod2()
{
    Mod2_Wave_Ramp[0] = Mod2_B_Rate_Up * Setting[e_Mod2Rate] * 10.0f;
    Mod2_Wave_Ramp[1] = Mod2_Wave_Ramp[0];
}

//R4.00 Initialize the Mod3 Chorus effect.
void MakoDist01AudioProcessor::Mako_FX_Setup_Mod3()
{
    Mod3_B_Rate[0] = Mod3_B_Rate_Up; Mod3_B_Rate[1] = Mod3_B_Rate_Up;
    Mod3_B_Offset[0] = 800.0f; Mod3_B_Offset[1] = 1000.0f;                                    //R4.00 Spread out offsets for Stereo effect.
    Mod3_B_Offset_Min[0] = 280.0f + (Setting[e_Mod3Depth] * 200.0f);                          //R4.00 Chorus buffer start position 280 - 480.
    Mod3_B_Offset_Min[1] = 280.0f + (Setting[e_Mod3Depth] * 200.0f);
    Mod3_B_Offset_Max[0] = Mod3_B_Offset_Min[0] + 200.0f + (Setting[e_Mod3Depth] * 2200.0f);  //R4.00 Chorus buffer End position = Start + 200 - 2400.
    Mod3_B_Offset_Max[1] = Mod3_B_Offset_Min[1] + 200.0f + (Setting[e_Mod3Depth] * 2200.0f);

    //R4.00 Stereo effect so adjust balance.
    if (Setting[e_Mod3Bal] < .5f)
    {
        Mod3_BalLR[0] = 1.0f;
        Mod3_BalLR[1] = Setting[e_Mod3Bal] * 2;
    }
    else
    {
        Mod3_BalLR[0] = 1.0f - ((Setting[e_Mod3Bal] - .5f) * 2.0f);
        Mod3_BalLR[1] = 1.0f;
    }
}

//R4.00 Initialize the Mod4 Wide Load effect.
void MakoDist01AudioProcessor::Mako_FX_Setup_Mod4()
{
    //R4.00 Apply FREQ Offset by making a stereo Mid F adjustment.
    Filter_BP_Coeffs((Setting[e_Mod4Foff]) * 6.0f, 500.0f, .35f, &filMod4[0]);
    Filter_BP_Coeffs((0.0f - Setting[e_Mod4Foff]) * 6.0f, 500.0f, .35f, &filMod4[1]);

    //R4.00 Stereo effect so adjust balance.
    if (Setting[e_Mod4Bal] < .5f)
    {
        Mod4_BalLR[0] = 1.0f;
        Mod4_BalLR[1] = Setting[e_Mod4Bal] * 2;
    }
    else
    {
        Mod4_BalLR[0] = 1.0f - ((Setting[e_Mod4Bal] - .5f) * 2.0f);
        Mod4_BalLR[1] = 1.0f;
    }
}

//R4.00 The heart of MD2. This func applies an IR to incoming samples that represents
//R4.00 the amplifier preamp EQ. It also applies gain, thinning, etc.
float MakoDist01AudioProcessor::Mako_Amp_InputIR(float tSample, int channel)
{
    int T1;
    int BitMask = Amp_Qual_Max - 1;
    float V = 0.0f;
    float V2 = 0.0f;
    float tComp;
    float tS_Peak;
    float tS = tSample; 
    float Vmix;

    //R2.22 Apply our Tube Thinner. Reduce volume during heavy distortion to reduce ghost notes.
    tS *= ((1.0f - Setting[e_AmpThin]) + (Amp_Thin_Adj * Setting[e_AmpThin]));

    //R4.00 Get our current buffer index and store the new sample in our buffer.
    T1 = InEQB_Ring1[channel];
    InEQ_AudioBuffer[channel][T1] = tS;

    //R2.22 Apply input EQ IR to the incoming sample.
    for (int t = 0; t < Amp_Qual_Max; t++)
    {
        V += (InEQ_AudioBuffer[channel][T1] * InputEQ[t]);

        //R1.00 Increment index, mask off bits past 1023 to keep our index between 0-1023.
        T1 = (T1 + 1) & BitMask;
    }

    //R1.00 Decrement our ring buffer index and loop around at 0. 
    InEQB_Ring1[channel]--;
    if (InEQB_Ring1[channel] < 0) InEQB_Ring1[channel] = BitMask;
    
    //R4.00 Store the CLEAN signal after the IR is applied so phase is correct.
    Vmix = tSample * (Setting[e_AmpClean] * 10.0f);

    //R2.00 Apply Drive.
    if (0.0f < Setting[e_AmpGain])
    {
        float AG = Setting[e_AmpGain] * Setting[e_AmpGain];

        //R4.00 Apply the Hard clipping gain.
        V2 = V * (1.0f + (AG * InputEQ_GainRange));
        tS_Peak = abs(V2);
        
        //R4.00 TUBE THINNING / COMPRESSION
        //R4.00 Calc compression from excursion of max volume sample 1.0.
        if (0.0f < Setting[e_AmpThin])
        {
            if (tS_Peak < 1.0f)
                tComp = 1.0f;
            else
                tComp = .1f / tS_Peak;        
            if (tComp < .001f) tComp = .001f; 

            Amp_Thin_Adj = (Amp_Thin_Adj * RATI100ms) + (tComp * RAT100ms);
            
            if (Amp_Thin_Adj < .1f) Amp_Thin_Adj = .1f; //R4.00 Was .5
        }
        
        //R4.00 Hard Clipping.
        if (.9999f < V2)
            V2 = .9999f;
        else if (V2 < -.9999f)
            V2 = -.9999f;

        //R4.00 Apply Soft clipping.
        V = tanhf(V * (1.0f + (AG * InputEQ_GainRange)));
        
        //R4.00 MIX Hard and Soft clipping styles.
        V = ((1.0f - Setting[e_AmpSlope]) * V2) + (Setting[e_AmpSlope] * V);        
    }

    return (V * InputEQ_IRGain) + Vmix;
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MakoDist01AudioProcessor();
}

//R4.00 Slightly optimized filter coefficients code for WAH since it gets called constantly.
void MakoDist01AudioProcessor::Mako_Calc_coeffs_Wah(float Q, float fc, int chan)
{    
    float K = pi2 * (fc * .5f) / makoSampleRate;
    float K2 = K * K;
    float V0 = 32.0f; // LinearGain_20dB;
    
    float dd = 1.0f / (1.0f + K / Q + K2);

    WAH_coeffs[chan].a0 = (1.0f + (V0 * K) / Q + K2) * dd;   //a
    WAH_coeffs[chan].a1 = (2.0f * (K2 - 1.0f)) * dd;         //b  
    WAH_coeffs[chan].a2 = (1.0f - (V0 * K) / Q + K2) * dd;   //g
    WAH_coeffs[chan].b1 = WAH_coeffs[chan].a1;               //b
    WAH_coeffs[chan].b2 = (1.0f - K / Q + K2) * dd;          //d
    //WAH_coeffs.c0 = 1.0f;
    //WAH_coeffs.d0 = 0.0f;
}

//R4.00 Slightly optimized filter coefficients code for Modulated Filter (MWAH) since it gets called constantly.
void MakoDist01AudioProcessor::Mako_Calc_coeffs_MWah(float Q, float fc, int chan)
{
    //R4.00 This filter code is slightly optimized for speed since it gets called constantly.
    float K = pi2 * (fc * .5f) / makoSampleRate;
    float K2 = K * K;
    float V0 = 32.0f; // LinearGain_20dB;

    float dd = 1.0f / (1.0f + K / Q + K2);

    MWAH_coeffs[chan].a0 = (1.0f + (V0 * K) / Q + K2) * dd;   //a
    MWAH_coeffs[chan].a1 = (2.0f * (K2 - 1.0f)) * dd;         //b  
    MWAH_coeffs[chan].a2 = (1.0f - (V0 * K) / Q + K2) * dd;   //g
    MWAH_coeffs[chan].b1 = MWAH_coeffs[chan].a1;               //b
    MWAH_coeffs[chan].b2 = (1.0f - K / Q + K2) * dd;          //d
    //MWAH_coeffs.c0 = 1.0f;
    //MWAH_coeffs.d0 = 0.0f;
}


//***************************************************************************************************************
//R4.00 GENERIC FILTER FUNCTIONS
//***************************************************************************************************************
//R4.00 This func applies the filter to a sample using coefficients calculated else where.
float MakoDist01AudioProcessor::Filter_Calc_BiQuad(float tSample, int channel, tp_filter* fn)
{
    float tS = tSample;

    fn->xn0[channel] = tS;
    tS = fn->a0 * fn->xn0[channel] + fn->a1 * fn->xn1[channel] + fn->a2 * fn->xn2[channel] - fn->b1 * fn->yn1[channel] - fn->b2 * fn->yn2[channel];
    fn->xn2[channel] = fn->xn1[channel]; fn->xn1[channel] = fn->xn0[channel]; fn->yn2[channel] = fn->yn1[channel]; fn->yn1[channel] = tS;

    return tS;
}

//R4.00 This version reduces calcs for when the filter is not being applied but the buffers should still be updated.
float MakoDist01AudioProcessor::Filter_Calc_BiQuad_Bypass(bool DoCalcs, float tSample, int channel, tp_filter* fn)
{
    float tS = tSample;

    fn->xn0[channel] = tS;
    if (DoCalcs) tS = fn->a0 * fn->xn0[channel] + fn->a1 * fn->xn1[channel] + fn->a2 * fn->xn2[channel] - fn->b1 * fn->yn1[channel] - fn->b2 * fn->yn2[channel];
    fn->xn2[channel] = fn->xn1[channel]; fn->xn1[channel] = fn->xn0[channel]; fn->yn2[channel] = fn->yn1[channel]; fn->yn1[channel] = tS;

    return tS;
}

//R4.00 Second order parametric/peaking boost filter with constant-Q
void MakoDist01AudioProcessor::Filter_BP_Coeffs(float Gain_dB, float Fc, float Q, tp_filter* fn)
{
    float K = pi2 * (Fc * .5f) / makoSampleRate;
    fn->K = K;
    float K2 = K * K;
    float V0 = pow(10.0, Gain_dB / 20.0);
    float GF = (V0 * K) / Q;
    float KQ = K / Q;

    float a = (1.0f + GF) + K2;
    float b = 2.0f * (K2 - 1.0f);
    float g = (1.0f - GF) + K2;
    float d = (1.0f - KQ) + K2;
    float dd = 1.0f / (1.0f + KQ + K2);

    fn->a0 = a * dd;
    fn->a1 = b * dd;
    fn->a2 = g * dd;
    fn->b1 = b * dd;
    fn->b2 = d * dd;
    fn->c0 = 1.0f;
    fn->d0 = 0.0f;
}

//R4.00 Optimized for speed when being called continuously.
void MakoDist01AudioProcessor::Filter_BP_Coeffs_GainOnly(float Gain_dB, float Fc, float Q, tp_filter* fn)
{
    //R4.00 Second order parametric/peaking boost filter with constant-Q
    float K2 = fn->K * fn->K;
    float V0 = pow(10.0, Gain_dB / 20.0);
    float GF = (V0 * fn->K) / Q;
    float KQ = fn->K / Q;

    float a = (1.0f + GF) + K2;
    float g = (1.0f - GF) + K2;
    float dd = 1.0f / (1.0f + KQ + K2);

    fn->a0 = a * dd;
    fn->a2 = g * dd;    
}

//R4.00 Second order LOW PASS filter. 
void MakoDist01AudioProcessor::Filter_LP_Coeffs(float fc, tp_filter* fn)
{ 
    float c = 1.0f / (tanf(pi * fc / makoSampleRate));
    fn->a0 = 1.0f / (1.0f + sqrt2 * c + (c * c));
    fn->a1 = 2.0f * fn->a0;
    fn->a2 = fn->a0;
    fn->b1 = 2.0f * fn->a0 * (1.0f - (c * c));
    fn->b2 = fn->a0 * (1.0f - sqrt2 * c + (c * c));
}

//F4.00 Second order butterworth High Pass.
void MakoDist01AudioProcessor::Filter_HP_Coeffs(float fc, tp_filter* fn)
{    
    float c = tanf(pi * fc / makoSampleRate);
    fn->a0 = 1.0f / (1.0f + sqrt2 * c + (c * c));
    fn->a1 = -2.0f * fn->a0;
    fn->a2 = fn->a0;
    fn->b1 = 2.0f * fn->a0 * ((c * c) - 1.0f);
    fn->b2 = fn->a0 * (1.0f - sqrt2 * c + (c * c));
}

//R4.00 Shelfs do not work in normal fashion.
void MakoDist01AudioProcessor::Filter_Shelf_HP_Coeffs(float Gain_dB, float fc, tp_filter* fn)
{
    float th = 2.0 * pi * fc / makoSampleRate;
    float m = pow(10.0, Gain_dB / 20.0);
    float b = (1.0 + m) / 4.0;
    float d = b * tan(th / 2.0);
    float g = (1.0 - d) / (1.0 + d);
    fn->a0 = (1.0 + g) / 2.0;
    fn->a1 = -((1.0 + g) / 2.0);
    fn->a2 = 0.0;
    fn->b1 = -g;
    fn->b2 = 0.0;
    fn->c0 = m - 1.0;
    fn->d0 = 1.0;    
}

//R4.00 Shelfs do not work in normal fashion.
void MakoDist01AudioProcessor::Filter_Shelf_LP_Coeffs(float Gain_dB, float fc, tp_filter* fn)
{
    float th = 2.0 * pi * fc / makoSampleRate;
    float m = pow(10.0, Gain_dB / 20.0);
    float b = 4.0 / (1.0 + m);
    float d = b * tan(th / 2.0);
    float g = (1.0 - d) / (1.0 + d);
    fn->a0 = (1.0 - g) / 2.0;
    fn->a1 = (1.0 - g) / 2.0;
    fn->a2 = 0.0;
    fn->b1 = -g;
    fn->b2 = 0.0;
    fn->c0 = m - 1.0;
    fn->d0 = 1.0;    
}

//R4.00 First Order High Pass.
void MakoDist01AudioProcessor::Filter_FO_HP_Coeffs(float fc, tp_filter* fn)
{
        float th = 2.0 * pi * fc / makoSampleRate;
        float g = cos(th) / (1.0 + sin(th));
        fn->a0 = (1.0 + g) / 2.0;
        fn->a1 = -((1.0 + g) / 2.0);
        fn->a2 = 0.0f;
        fn->b1 = -g;
        fn->b2 = 0.0f;
        fn->c0 = 0.0f;
        fn->d0 = 0.0f;
}

//R4.00 First Order Low Pass.
void MakoDist01AudioProcessor::Filter_FO_LP_Coeffs(float fc, tp_filter* fn)
{
    float th = 2.0 * pi * fc / makoSampleRate;
    float g = cos(th) / (1.0 + sin(th));
    fn->a0 = (1.0 - g) / 2.0;
    fn->a1 = (1.0 - g) / 2.0;
    fn->a2 = 0.0;
    fn->b1 = -g;
    fn->b2 = 0.0;
    fn->c0 = 0.0f;
    fn->d0 = 0.0f;
}

//*****************************************************************
// F X   S E T T I N G   P R O C E S S I N G
//*****************************************************************
//R4.00 Pedal 1 - NOISE GATE limit calculations.
void MakoDist01AudioProcessor::Mako_FX_Setup_NoiseGate(int pos)
{
    //R4.00 Precalc some LIMITs to speed up processing.
    makoPedal[pos].Var3[0] = Setting[e_GateAvg] * Setting[e_GateAvg] * Setting[e_GateAvg];     //R4.00 AVG Limit.  
    makoPedal[pos].Var4[0] = Setting[e_GateFreq] * Setting[e_GateFreq] * Setting[e_GateFreq];  //R4.00 Freq Limit.
    makoPedal[pos].Var7[0] = 100.0f + (Setting[e_GateWahRange] * 50000.0f);                    //R4.00 Range.
}

//R4.00 Pedal 2 - COMPRESSOR frequency definitions.
void MakoDist01AudioProcessor::Mako_FX_Setup_Compressor(int pos)
{
    //R4.00 Calc the THRESHOLD limit.
    makoPedal[pos].Var1[0] = Setting[e_CompThr] * Setting[e_CompThr] * Setting[e_CompThr];

    //R4.00 Future Use.
    //Filter_FO_LP_Coeffs(50.0f + (Setting[id + 9] * 2000.0f), &makoPedal[pos].Filter1);
    //Filter_FO_HP_Coeffs(50.0f + (Setting[id + 10] * 5000.0f), &makoPedal[pos].Filter2);
}

//R4.00 Pedal 3 - DISTORTION 1 frequency definitions.
void MakoDist01AudioProcessor::Mako_FX_Setup_Distortion1(int pos)
{
    Filter_BP_Coeffs(Setting[e_Dist1Mid] * 30.0f, Setting[e_Dist1MidF], .1f + (Setting[e_Dist1MidQ] * 2.9f), &makoPedal[pos].Filter1);
    Filter_LP_Coeffs(Setting[e_Dist1Cut], &makoPedal[pos].Filter2);
    //Filter_FO_HP_Coeffs(250.0f, &makoPedal[pos].Filter3);
}

//R4.00 Pedal 4 - DISTORTION 2 frequency definitions.
void MakoDist01AudioProcessor::Mako_FX_Setup_Distortion2(int pos)
{
    Filter_BP_Coeffs(Setting[e_Dist2Mid] * 30.0f, Setting[e_Dist2MidF], .1f + (Setting[e_Dist2MidQ] * 2.9f), &makoPedal[pos].Filter1);
    Filter_LP_Coeffs(Setting[e_Dist2Cut], &makoPedal[pos].Filter2);
}

//R4.00 Pedal 5 - SIX BAND EQ frequency definitions.
void MakoDist01AudioProcessor::Mako_FX_Setup_EQ(int pos)
{
    Filter_BP_Coeffs(Setting[e_EQF1], 100.0f,   .35f, &makoPedal[pos].Filter1); //R4.00 Bass
    Filter_BP_Coeffs(Setting[e_EQF2], 350.0f,  .707f, &makoPedal[pos].Filter2); //R4.00 Low Mid
    Filter_BP_Coeffs(Setting[e_EQF3], 700.0f,   .35f, &makoPedal[pos].Filter3); //R4.00 Mid 1
    Filter_BP_Coeffs(Setting[e_EQF4], 1400.0f, .707f, &makoPedal[pos].Filter4); //R4.00 Mid 2
    Filter_BP_Coeffs(Setting[e_EQF5], 2100.0f, .707f, &makoPedal[pos].Filter5); //R4.00 High
    Filter_BP_Coeffs(Setting[e_EQF6], 5000.0f,  .35f, &makoPedal[pos].Filter6); //R4.00 Presence
}

//*****************************************************************
// FX   A U D I O   P R O C E S S I N G
//*****************************************************************
//R4.00 Pedal 1 - Apply the triple NOISE GATE and AUTOWAH effects to the incoming sample.
float MakoDist01AudioProcessor::Mako_FX_Audio_NoiseGate(float tSample, int channel, int pos)
{
    float tS = tSample;
    float Gain;

    //R4.00 Apply Downward expander style gate.
    Gain = (1.0f - Setting[e_GateExp]) + (tS * tS * Setting[e_GateExp]);
    tS *= Gain;

    //R4.00 Apply Average volume gate.
    makoPedal[pos].Var1[channel] = (makoPedal[pos].Var1[channel] * RATI50ms) + (abs(tS) * RAT50ms);

    //R4.00 AutoWAH.
    //if (0.001f < Setting[e_GateWah]) tS = Mako_FX_Audio_AutoWah_Avg(tS, makoPedal[idx].Var1[channel], channel);
    if (0.001f < Setting[e_GateWah]) tS = Mako_FX_Audio_AutoWah_Peak(tS, channel, 1);

    //R4.00 AVERAGE signal level gate.
    if (makoPedal[pos].Var1[channel] < makoPedal[pos].Var3[0])
    {
        Gain = 1.0f;
        Gain = 1.0f - ((makoPedal[pos].Var3[0] - makoPedal[pos].Var1[channel]) / makoPedal[pos].Var3[0]);
        tS *= Gain;
    }
    
    //R4.00 FREQUENCY filter gate.
    if (makoPedal[pos].Var1[channel] < makoPedal[pos].Var4[0])
    {        
        Gain = 1.0f - ((makoPedal[pos].Var4[0] - makoPedal[pos].Var1[channel]) / makoPedal[pos].Var4[0]);  //R4.00 Calc how much to filter.
        if (Gain < 0.0f) Gain = 0.0f;                                                                      //R4.00 Verify we dont go negative.
        tS = (tS * Gain) + (makoPedal[pos].Var2[channel] * (1.0f - Gain));                                 //R4.00 Apply Low Pass filter.
        makoPedal[pos].Var2[channel] = tS;                                                                 //R4.00 Store our last sample for filter calc.
    }    

    //R4.00 Volume/Gain adjust. Should be first so we can control input vol, but then
    //R4.00 changes will affect Gate settings and throw everything off.
    return tS * Setting[e_GateVol] * 5.0f;
}

//R4.00 Pedal 1 - Apply the GATE AUTO WAH effect to the incoming sample.
float MakoDist01AudioProcessor::Mako_FX_Audio_AutoWah_Peak(float tSample, int channel, int pos)
{
    float tS = abs(tSample);
    float Temp_F;
    int c = channel;

    //R4.00 Track our peak signal. Fast On / Slow Off.
    //R4.00 makoPedal[1].Var5[c] = Peak
    //R4.00 makoPedal[1].Var6[c] = Last Peak, Only used to track changes in sample to trigger recalcs.
    //R4.00 makoPedal[1].Var7[c] = The range of freqs to sweep.
    if (makoPedal[pos].Var5[c] < tS)
    {
        //R4.00 Fast On
        makoPedal[pos].Var5[c] = tS;
        if (1.0f < makoPedal[pos].Var5[c]) makoPedal[pos].Var5[c] = 1.0f;   //R1.00 Clip here so we do not do broken math below. 
    }
    else
    {
        //R4.00 Slow Off.
        makoPedal[pos].Var5[c] -= Release_500mS;
        if (makoPedal[pos].Var5[c] < 0.0f) makoPedal[pos].Var5[c] = 0.0f;
    }

    //R4.00 Try not to do calcs so often. Trigger off the size of the signal change.
    //R4.00 If the WAH is warbling, get rid of this check and calc every sample. 
    if (.00005f < abs(makoPedal[pos].Var5[c] - makoPedal[pos].Var6[c]))
    {
        makoPedal[pos].Var6[c] = makoPedal[pos].Var5[c];
        Temp_F = 400.0f + (makoPedal[pos].Var5[c] * makoPedal[pos].Var7[c]);
        if (15000 < Temp_F) Temp_F = 15000;
        Mako_Calc_coeffs_Wah(Setting[e_GateWahQ], Temp_F, c);
    }

    WAH_xn[c] = tSample;
    tS = WAH_coeffs[c].a0 * WAH_xn[c] + WAH_coeffs[c].a1 * WAH_xnz1[c] + WAH_coeffs[c].a2 * WAH_xnz2[c] - WAH_coeffs[c].b1 * WAH_ynz1[c] - WAH_coeffs[c].b2 * WAH_ynz2[c];
    WAH_xnz2[c] = WAH_xnz1[c]; WAH_xnz1[c] = WAH_xn[c]; WAH_ynz2[c] = WAH_ynz1[c]; WAH_ynz1[c] = tS;

    return (tSample * (1.0f - Setting[e_GateWah])) + (tS * Setting[e_GateWah] * .2f);
}

float MakoDist01AudioProcessor::Mako_FX_Audio_AutoWah_Avg(float tSample, float avg, int channel, int pos)
{
    float tS = abs(tSample);
    float Temp_F;
    int c = channel;

    //R4.00 Track our peak signal. Fast On / Slow Off.
    //R4.00 makoPedal[1].Var5[c] = Peak
    //R4.00 makoPedal[1].Var6[c] = Last Peak, Only used to track changes in sample to trigger recalcs.
    //R4.00 makoPedal[1].Var7[c] = The range of freqs to sweep.
    if (makoPedal[pos].Var5[c] < avg)
    {
        //R4.00 Fast On
        makoPedal[pos].Var5[c] = tS;
        if (1.0f < makoPedal[pos].Var5[c]) makoPedal[pos].Var5[c] = 1.0f;   //R1.00 Clip here so we do not do broken math below. 
    }
    else
    {
        //R4.00 Slow Off.
        makoPedal[pos].Var5[c] -= Release_500mS;
        if (makoPedal[pos].Var5[c] < 0.0f) makoPedal[pos].Var5[c] = 0.0f;
    }

    //R4.00 Try not to do calcs so often. Trigger off the size of the signal change. 
    if (.00005f < abs(makoPedal[pos].Var5[c] - makoPedal[pos].Var6[c]))
    {
        makoPedal[pos].Var6[c] = makoPedal[pos].Var5[c];
        Temp_F = 400.0f + (makoPedal[pos].Var5[c] * makoPedal[pos].Var7[c]);
        if (15000 < Temp_F) Temp_F = 15000;
        Mako_Calc_coeffs_Wah(Setting[e_GateWahQ], Temp_F, c);
    }

    //R4.00 Apply the Wah filter to the incoming signal.
    WAH_xn[c] = tSample;
    tS = WAH_coeffs[c].a0 * WAH_xn[c] + WAH_coeffs[c].a1 * WAH_xnz1[c] + WAH_coeffs[c].a2 * WAH_xnz2[c] - WAH_coeffs[c].b1 * WAH_ynz1[c] - WAH_coeffs[c].b2 * WAH_ynz2[c];
    WAH_xnz2[c] = WAH_xnz1[c]; WAH_xnz1[c] = WAH_xn[c]; WAH_ynz2[c] = WAH_ynz1[c]; WAH_ynz1[c] = tS;

    return (tSample * (1.0f - Setting[e_GateWah])) + (tS * Setting[e_GateWah] * .2f);
}


//R4.00 Pedal 2 - Apply the COMPRESSOR to the incoming sample.
float MakoDist01AudioProcessor::Mako_FX_Audio_Compressor(float tSample, int channel, int pos)
{
    //Var2 = Max signal Buffer
    //4 = Release
    //7 = Threshold
    //5 = Attack
    //6 = Drive
    //8 = Ratio
    //9 = High Pass Detection filter

    float tThresh = makoPedal[pos].Var1[0]; // Setting[e_CompThr] * Setting[e_CompThr] * Setting[e_CompThr]; //R1.00 Square THRESH to give us more range on the knob.
    float diff;
    float tSa;

    //R4.00 Put our detection signal thru a Low/High Pass. 
    tSa = tSample;

    //R4.00 Optional filtering of trigger signal.
    //if (Setting[xxx] < .999f) tSa = Filter_Calc_BiQuad(tSa, channel, &makoPedal[idx].Filter1);  //R4.00 Future use.
    //if (0.001f < Setting[xxx]) tSa = Filter_Calc_BiQuad(tSa, channel, &makoPedal[idx].Filter2); //R4.00 Future use.
    
    tSa = abs(tSa);

    //R1.00 If our signal is above the Threshold.
    if (tThresh < tSa)
    {
        //R1.00 Get Difference in Gain and Threshold.    
        diff = tSa - tThresh;

        //R3.00 Calc what our new gain reduction value should be.
        makoPedal[pos].Var2[channel] = (tThresh + (diff * Setting[e_CompRat])) / tSa;

        //R3.00 Slowly modify our GAIN adjuster (Var1) to the new gain value (Var2). 
        if (makoPedal[pos].Var2[channel] < makoPedal[pos].Var1[channel])
        {
            //R1.00 To have a comp attack we need a 2nd var that we adjust up to the actual max. So the comp slowly begins working.
            makoPedal[pos].Var1[channel] -= (Release_500mS * (1.0f + (1.0f - Setting[e_CompAtt]) * 20.0f));
            if (makoPedal[pos].Var1[channel] < 0.0f) makoPedal[pos].Var1[channel] = 0.0f;
        }
        else
        {
            //R1.00 Reduce the Gain adjust back to 1.0f.
            makoPedal[pos].Var1[channel] += (Release_500mS * (1.0f + (1.0f - Setting[e_CompRel]) * 10.0f));
            if (1.0f < makoPedal[pos].Var1[channel]) makoPedal[pos].Var1[channel] = 1.0f;
        }

        //R4.00 Flag the editor that our compressor is triggered.
        makoPedal2CompLight = true;
    }
    else
    {
        //R1.00 Signal is BELOW the threshold, reduce the Gain adjust back 1.0f.
        makoPedal[pos].Var1[channel] += (Release_500mS * (1.0f + (1.0f - Setting[e_CompRel]) * 10.0f));
        if (1.0f < makoPedal[pos].Var1[channel]) makoPedal[pos].Var1[channel] = 1.0f;

        //R4.00 Flag the editor that our compressor is NOT triggered.
        makoPedal2CompLight = false;
    }

    //R2.09 Apply GAIN and DRIVE if needed.
    if (Setting[e_CompDrv] < .0001f)
        return tSample * makoPedal[pos].Var1[channel] * Setting[e_CompVol] * 4.0f;
    else
        return tanh(tSample * makoPedal[pos].Var1[channel] * (1.0f + (Setting[e_CompDrv] * 10.0f))) * Setting[e_CompVol] * 2.0f;

}

//R4.00 Pedal 3 - Apply the DISTORTION 1 to the incoming sample.
float MakoDist01AudioProcessor::Mako_FX_Audio_Distortion1(float tSample, int channel, int pos)
{
    float tS = tSample;

    //R4.00 Low Cut can help if there is too much flubby bass.
    //tS = Filter_Calc_BiQuad(tS, channel, &makoPedal[idx].Filter3);

    //R2.00 BP - Mid boost.
    tS = Filter_Calc_BiQuad(tS, channel, &makoPedal[pos].Filter1);

    //R2.21 Apply Drive.
    tS = tanhf(tS * (1.0f + (Setting[e_Dist1Drv] * Setting[e_Dist1Drv] * 500.0f)));

    //R2.00 LP - Fizz filter.
    if (Setting[e_Dist1Cut] < 7000.0f) tS = Filter_Calc_BiQuad(tS, channel, &makoPedal[pos].Filter2);

    //Volume/Gain adjust. Reduction only.
    tS *= Setting[e_Dist1Vol];

    return tS;
}

//R4.00 Pedal 4 - Apply the DISTORTION 2 to the incoming sample.
float MakoDist01AudioProcessor::Mako_FX_Audio_Distortion2(float tSample, int channel, int pos)
{
    float tS = tSample;

    //R2.00 BP - Mid boost.
    tS = Filter_Calc_BiQuad(tS, channel, &makoPedal[pos].Filter1);

    //R2.21 Apply Drive.
    tS = tanhf(tS * (1.0f + (Setting[e_Dist2Drv] * Setting[e_Dist2Drv] * 500.0f)));

    //R2.00 LP - Fizz filter.
    tS = Filter_Calc_BiQuad(tS, channel, &makoPedal[pos].Filter2);

    //Volume/Gain adjust. Reduction only.
    tS *= Setting[e_Dist2Vol];

    return tS;
}

//R4.00 Pedal 5 - Apply the 6 band EQ Pedal to the incoming sample.
float MakoDist01AudioProcessor::Mako_FX_Audio_EQ(float tSample, int channel, int Pos)
{
    float tS = tSample;
    if (DoEQ1) tS = Filter_Calc_BiQuad(tS, channel, &makoPedal[Pos].Filter1); //R4.00 100
    if (DoEQ2) tS = Filter_Calc_BiQuad(tS, channel, &makoPedal[Pos].Filter2); //R4.00 350
    if (DoEQ3) tS = Filter_Calc_BiQuad(tS, channel, &makoPedal[Pos].Filter3); //R4.00 700
    if (DoEQ4) tS = Filter_Calc_BiQuad(tS, channel, &makoPedal[Pos].Filter4); //R4.00 1400
    if (DoEQ5) tS = Filter_Calc_BiQuad(tS, channel, &makoPedal[Pos].Filter5); //R4.00 2800
    if (DoEQ6) tS = Filter_Calc_BiQuad(tS, channel, &makoPedal[Pos].Filter6); //R4.00 5000

    //Volume/Gain adjust. Reduction only.
    return tS * Setting[e_EQVol];
}

//R1.00 Clip signal to -1 and 1.
float MakoDist01AudioProcessor::Mako_Clip(float tSample, int ClipStage)
{
    float tS = tSample;

    if (.9999f < tS)
    {
        tS = .9999f;
        makoClipStage = ClipStage;
    }
    else if (tS < -.9999f)
    {
        tS = -.9999f;
        makoClipStage = ClipStage;
    }
   
    return tS;
}

//R4.00 Apply the AMPLIFIER COMPRESSOR effect to the incoming sample.
float MakoDist01AudioProcessor::Mako_Amp_Comp(float tSample, int channel)
{
    float tSA;
    
    //R2.00 Store MAX input signal.    
    tSA = abs(tSample);
    if (Amp_Comp_Max[channel] < tSA)
    {
        Amp_Comp_Max[channel] = tSA;
    }
    else
    {
        //R4.00 Increase the Release time if the signal starts modulating due to adjusting too fast.
        Amp_Comp_Max[channel] -= Release_500mS; 
        if (Amp_Comp_Max[channel] < 0.0f) Amp_Comp_Max[channel] = 0.0f;
    }

    float Gain = 1.0f;
    float tComp = Setting[e_AmpComp] * Setting[e_AmpComp]; //R2.00 Square this to give us more range on the knob.

    //R4.00 If Comp is enabled (<1) and our Limit < Signal, apply compression.
    if ((Setting[e_AmpComp] < 1.0f) && (tComp < Amp_Comp_Max[channel]))
    {
        float diff = Amp_Comp_Max[channel] - tComp;
        Gain = (tComp + (diff * Setting[e_AmpCRatio])) / Amp_Comp_Max[channel];
        
        //R4.00 Let Editor know we are triggered.
        makoAmpCompLight = true;
    }
    else
    {
        //R4.00 Let Editor know we are NOT triggered.
        makoAmpCompLight = false;
    }

    return tSample * Gain;
}

//R4.00 Apply the Amplifier EQ to incoming audio sample.
float MakoDist01AudioProcessor::Mako_Amp_EQ(float tSample, int channel)
{   
    float tS = tSample;

    //R4.00 Do EQ calcs, only do buffer moves if EQ is not used. 
    //R4.00 Need buffer updates or get crackles when automated/spun.
    tS = Filter_Calc_BiQuad_Bypass(DoAmpEQ1, tS, channel, &fil_Amp_EQ1);
    tS = Filter_Calc_BiQuad_Bypass(DoAmpEQ2, tS, channel, &fil_Amp_EQ2);
    tS = Filter_Calc_BiQuad_Bypass(DoAmpEQ3, tS, channel, &fil_Amp_EQ3);
    tS = Filter_Calc_BiQuad_Bypass(DoAmpEQ4, tS, channel, &fil_Amp_EQ4);
    tS = Filter_Calc_BiQuad_Bypass(DoAmpEQ5, tS, channel, &fil_Amp_EQ5);
    tS = Filter_Calc_BiQuad_Bypass(DoAmpEQ6, tS, channel, &fil_Amp_EQ6);

    return tS;
}

//R1.00 1024 sample IMPULSE RESPONSE calcs.
float MakoDist01AudioProcessor::Mako_CabSim(float tSample, int channel)
{
    int T1;
    float V = 0.0f;
    
    //R1.00 NO CAB selected. 
    if (!Setting[e_IR] && (channel == 0)) return tSample;
   
    T1 = IRB_Ring1[channel];
    IR_AudioBuffer[channel][T1] = tSample;
        
    //R1.00 Apply Left cab (0) to ALL OTHER channels. Should only ever be 0.
    for (int t = 0; t < 1024; t++)
    {
        V += (IR_AudioBuffer[channel][T1] * IR_Speaker[t]);

        //R1.00 Increment index, mask off bits past 1023 to keep our index between 0-1023.
        T1 = (T1 + 1) & 0x3FF;            
    }
    V *= Setting[e_IRVol];
  
    //R1.00 Decrement our ring buffer index and loop around at 0. 
    IRB_Ring1[channel]--;
    if (IRB_Ring1[channel] < 0) IRB_Ring1[channel] = 1023;
    
    //R1.00 We gain volume here so reduce it to some random level. Need good math here.
    return V * .25f;  
}

//R2.22 Added.
float MakoDist01AudioProcessor::Mako_FX_Reverb_Ducking(float tSample, int channel)
{
    if (Setting[e_RevDuck] < 0.001f) return tSample;

    //R2.00 Create a volume envelope based on Signal Average.
    Reverb_Duck_Fac[channel] = 1.0f - (Reverb_Duck_Peak[channel] * 5.0f * Setting[e_RevDuck]);
    if (Reverb_Duck_Fac[channel] < 0.0f) Reverb_Duck_Fac[channel] = 0.0f;

    return tSample * Reverb_Duck_Fac[channel];
}

//R1.00 DIGITAL DELAY.
float MakoDist01AudioProcessor::Mako_FX_Delay(float tSample, int channel)
{
    //R2.22 Keep track of our volume for ducking.
    Delay_Duck_Peak[channel] *= RATI1000ms; 
    if (Delay_Duck_Peak[channel] < abs(tSample)) Delay_Duck_Peak[channel] = abs(tSample);

    long idx = Delay_B_Ring1[channel];
    
    float V = (tSample * Delay_Dry) + (Mako_FX_Delay_Ducking(Delay_B[channel][idx] * Delay_Wet, channel) * Delay_BalLR[channel]);
    Delay_B[channel][idx] = .5f * (tSample + (Delay_B[channel][idx] * Setting[e_DelRepeat]));

    //R1.00 Update echo ring buffer position. 
    Delay_B_Ring1[channel]++;
    if (Delay_B_Ring1_Max[channel] < Delay_B_Ring1[channel]) Delay_B_Ring1[channel] = 0;

    return V;
}

//R4.00 Calc signal average and apply ducking.
float MakoDist01AudioProcessor::Mako_FX_Delay_Ducking(float tSample, int channel)
{
    if (Setting[e_DelDuck] < 0.001f) return tSample;

    //R2.00 Create a volume envelope based on Signal Average.
    Delay_Duck_Fac[channel] = 1.0f - (Delay_Duck_Peak[channel] * 5.0f * Setting[e_DelDuck]);
    if (Delay_Duck_Fac[channel] < 0.0f) Delay_Duck_Fac[channel] = 0.0f;

    return tSample * Delay_Duck_Fac[channel];
}

//****************************************************************
// M O D U L A T I O N   E F F E C T S
//****************************************************************
//R4.00 Apply the CHORUS effect to incoming audio sample.
float MakoDist01AudioProcessor::Mako_FX_Audio_Chorus(float tSample, int channel)
{
    float tS;
    float LP = Setting[e_Mod3Tone];
    if (LP < .001f) LP = .01f;

    //R1.00 Store new sample in Ring buffer. 
    Mod3_B[channel][Mod3_B_Ring1[channel]] = (tSample * LP) + (Mod3_B_Last[channel] * (1.0f - LP));
    Mod3_B_Last[channel] = Mod3_B[channel][Mod3_B_Ring1[channel]];

    //R1.00 Calc rotating index.
    int idx = Mod3_B_Ring1[channel] - Mod3_B_Offset[channel];
    if (idx < 0) idx = idx + Mod3_B_Ring1_Max[channel];

    //R1.00 Update our rotating index.
    Mod3_B_Offset[channel] *= (1.0f + (Mod3_B_Rate[channel] * Setting[e_Mod3Rate]));

    //R1.00 Alter our delay direction if needed.
    if (Mod3_B_Offset[channel] < Mod3_B_Offset_Min[channel])
        Mod3_B_Rate[channel] = Mod3_B_Rate_Up;
    else if (Mod3_B_Offset_Max[channel] < Mod3_B_Offset[channel])
        Mod3_B_Rate[channel] = Mod3_B_Rate_Down;

    //R1.00 Update ring buffer position. 
    Mod3_B_Ring1[channel]++;
    if (Mod3_B_Ring1_Max[channel] < Mod3_B_Ring1[channel]) Mod3_B_Ring1[channel] = 0;

    tS = (tSample * (1.0f - Setting[e_Mod3Mix])) + (Mod3_B[channel][idx] * Setting[e_Mod3Mix]) * Mod3_BalLR[channel];

    return tS * 2.5f * Setting[e_Mod3Vol];
}

//R4.00 Apply the MOD WAH effect to incoming audio sample.
float MakoDist01AudioProcessor::Mako_FX_Audio_ModWah(float tSample, int channel)
{
    float tS;
    int c = channel;

    //R4.00 Update our oscillating waveform.
    Mod2_Wave[c] += Mod2_Wave_Ramp[c];
    if (Mod2_Wave[c] < .01f)
    {
        Mod2_Wave[c] = .01f;
        Mod2_Wave_Ramp[c] = Mod2_B_Rate_Up * Setting[e_Mod2Rate] * 20.0f;   //R1.00 10 Hz max rate.
    }
    if (.99f < Mod2_Wave[c])
    {
        Mod2_Wave[c] = .99f;
        Mod2_Wave_Ramp[c] = Mod2_B_Rate_Up * Setting[e_Mod2Rate] * -20.0f;  //R1.00 10 Hz max rate.
    }

    //R4.00 Calc filter coeffs. This is expensive and should be optimized more if possible. Like call it every X samples.
    Mako_Calc_coeffs_MWah(.707f, Setting[e_Mod2FStart] + (Mod2_Wave[c] * Setting[e_Mod2Range]), c);
    
    //R4.00 Apply the Wah filter to the incoming signal.
    MWAH_xn[channel] = tSample;
    tS = MWAH_coeffs[c].a0 * MWAH_xn[c] + MWAH_coeffs[c].a1 * MWAH_xnz1[c] + MWAH_coeffs[c].a2 * MWAH_xnz2[c] - MWAH_coeffs[c].b1 * MWAH_ynz1[c] - MWAH_coeffs[c].b2 * MWAH_ynz2[c];
    MWAH_xnz2[c] = MWAH_xnz1[c]; MWAH_xnz1[c] = MWAH_xn[c]; MWAH_ynz2[c] = MWAH_ynz1[c]; MWAH_ynz1[c] = tS;

    //R4.00 Mix the incoming sample with the Wah effected sample.
    tS = (tSample * (1.0f - Setting[e_Mod2Mix])) + (tS * Setting[e_Mod2Mix] * .075f);   

    return tS * 2.0f * Setting[e_Mod2Vol];
}

//R4.00 Apply the FLANGE effect to incoming audio sample.
float MakoDist01AudioProcessor::Mako_FX_Audio_Flange(float tSample, int channel)
{
    float tS = tSample;

    //R1.00 Store new sample in Ring buffer. 
    Mod1_B[channel][Mod1_B_Ring1[channel]] = tSample;
    
    //R1.00 Calc rotating index.
    int idx = Mod1_B_Ring1[channel] - int(Mod1_B_Offset[channel]);
    if (idx < 0) idx = idx + Mod1_B_Ring1_Max[channel];

    //R1.00 Update our rotating index.
    Mod1_B_Offset[channel] += (Mod1_B_Rate[channel] * Setting[e_Mod1Rate] * 500.0f);
    
    //R1.00 Alter our delay direction if needed (Min=10, Max=20+500*Depth).
    if (Mod1_B_Offset[channel] < 10) {
        Mod1_B_Offset[channel] = 10;
        Mod1_B_Rate[channel] = Mod1_B_Rate_Up; 
    }
    else if ((20 + int(500.0f * Setting[e_Mod1Depth])) < Mod1_B_Offset[channel])
        Mod1_B_Rate[channel] = Mod1_B_Rate_Down;

    //R1.00 Update ring buffer position. 
    Mod1_B_Ring1[channel]++;
    if (Mod1_B_Ring1_Max[channel] < Mod1_B_Ring1[channel]) Mod1_B_Ring1[channel] = 0;
   
    //R4.00 Mix the incoming sample with the effected sample.
    tS = (tS * (1 - Setting[e_Mod1Mix])) + (Mod1_B[channel][idx] * Setting[e_Mod1Mix]);

    //R4.00 Apply Synthesizer effect. Squared to get more usable knob range.
    if (Setting[e_Mod1Synth]) tS = sin(tS * Setting[e_Mod1Synth] * Setting[e_Mod1Synth] * 500.0f) * .2f;

    //R4.00 Apply rectified Bronz effect (tS * tS). Squaring the signal creates interesting even order harmonic effects.
    if (Setting[e_Mod1Bronz])
    {
        tS = (tS * (1.0f - Setting[e_Mod1Bronz])) + (2.0f * (tS * tS) * Setting[e_Mod1Bronz]);
        tS = Filter_Calc_BiQuad(tS, channel, &fil_Mod1_DCRemoval); //R4.00 Apply DC Offset filter.
    }

    return tS * 2.0f * Setting[e_Mod1Vol];
}

//R4.00 Apply the WIDE LOAD effect to incoming audio sample.
float MakoDist01AudioProcessor::Mako_FX_Audio_WideLoad(float tSample, int channel)
{    
    int depth = 0; 
    float tS = tSample;
    
    //R4.00 Apply stereo filter and delays based on channel and Delay Offset.
    if (Setting[e_Mod4Doff] <= .0f)
    {
        depth = abs(Setting[e_Mod4Doff]) * 4000.0f;
        if (channel == 0)
        {
            if (0.0f < Setting[e_Mod4Foff])
            {
                tS = Filter_Calc_BiQuad(tS, channel, &filMod4[0]); //R4.00 Apply Freq Offset filter.
                tS *= 1.0f - (Setting[e_Mod4Foff] * .8f);          //R4.00 Try to adjust gain for new filter.
            }
        }
        else
        {
            if (0.0f < Setting[e_Mod4Foff]) tS = Filter_Calc_BiQuad(tS, channel, &filMod4[1]);
            if (.999f < Setting[e_Mod1Mix])
                depth = 0;
            else
                depth = depth / 2;
        }
    }
    if (.0f < Setting[e_Mod4Doff])
    {
        depth = abs(Setting[e_Mod4Doff]) * 4000.0f;
        if (channel == 1)
        {
            if (0.0f < Setting[e_Mod4Foff])
            {
                tS = Filter_Calc_BiQuad(tS, channel, &filMod4[0]); //R4.00 Apply Freq Offset filter.
                tS *= 1.0f - (Setting[e_Mod4Foff] * .8f);          //R4.00 Try to adjust gain for new filter.
            }
        }
        else
        {
            if (0.0f < Setting[e_Mod4Foff]) tS = Filter_Calc_BiQuad(tS, channel, &filMod4[1]);
            if (.999f < Setting[e_Mod4Mix])
                depth = 0;
            else
                depth = depth / 2;
        }
    }

    //R4.00 Apply TONE filter and then store the new sample in Delay Ring buffer. 
    float LP = (Setting[e_Mod4Tone] * Setting[e_Mod4Tone]) + .01f;
    Mod4_B[channel][Mod4_B_Ring1[channel]] = (tS * LP) + (Mod4_B_Last[channel] * (1.0f - LP));
    Mod4_B_Last[channel] = Mod4_B[channel][Mod4_B_Ring1[channel]];

    //R1.00 Calc rotating index.
    int idx = Mod4_B_Ring1[channel] - depth;
    if (idx < 0) idx = idx + Mod4_B_Ring1_Max[channel];

    //R1.00 Update ring buffer position. 
    Mod4_B_Ring1[channel]++;
    if (Mod4_B_Ring1_Max[channel] < Mod4_B_Ring1[channel]) Mod4_B_Ring1[channel] = 0;

    //R4.00 Apply clean/effect mix and effect balance.
    tS = (tSample * (1.0f - Setting[e_Mod4Mix])) + (Mod4_B[channel][idx] * Setting[e_Mod4Mix]) * Mod4_BalLR[channel];    

    return tS * 2.0f * Setting[e_Mod4Vol];
}

//R4.00 LOW CUT and removal of DC offset from Asymmetry code!!! 
float MakoDist01AudioProcessor::Mako_EQ_Output_Filters(float tSample, int channel)
{
    return Filter_Calc_BiQuad(tSample, channel, &fil_Out_HP);
}


//****************************************************************
// R E V E R B   E F F E C T S
//****************************************************************
void MakoDist01AudioProcessor::Mako_FX_Reverb_CalcSize()
{
    //R2.00 Set the REVERB coefficients for largest room possible.
    //REVERB COUNT

    int RM = int(Setting[e_RevMode]) - 1;
    if (RM < 0) RM = 0;

    switch (RM)
    {
    case 0:Mako_FX_Reverb_SetMode0(); Reverb_Render_Mode = 0; break;    //R4.00 Basic
    case 1:Mako_FX_Reverb_SetMode1(); Reverb_Render_Mode = 1; break;    //R4.00 Slap
    case 2:Mako_FX_Reverb_SetMode2(); Reverb_Render_Mode = 0; break;    //R4.00 Haunted
    case 3:Mako_FX_Reverb_SetMode3(); Reverb_Render_Mode = 0; break;    //R4.00 Dreamy
    case 4:Mako_FX_Reverb_SetMode4(); Reverb_Render_Mode = 0; break;    //R4.00 Club
    case 5:Mako_FX_Reverb_SetMode5(); Reverb_Render_Mode = 0; break;    //R4.00 Darkness
    case 6:Mako_FX_Reverb_SetMode6(); Reverb_Render_Mode = 0; break;    //R4.00 Medium room
    case 7:Mako_FX_Reverb_SetMode7(); Reverb_Render_Mode = 0; break;    //R4.00 Small Room
    case 8:Mako_FX_Reverb_SetMode8(); Reverb_Render_Mode = 0; break;    //R4.00 Box
    case 9:Mako_FX_Reverb_SetMode9(); Reverb_Render_Mode = 0; break;    //R4.00 DigiVerb
    case 10:Mako_FX_Reverb_SetMode10(); Reverb_Render_Mode = 0; break;  //R4.00 Space
    case 11:Mako_FX_Reverb_SetMode11(); Reverb_Render_Mode = 1; break;  //R4.00 Garage
    case 12:Mako_FX_Reverb_SetMode12(); Reverb_Render_Mode = 1; break;  //R4.00 Fused Echo
    case 13:Mako_FX_Reverb_SetMode13(); Reverb_Render_Mode = 1; break;  //R4.00 Medium Tile
    case 14:Mako_FX_Reverb_SetMode14(); Reverb_Render_Mode = 1; break;  //R4.0 Arena
    case 15:Mako_FX_Reverb_SetMode15(); Reverb_Render_Mode = 1; break;  //R4.00 Garage 2
    case 16:Mako_FX_Reverb_SetMode16(); Reverb_Render_Mode = 1; break;  //R4.00 Inverted
    case 17:Mako_FX_Reverb_SetMode17(); Reverb_Render_Mode = 1; break;  //R4.00 Hanger
    case 18:Mako_FX_Reverb_SetMode18(); Reverb_Render_Mode = 1; break;  //R4.00 Inverted 2
    case 19:Mako_FX_Reverb_SetMode19(); Reverb_Render_Mode = 1; break;  //R4.00 Regent
    }

    //R2.22 Calc our PreDelay. Max 200mS or 38400 samples. 
    PreDEnd = int(Setting[e_RevPreD] * .001f * makoSampleRate);
    if (PreDMax < PreDEnd) PreDEnd = PreDMax;
    
    //R2.00 Calc the gain on DENSE verbs, used so we dont exceed gain of 1.
    float tG = 0.0f;
    if (Reverb_Render_Mode == 1)
    {
        for (int t = 0; t < 16; t++) tG = tG + abs(RevB_Gain[t]);  //R2.14 Added new Rev mode gain calcs.
        if (0.0f == tG)
            Reverb_Gain_Factor = 1.0f;
        else
            Reverb_Gain_Factor = 1.0f / tG;
    }
    else
    {
        for (int t = 0; t < 64; t++) tG = tG + abs(Reverb_B_Ring1_Gain[t]); //R2.13 Added ABS for gain check.
        if (0.0f == tG)
            Reverb_Gain_Factor = 1.0f;
        else
            Reverb_Gain_Factor = 1.0f / tG;
    }
    
    //R2.00 Reduce the Coeffs for smaller rooms. 
    float fac = Setting[e_RevSize] * 4.0f;
    if (fac < .01f) fac = .01f;

    for (int t = 0; t < 64; t++)
    {
        Reverb_B_Ring1[0][t] = int(Reverb_B_Ring1[0][t] * fac);
        Reverb_B_Ring1[1][t] = int(Reverb_B_Ring1[1][t] * fac);
    }

    float tFac = Setting[e_RevSize];
    if (tFac < .01f) tFac = .01f;
    for (int t = 0; t < 16; t++)
    {
        RevB_Max[0][t] = int(RevB_Max[0][t] * tFac);
        RevB_Max[1][t] = int(RevB_Max[1][t] * tFac);
    }

    if ((Reverb_B_Ring1_Max[0] < 0) || (192000 < Reverb_B_Ring1_Max[0])) Reverb_B_Ring1_Max[0] = 16000;
    if ((Reverb_B_Ring1_Max[1] < 0) || (192000 < Reverb_B_Ring1_Max[1])) Reverb_B_Ring1_Max[1] = 16000;
    Reverb_B_Ring1_Max[0] = int(Reverb_B_Ring1_Max[0] * fac);
    Reverb_B_Ring1_Max[1] = int(Reverb_B_Ring1_Max[1] * fac);

    //R2.00 Clear out the area outside our buffer.
    for (int t = Reverb_B_Ring1_Max[0]; t < 192000; t++)
    {
        Reverb_B[0][t] = 0.0f;
        Reverb_B[1][t] = 0.0f;
    }

    //R1.00 Reset our buffer indexes.
    for (int t = 0; t < 16; t++)
    {
        RevB_Idx[0][t] = 0;
        RevB_Idx[1][t] = 0;
    }

}

//R4.00 BASIC.
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode0()
{    
    //R1.00 Define our Reverb Vars.
    Reverb_B_Ring1_Gain[0] = 0.5f;
    Reverb_B_Ring1_GainInv[0] = .5f;
    Reverb_B_Ring1[0][0] = 1210;
    Reverb_B_Ring1[1][0] = 2210;
    Reverb_B_Ring1_Gain[1] = 6.0f;
    Reverb_B_Ring1_GainInv[0] = .4f;
    Reverb_B_Ring1[0][1] = 210;
    Reverb_B_Ring1[1][1] = 410;
    Reverb_B_Ring1_Gain[2] = .75f;
    Reverb_B_Ring1_GainInv[2] = .25f;
    Reverb_B_Ring1[0][2] = 158;
    Reverb_B_Ring1[1][2] = 358;
    Reverb_B_Ring1_Gain[3] = .625f;
    Reverb_B_Ring1_GainInv[3] = .375f;
    Reverb_B_Ring1[0][3] = 561;
    Reverb_B_Ring1[1][3] = 761;
    Reverb_B_Ring1_Gain[4] = .625f;
    Reverb_B_Ring1_GainInv[4] = .375f;
    Reverb_B_Ring1[0][4] = 410;
    Reverb_B_Ring1[1][4] = 610;
    Reverb_B_Ring1_Gain[5] = .7f;
    Reverb_B_Ring1_GainInv[5] = .3f;
    Reverb_B_Ring1[0][5] = 908;
    Reverb_B_Ring1[1][5] = 1108;
    Reverb_B_Ring1_Gain[6] = .7f;
    Reverb_B_Ring1_GainInv[6] = .3f;
    Reverb_B_Ring1[0][6] = 1343;
    Reverb_B_Ring1[1][6] = 1543;
    Reverb_B_Ring1_Gain[7] = .3f;
    Reverb_B_Ring1_GainInv[7] = .7f;
    Reverb_B_Ring1[0][7] = 995;
    Reverb_B_Ring1[1][7] = 1195;
    Reverb_B_Ring1_Gain[8] = .3f;
    Reverb_B_Ring1_GainInv[8] = .7f;
    Reverb_B_Ring1[0][8] = 6590;
    Reverb_B_Ring1[1][8] = 6790;
    Reverb_B_Ring1_Gain[9] = .5f;
    Reverb_B_Ring1_GainInv[9] = .5f;
    Reverb_B_Ring1[0][9] = 3931;
    Reverb_B_Ring1[1][9] = 4131;
    Reverb_B_Ring1_Gain[10] = .5f;
    Reverb_B_Ring1_GainInv[10] = .5f;
    Reverb_B_Ring1[0][10] = 2664;
    Reverb_B_Ring1[1][10] = 2864;
    Reverb_B_Ring1_Gain[11] = .3f;
    Reverb_B_Ring1_GainInv[11] = .7f;
    Reverb_B_Ring1[0][11] = 4641;
    Reverb_B_Ring1[1][11] = 4841;
    Reverb_B_Ring1_Gain[12] = .3f;
    Reverb_B_Ring1_GainInv[12] = .7f;
    Reverb_B_Ring1[0][12] = 5505;
    Reverb_B_Ring1[1][12] = 5705;
    Reverb_B_Ring1_Gain[13] = .25f;
    Reverb_B_Ring1_GainInv[13] = .75f;
    Reverb_B_Ring1[0][13] = 9300;
    Reverb_B_Ring1[1][13] = 9500;
    Reverb_B_Ring1_Gain[14] = .25f;
    Reverb_B_Ring1_GainInv[14] = .75f;
    Reverb_B_Ring1[0][14] = 12300;
    Reverb_B_Ring1[1][14] = 12500;
    Reverb_B_Ring1_Gain[15] = .2f;
    Reverb_B_Ring1_GainInv[15] = .8f;
    Reverb_B_Ring1[0][15] = 14500;
    Reverb_B_Ring1[1][15] = 14700;

    Reverb_B_Ring1_Max[0] = 16300;
    Reverb_B_Ring1_Max[1] = 16300;
}

//R4.00 SLAP SMEAR.
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode1()
{
    int Del = 1000;
    int Del2 = 1200;
    
    RevB_Gain[0] = -1.185651E-03f;
    RevB_Max[0][0] = 4332;
    RevB_Max[1][0] = 5132;
    RevB_Gain[1] = .1188914f;
    RevB_Max[0][1] = 8402;
    RevB_Max[1][1] = 9239;
    RevB_Gain[2] = -.3848826f;
    RevB_Max[0][2] = 13317;
    RevB_Max[1][2] = 14223;
    RevB_Gain[3] = .3194438f;
    RevB_Max[0][3] = 17367;
    RevB_Max[1][3] = 18007;
    RevB_Gain[4] = .1176973f;
    RevB_Max[0][4] = 22903;
    RevB_Max[1][4] = 22174;
    RevB_Gain[5] = -.5027322f;
    RevB_Max[0][5] = 26266;
    RevB_Max[1][5] = 26931;
    RevB_Gain[6] = .7059703f;
    RevB_Max[0][6] = 32144;
    RevB_Max[1][6] = 30791;
    RevB_Gain[7] = -.7168727f;
    RevB_Max[0][7] = 35775;
    RevB_Max[1][7] = 36704;
    RevB_Gain[8] = .992442f;
    RevB_Max[0][8] = 39802;
    RevB_Max[1][8] = 39754;
    RevB_Gain[9] = -1.0f;
    RevB_Max[0][9] = 45444;
    RevB_Max[1][9] = 45392;
    RevB_Gain[10] = .6304718f;
    RevB_Max[0][10] = 49529;
    RevB_Max[1][10] = 49081;
    RevB_Gain[11] = -.2074285f;
    RevB_Max[0][11] = 53819;
    RevB_Max[1][11] = 54077;
    RevB_Gain[12] = -4.904115E-02f;
    RevB_Max[0][12] = 58918;
    RevB_Max[1][12] = 58958;
    RevB_Gain[13] = .3219758f;
    RevB_Max[0][13] = 63431;
    RevB_Max[1][13] = 62385;
    RevB_Gain[14] = -.5606492f;
    RevB_Max[0][14] = 66935;
    RevB_Max[1][14] = 66956;
    RevB_Gain[15] = .6529667f;
    RevB_Max[0][15] = 72664;
    RevB_Max[1][15] = 71883;

    Reverb_B_Ring1_Max[0] = 80000;
    Reverb_B_Ring1_Max[1] = 80000;
}

//R4.00 HAUNTED.
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode2()
{
    //R1.00 Define our Reverb Vars.
    for (int t = 0; t < 16; t = t + 2)
    {
        Reverb_B_Ring1_Gain[t] = .15f + ((1.0f - (float(t) / 16.0f)) * .3f);
        Reverb_B_Ring1[0][t] = t * 200 + t * 9;
        Reverb_B_Ring1[1][t] = t * 400;
    }
    for (int t = 1; t < 16; t = t + 2)
    {
        Reverb_B_Ring1_Gain[t] = .55f - ((1.0f - (float(t) / 16.0f)) * .3f);
        Reverb_B_Ring1[0][t] = t * 800;
        Reverb_B_Ring1[1][t] = t * 900 + t * 13;
    }

    for (int t = 0; t < 16; t++) Reverb_B_Ring1_GainInv[t] = 1.0f - Reverb_B_Ring1_Gain[t];

    Reverb_B_Ring1_Max[0] = 17000;
    Reverb_B_Ring1_Max[1] = 17000;
}

//R4.00 DREAMY
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode3()
{
    //R1.00 Define our Reverb Vars.
    Reverb_B_Ring1_Gain[0] = 0.5f;
    Reverb_B_Ring1_GainInv[0] = .5f;
    Reverb_B_Ring1[0][0] = 1210;
    Reverb_B_Ring1[1][0] = 2210;
    Reverb_B_Ring1_Gain[1] = 6.0f;
    Reverb_B_Ring1_GainInv[0] = .4f;
    Reverb_B_Ring1[0][1] = 210;
    Reverb_B_Ring1[1][1] = 410;
    Reverb_B_Ring1_Gain[2] = .75f;
    Reverb_B_Ring1_GainInv[2] = .25f;
    Reverb_B_Ring1[0][2] = 158;
    Reverb_B_Ring1[1][2] = 358;
    Reverb_B_Ring1_Gain[3] = .625f;
    Reverb_B_Ring1_GainInv[3] = .375f;
    Reverb_B_Ring1[0][3] = 561;
    Reverb_B_Ring1[1][3] = 761;
    Reverb_B_Ring1_Gain[4] = .625f;
    Reverb_B_Ring1_GainInv[4] = .375f;
    Reverb_B_Ring1[0][4] = 410;
    Reverb_B_Ring1[1][4] = 610;
    Reverb_B_Ring1_Gain[5] = .7f;
    Reverb_B_Ring1_GainInv[5] = .3f;
    Reverb_B_Ring1[0][5] = 908;
    Reverb_B_Ring1[1][5] = 1108;
    Reverb_B_Ring1_Gain[6] = .7f;
    Reverb_B_Ring1_GainInv[6] = .3f;
    Reverb_B_Ring1[0][6] = 1343;
    Reverb_B_Ring1[1][6] = 1543;
    Reverb_B_Ring1_Gain[7] = .3f;
    Reverb_B_Ring1_GainInv[7] = .7f;
    Reverb_B_Ring1[0][7] = 995;
    Reverb_B_Ring1[1][7] = 1195;
    Reverb_B_Ring1_Gain[8] = .3f;
    Reverb_B_Ring1_GainInv[8] = .7f;
    Reverb_B_Ring1[0][8] = 6590;
    Reverb_B_Ring1[1][8] = 6790;
    Reverb_B_Ring1_Gain[9] = .5f;
    Reverb_B_Ring1_GainInv[9] = .5f;
    Reverb_B_Ring1[0][9] = 3931;
    Reverb_B_Ring1[1][9] = 4131;
    Reverb_B_Ring1_Gain[10] = .5f;
    Reverb_B_Ring1_GainInv[10] = .5f;
    Reverb_B_Ring1[0][10] = 2664;
    Reverb_B_Ring1[1][10] = 2864;
    Reverb_B_Ring1_Gain[11] = .3f;
    Reverb_B_Ring1_GainInv[11] = .7f;
    Reverb_B_Ring1[0][11] = 4641;
    Reverb_B_Ring1[1][11] = 4841;
    Reverb_B_Ring1_Gain[12] = .3f;
    Reverb_B_Ring1_GainInv[12] = .7f;
    Reverb_B_Ring1[0][12] = 5505;
    Reverb_B_Ring1[1][12] = 5705;
    Reverb_B_Ring1_Gain[13] = .25f;
    Reverb_B_Ring1_GainInv[13] = .75f;
    Reverb_B_Ring1[0][13] = 9300;
    Reverb_B_Ring1[1][13] = 9500;
    Reverb_B_Ring1_Gain[14] = .25f;
    Reverb_B_Ring1_GainInv[14] = .75f;
    Reverb_B_Ring1[0][14] = 12300;
    Reverb_B_Ring1[1][14] = 12500;
    Reverb_B_Ring1_Gain[15] = .2f;
    Reverb_B_Ring1_GainInv[15] = .8f;
    Reverb_B_Ring1[0][15] = 14500;
    Reverb_B_Ring1[1][15] = 14700;

    for (int t = 0; t < 16; t++)
    {
        Reverb_B_Ring1_Gain[t + 16] = Reverb_B_Ring1_Gain[t];
        Reverb_B_Ring1_GainInv[t + 16] = 1.0f - Reverb_B_Ring1_Gain[t + 16];
        Reverb_B_Ring1[0][t + 16] = int(16300.0f - (t / 15.0f) * 15000);
        Reverb_B_Ring1[1][t + 16] = int(16300.0f - (t / 15.0f) * 13330);
    }

    Reverb_B_Ring1_Max[0] = 16300;
    Reverb_B_Ring1_Max[1] = 16300;
}

//R4.00 CLUB
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode4()
{       
    Reverb_B_Ring1_Gain[0] = 0.399091f;
    Reverb_B_Ring1_GainInv[0] = 0.600909f;
    Reverb_B_Ring1[0][0] = 1022;
    Reverb_B_Ring1[1][0] = 1291;
    Reverb_B_Ring1_Gain[1] = 0.949644f;
    Reverb_B_Ring1_GainInv[1] = 5.035597E-02f;
    Reverb_B_Ring1[0][1] = 13332;
    Reverb_B_Ring1[1][1] = 13334;
    Reverb_B_Ring1_Gain[2] = 0.827204f;
    Reverb_B_Ring1_GainInv[2] = 0.172796f;
    Reverb_B_Ring1[0][2] = 16468;
    Reverb_B_Ring1[1][2] = 16526;
    Reverb_B_Ring1_Gain[3] = 0.339763f;
    Reverb_B_Ring1_GainInv[3] = 0.660237f;
    Reverb_B_Ring1[0][3] = 2548;
    Reverb_B_Ring1[1][3] = 2838;
    Reverb_B_Ring1_Gain[4] = 0.667562f;
    Reverb_B_Ring1_GainInv[4] = 0.332438f;
    Reverb_B_Ring1[0][4] = 20562;
    Reverb_B_Ring1[1][4] = 20690;
    Reverb_B_Ring1_Gain[5] = 0.684042f;
    Reverb_B_Ring1_GainInv[5] = 0.315958f;
    Reverb_B_Ring1[0][5] = 20142;
    Reverb_B_Ring1[1][5] = 20260;
    Reverb_B_Ring1_Gain[6] = 0.346416f;
    Reverb_B_Ring1_GainInv[6] = 0.653584f;
    Reverb_B_Ring1[0][6] = 2387;
    Reverb_B_Ring1[1][6] = 2665;
    Reverb_B_Ring1_Gain[7] = 0.589465f;
    Reverb_B_Ring1_GainInv[7] = 0.410535f;
    Reverb_B_Ring1[0][7] = 22568;
    Reverb_B_Ring1[1][7] = 22727;
    Reverb_B_Ring1_Gain[8] = 0.572497f;
    Reverb_B_Ring1_GainInv[8] = 0.427503f;
    Reverb_B_Ring1[0][8] = 23007;
    Reverb_B_Ring1[1][8] = 23170;
    Reverb_B_Ring1_Gain[9] = 0.778375f;
    Reverb_B_Ring1_GainInv[9] = 0.221625f;
    Reverb_B_Ring1[0][9] = 17738;
    Reverb_B_Ring1[1][9] = 17801;
    Reverb_B_Ring1_Gain[10] = 0.321482f;
    Reverb_B_Ring1_GainInv[10] = 0.678518f;
    Reverb_B_Ring1[0][10] = 3036;
    Reverb_B_Ring1[1][10] = 3315;
    Reverb_B_Ring1_Gain[11] = 0.833552f;
    Reverb_B_Ring1_GainInv[11] = 0.166448f;
    Reverb_B_Ring1[0][11] = 16331;
    Reverb_B_Ring1[1][11] = 16361;
    Reverb_B_Ring1_Gain[12] = 0.502457f;
    Reverb_B_Ring1_GainInv[12] = 0.497543f;
    Reverb_B_Ring1[0][12] = 24810;
    Reverb_B_Ring1[1][12] = 24996;
    Reverb_B_Ring1_Gain[13] = 0.492355f;
    Reverb_B_Ring1_GainInv[13] = 0.507645f;
    Reverb_B_Ring1[0][13] = 25072;
    Reverb_B_Ring1[1][13] = 25260;
    Reverb_B_Ring1_Gain[14] = 0.197272f;
    Reverb_B_Ring1_GainInv[14] = 0.802728f;
    Reverb_B_Ring1[0][14] = 6226;
    Reverb_B_Ring1[1][14] = 6553;
    Reverb_B_Ring1_Gain[15] = 0.564928f;
    Reverb_B_Ring1_GainInv[15] = 0.435072f;
    Reverb_B_Ring1[0][15] = 23219;
    Reverb_B_Ring1[1][15] = 23367;
    Reverb_B_Ring1_Gain[16] = 0.995483f;
    Reverb_B_Ring1_GainInv[16] = 4.517019E-03f;
    Reverb_B_Ring1[0][16] = 12097;
    Reverb_B_Ring1[1][16] = 12035;
    Reverb_B_Ring1_Gain[17] = 0.660512f;
    Reverb_B_Ring1_GainInv[17] = 0.339488f;
    Reverb_B_Ring1[0][17] = 17458;
    Reverb_B_Ring1[1][17] = 17494;
    Reverb_B_Ring1_Gain[18] = 0.624104f;
    Reverb_B_Ring1_GainInv[18] = 0.375896f;
    Reverb_B_Ring1[0][18] = 18044;
    Reverb_B_Ring1[1][18] = 18088;
    Reverb_B_Ring1_Gain[19] = 0.871426f;
    Reverb_B_Ring1_GainInv[19] = 0.128574f;
    Reverb_B_Ring1[0][19] = 14090;
    Reverb_B_Ring1[1][19] = 14057;
    Reverb_B_Ring1_Gain[20] = 0.172674f;
    Reverb_B_Ring1_GainInv[20] = 0.827326f;
    Reverb_B_Ring1[0][20] = 25272;
    Reverb_B_Ring1[1][20] = 25447;
    Reverb_B_Ring1_Gain[21] = 0.266701f;
    Reverb_B_Ring1_GainInv[21] = 0.733299f;
    Reverb_B_Ring1[0][21] = 23770;
    Reverb_B_Ring1[1][21] = 23913;
    Reverb_B_Ring1_Gain[22] = 0.10886f;
    Reverb_B_Ring1_GainInv[22] = 0.89114f;
    Reverb_B_Ring1[0][22] = 26299;
    Reverb_B_Ring1[1][22] = 86;
    Reverb_B_Ring1_Gain[23] = 0.35667f;
    Reverb_B_Ring1_GainInv[23] = 0.64333f;
    Reverb_B_Ring1[0][23] = 22336;
    Reverb_B_Ring1[1][23] = 22447;
    Reverb_B_Ring1_Gain[24] = 0.349406f;
    Reverb_B_Ring1_GainInv[24] = 0.650594f;
    Reverb_B_Ring1[0][24] = 22455;
    Reverb_B_Ring1[1][24] = 22566;
    Reverb_B_Ring1_Gain[25] = 0.485519f;
    Reverb_B_Ring1_GainInv[25] = 0.514481f;
    Reverb_B_Ring1[0][25] = 20280;
    Reverb_B_Ring1[1][25] = 20347;
    Reverb_B_Ring1_Gain[26] = 0.26603f;
    Reverb_B_Ring1_GainInv[26] = 0.73397f;
    Reverb_B_Ring1[0][26] = 23794;
    Reverb_B_Ring1[1][26] = 23925;
    Reverb_B_Ring1_Gain[27] = 0.262825f;
    Reverb_B_Ring1_GainInv[27] = 0.737175f;
    Reverb_B_Ring1[0][27] = 23848;
    Reverb_B_Ring1[1][27] = 23977;
    Reverb_B_Ring1_Gain[28] = 0.042909f;
    Reverb_B_Ring1_GainInv[28] = 0.957091f;
    Reverb_B_Ring1[0][28] = 969;
    Reverb_B_Ring1[1][28] = 1161;
    Reverb_B_Ring1_Gain[29] = 0.534166f;
    Reverb_B_Ring1_GainInv[29] = 0.465834f;
    Reverb_B_Ring1[0][29] = 19513;
    Reverb_B_Ring1[1][29] = 19554;
    Reverb_B_Ring1_Gain[30] = 0.636738f;
    Reverb_B_Ring1_GainInv[30] = 0.363262f;
    Reverb_B_Ring1[0][30] = 17874;
    Reverb_B_Ring1[1][30] = 17882;
    Reverb_B_Ring1_Gain[31] = 0.887814f;
    Reverb_B_Ring1_GainInv[31] = 0.112186f;
    Reverb_B_Ring1[0][31] = 13859;
    Reverb_B_Ring1[1][31] = 13789;

    Reverb_B_Ring1_Max[0] = 16500;
    Reverb_B_Ring1_Max[1] = 16500;
}

//R4.00 Darkness
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode5()
{  
    Reverb_B_Ring1_Gain[0] = 0.278359f;
    Reverb_B_Ring1_GainInv[0] = 0.721641f;
    Reverb_B_Ring1[0][0] = 21447;
    Reverb_B_Ring1[1][0] = 21719;
    Reverb_B_Ring1_Gain[1] = 0.146031f;
    Reverb_B_Ring1_GainInv[1] = 0.853969f;
    Reverb_B_Ring1[0][1] = 24834;
    Reverb_B_Ring1[1][1] = 25173;
    Reverb_B_Ring1_Gain[2] = 0.985321f;
    Reverb_B_Ring1_GainInv[2] = 1.467901E-02f;
    Reverb_B_Ring1[0][2] = 3348;
    Reverb_B_Ring1[1][2] = 3285;
    Reverb_B_Ring1_Gain[3] = 0.873531f;
    Reverb_B_Ring1_GainInv[3] = 0.126469f;
    Reverb_B_Ring1[0][3] = 6210;
    Reverb_B_Ring1[1][3] = 6204;
    Reverb_B_Ring1_Gain[4] = 0.292093f;
    Reverb_B_Ring1_GainInv[4] = 0.707907f;
    Reverb_B_Ring1[0][4] = 21095;
    Reverb_B_Ring1[1][4] = 21371;
    Reverb_B_Ring1_Gain[5] = 0.382855f;
    Reverb_B_Ring1_GainInv[5] = 0.617145f;
    Reverb_B_Ring1[0][5] = 18772;
    Reverb_B_Ring1[1][5] = 19007;
    Reverb_B_Ring1_Gain[6] = 0.782434f;
    Reverb_B_Ring1_GainInv[6] = 0.217566f;
    Reverb_B_Ring1[0][6] = 8543;
    Reverb_B_Ring1[1][6] = 8589;
    Reverb_B_Ring1_Gain[7] = 0.934049f;
    Reverb_B_Ring1_GainInv[7] = 6.595099E-02f;
    Reverb_B_Ring1[0][7] = 4662;
    Reverb_B_Ring1[1][7] = 4637;
    Reverb_B_Ring1_Gain[8] = 0.83108f;
    Reverb_B_Ring1_GainInv[8] = 0.16892f;
    Reverb_B_Ring1[0][8] = 7297;
    Reverb_B_Ring1[1][8] = 7325;
    Reverb_B_Ring1_Gain[9] = 0.375896f;
    Reverb_B_Ring1_GainInv[9] = 0.624104f;
    Reverb_B_Ring1[0][9] = 18950;
    Reverb_B_Ring1[1][9] = 19200;
    Reverb_B_Ring1_Gain[10] = 0.659017f;
    Reverb_B_Ring1_GainInv[10] = 0.340983f;
    Reverb_B_Ring1[0][10] = 11702;
    Reverb_B_Ring1[1][10] = 11818;
    Reverb_B_Ring1_Gain[11] = 0.680593f;
    Reverb_B_Ring1_GainInv[11] = 0.319407f;
    Reverb_B_Ring1[0][11] = 11150;
    Reverb_B_Ring1[1][11] = 11258;
    Reverb_B_Ring1_Gain[12] = 0.632435f;
    Reverb_B_Ring1_GainInv[12] = 0.367565f;
    Reverb_B_Ring1[0][12] = 12383;
    Reverb_B_Ring1[1][12] = 12517;
    Reverb_B_Ring1_Gain[13] = 0.339f;
    Reverb_B_Ring1_GainInv[13] = 0.661f;
    Reverb_B_Ring1[0][13] = 19895;
    Reverb_B_Ring1[1][13] = 20173;
    Reverb_B_Ring1_Gain[14] = 0.197607f;
    Reverb_B_Ring1_GainInv[14] = 0.802393f;
    Reverb_B_Ring1[0][14] = 23514;
    Reverb_B_Ring1[1][14] = 23862;
    Reverb_B_Ring1_Gain[15] = 0.193121f;
    Reverb_B_Ring1_GainInv[15] = 0.806879f;
    Reverb_B_Ring1[0][15] = 23630;
    Reverb_B_Ring1[1][15] = 23983;
    Reverb_B_Ring1_Gain[16] = 0.473464f;
    Reverb_B_Ring1_GainInv[16] = 0.526536f;
    Reverb_B_Ring1[0][16] = 11338;
    Reverb_B_Ring1[1][16] = 11464;
    Reverb_B_Ring1_Gain[17] = 0.388897f;
    Reverb_B_Ring1_GainInv[17] = 0.611103f;
    Reverb_B_Ring1[0][17] = 12691;
    Reverb_B_Ring1[1][17] = 12846;
    Reverb_B_Ring1_Gain[18] = 0.201819f;
    Reverb_B_Ring1_GainInv[18] = 0.798181f;
    Reverb_B_Ring1[0][18] = 15684;
    Reverb_B_Ring1[1][18] = 15897;
    Reverb_B_Ring1_Gain[19] = 0.099399f;
    Reverb_B_Ring1_GainInv[19] = 0.900601f;
    Reverb_B_Ring1[0][19] = 17323;
    Reverb_B_Ring1[1][19] = 17570;
    Reverb_B_Ring1_Gain[20] = 0.85519f;
    Reverb_B_Ring1_GainInv[20] = 0.14481f;
    Reverb_B_Ring1[0][20] = 5230;
    Reverb_B_Ring1[1][20] = 5253;
    Reverb_B_Ring1_Gain[21] = 0.369823f;
    Reverb_B_Ring1_GainInv[21] = 0.630177f;
    Reverb_B_Ring1[0][21] = 12996;
    Reverb_B_Ring1[1][21] = 13167;
    Reverb_B_Ring1_Gain[22] = 0.597583f;
    Reverb_B_Ring1_GainInv[22] = 0.402417f;
    Reverb_B_Ring1[0][22] = 9352;
    Reverb_B_Ring1[1][22] = 9457;
    Reverb_B_Ring1_Gain[23] = 0.7463f;
    Reverb_B_Ring1_GainInv[23] = 0.2537f;
    Reverb_B_Ring1[0][23] = 6973;
    Reverb_B_Ring1[1][23] = 7036;
    Reverb_B_Ring1_Gain[24] = 0.86346f;
    Reverb_B_Ring1_GainInv[24] = 0.13654f;
    Reverb_B_Ring1[0][24] = 5098;
    Reverb_B_Ring1[1][24] = 5128;
    Reverb_B_Ring1_Gain[25] = 0.14481f;
    Reverb_B_Ring1_GainInv[25] = 0.85519f;
    Reverb_B_Ring1[0][25] = 16597;
    Reverb_B_Ring1[1][25] = 16846;
    Reverb_B_Ring1_Gain[26] = 0.933836f;
    Reverb_B_Ring1_GainInv[26] = 6.616402E-02f;
    Reverb_B_Ring1[0][26] = 3972;
    Reverb_B_Ring1[1][26] = 3986;
    Reverb_B_Ring1_Gain[27] = 0.572192f;
    Reverb_B_Ring1_GainInv[27] = 0.427808f;
    Reverb_B_Ring1[0][27] = 9758;
    Reverb_B_Ring1[1][27] = 9884;
    Reverb_B_Ring1_Gain[28] = 0.426649f;
    Reverb_B_Ring1_GainInv[28] = 0.573351f;
    Reverb_B_Ring1[0][28] = 12087;
    Reverb_B_Ring1[1][28] = 12259;
    Reverb_B_Ring1_Gain[29] = 0.697714f;
    Reverb_B_Ring1_GainInv[29] = 0.302286f;
    Reverb_B_Ring1[0][29] = 7750;
    Reverb_B_Ring1[1][29] = 7840;
    Reverb_B_Ring1_Gain[30] = 0.451949f;
    Reverb_B_Ring1_GainInv[30] = 0.548051f;
    Reverb_B_Ring1[0][30] = 11682;
    Reverb_B_Ring1[1][30] = 11846;
    Reverb_B_Ring1_Gain[31] = 0.774438f;
    Reverb_B_Ring1_GainInv[31] = 0.225562f;
    Reverb_B_Ring1[0][31] = 6522;
    Reverb_B_Ring1[1][31] = 6589;

    Reverb_B_Ring1_Max[0] = 17500;
    Reverb_B_Ring1_Max[1] = 17500;

}

//R4.00 MEDIUM ROOM
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode6()
{
  Reverb_B_Ring1_Gain[0] = .3821262f;
  Reverb_B_Ring1[0][0] = 505;
  Reverb_B_Ring1[1][0] = 567;
  Reverb_B_Ring1_Gain[1] = -.9110762f;
  Reverb_B_Ring1[0][1] = 1115;
  Reverb_B_Ring1[1][1] = 988;
  Reverb_B_Ring1_Gain[2] = .1839826f;
  Reverb_B_Ring1[0][2] = 1657;
  Reverb_B_Ring1[1][2] = 1250;
  Reverb_B_Ring1_Gain[3] = .4453132f;
  Reverb_B_Ring1[0][3] = 1769;
  Reverb_B_Ring1[1][3] = 2038;
  Reverb_B_Ring1_Gain[4] = .1792066f;
  Reverb_B_Ring1[0][4] = 2598;
  Reverb_B_Ring1[1][4] = 2289;
  Reverb_B_Ring1_Gain[5] = -.2081006f;
  Reverb_B_Ring1[0][5] = 3193;
  Reverb_B_Ring1[1][5] = 2766;
  Reverb_B_Ring1_Gain[6] = .4592228f;
  Reverb_B_Ring1[0][6] = 3542;
  Reverb_B_Ring1[1][6] = 3694;  
  Reverb_B_Ring1_Gain[7] = -.8998467f;
  Reverb_B_Ring1[0][7] = 3891;
  Reverb_B_Ring1[1][7] = 4060;
  Reverb_B_Ring1_Gain[8] = .4299314f;
  Reverb_B_Ring1[0][8] = 4610;
  Reverb_B_Ring1[1][8] = 4262;
  Reverb_B_Ring1_Gain[9] = -.1294288f;
  Reverb_B_Ring1[0][9] = 4997;
  Reverb_B_Ring1[1][9] = 4932;
  Reverb_B_Ring1_Gain[10] = 8.573208E-02f;
  Reverb_B_Ring1[0][10] = 5548;
  Reverb_B_Ring1[1][10] = 5407;
  Reverb_B_Ring1_Gain[11] = 4.716557E-02f;
  Reverb_B_Ring1[0][11] = 5807;
  Reverb_B_Ring1[1][11] = 5899;
  Reverb_B_Ring1_Gain[12] = -.1759361f;
  Reverb_B_Ring1[0][12] = 6587;
  Reverb_B_Ring1[1][12] = 6596;
  Reverb_B_Ring1_Gain[13] = .4485902f;
  Reverb_B_Ring1[0][13] = 6945;
  Reverb_B_Ring1[1][13] = 6993;
  Reverb_B_Ring1_Gain[14] = -.7520947f;
  Reverb_B_Ring1[0][14] = 7413;
  Reverb_B_Ring1[1][14] = 7427;
  Reverb_B_Ring1_Gain[15] = .8129733f;
  Reverb_B_Ring1[0][15] = 8153;
  Reverb_B_Ring1[1][15] = 7891;
  Reverb_B_Ring1_Gain[16] = .4778493f;
  Reverb_B_Ring1[0][16] = 8272;
  Reverb_B_Ring1[1][16] = 8317;
  Reverb_B_Ring1_Gain[17] = -1.0f;
  Reverb_B_Ring1[0][17] = 9207;
  Reverb_B_Ring1[1][17] = 9177;
  Reverb_B_Ring1_Gain[18] = .6981052f;
  Reverb_B_Ring1[0][18] = 9694;
  Reverb_B_Ring1[1][18] = 9330;
  Reverb_B_Ring1_Gain[19] = -.7577275f;
  Reverb_B_Ring1[0][19] = 9951;
  Reverb_B_Ring1[1][19] = 10222;
  Reverb_B_Ring1_Gain[20] = -8.149301E-02f;
  Reverb_B_Ring1[0][20] = 10350;
  Reverb_B_Ring1[1][20] = 10615;
  Reverb_B_Ring1_Gain[21] = .1999508f;
  Reverb_B_Ring1[0][21] = 10966;
  Reverb_B_Ring1[1][21] = 11193;
  Reverb_B_Ring1_Gain[22] = -.4817854f;
  Reverb_B_Ring1[0][22] = 11521;
  Reverb_B_Ring1[1][22] = 11482;
  Reverb_B_Ring1_Gain[23] = .6311288f;
  Reverb_B_Ring1[0][23] = 11855;
  Reverb_B_Ring1[1][23] = 12183;
  Reverb_B_Ring1_Gain[24] = -.2331972f;
  Reverb_B_Ring1[0][24] = 12704;
  Reverb_B_Ring1[1][24] = 12594;
  Reverb_B_Ring1_Gain[25] = .3141531f;
  Reverb_B_Ring1[0][25] = 13160;
  Reverb_B_Ring1[1][25] = 12974;
  Reverb_B_Ring1_Gain[26] = -.4107172f;
  Reverb_B_Ring1[0][26] = 13340;
  Reverb_B_Ring1[1][26] = 13287;
  Reverb_B_Ring1_Gain[27] = -.598618f;
  Reverb_B_Ring1[0][27] = 13900;
  Reverb_B_Ring1[1][27] = 13914;
  Reverb_B_Ring1_Gain[28] = .6999026f;
  Reverb_B_Ring1[0][28] = 14661;
  Reverb_B_Ring1[1][28] = 14520;
  Reverb_B_Ring1_Gain[29] = -.5852267f;
  Reverb_B_Ring1[0][29] = 15235;
  Reverb_B_Ring1[1][29] = 14930;
  Reverb_B_Ring1_Gain[30] = .8004726f;
  Reverb_B_Ring1[0][30] = 15671;
  Reverb_B_Ring1[1][30] = 15440;
  Reverb_B_Ring1_Gain[31] = -.4f;
  Reverb_B_Ring1[0][31] = 15756;
  Reverb_B_Ring1[1][31] = 16158;

  Reverb_B_Ring1_Max[0] = 16200;
  Reverb_B_Ring1_Max[1] = 16200;

}

//R4.00 SMALL ROOM
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode7()
{  
    Reverb_B_Ring1_Gain[0] = 0.998749f;
    Reverb_B_Ring1_GainInv[0] = 1.250982E-03f;
    Reverb_B_Ring1[0][0] = 5421;
    Reverb_B_Ring1[1][0] = 5421;
    Reverb_B_Ring1_Gain[1] = 0.436415f;
    Reverb_B_Ring1_GainInv[1] = 0.563585f;
    Reverb_B_Ring1[0][1] = 19816;
    Reverb_B_Ring1[1][1] = 20086;
    Reverb_B_Ring1_Gain[2] = 0.806696f;
    Reverb_B_Ring1_GainInv[2] = 0.193304f;
    Reverb_B_Ring1[0][2] = 10336;
    Reverb_B_Ring1[1][2] = 10427;
    Reverb_B_Ring1_Gain[3] = 0.19126f;
    Reverb_B_Ring1_GainInv[3] = 0.80874f;
    Reverb_B_Ring1[0][3] = 26091;
    Reverb_B_Ring1[1][3] = 77;
    Reverb_B_Ring1_Gain[4] = 0.414991f;
    Reverb_B_Ring1_GainInv[4] = 0.585009f;
    Reverb_B_Ring1[0][4] = 20365;
    Reverb_B_Ring1[1][4] = 20645;
    Reverb_B_Ring1_Gain[5] = 0.520127f;
    Reverb_B_Ring1_GainInv[5] = 0.479873f;
    Reverb_B_Ring1[0][5] = 17672;
    Reverb_B_Ring1[1][5] = 17901;
    Reverb_B_Ring1_Gain[6] = 0.649709f;
    Reverb_B_Ring1_GainInv[6] = 0.350291f;
    Reverb_B_Ring1[0][6] = 14355;
    Reverb_B_Ring1[1][6] = 14523;
    Reverb_B_Ring1_Gain[7] = 0.104038f;
    Reverb_B_Ring1_GainInv[7] = 0.895962f;
    Reverb_B_Ring1[0][7] = 1924;
    Reverb_B_Ring1[1][7] = 2352;
    Reverb_B_Ring1_Gain[8] = 0.17716f;
    Reverb_B_Ring1_GainInv[8] = 0.82284f;
    Reverb_B_Ring1[0][8] = 52;
    Reverb_B_Ring1[1][8] = 445;
    Reverb_B_Ring1_Gain[9] = 0.253395f;
    Reverb_B_Ring1_GainInv[9] = 0.746605f;
    Reverb_B_Ring1[0][9] = 24501;
    Reverb_B_Ring1[1][9] = 24857;
    Reverb_B_Ring1_Gain[10] = 0.825892f;
    Reverb_B_Ring1_GainInv[10] = 0.174108f;
    Reverb_B_Ring1[0][10] = 9845;
    Reverb_B_Ring1[1][10] = 9928;
    Reverb_B_Ring1_Gain[11] = 0.141057f;
    Reverb_B_Ring1_GainInv[11] = 0.858943f;
    Reverb_B_Ring1[0][11] = 976;
    Reverb_B_Ring1[1][11] = 1388;
    Reverb_B_Ring1_Gain[12] = 0.289499f;
    Reverb_B_Ring1_GainInv[12] = 0.710501f;
    Reverb_B_Ring1[0][12] = 23577;
    Reverb_B_Ring1[1][12] = 23918;
    Reverb_B_Ring1_Gain[13] = 0.486465f;
    Reverb_B_Ring1_GainInv[13] = 0.513535f;
    Reverb_B_Ring1[0][13] = 18534;
    Reverb_B_Ring1[1][13] = 18781;
    Reverb_B_Ring1_Gain[14] = 0.696005f;
    Reverb_B_Ring1_GainInv[14] = 0.303995f;
    Reverb_B_Ring1[0][14] = 13169;
    Reverb_B_Ring1[1][14] = 13315;
    Reverb_B_Ring1_Gain[15] = 0.985015f;
    Reverb_B_Ring1_GainInv[15] = 1.498502E-02f;
    Reverb_B_Ring1[0][15] = 5771;
    Reverb_B_Ring1[1][15] = 5777;
    Reverb_B_Ring1_Gain[16] = 0.908597f;
    Reverb_B_Ring1_GainInv[16] = 9.140301E-02f;
    Reverb_B_Ring1[0][16] = 6791;
    Reverb_B_Ring1[1][16] = 6818;
    Reverb_B_Ring1_Gain[17] = 0.635548f;
    Reverb_B_Ring1_GainInv[17] = 0.364452f;
    Reverb_B_Ring1[0][17] = 11160;
    Reverb_B_Ring1[1][17] = 11269;
    Reverb_B_Ring1_Gain[18] = 0.852687f;
    Reverb_B_Ring1_GainInv[18] = 0.147313f;
    Reverb_B_Ring1[0][18] = 7686;
    Reverb_B_Ring1[1][18] = 7730;
    Reverb_B_Ring1_Gain[19] = 0.834101f;
    Reverb_B_Ring1_GainInv[19] = 0.165899f;
    Reverb_B_Ring1[0][19] = 7983;
    Reverb_B_Ring1[1][19] = 8032;
    Reverb_B_Ring1_Gain[20] = 0.011475f;
    Reverb_B_Ring1_GainInv[20] = 0.988525f;
    Reverb_B_Ring1[0][20] = 21145;
    Reverb_B_Ring1[1][20] = 21441;
    Reverb_B_Ring1_Gain[21] = 0.554308f;
    Reverb_B_Ring1_GainInv[21] = 0.445692f;
    Reverb_B_Ring1[0][21] = 12460;
    Reverb_B_Ring1[1][21] = 12593;
    Reverb_B_Ring1_Gain[22] = 0.880917f;
    Reverb_B_Ring1_GainInv[22] = 0.119083f;
    Reverb_B_Ring1[0][22] = 7234;
    Reverb_B_Ring1[1][22] = 7269;
    Reverb_B_Ring1_Gain[23] = 0.995331f;
    Reverb_B_Ring1_GainInv[23] = 4.669011E-03f;
    Reverb_B_Ring1[0][23] = 5403;
    Reverb_B_Ring1[1][23] = 5404;
    Reverb_B_Ring1_Gain[24] = 0.991089f;
    Reverb_B_Ring1_GainInv[24] = 8.911014E-03f;
    Reverb_B_Ring1[0][24] = 5471;
    Reverb_B_Ring1[1][24] = 5473;
    Reverb_B_Ring1_Gain[25] = 0.62212f;
    Reverb_B_Ring1_GainInv[25] = 0.37788f;
    Reverb_B_Ring1[0][25] = 11375;
    Reverb_B_Ring1[1][25] = 11488;
    Reverb_B_Ring1_Gain[26] = 0.468337f;
    Reverb_B_Ring1_GainInv[26] = 0.531663f;
    Reverb_B_Ring1[0][26] = 13835;
    Reverb_B_Ring1[1][26] = 13994;
    Reverb_B_Ring1_Gain[27] = 0.428816f;
    Reverb_B_Ring1_GainInv[27] = 0.571184f;
    Reverb_B_Ring1[0][27] = 14467;
    Reverb_B_Ring1[1][27] = 14638;
    Reverb_B_Ring1_Gain[28] = 0.398236f;
    Reverb_B_Ring1_GainInv[28] = 0.601764f;
    Reverb_B_Ring1[0][28] = 14957;
    Reverb_B_Ring1[1][28] = 15137;
    Reverb_B_Ring1_Gain[29] = 0.392834f;
    Reverb_B_Ring1_GainInv[29] = 0.607166f;
    Reverb_B_Ring1[0][29] = 15043;
    Reverb_B_Ring1[1][29] = 15225;
    Reverb_B_Ring1_Gain[30] = 0.833766f;
    Reverb_B_Ring1_GainInv[30] = 0.166234f;
    Reverb_B_Ring1[0][30] = 7988;
    Reverb_B_Ring1[1][30] = 8037;
    Reverb_B_Ring1_Gain[31] = 0.336955f;
    Reverb_B_Ring1_GainInv[31] = 0.663045f;
    Reverb_B_Ring1[0][31] = 15937;
    Reverb_B_Ring1[1][31] = 16135;


    Reverb_B_Ring1_Max[0] = 16500;
    Reverb_B_Ring1_Max[1] = 16500;
}

//R4.00 BOX
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode8()
{   
    Reverb_B_Ring1_Gain[0] = .025f;
    Reverb_B_Ring1_GainInv[0] = .975f;
    Reverb_B_Ring1[0][0] = 453;
    Reverb_B_Ring1[1][0] = 253;
    Reverb_B_Ring1_Gain[1] = 0.032258064516129f;
    Reverb_B_Ring1_GainInv[1] = 0.967741935483871f;
    Reverb_B_Ring1[0][1] = 792;
    Reverb_B_Ring1[1][1] = 592;
    Reverb_B_Ring1_Gain[2] = 6.45161290322581E-02f;
    Reverb_B_Ring1_GainInv[2] = 0.935483870967742f;
    Reverb_B_Ring1[0][2] = 1676;
    Reverb_B_Ring1[1][2] = 1476;
    Reverb_B_Ring1_Gain[3] = 9.67741935483871E-02f;
    Reverb_B_Ring1_GainInv[3] = 0.903225806451613f;
    Reverb_B_Ring1[0][3] = 1946;
    Reverb_B_Ring1[1][3] = 1746;
    Reverb_B_Ring1_Gain[4] = 0.129032258064516f;
    Reverb_B_Ring1_GainInv[4] = 0.870967741935484f;
    Reverb_B_Ring1[0][4] = 2409;
    Reverb_B_Ring1[1][4] = 2209;
    Reverb_B_Ring1_Gain[5] = 0.161290322580645f;
    Reverb_B_Ring1_GainInv[5] = 0.838709677419355f;
    Reverb_B_Ring1[0][5] = 3167;
    Reverb_B_Ring1[1][5] = 2967;
    Reverb_B_Ring1_Gain[6] = 0.193548387096774f;
    Reverb_B_Ring1_GainInv[6] = 0.806451612903226f;
    Reverb_B_Ring1[0][6] = 3505;
    Reverb_B_Ring1[1][6] = 3305;
    Reverb_B_Ring1_Gain[7] = 0.225806451612903f;
    Reverb_B_Ring1_GainInv[7] = 0.774193548387097f;
    Reverb_B_Ring1[0][7] = 3938;
    Reverb_B_Ring1[1][7] = 3738;
    Reverb_B_Ring1_Gain[8] = 0.258064516129032f;
    Reverb_B_Ring1_GainInv[8] = 0.741935483870968f;
    Reverb_B_Ring1[0][8] = 4607;
    Reverb_B_Ring1[1][8] = 4407;
    Reverb_B_Ring1_Gain[9] = 0.290322580645161f;
    Reverb_B_Ring1_GainInv[9] = 0.709677419354839f;
    Reverb_B_Ring1[0][9] = 4862;
    Reverb_B_Ring1[1][9] = 4662;
    Reverb_B_Ring1_Gain[10] = 0.32258064516129f;
    Reverb_B_Ring1_GainInv[10] = 0.67741935483871f;
    Reverb_B_Ring1[0][10] = 5471;
    Reverb_B_Ring1[1][10] = 5271;
    Reverb_B_Ring1_Gain[11] = 0.354838709677419f;
    Reverb_B_Ring1_GainInv[11] = 0.645161290322581f;
    Reverb_B_Ring1[0][11] = 6051;
    Reverb_B_Ring1[1][11] = 5851;
    Reverb_B_Ring1_Gain[12] = 0.387096774193548f;
    Reverb_B_Ring1_GainInv[12] = 0.612903225806452f;
    Reverb_B_Ring1[0][12] = 6409;
    Reverb_B_Ring1[1][12] = 6209;
    Reverb_B_Ring1_Gain[13] = 0.419354838709677f;
    Reverb_B_Ring1_GainInv[13] = 0.580645161290323f;
    Reverb_B_Ring1[0][13] = 7130;
    Reverb_B_Ring1[1][13] = 6930;
    Reverb_B_Ring1_Gain[14] = 0.451612903225806f;
    Reverb_B_Ring1_GainInv[14] = 0.548387096774194f;
    Reverb_B_Ring1[0][14] = 7361;
    Reverb_B_Ring1[1][14] = 7161;
    Reverb_B_Ring1_Gain[15] = 0.483870967741936f;
    Reverb_B_Ring1_GainInv[15] = 0.516129032258065f;
    Reverb_B_Ring1[0][15] = 7871;
    Reverb_B_Ring1[1][15] = 7671;
    Reverb_B_Ring1_Gain[16] = 0.516129032258065f;
    Reverb_B_Ring1_GainInv[16] = 0.483870967741936f;
    Reverb_B_Ring1[0][16] = 8590;
    Reverb_B_Ring1[1][16] = 8390;
    Reverb_B_Ring1_Gain[17] = 0.548387096774194f;
    Reverb_B_Ring1_GainInv[17] = 0.451612903225806f;
    Reverb_B_Ring1[0][17] = 8952;
    Reverb_B_Ring1[1][17] = 8752;
    Reverb_B_Ring1_Gain[18] = 0.580645161290323f;
    Reverb_B_Ring1_GainInv[18] = 0.419354838709677f;
    Reverb_B_Ring1[0][18] = 9501;
    Reverb_B_Ring1[1][18] = 9301;
    Reverb_B_Ring1_Gain[19] = 0.612903225806452f;
    Reverb_B_Ring1_GainInv[19] = 0.387096774193548f;
    Reverb_B_Ring1[0][19] = 9836;
    Reverb_B_Ring1[1][19] = 9636;
    Reverb_B_Ring1_Gain[20] = 0.645161290322581f;
    Reverb_B_Ring1_GainInv[20] = 0.354838709677419f;
    Reverb_B_Ring1[0][20] = 10590;
    Reverb_B_Ring1[1][20] = 10390;
    Reverb_B_Ring1_Gain[21] = 0.67741935483871f;
    Reverb_B_Ring1_GainInv[21] = 0.32258064516129f;
    Reverb_B_Ring1[0][21] = 10785;
    Reverb_B_Ring1[1][21] = 10585;
    Reverb_B_Ring1_Gain[22] = 0.709677419354839f;
    Reverb_B_Ring1_GainInv[22] = 0.290322580645161f;
    Reverb_B_Ring1[0][22] = 11508;
    Reverb_B_Ring1[1][22] = 11308;
    Reverb_B_Ring1_Gain[23] = 0.741935483870968f;
    Reverb_B_Ring1_GainInv[23] = 0.258064516129032f;
    Reverb_B_Ring1[0][23] = 11985;
    Reverb_B_Ring1[1][23] = 11785;
    Reverb_B_Ring1_Gain[24] = 0.774193548387097f;
    Reverb_B_Ring1_GainInv[24] = 0.225806451612903f;
    Reverb_B_Ring1[0][24] = 12262;
    Reverb_B_Ring1[1][24] = 12062;
    Reverb_B_Ring1_Gain[25] = 0.806451612903226f;
    Reverb_B_Ring1_GainInv[25] = 0.193548387096774f;
    Reverb_B_Ring1[0][25] = 12813;
    Reverb_B_Ring1[1][25] = 12613;
    Reverb_B_Ring1_Gain[26] = 0.838709677419355f;
    Reverb_B_Ring1_GainInv[26] = 0.161290322580645f;
    Reverb_B_Ring1[0][26] = 13535;
    Reverb_B_Ring1[1][26] = 13335;
    Reverb_B_Ring1_Gain[27] = 0.870967741935484f;
    Reverb_B_Ring1_GainInv[27] = 0.129032258064516f;
    Reverb_B_Ring1[0][27] = 13709;
    Reverb_B_Ring1[1][27] = 13509;
    Reverb_B_Ring1_Gain[28] = 0.903225806451613f;
    Reverb_B_Ring1_GainInv[28] = 9.67741935483871E-02f;
    Reverb_B_Ring1[0][28] = 14625;
    Reverb_B_Ring1[1][28] = 14425;
    Reverb_B_Ring1_Gain[29] = 0.935483870967742f;
    Reverb_B_Ring1_GainInv[29] = 6.45161290322581E-02f;
    Reverb_B_Ring1[0][29] = 15196;
    Reverb_B_Ring1[1][29] = 14996;
    Reverb_B_Ring1_Gain[30] = 0.967741935483871f;
    Reverb_B_Ring1_GainInv[30] = 0.032258064516129f;
    Reverb_B_Ring1[0][30] = 15525;
    Reverb_B_Ring1[1][30] = 15325;
    Reverb_B_Ring1_Gain[31] = 1.0f;
    Reverb_B_Ring1_GainInv[31] = 0.0f;
    Reverb_B_Ring1[0][31] = 16001;
    Reverb_B_Ring1[1][31] = 15801;

    
   float R = 0.0f;
   float R2 = 0.0f;
   float Dist = 0.0f;
   for (int t = 0; t < 32; t++)
   {
       R = rand() / (1.0f * RAND_MAX);
       R2 = rand() / (1.0f * RAND_MAX);
       Dist = ((t + 1.0f) / 33.0f);

       Reverb_B_Ring1_Gain[t] = 1.0f - R;
       Reverb_B_Ring1_GainInv[t] = 1.0f - Reverb_B_Ring1_Gain[t];
       Reverb_B_Ring1[0][t] = 100 + int(R * 16000.0f);
       Reverb_B_Ring1[1][t] = 100 + int(R2 * 16000.0f);  //Reverb_B_Ring1[0][t] + (R * 300);
   }
   
    Reverb_B_Ring1_Max[0] = 16500;
    Reverb_B_Ring1_Max[1] = 16500;
}

//R4.00 DIGIVERB
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode9()
{
    Reverb_B_Ring1_Gain[0] = 9.845117E-02f;
    Reverb_B_Ring1[0][0] = 437;
    Reverb_B_Ring1[1][0] = 410;
    Reverb_B_Ring1_Gain[1] = .2931626f;
    Reverb_B_Ring1[0][1] = 1163;
    Reverb_B_Ring1[1][1] = 1108;
    Reverb_B_Ring1_Gain[2] = -.2914319f;
    Reverb_B_Ring1[0][2] = 1503;
    Reverb_B_Ring1[1][2] = 1603;
    Reverb_B_Ring1_Gain[3] = 6.356899E-03f;
    Reverb_B_Ring1[0][3] = 2038;
    Reverb_B_Ring1[1][3] = 2247;
    Reverb_B_Ring1_Gain[4] = .5874946f;
    Reverb_B_Ring1[0][4] = 2423;
    Reverb_B_Ring1[1][4] = 2453;
    Reverb_B_Ring1_Gain[5] = -1.0f;
    Reverb_B_Ring1[0][5] = 2779;
    Reverb_B_Ring1[1][5] = 2772;
    Reverb_B_Ring1_Gain[6] = -2.741253E-02f;
    Reverb_B_Ring1[0][6] = 3726;
    Reverb_B_Ring1[1][6] = 3532;
    Reverb_B_Ring1_Gain[7] = .5311422f;
    Reverb_B_Ring1[0][7] = 4206;
    Reverb_B_Ring1[1][7] = 4133;
    Reverb_B_Ring1_Gain[8] = 7.429912E-03f;
    Reverb_B_Ring1[0][8] = 4344;
    Reverb_B_Ring1[1][8] = 4700;
    Reverb_B_Ring1_Gain[9] = -5.662441E-02f;
    Reverb_B_Ring1[0][9] = 4762;
    Reverb_B_Ring1[1][9] = 4904;
    Reverb_B_Ring1_Gain[10] = -.5364334f;
    Reverb_B_Ring1[0][10] = 5337;
    Reverb_B_Ring1[1][10] = 5433;
    Reverb_B_Ring1_Gain[11] = .5683663f;
    Reverb_B_Ring1[0][11] = 6057;
    Reverb_B_Ring1[1][11] = 6164;
    Reverb_B_Ring1_Gain[12] = .4668762f;
    Reverb_B_Ring1[0][12] = 6746;
    Reverb_B_Ring1[1][12] = 6261;
    Reverb_B_Ring1_Gain[13] = -.8122798f;
    Reverb_B_Ring1[0][13] = 7028;
    Reverb_B_Ring1[1][13] = 7008;
    Reverb_B_Ring1_Gain[14] = .1791106f;
    Reverb_B_Ring1[0][14] = 7571;
    Reverb_B_Ring1[1][14] = 7344;
    Reverb_B_Ring1_Gain[15] = -.200953f;
    Reverb_B_Ring1[0][15] = 8230;
    Reverb_B_Ring1[1][15] = 8175;
    Reverb_B_Ring1_Gain[16] = -8.675978E-02f;
    Reverb_B_Ring1[0][16] = 8370;
    Reverb_B_Ring1[1][16] = 8662;
    Reverb_B_Ring1_Gain[17] = .3683352f;
    Reverb_B_Ring1[0][17] = 8750;
    Reverb_B_Ring1[1][17] = 8813;
    Reverb_B_Ring1_Gain[18] = .2446921f;
    Reverb_B_Ring1[0][18] = 9488;
    Reverb_B_Ring1[1][18] = 9341;
    Reverb_B_Ring1_Gain[19] = -.4359645f;
    Reverb_B_Ring1[0][19] = 10246;
    Reverb_B_Ring1[1][19] = 9855;
    Reverb_B_Ring1_Gain[20] = .8281444f;
    Reverb_B_Ring1[0][20] = 10404;
    Reverb_B_Ring1[1][20] = 10594;
    Reverb_B_Ring1_Gain[21] = -.4322748f;
    Reverb_B_Ring1[0][21] = 11109;
    Reverb_B_Ring1[1][21] = 11029;
    Reverb_B_Ring1_Gain[22] = -.5024753f;
    Reverb_B_Ring1[0][22] = 11714;
    Reverb_B_Ring1[1][22] = 11291;
    Reverb_B_Ring1_Gain[23] = 9.639473E-02f;
    Reverb_B_Ring1[0][23] = 11757;
    Reverb_B_Ring1[1][23] = 11992;
    Reverb_B_Ring1_Gain[24] = -6.157491E-02f;
    Reverb_B_Ring1[0][24] = 12729;
    Reverb_B_Ring1[1][24] = 12658;
    Reverb_B_Ring1_Gain[25] = .4133125f;
    Reverb_B_Ring1[0][25] = 13044;
    Reverb_B_Ring1[1][25] = 13088;
    Reverb_B_Ring1_Gain[26] = 5.196083E-02f;
    Reverb_B_Ring1[0][26] = 13700;
    Reverb_B_Ring1[1][26] = 13597;
    Reverb_B_Ring1_Gain[27] = -.5481797f;
    Reverb_B_Ring1[0][27] = 13801;
    Reverb_B_Ring1[1][27] = 14228;
    Reverb_B_Ring1_Gain[28] = -7.678449E-03f;
    Reverb_B_Ring1[0][28] = 14673;
    Reverb_B_Ring1[1][28] = 14625;
    Reverb_B_Ring1_Gain[29] = .5005625f;
    Reverb_B_Ring1[0][29] = 15000;
    Reverb_B_Ring1[1][29] = 14901;
    Reverb_B_Ring1_Gain[30] = -.2482897f;
    Reverb_B_Ring1[0][30] = 15463;
    Reverb_B_Ring1[1][30] = 15453;
    Reverb_B_Ring1_Gain[31] = .3588121f;
    Reverb_B_Ring1[0][31] = 16046;
    Reverb_B_Ring1[1][31] = 15804;

    Reverb_B_Ring1_Max[0] = 16050;
    Reverb_B_Ring1_Max[1] = 16050;
}

//R4.00 SPACE
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode10()
{
    Reverb_B_Ring1_Gain[0] = .4f;
    Reverb_B_Ring1[0][0] = 388;
    Reverb_B_Ring1[1][0] = 643;
    Reverb_B_Ring1_Gain[1] = .2940496f;
    Reverb_B_Ring1[0][1] = 1206;
    Reverb_B_Ring1[1][1] = 754;
    Reverb_B_Ring1_Gain[2] = -.3521848f;
    Reverb_B_Ring1[0][2] = 1744;
    Reverb_B_Ring1[1][2] = 1342;
    Reverb_B_Ring1_Gain[3] = -.26898996f;
    Reverb_B_Ring1[0][3] = 1939;
    Reverb_B_Ring1[1][3] = 2190;
    Reverb_B_Ring1_Gain[4] = -.35566038f;
    Reverb_B_Ring1[0][4] = 2647;
    Reverb_B_Ring1[1][4] = 2617;
    Reverb_B_Ring1_Gain[5] = .46955155f;
    Reverb_B_Ring1[0][5] = 2780;
    Reverb_B_Ring1[1][5] = 2792;
    Reverb_B_Ring1_Gain[6] = .4f; //  1;
    Reverb_B_Ring1[0][6] = 3596;
    Reverb_B_Ring1[1][6] = 3634;
    Reverb_B_Ring1_Gain[7] = -.5077751f;
    Reverb_B_Ring1[0][7] = 3765;
    Reverb_B_Ring1[1][7] = 3853;
    Reverb_B_Ring1_Gain[8] = -.3390408f;
    Reverb_B_Ring1[0][8] = 4490;
    Reverb_B_Ring1[1][8] = 4349;
    Reverb_B_Ring1_Gain[9] = -.4819373f;
    Reverb_B_Ring1[0][9] = 5058;
    Reverb_B_Ring1[1][9] = 4790;
    Reverb_B_Ring1_Gain[10] = .1f;
    Reverb_B_Ring1[0][10] = 5358;
    Reverb_B_Ring1[1][10] = 5329;
    Reverb_B_Ring1_Gain[11] = .4395841f;
    Reverb_B_Ring1[0][11] = 5986;
    Reverb_B_Ring1[1][11] = 6027;
    Reverb_B_Ring1_Gain[12] = .3517525f;
    Reverb_B_Ring1[0][12] = 6712;
    Reverb_B_Ring1[1][12] = 6700;
    Reverb_B_Ring1_Gain[13] = .3341072f;
    Reverb_B_Ring1[0][13] = 7205;
    Reverb_B_Ring1[1][13] = 7002;
    Reverb_B_Ring1_Gain[14] = -.50578f;
    Reverb_B_Ring1[0][14] = 7518;
    Reverb_B_Ring1[1][14] = 7713;
    Reverb_B_Ring1_Gain[15] = -.5532824f;
    Reverb_B_Ring1[0][15] = 7988;
    Reverb_B_Ring1[1][15] = 7799;
    Reverb_B_Ring1_Gain[16] = .47f;
    Reverb_B_Ring1[0][16] = 8305;
    Reverb_B_Ring1[1][16] = 8445;
    Reverb_B_Ring1_Gain[17] = .4176289f;
    Reverb_B_Ring1[0][17] = 9143;
    Reverb_B_Ring1[1][17] = 8908;
    Reverb_B_Ring1_Gain[18] = .3363495f;
    Reverb_B_Ring1[0][18] = 9384;
    Reverb_B_Ring1[1][18] = 9614;
    Reverb_B_Ring1_Gain[19] = .3f; 
    Reverb_B_Ring1[0][19] = 10045;
    Reverb_B_Ring1[1][19] = 10106;
    Reverb_B_Ring1_Gain[20] = -.4f;
    Reverb_B_Ring1[0][20] = 10705;
    Reverb_B_Ring1[1][20] = 10526;
    Reverb_B_Ring1_Gain[21] = -.449884f;
    Reverb_B_Ring1[0][21] = 10804;
    Reverb_B_Ring1[1][21] = 10966;
    Reverb_B_Ring1_Gain[22] = -.5307959f;
    Reverb_B_Ring1[0][22] = 11333;
    Reverb_B_Ring1[1][22] = 11646;
    Reverb_B_Ring1_Gain[23] = .4743655f;
    Reverb_B_Ring1[0][23] = 12058;
    Reverb_B_Ring1[1][23] = 11988;
    Reverb_B_Ring1_Gain[24] = .2549816f;
    Reverb_B_Ring1[0][24] = 12546;
    Reverb_B_Ring1[1][24] = 12295;
    Reverb_B_Ring1_Gain[25] = -3.252953E-02f;
    Reverb_B_Ring1[0][25] = 12877;
    Reverb_B_Ring1[1][25] = 12860;
    Reverb_B_Ring1_Gain[26] = -.3232982f;
    Reverb_B_Ring1[0][26] = 13565;
    Reverb_B_Ring1[1][26] = 13465;
    Reverb_B_Ring1_Gain[27] = -.461474f;
    Reverb_B_Ring1[0][27] = 13814;
    Reverb_B_Ring1[1][27] = 13836;
    Reverb_B_Ring1_Gain[28] = .25f; 
    Reverb_B_Ring1[0][28] = 14404;
    Reverb_B_Ring1[1][28] = 14269;
    Reverb_B_Ring1_Gain[29] = .2440197f;
    Reverb_B_Ring1[0][29] = 15059;
    Reverb_B_Ring1[1][29] = 15249;
    Reverb_B_Ring1_Gain[30] = .4150768f;
    Reverb_B_Ring1[0][30] = 15347;
    Reverb_B_Ring1[1][30] = 15513;
    Reverb_B_Ring1_Gain[31] = .1883435f;
    Reverb_B_Ring1[0][31] = 16233;
    Reverb_B_Ring1[1][31] = 16142;

    for (int t = 0; t < 31; t++)
    {
        Reverb_B_Ring1_Gain[t] = .01f + (t / 36.0f);
    }

    Reverb_B_Ring1_Max[0] = 16250;
    Reverb_B_Ring1_Max[1] = 16250;
}

//R4.00 Garage
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode11()
{
    RevB_Gain[0] = -1.185651E-03f;
    RevB_Max[0][0] = 4332;
    RevB_Max[1][0] = 5132;
    RevB_Gain[1] = .1188914f;
    RevB_Max[0][1] = 8402;
    RevB_Max[1][1] = 9239;
    RevB_Gain[2] = -.3848826f;
    RevB_Max[0][2] = 13317;
    RevB_Max[1][2] = 14223;
    RevB_Gain[3] = .3194438f;
    RevB_Max[0][3] = 17367;
    RevB_Max[1][3] = 18007;
    RevB_Gain[4] = .1176973f;
    RevB_Max[0][4] = 22903;
    RevB_Max[1][4] = 22174;
    RevB_Gain[5] = -.5027322f;
    RevB_Max[0][5] = 26266;
    RevB_Max[1][5] = 26931;
    RevB_Gain[6] = .7059703f;
    RevB_Max[0][6] = 32144;
    RevB_Max[1][6] = 30791;
    RevB_Gain[7] = -.7168727f;
    RevB_Max[0][7] = 35775;
    RevB_Max[1][7] = 36704;
    RevB_Gain[8] = .992442f;
    RevB_Max[0][8] = 39802;
    RevB_Max[1][8] = 39754;
    RevB_Gain[9] = -1.0f;
    RevB_Max[0][9] = 45444;
    RevB_Max[1][9] = 45392;
    RevB_Gain[10] = .6304718f;
    RevB_Max[0][10] = 49529;
    RevB_Max[1][10] = 49081;
    RevB_Gain[11] = -.2074285f;
    RevB_Max[0][11] = 53819;
    RevB_Max[1][11] = 54077;
    RevB_Gain[12] = -4.904115E-02f;
    RevB_Max[0][12] = 58918;
    RevB_Max[1][12] = 58958;
    RevB_Gain[13] = .3219758f;
    RevB_Max[0][13] = 63431;
    RevB_Max[1][13] = 62385;
    RevB_Gain[14] = -.5606492f;
    RevB_Max[0][14] = 66935;
    RevB_Max[1][14] = 66956;
    RevB_Gain[15] = .6529667f;
    RevB_Max[0][15] = 72664;
    RevB_Max[1][15] = 71883;
}

//R4.00 Fused Echo
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode12()
{       
    RevB_Gain[0] = -1.185651E-03f;
    RevB_Max[0][0] = 4332;
    RevB_Max[1][0] = 5132;
    RevB_Gain[1] = .1188914f;
    RevB_Max[0][1] = 8402;
    RevB_Max[1][1] = 9239;
    RevB_Gain[2] = -.3848826f;
    RevB_Max[0][2] = 13317;
    RevB_Max[1][2] = 14223;
    RevB_Gain[3] = .3194438f;
    RevB_Max[0][3] = 17367;
    RevB_Max[1][3] = 18007;
    RevB_Gain[4] = .1176973f;
    RevB_Max[0][4] = 22903;
    RevB_Max[1][4] = 22174;
    RevB_Gain[5] = -.5027322f;
    RevB_Max[0][5] = 26266;
    RevB_Max[1][5] = 26931;
    RevB_Gain[6] = .7059703f;
    RevB_Max[0][6] = 32144;
    RevB_Max[1][6] = 30791;
    RevB_Gain[7] = -.7168727f;
    RevB_Max[0][7] = 35775;
    RevB_Max[1][7] = 36704;
    RevB_Gain[8] = .992442f;
    RevB_Max[0][8] = 39802;
    RevB_Max[1][8] = 39754;
    RevB_Gain[9] = -1.0f;
    RevB_Max[0][9] = 45444;
    RevB_Max[1][9] = 45392;
    RevB_Gain[10] = .6304718f;
    RevB_Max[0][10] = 49529;
    RevB_Max[1][10] = 49081;
    RevB_Gain[11] = -.2074285f;
    RevB_Max[0][11] = 53819;
    RevB_Max[1][11] = 54077;
    RevB_Gain[12] = -4.904115E-02f;
    RevB_Max[0][12] = 58918;
    RevB_Max[1][12] = 58958;
    RevB_Gain[13] = .3219758f;
    RevB_Max[0][13] = 63431;
    RevB_Max[1][13] = 62385;
    RevB_Gain[14] = -.5606492f;
    RevB_Max[0][14] = 66935;
    RevB_Max[1][14] = 66956;
    RevB_Gain[15] = .6529667f;
    RevB_Max[0][15] = 72664;
    RevB_Max[1][15] = 71883;

}

//R4.00 MEDIUM TILE
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode13()
{
    RevB_Gain[0] = -.3509227f;
    RevB_Max[0][0] = 5117;
    RevB_Max[1][0] = 4444;
    RevB_Gain[1] = -.1050734f;
    RevB_Max[0][1] = 8476;
    RevB_Max[1][1] = 8995;
    RevB_Gain[2] = 1.0f;
    RevB_Max[0][2] = 14120;
    RevB_Max[1][2] = 13104;
    RevB_Gain[3] = -.6315676f;
    RevB_Max[0][3] = 18359;
    RevB_Max[1][3] = 17634;
    RevB_Gain[4] = -2.099007E-02f;
    RevB_Max[0][4] = 22219;
    RevB_Max[1][4] = 22141;
    RevB_Gain[5] = .2094507f;
    RevB_Max[0][5] = 26623;
    RevB_Max[1][5] = 26586;
    RevB_Gain[6] = -.1916206f;
    RevB_Max[0][6] = 30998;
    RevB_Max[1][6] = 31768;
    RevB_Gain[7] = .3985071f;
    RevB_Max[0][7] = 35725;
    RevB_Max[1][7] = 35703;
    RevB_Gain[8] = -.4198231f;
    RevB_Max[0][8] = 40251;
    RevB_Max[1][8] = 40072;
    RevB_Gain[9] = 9.180727E-02f;
    RevB_Max[0][9] = 45498;
    RevB_Max[1][9] = 45205;
    RevB_Gain[10] = .0670765f;
    RevB_Max[0][10] = 50160;
    RevB_Max[1][10] = 49975;
    RevB_Gain[11] = -3.344628E-02f;
    RevB_Max[0][11] = 53268;
    RevB_Max[1][11] = 53831;
    RevB_Gain[12] = -4.036823E-02f;
    RevB_Max[0][12] = 57851;
    RevB_Max[1][12] = 58572;
    RevB_Gain[13] = -6.195487E-02f;
    RevB_Max[0][13] = 63347;
    RevB_Max[1][13] = 63363;
    RevB_Gain[14] = .1139222f;
    RevB_Max[0][14] = 67821;
    RevB_Max[1][14] = 67838;
    RevB_Gain[15] = -.08f;
    RevB_Max[0][15] = 71401;
    RevB_Max[1][15] = 71525;
}

//R4.00 ARENA
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode14()
{
    RevB_Gain[0] = -.8458497f;
    RevB_Max[0][0] = 4286;
    RevB_Max[1][0] = 4826;
    RevB_Gain[1] = .5197228f;
    RevB_Max[0][1] = 9158;
    RevB_Max[1][1] = 8964;
    RevB_Gain[2] = -1.0f;
    RevB_Max[0][2] = 14168;
    RevB_Max[1][2] = 13101;
    RevB_Gain[3] = .5348588f;
    RevB_Max[0][3] = 17945;
    RevB_Max[1][3] = 18641;
    RevB_Gain[4] = -.1040258f;
    RevB_Max[0][4] = 22899;
    RevB_Max[1][4] = 22492;
    RevB_Gain[5] = .3042369f;
    RevB_Max[0][5] = 27398;
    RevB_Max[1][5] = 26278;
    RevB_Gain[6] = 7.339622E-02f;
    RevB_Max[0][6] = 31876;
    RevB_Max[1][6] = 30774;
    RevB_Gain[7] = -.1582493f;
    RevB_Max[0][7] = 35287;
    RevB_Max[1][7] = 35286;
    RevB_Gain[8] = .1131914f;
    RevB_Max[0][8] = 40706;
    RevB_Max[1][8] = 41096;
    RevB_Gain[9] = -4.286283E-02f;
    RevB_Max[0][9] = 44256;
    RevB_Max[1][9] = 44948;
    RevB_Gain[10] = .2538679f;
    RevB_Max[0][10] = 49385;
    RevB_Max[1][10] = 49443;
    RevB_Gain[11] = -.3058398f;
    RevB_Max[0][11] = 54445;
    RevB_Max[1][11] = 54566;
    RevB_Gain[12] = 3.067049E-02f;
    RevB_Max[0][12] = 57848;
    RevB_Max[1][12] = 57971;
    RevB_Gain[13] = -.245273f;
    RevB_Max[0][13] = 62478;
    RevB_Max[1][13] = 63486;
    RevB_Gain[14] = .1119217f;
    RevB_Max[0][14] = 67407;
    RevB_Max[1][14] = 67719;
    RevB_Gain[15] = -4.996012E-02f;
    RevB_Max[0][15] = 71334;
    RevB_Max[1][15] = 71491;
}

//R4.00 Garage 2
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode15()
{
    RevB_Gain[0] = 1.000000f;
    RevB_Max[0][0] = 4582;
    RevB_Max[1][0] = 4125;
    RevB_Gain[1] = -.789672f;
    RevB_Max[0][1] = 9198;
    RevB_Max[1][1] = 9032;
    RevB_Gain[2] = -.257851f;
    RevB_Max[0][2] = 13401;
    RevB_Max[1][2] = 14091;
    RevB_Gain[3] = .237319f;
    RevB_Max[0][3] = 17413;
    RevB_Max[1][3] = 17424;
    RevB_Gain[4] = -.310135f;
    RevB_Max[0][4] = 22990;
    RevB_Max[1][4] = 22406;
    RevB_Gain[5] = .129198f;
    RevB_Max[0][5] = 27092;
    RevB_Max[1][5] = 27383;
    RevB_Gain[6] = .164248f;
    RevB_Max[0][6] = 30997;
    RevB_Max[1][6] = 32097;
    RevB_Gain[7] = -.072691f;
    RevB_Max[0][7] = 35253;
    RevB_Max[1][7] = 35574;
    RevB_Gain[8] = .021599f;
    RevB_Max[0][8] = 40780;
    RevB_Max[1][8] = 41063;
    RevB_Gain[9] = .125907f;
    RevB_Max[0][9] = 45292;
    RevB_Max[1][9] = 45028;
    RevB_Gain[10] = -.083202f;
    RevB_Max[0][10] = 50082;
    RevB_Max[1][10] = 49038;
    RevB_Gain[11] = -.005068f;
    RevB_Max[0][11] = 53597;
    RevB_Max[1][11] = 54298;
    RevB_Gain[12] = .014442f;
    RevB_Max[0][12] = 58091;
    RevB_Max[1][12] = 58928;
    RevB_Gain[13] = -.023028f;
    RevB_Max[0][13] = 63543;
    RevB_Max[1][13] = 63478;
    RevB_Gain[14] = .013807f;
    RevB_Max[0][14] = 66868;
    RevB_Max[1][14] = 68024;
    RevB_Gain[15] = .003245f;
    RevB_Max[0][15] = 71359;
    RevB_Max[1][15] = 71604;
}

//R4.00 INVERTED
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode16()
{   
    RevB_Gain[0] = -.002118f;
    RevB_Max[0][0] = 4076;
    RevB_Max[1][0] = 3934;
    RevB_Gain[1] = -.059591f;
    RevB_Max[0][1] = 8904;
    RevB_Max[1][1] = 9517;
    RevB_Gain[2] = .100794f;
    RevB_Max[0][2] = 13141;
    RevB_Max[1][2] = 12783;
    RevB_Gain[3] = .008219f;
    RevB_Max[0][3] = 18593;
    RevB_Max[1][3] = 17609;
    RevB_Gain[4] = .122832f;
    RevB_Max[0][4] = 21753;
    RevB_Max[1][4] = 22156;
    RevB_Gain[5] = -.049918f;
    RevB_Max[0][5] = 27725;
    RevB_Max[1][5] = 27334;
    RevB_Gain[6] = -.237285f;
    RevB_Max[0][6] = 31590;
    RevB_Max[1][6] = 31984;
    RevB_Gain[7] = .035933f;
    RevB_Max[0][7] = 36600;
    RevB_Max[1][7] = 36011;
    RevB_Gain[8] = -.184627f;
    RevB_Max[0][8] = 41039;
    RevB_Max[1][8] = 40074;
    RevB_Gain[9] = .062678f;
    RevB_Max[0][9] = 44392;
    RevB_Max[1][9] = 44328;
    RevB_Gain[10] = .420404f;
    RevB_Max[0][10] = 48878;
    RevB_Max[1][10] = 49065;
    RevB_Gain[11] = 1.000000f;
    RevB_Max[0][11] = 53774;
    RevB_Max[1][11] = 54494;
    RevB_Gain[12] = -.245439f;
    RevB_Max[0][12] = 58324;
    RevB_Max[1][12] = 58770;
    RevB_Gain[13] = -.010151f;
    RevB_Max[0][13] = 62400;
    RevB_Max[1][13] = 62759;
    RevB_Gain[14] = -.453049f;
    RevB_Max[0][14] = 67969;
    RevB_Max[1][14] = 67886;
    RevB_Gain[15] = -.241916f;
    RevB_Max[0][15] = 71280;
    RevB_Max[1][15] = 71816;
}

//R2.14 HANGAR
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode17()
{   
    RevB_Gain[0] = .747714f;
    RevB_Max[0][0] = 4820;
    RevB_Max[1][0] = 4701;
    RevB_Gain[1] = -1.000000f;
    RevB_Max[0][1] = 9601;
    RevB_Max[1][1] = 8925;
    RevB_Gain[2] = .509121f;
    RevB_Max[0][2] = 13416;
    RevB_Max[1][2] = 13457;
    RevB_Gain[3] = -.597189f;
    RevB_Max[0][3] = 18072;
    RevB_Max[1][3] = 17674;
    RevB_Gain[4] = .511270f;
    RevB_Max[0][4] = 23059;
    RevB_Max[1][4] = 22736;
    RevB_Gain[5] = -.238252f;
    RevB_Max[0][5] = 27485;
    RevB_Max[1][5] = 26555;
    RevB_Gain[6] = .380602f;
    RevB_Max[0][6] = 30927;
    RevB_Max[1][6] = 30971;
    RevB_Gain[7] = .313804f;
    RevB_Max[0][7] = 36641;
    RevB_Max[1][7] = 35888;
    RevB_Gain[8] = .321968f;
    RevB_Max[0][8] = 39908;
    RevB_Max[1][8] = 40466;
    RevB_Gain[9] = -.304256f;
    RevB_Max[0][9] = 44715;
    RevB_Max[1][9] = 44861;
    RevB_Gain[10] = -.236970f;
    RevB_Max[0][10] = 48778;
    RevB_Max[1][10] = 49363;
    RevB_Gain[11] = .197365f;
    RevB_Max[0][11] = 54377;
    RevB_Max[1][11] = 54183;
    RevB_Gain[12] = .165404f;
    RevB_Max[0][12] = 57755;
    RevB_Max[1][12] = 58975;
    RevB_Gain[13] = -.174817f;
    RevB_Max[0][13] = 63285;
    RevB_Max[1][13] = 63105;
    RevB_Gain[14] = -.144482f;
    RevB_Max[0][14] = 66932;
    RevB_Max[1][14] = 67997;
    RevB_Gain[15] = .102314f;
    RevB_Max[0][15] = 72446;
    RevB_Max[1][15] = 71318;
}

//R4.00 INVERTED 2
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode18()
{    
    RevB_Gain[0] = .102314f;
    RevB_Max[0][0] = 3969;
    RevB_Max[1][0] = 4816;
    RevB_Gain[1] = -.144482f;
    RevB_Max[0][1] = 9071;
    RevB_Max[1][1] = 9577;
    RevB_Gain[2] = -.174817f;
    RevB_Max[0][2] = 12774;
    RevB_Max[1][2] = 12947;
    RevB_Gain[3] = .165404f;
    RevB_Max[0][3] = 17914;
    RevB_Max[1][3] = 18709;
    RevB_Gain[4] = .197365f;
    RevB_Max[0][4] = 22765;
    RevB_Max[1][4] = 22680;
    RevB_Gain[5] = -.236970f;
    RevB_Max[0][5] = 27465;
    RevB_Max[1][5] = 27177;
    RevB_Gain[6] = -.304256f;
    RevB_Max[0][6] = 31873;
    RevB_Max[1][6] = 31697;
    RevB_Gain[7] = .321968f;
    RevB_Max[0][7] = 35774;
    RevB_Max[1][7] = 36050;
    RevB_Gain[8] = .313804f;
    RevB_Max[0][8] = 40734;
    RevB_Max[1][8] = 40053;
    RevB_Gain[9] = .380602f;
    RevB_Max[0][9] = 44267;
    RevB_Max[1][9] = 45266;
    RevB_Gain[10] = -.238252f;
    RevB_Max[0][10] = 48874;
    RevB_Max[1][10] = 49639;
    RevB_Gain[11] = .511270f;
    RevB_Max[0][11] = 53364;
    RevB_Max[1][11] = 53285;
    RevB_Gain[12] = -.597189f;
    RevB_Max[0][12] = 58762;
    RevB_Max[1][12] = 58017;
    RevB_Gain[13] = .509121f;
    RevB_Max[0][13] = 62972;
    RevB_Max[1][13] = 63106;
    RevB_Gain[14] = -1.000000f;
    RevB_Max[0][14] = 68240;
    RevB_Max[1][14] = 67634;
    RevB_Gain[15] = .747714f;
    RevB_Max[0][15] = 71849;
    RevB_Max[1][15] = 72172;
}

//R4.00 REGENT
void MakoDist01AudioProcessor::Mako_FX_Reverb_SetMode19()
{   
    RevB_Gain[0] = 0.5f;
    RevB_Max[0][0] = 1210;
    RevB_Max[1][0] = 2210;
    RevB_Gain[1] = 6.0f;
    RevB_Max[0][1] = 210;
    RevB_Max[1][1] = 410;
    RevB_Gain[2] = .75f;
    RevB_Max[0][2] = 158;
    RevB_Max[1][2] = 358;
    RevB_Gain[3] = .625f;
    RevB_Max[0][3] = 561;
    RevB_Max[1][3] = 761;
    RevB_Gain[4] = .625f;
    RevB_Max[0][4] = 410;
    RevB_Max[1][4] = 610;
    RevB_Gain[5] = .7f;
    RevB_Max[0][5] = 908;
    RevB_Max[1][5] = 1108;
    RevB_Gain[6] = .7f;
    RevB_Max[0][6] = 1343;
    RevB_Max[1][6] = 1543;
    RevB_Gain[7] = .3f;
    RevB_Max[0][7] = 995;
    RevB_Max[1][7] = 1195;
    RevB_Gain[8] = .3f;
    RevB_Max[0][8] = 6590;
    RevB_Max[1][8] = 6790;
    RevB_Gain[9] = .5f;
    RevB_Max[0][9] = 3931;
    RevB_Max[1][9] = 4131;
    RevB_Gain[10] = .5f;
    RevB_Max[0][10] = 2664;
    RevB_Max[1][10] = 2864;
    RevB_Gain[11] = .3f;
    RevB_Max[0][11] = 4641;
    RevB_Max[1][11] = 4841;
    RevB_Gain[12] = .3f;
    RevB_Max[0][12] = 5505;
    RevB_Max[1][12] = 5705;
    RevB_Gain[13] = .25f;
    RevB_Max[0][13] = 9300;
    RevB_Max[1][13] = 9500;
    RevB_Gain[14] = .25f;
    RevB_Max[0][14] = 12300;
    RevB_Max[1][14] = 12500;
    RevB_Gain[15] = .2f;
    RevB_Max[0][15] = 14500;
    RevB_Max[1][15] = 14700;

    for (int t = 0; t < 16; t++)
    {
        RevB_Max[0][t] *= 5;
        RevB_Max[1][t] *= 5;
    }

}

//R4.00 Apply reverb to the incoming sample.
float MakoDist01AudioProcessor::Mako_FX_Reverb(float tSample, int channel)
{
    float tS;
        
    //R2.22 Deal with PreDelay. 
    tS = PreDBuff[channel][PreDCurrIdx[channel]];
    PreDBuff[channel][PreDCurrIdx[channel]] = tSample;
    PreDCurrIdx[channel]++; if (PreDEnd < PreDCurrIdx[channel]) PreDCurrIdx[channel] = 0;

    //R2.22 Keep track of our volume for ducking.
    Reverb_Duck_Peak[channel] *= RATI1000ms; 
    if (Reverb_Duck_Peak[channel] < abs(tS)) Reverb_Duck_Peak[channel] = abs(tS);

    //R2.22 Render the reverb, passing in both the sample and the delayed sample (tS).
    if (Reverb_Render_Mode)
        tS = Mako_FX_Reverb_Buffer(tSample, tS, channel);
    else
        tS = Mako_FX_Reverb_Normal(tSample, tS, channel);

    return tS;
}

//R4.00 Single echo buffer reverb.
float MakoDist01AudioProcessor::Mako_FX_Reverb_Normal(float tSample, float tSVerb, int channel)
{
    float V = tSVerb; //R2.22 Added PreDelay.

    //R1.00 Add echoes. 
    for (int t = 0; t < 15; t++)
    {
        V = (V * Reverb_B_Ring1_GainInv[t]) + (Reverb_B[channel][Reverb_B_Ring1[channel][t]] * Reverb_B_Ring1_Gain[t]);

        Reverb_B_Ring1[channel][t]++;
        if (Reverb_B_Ring1_Max[channel] < Reverb_B_Ring1[channel][t]) Reverb_B_Ring1[channel][t] = 0;
    }

    //R1.00 Apply MODULATIONs.
    V = Mako_FX_ReverbChorus(V, channel);                    //R1.00 Reverb chorus. Fixed settings.

    //R1.00 Clip reverb or it runs away.
    V = Mako_Clip(V, vu_Rev);

    //R2.22 Apply filters.
    float tS = tSVerb; 
    tS = Filter_Calc_BiQuad(tS, channel, &fil_Rev_LP); 
    if (50.001f < Setting[e_RevHP]) tS = Filter_Calc_BiQuad(tS, channel, &fil_Rev_HP);

    //R1.00 Store new echo.
    Reverb_B[channel][Reverb_B_Ring1[channel][0]] = (((Reverb_B[channel][Reverb_B_Ring1[channel][0]] + Reverb_B_Last[channel]) * .5f) * Setting[e_RevTime] + ((tS - V) * .5f) * Reverb_TimeInv);
    Reverb_B_Last[channel] = Reverb_B[channel][Reverb_B_Ring1[channel][0]];

    //R2.22 We need to add some vol for long decay times. Added Ducking.
    return (tSample * Reverb_Dry) + Mako_FX_Reverb_Ducking(V * (4.0f + Setting[e_RevTime] * 8.0f) * Reverb_Wet,channel) * Reverb_BalLR[channel];
}

//R4.00 16 echo buffer reverb.
float MakoDist01AudioProcessor::Mako_FX_Reverb_Buffer(float tSample, float tSVerb, int channel)
{
    float V = 0.0f;

    //R1.00 Add echoes. 
    for (int t = 0; t < 16; t++)
    {
        V = V + (Rev_Buffer[channel][t][RevB_Idx[channel][t]] * RevB_Gain[t]);
    }
    V = V * Reverb_Gain_Factor; //R4.00 Adjust any vol we made during this calc.

    //R2.00 Apply Reverb MODULATION.
    V = Mako_FX_ReverbChorus(V, channel);

    //R1.00 Clip reverb or it runs away.
    V = Mako_Clip(V, vu_Rev);

    //R2.22 Apply filters.
    float tS = tSVerb; //R2.22 Added PreDelay.
    tS = Filter_Calc_BiQuad(tS, channel, &fil_Rev_LP);
    if (50.001f < Setting[e_RevHP]) tS = Filter_Calc_BiQuad(tS, channel, &fil_Rev_HP);

    //R1.00 Store new echoes.
    for (int t = 0; t < 16; t++)
    {
        //R2.14 Evenly mix Buffer + New(tS) + Echoes(V)
        Rev_Buffer[channel][t][RevB_Idx[channel][t]] = ((Rev_Buffer[channel][t][RevB_Idx[channel][t]] * Setting[e_RevTime]) + ((tS + V) * .5f)) * .5f;

        //R2.14 Inc buffer Index.
        RevB_Idx[channel][t]++;
        if (RevB_Max[channel][t] < RevB_Idx[channel][t]) RevB_Idx[channel][t] = 0;        
    }

    //R1.00 Mix the dry and wet (reverb) signals. We need to add some vol for long decay times (x20).
    return (tSample * Reverb_Dry) + Mako_FX_Reverb_Ducking(V * (Setting[e_RevTime] * 20.0f) * Reverb_Wet, channel) * Reverb_BalLR[channel];
}

//R4.00 Fixed setting Chorus used in the reverb block.
float MakoDist01AudioProcessor::Mako_FX_ReverbChorus(float tSample, int channel)
{
    //R1.00 Store new sample in Ring buffer. 
    RevMod_B[channel][RevMod_B_Ring1[channel]] = tSample;

    //R1.00 Calc rotating index.
    int idx = int(RevMod_B_Ring1[channel] - RevMod_B_Offset[channel]);
    if (idx < 0) idx += RevMod_B_Ring1_Max[channel];

    //R1.00 Update our rotating index.
    RevMod_B_Offset[channel] *= (1.0f + (RevMod_B_Rate * .15f)); 

    //R1.00 Alter our delay direction if needed.
    if (RevMod_B_Offset[channel] < RevMod_B_Offset_Min[channel])
        RevMod_B_Rate = RevMod_B_Rate_Up;
    else if (RevMod_B_Offset_Max[channel] < RevMod_B_Offset[channel])
        RevMod_B_Rate = RevMod_B_Rate_Down;

    //R1.00 Update ring buffer position. Wrap around to 0 when we exceed the buffer size.
    RevMod_B_Ring1[channel]++;
    if (RevMod_B_Ring1_Max[channel] < RevMod_B_Ring1[channel]) RevMod_B_Ring1[channel] = 0;

    //R4.00 Mix the dry and wet (chorus) signals. 
    return (tSample * (1.0f - Setting[e_RevChor])) + (RevMod_B[channel][idx] * Setting[e_RevChor]);
}


//**********************************************************************
//S P E A K E R   I M P U L S E   R E S P O N S E 
//**********************************************************************
void MakoDist01AudioProcessor::Mako_SetIR()
{   
    Mako_SetIR_Fill(int(Setting[e_IR]), &IR_Speaker[0]);
}

//R4.00 Fill an IR buffer with the selected IR.
void MakoDist01AudioProcessor::Mako_SetIR_Fill(int Idx, float IR[1024])
{    
    float Max = 0.0f;

    //R4.00 Set IR to a preset IR (0 - 19) or the current database IR (IR_Buffer) from the editor.
    switch (Idx)
    {
    case 0: IR_Build[0] = 1.0f; for (int t = 1; t < 1024; t++) IR_Build[t] = 0.0f; break;
    case 1:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR1[t]; break;
    case 2:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR2[t]; break;
    case 3:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR3[t]; break;
    case 4:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR4[t]; break;
    case 5:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR5[t]; break;
    case 6:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR6[t]; break;
    case 7:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR7[t]; break;
    case 8:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR8[t]; break;
    case 9:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR9[t]; break;
    case 10:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR10[t]; break;
    case 11:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR11[t]; break;
    case 12:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR12[t]; break;
    case 13:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR13[t]; break;
    case 14:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR14[t]; break;
    case 15:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR15[t]; break;
    case 16:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR16[t]; break;
    case 17:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR17[t]; break;
    case 18:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR18[t]; break;
    case 19:  for (int t = 0; t < 1024; t++) IR_Build[t] = TIR19[t]; break;
    default:  for (int t = 0; t < 1024; t++) IR_Build[t] = IR_FromFile[t]; break;
    }


    //R1.00 Apply speaker size if selected. 
    if ((Setting[e_IRSize] < .0f) || (.0f < Setting[e_IRSize]))
    {
        int t = 0;
        float t2 = 0.0f;
        int t2i = 0;
        float t2diff = 0.0f;
        float t2s = (1024.0f + ((Setting[e_IRSize] * -.5f) * 1000.0f)) / 1024.0f;

        //R1.00 Resample the IR using new specs.
        while ((t < 1024) && (t2 < 1023.0001f))
        {
            t2i = int(t2);
            t2diff = t2 - t2i;
            IR[t] = (IR_Build[t2i] * (1.0f - t2diff)) + (IR_Build[t2i + 1] * t2diff);
            if (Max < abs(IR[t])) Max = abs(IR[t]);
            t++;
            t2 += t2s;
        }

        //R1.00 Clear the unused values or the volume explodes when Maxed/Normalized.
        if (t < 1024)
        {
            while (t < 1024)
            {
                IR[t] = 0.0f;
                t++;
            }
        }
    }
    else
    {
        //R1.00 Default of 0 is selected.
        //R1.00 Do not apply Speaker Size/Time stretching.
        for (int t = 0; t < 1024; t++)
        {
            IR[t] = IR_Build[t];
            if (Max < abs(IR[t])) Max = abs(IR[t]);
        }
    }

    //R1.00 Normalize.
    if ((0.0f < Max) && (Max < 1.0f))
        Max = 1.0f / Max;
    else
        Max = 1.0f;

    for (int t = 0; t < 1024; t++) IR[t] *= Max;

}

