/*This program was written by zuhail pm/
<november-8-2021>
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
#define MIX_L       0x00 //mixing right
#define MIX_R       0x00 //mixing left
#define EN_MIX      0x00 //enable mixing
#define EN_SUB      0x00 //enable subwoofer
#define MX_GAIN_EF  0x00 //gain effect hpf-22db
#define SD_SRC      0x04 //second source select
#define SD_GAIN     0x00 //second source gain
#define SEL_SRC     0x00 //select main source

#include <Arduino.h>

class TDA7419{
public:
	TDA7419();

	void input(int in, int gain);
	void setLoud(int att_loud, int cf);
	void softMute(int mute);

	void setVol(int vol);
	void setFl(int fl);
	void setFr(int fl);
	void setRl(int sl);
	void setRr(int sr);
	void setSub(int sub);
	void setMix(int mix);

	void sdInput(int src ,int sel);
	void setTreble(int treble, int tr_cn);
	void setMid(int mid);
	void setBass(int bass);
	void setFreq(int sub_c, int mid_c, int bass_c);
	void setMixGain(int ml,int mr,int en_mx,int en_sub,int gain_f);
	void specAnalyse(int spc_filter,int rst_mode,int spc_src, int spc_run, int rst, int clk, int ac);

private:
void writeWire(char a,char b);
};

#endif