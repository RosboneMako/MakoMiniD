# Mako Mini Distortion
* JUCE VST guitar processor.
* Tested on Windows only.
* Written in Visual C++ 2022.
* Version: 4.10
* Posted: July 7, 2025

![Demo Image](docs/mmd_back_01.png)

VERSION
------------------------------------------------------------------
VERSION 4.00
* Initial release.<br/>  

VERSION 4.10
* Fixed pedal compressor bug.<br/>
* Added more saving code to IR databse.<br/>
* Rework Flanger to closer resemble standard unit.<br/>
* Rework Chorus to closer resemble standard unit.<br/>

VERSION 4.20s
* EXE and VST are compiled using STATIC lib.<br/>
* 4x Over Sampling added for anti aliasing.<br/>  

NOTE:<br/>
Version 4.20s files use the static runtime library option in JUCE.
Previous versions used dynamic linking and may not run in certain instances.
An s is added to the version number for identification. 

SUMMARY
------------------------------------------------------------------
A Juce/C++ VST3 guitar processor. Complete end to end processing
abilities like effects, amplifiers, EQ, etc.

This VST is under constant development and changes daily. 

THINGS TO KNOW
------------------------------------------------------------------
VST LIMITATIONS<br/>
VST's do not save string variables. This means things like 
WAVE files used in the VST will NOT BE AVAILABLE
from within the DAW at startup or its presets. 

In order to utilize external files in the VST a database must be used.
MMD uses an external database to store Amplifier and Speaker IR paths
so they can easily be recalled from within a DAW.

DATABASE OPERATION<br/>
MMD has 20 built in amplifier models and 20 built in speaker IRs. <br/>
MMD can access external amps and IRs if they are defined in the Amp/IR database. <br/>

To edit the database, select the DEFINE AMPS or DEFINE IRS buttons on the main menu.
When editing, simply left mouse click to add items or right click to delete items.
Changes are automatically saved as you edit. Amps or IRs should be reselected if they
were in use while editing.

The Amps and Speaker IRs are referenced in the DAW and MMD by slot number only. Changing an amp in
the database will change it in any DAW or MMD preset. 

HINT: It is advised to rename the external files to the database slot they will be assigned.
Numbering the files will greatly simplify use on multiple computers.<br/>

For example a v30 IR could be called 55_v30. When setting up the VST on another PC, it will
be easy to organize the IRs, 55_v30 needs to be loaded into slot 55.


MMD PRESETS<br/>
In its normal use, the MMD VST should use the DAW preset save/load. When using
the standalone EXE, MMD can load/save complete setups by selecting Load/Save
preset buttons. It may be easier to use the MMD presets when sharing across multiple PCs.


SIGNAL PATH
------------------------------------------------------------------
MMD has a fixed signal path. All audio moves from left to right from the signal path diagram.
Optionally, the Delay and Reverb blocks can be placed in parallel. The UI diagram will not
be changed when in parallel.

AMPLIFIER BLOCK<br/>
The amplifier has 200 possible channel settings. There are 30 built in amplifiers
and 169 user programmable amplifiers (database). Select an amp using the AMP drop
down list in the amplifier block.

IMPULSE RESPONSE (IR) BLOCK<br/>
An Impulse Response is a wave file the captures the frequency response
of a system. Here they represent the speaker being used by the amplifier
block. There are adjustements here for bass/treble balance (voice) and
resampling of the IR (Size).

There are 20 programmed IRs built in to the VST. An additional 179 IRs can be added
to the IR databse. IRs can be resampled using the SIZE control. The VOICE option 
will modify the frequency response of the IR.

Voice only adds brightness to an external IR. Its default value should be 0.

As with the Amp database, an IR is referenced by its slot number in both MMD and the DAW.
Changing an IR in the databse will change it everywhere it is referenced. Again, best
practice would be to number the IR files with a slot number prefix.

DELAY AND REVERB BLOCKS<br/>
These effects are fixed at these positions. The effects are always "ON".
To bypass the effect set its MIX value to 0 (zero).


ON SCREEN CONTROL OPERATION
------------------------------------------------------------------
There are two basic controls used for setting in MMD: Slider and knob.
- Mouse click sets a value.
- Most controls support dragging to adjust values. Try not to drag if issues arise.
- Double Click will set the control to its default (Except Pedals and Mods).
- Pedals and Mods have an extension that adds V3 functionality.

STEREO/MONO SIGNAL PATH 
------------------------------------------------------------------
This VST is a mono processing effect until the post amp EQ. The processing is stereo
from that point on. All effects are mono except Chorus, Wide Load, Delay, and Reverb.

AMPLIFIER AND SPEAKER IR DATABASE
------------------------------------------------------------------
A total of 200 amps and 200 IRs are defined in the database. 

To edit the amps in the database select the DEFINE AMPS button at the main menu. A simple grid
will appear that allows you to page thru the 200 amps. By left clicking the Low, Med, High grid
cells you can set the gain range of the amplifier. Left click the next cell to locate the 
amp IR file to use for this amplifier. Right clicking an amp path cell will delete that amp from the list.

To edit the Speaker IR database select the DEFINE IRs button at the main menu. Follow the same steps laid for amps out to edit.

AMPLIFIER BLOCK
------------------------------------------------------------------
The amplifier setion has 30 amps programmed. These amps can be adjusted
using the various controls. Select which amp to use with the Amplifier drop down list.
An additional 169 amps can be added by editing the AMP database. The database
holds the file paths for the amp input EQ IRs. Once an amp is added, it can be 
recalled by selecting it from the AMP drop down list in the amp block.

QUALITY<br/>
Each amplifier uses a 1024 sample IR for its frequency definition. IRs
are very CPU intensive. Changing the quality reduces CPU usage by using
a small section of the IR instead of the whole IR. The tradeoff is poor
low frequency definition, which can help heavier gain sounds.

BOOM and CRISP<br/>
Boom and Crisp are Hi/Lo cut filters to let you adjust the EQ going into the Amp.
Boom and Crisp are typical 1st order Low/High Cut filters.

SLOPE<br/>
MMD amps use two styles of clipping: Hard and Soft. The slope control mixes
between the two types. Soft is best used for low gain and hard for high gain.

THUMP, AIR, POWER, SAG, and THIN<br/>
Thump and Air boost the low and high freqs using a distorting circuit.<br/>
Power adds the same distortion across the full range. <br/>
Sag limits fast transients to simulate an amp running out of power.<br/>
Thin compresses the signal if a lot of gain is being used.<br/>
These controls are very helpful for edge of breakup sounds. Adding some
additional gain with the Power/Thump/Air controls will liven up a cleaner
amp.

CLEAN VOL<br/>
This control adjusts the mix betweem the driven signal and a clean signal. 
The clean signal goes thru the Amp IR process and has its EQ applied but no gain
is applied. This can be helpful on clean amps for finer control of gain by
mixing low gain with no gain.

EQ MODE AND LOW PASS<br/>
There are 10 EQs prgrammed into the VST. Change EQs by changing the MODE
setting. The low pass is used to filter out harshness from any distortion
being used.

TIPS AND TRICKS<br/>
Thump and Air are very useful. They are useful for most gain settings.

Sag is best used on low gain amps to soften crunch. It will reduce clarity 
on high gain amps. Best to turn it off for high gain.

Thin reduces overall gain. Its main goal is to reduce swirling ghost notes
caused by the VST having more gain that tubes are capable of doing. So thin 
is best used for super high gain amps while soloing. 


EFFECTS BLOCKS<br/>
------------------------------------------------------------------
This VST uses a fixed signal path. All effects are in place and cant be moved.


DELAY BLOCK<br/>
------------------------------------------------------------------
The delay is a generic simple delay with mix, time, repeats, etc.
The offset control changes the left delay time from the right time
to create a stereo delay effect. Ducking will hold down the delay
volume while playing and increase it when not playing.

Modulation effect #1 can be placed in the delays wet mix allowing for
some interesting things like a fifth note chorus on the delay only.

REVERB BLOCK<br/>
------------------------------------------------------------------
MMD has a very simplistic reverb based on 16 different delays being
mixed together. Reverbs 0-10 use a simpler algorithm and sound very
delay-like. Reverbs 11-19 are denser and come closer to a normal reverb
effect.

The reverb effet can be tweaked using the available controls. Specifically
room size which expands the delays in time. The low pass filter and built-in
chorus applied to the wet effect helps smooth the echoes.

Modulation Effect #2 can be applied to the wet effect. 


EXTERNAL IMPULSE RESPONSES<br/>
------------------------------------------------------------------
MMD uses 1024 sample IRs. The program does not try to parse out WAVE
files. It assumes the first 1024 samples are the correct IR data. It
is best to verify the files in an external program before use. 

IR files are created for a certain sample rate. The VST is designed
to run at a 48 kHz sample rate. 48 kHz IR files should be used. The
frequency response of the IR is tightly tied to the sample rate. So 
the correct sample rate is very important. Once the IR is loaded into
the VST, it can be resampled using the SIZE control to get close to
its original frequency response.

Any IRs that are loaded manually have a copy saved inside the MMD
preset file, so it will not be necessary to hunt them down manually
at a later date. They will NOT be availabe from any DAW related presets.


DIALING IN AN AMPLIFIER<br/> 
------------------------------------------------------------------
The most important thing to set up is the frequencies presented to
the amplifier section. The BOOM and CRISP controls are designed to 
dial in the response of the AMP IR.

BOOMINESS<br/>
Some amps have a lot of low end present. It may be good to reduce your
guitars low end before the amp. This can be done with the BOOM setting in the amp block.
another option is the INPUT sections HIGH PASS filter. Values in the 80-150 Hz can be very good.
some pedals have Low EQ options as well. Experiment with each as they have different slopes
and will provide varying results.

Using an OD pedal or the Dist EQ pedals will provide furtherr control and can be used to
simulate other amps by boosting gain or freqs such as mids.

CRISPY HARSHNESS<br/>
Some amps have a lot of high freqs in them. This results in harshness
as the gain goes up. There are 6 tools that can be used.

1) Guitar tone control. Last resort?
2) CRISP control in the AMP section. Good start is .1 then move up.
3) Use a pedal (DIST EQ HI CUT) before the amp block.
4) use the LOW PASS filter in the amp block.
5) Use a smoother sounding IR.
6) Add amp sag in the AMP BLOCK.

RULES OF THUMB<br/>
Lower gain amps will get more from low pass filters, Thump/Air, and
pregain EQ. Post amp Mid Bass and Mid EQ can help fatten thin sounds.

Higher gain amps will benefit more from less bass before the amplifier
section. Thump can add some heavy feel but the added gain from Thump,
Air, and Power may clutter up the sound. Use sparingly. Best results may
come from adjusting normal EQ settings.


ROOM EQ WIZARD - Sweeping your own amps 
------------------------------------------------------------------
Room EQ Wizard is an amazing software package that lets you make frequency sweeps. It
can be used to make amplifier IRs for use in Mako Distortion 2.

The concept of making an Amp IR is:
1) Set the amp to edge of distortion.
2) Sweep the amp at a high input volume.
3) Sweep the amp at a very low input volume.
4) The difference of those two sweeps is the input EQ response of the amp.
5) Convert the difference sweep to an IR.

![Demo REW Image](docs/rew_sweeps_01.png)

QUICK GUIDE
1) Download REW, install, etc.
2) Connect your measurement devices need to drive the amp and record the amps output.
3) Set the amplifier gain so the amp is on the edge of distorting. 
4) In REW select PREFERENCES and select the input and output ports of the sound card being used.
5) Select Measure, the measurement dialog will open. Ignore calibration warnings for now.
6) Set the output LEVEL to 0 dBFS and set the NAME to 0dB.
7) Select START to sweep the amplifier. Verify nothing is clipping. Adjust if needed.
8) A sweep should be made and you should be returned to the main menu with your trace displayed.
9) Select Measure, the measurement dialog will open. Ignore calibration warnings for now.
10) Set the output LEVEL to -40 dBFS and set the NAME to 40dB.
11) Select START to sweep the amplifier.
12) A sweep should be made and you should be returned to the main menu with your trace displayed.
13) You should have two sweeps visible at the main menu.
14) Select the ALL SPL button to view both traces at once with no phase. This is a REQUIRED step.
15) Select the ACTIONS button. A dialog should appear with many buttons.
16) Select TRACE ARITHMETIC. A 2nd dialog will appear.
17) If your traces are noisey, you may choose to SMOOTH them at this point. 
18) Set the A trace to be 40dB, B Trace to be 0dB, A/B arithmetic, and select Generate button. 
19) Close the two dialogs and return to the SPL & PHASE view by selecting that button.
20) In the main menu under select FILE -> EXPORT -> EXPORT IMPULSE RESPONSE AS WAVE FILE.
21) Select NORMALISE and select your sample rate to 48 kHz. Continue thru to save the file. 
22) You are done with REW, however, the resulting IR wave file is not valid for use in any programs yet.
23) Start your AUDIO EDITING PROGRAM (Goldwave).

This looks like a lot of steps. After doing this a couple of times and getting the hang of it, you can sweep an amplifier in a minute or two.

NOTE: If you save individual traces and try to merge them later, REW may not understand the phase information. It may be necessary to GENERATE MINIMUM PHASE
for the final trace before it can be exported as an IR wave file.

BEST CASE SCENARIOS AND THE NEXT LEVEL<br/>
You may get best results when the amp EQ knobs are set pretty flat. Experiment as needed.

If you dial in the amp EQ to sound good to your ears, you can use the 0 dB sweep as your output/speaker IR. This will form a better picture of the amp.
Since we assume that the input EQ is flattened at high gain, the 0 dB sweep should be the amps output EQ/Speaker/Mics/etc. 



AUDIO EDITING SOFTWARE - Editing the REW Wave File 
------------------------------------------------------------------
The resulting wave file from REW is very large and is not valid for programs. It needs to be cropped and edited 
in an audio editing program such as Goldwave.

1) The resulting wave file will be about 512 kB with the actual IR located about 8000 samples into the wave file. 
2) Crop the wave file at the very start of the IR pulse and to at least 1024 samples after that.
3) Some IRs will have long sloping waves before the full vol peak. Each point before the peak = lag. Trim pre-peaks as needed. 
4) At this point you will need to adjust for noise and errors in the sweeps if any.
5) You may wish to apply a low pass filter to remove noise above 5 kHz.
6) You may wish to apply a high pass filter to tune the bass response. 

In many situations you may have severe noise above 5 kHz. This should be filtered out. It breaks the volume of the IR and adds
terrible noise and harshness. Your guitar will also probably never create freqs above 2.5 kHz. Since you have the IR file,
you can adjust until it sounds good to your ears. 

EXAMPLE IR PULSE IN GOLDWAVE<br/>
![Demo GoldWave Image](docs/gw_ir_file_01.png)

EXAMPLE FFT VIEW OF IR PULSE IN GOLDWAVE<br/>
![Demo IR FFT Image](docs/gw_ir_file_02.png)

GOLDWAVE SPECIFIC HELP<br/>
Goldwave is a dedicated audio editor. It has a MAIN window and a CONTROL window. CONTROL lets you monitor the output of the wave file and lets 
you select sources to record from etc. The important setting here is the VU meters.

To understand what our AMP IR is doing we need to see a FREQUENCY graph of it. This can be done two ways: Play the wave file or right click in the wave file.

Right click the VU meter to select the type of meter shown. For our work we want SPECTRUM. This lets us see an FFT frequency spectrum
of the waveform being played. If set to SPECTRUM, we can right click in the Main window and see an FFT of the selected area of our wave file.

GW has 3 PLAY options. You can set the middle PLAY option to loop playback for 100 times. This lets you play the IR and get a feel for what it will sound like.
In CONTROL select the blue checkmark (Control Properties). Set PLAY 2 to SECLECTION, Loop 100.

GW has two useful filtering options that work well with IR editing: 
LOW/HIGH PASS
PARAMETRIC EQ

These two filters will let you form the IR to a useful state without destroying the phase relationship of the waveform.

The EQUALIZER destroys phase info. This can be helpful if you need more gain from the IR. IRs are normally a large pulse. By destroying the phase you 
get a more sinusoidal waveform that has an overall volume much higher than a pulse. You can get 3-6 dB more volume if needed.

GQ also has a MAXIMIZE button that lets you normalize your IR to MAX volume. This will be REQUIRED after applying filters. 








