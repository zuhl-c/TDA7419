/*TDA7419 library code for Arduino,
This program was written by zuhail pm.
<updated on november-8-2021>
<developer.zuhail@gmial.com>
<github.com/zuhl-c*/

#ifndef TDA7419_H
#define TDA7419_H

#define IC_ADDRESS  0x44
#define SOURCE      0x40 //main source -input selector
#define MAIN_LOUD   0x41 //main loudness
#define SOFT_MUTE   0x42 //soft mute /clock-generator
#define VOLUME      0x43 //volume
#define TREBLE      0x44 //treble
#define MIDDLE      0x45 //mid
#define BASS        0x46 //bass
#define SD_SOURCE   0x47 //second source -input selector
#define SUB_M_BASS  0x48 //sub,mid,bass
#define MX_GAIN     0x49 //mixing /gain effect
#define ATT_FL      0x4A //speaker front left attenuator
#define ATT_FR      0x4B //speaker front right attenuator
#define ATT_RL      0x4C //speaker rear left attenuator
#define ATT_RR      0x4D //speaker rear right attenuator
#define MX_LEVEL    0x4E //mixing level control
#define ATT_SUB     0x4F //subwoofer attenuator
#define SPEC_ANL    0x50 //spectrum analyser /clock source /ac mode


#define  A_ZERO     0x00 //auto_zero
#define  MUTE_IIC   0x00 //mute by ic only
#define  MUTE_TIME  0x08 //123ms
#define  SOFT_STEP  0x00 //soft_Step enable
#define  STEP_TIME  0x70 //20.48ms
#define  CLK_FST    0x00 //clock fastmode
#define HIGH_BOOST  0x00 //high boost on
#define V_REF       0x00 //external vref
#define Q_FACTOR    0x00 //q-factor
#define DC_BASS     0x00 //bass dc mode
#define SM_FILTER   0x00 //smoth filter
#define TRIM_VOL    0xAE //trim volume
#define MAIN_VOL    0x4F //minimum volume

#include <Arduino.h>

class TDA7419{
public:
	TDA7419();
	//main source input(0,1,2,3) and gain (0..+15dB)
	void input(int in, int gain);
	//second source input(0,1,2,3), gain(0..+15dB),source slection
	void sInput(int s_in,int s_gain,int sel_in);
	//loud(0..15dB)center frequency(0,1,2,3/flat,400hz,800hz,2400hz)
	void setLoud(int att_loud, int cf);
	void softMute(int mute);//mute(0/1)

	void setVol(int vol);//volume(0...-79dB)
	void setFl(int fl);//fr,fl,sl,sr,sub,mix(0-15)
	void setFr(int fl);
	void setRl(int sl);
	void setRr(int sr);
	void setSub(int sub);
	void setMix(int mix);

	//treble(-15dB..0...+15dB)center frequency(0,1,2,3/10khz,12.5kz,15khz,17.5khz)
	void setTreble(int treble, int tr_cn);
	void setMid(int mid);//mid(-15dB..0...+15dB)
	void setBass(int bass);//bass(-15dB..0...+15dB)
	//subwoofer cutoff frequency (0,1,2,3/flat,80hz,120hz,160hz)
	//mid center frequency (0,1,2,3/500hz,1000hz,1500hz,2500hz)
	//bass center frequency (0,1,2,3/60hz,80hz,100hz,200hz)
	void setFreq(int sub_c, int mid_c, int bass_c);
	//mixing(0,1),subwoofer enable(0,1),HPF gain effect(0..10/0dB...22dB)
	void mixFunc(int mx_l,int mx_r,int mx_en,int sub_en,int hpf);
	//spectrum analyser- q_factor(0/1),reset mode(0/1),source(0/1),run(0/1),reset(0/1)
	//clock source(0/1-internal/external),dc coupling mode(0..3)
	void spectrum(int sq,int rst_md,int s_src,int s_run,int rst,int s_clk,int cp_md);
private:
void writeWire(char a,char b);
};

#endif