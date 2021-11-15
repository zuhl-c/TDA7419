
#include "TDA7419.h"
#include <Arduino.h>
#include <Wire.h>

TDA7419::TDA7419(){
    Wire.begin(); 
}
void TDA7419::input(int in,int gain){
    switch(in){
        case 0:in=0x00;break;//QD or SE ,default QD
        case 1:in=0x01;break;//SE1
        case 2:in=0x02;break;//SE2
        case 3:in=0x03;break;//SE3
        case 4:in=0x04;break;//QD or SE ,default SE
        case 5:in=0x05;break;//mute
    }
    //input gain 0dB to 15dB
    switch(gain){
        case 0 :gain=0x00;break;
        case 1 :gain=0x08;break;
        case 2 :gain=0x10;break;
        case 3 :gain=0x18;break;
        case 4 :gain=0x20;break;
        case 5 :gain=0x28;break;
        case 6 :gain=0x30;break;
        case 7 :gain=0x38;break;
        case 8 :gain=0x40;break;
        case 9 :gain=0x48;break;
        case 10:gain=0x50;break;
        case 11:gain=0x58;break;
        case 12:gain=0x60;break;
        case 13:gain=0x68;break;
        case 14:gain=0x70;break;
        case 15:gain=0x78;break;
    }
    int in_sett=in+gain+A_ZERO;
    writeWire(SOURCE,in_sett);
}
void TDA7419::sInput(int sin,int s_gain,int sel_in){
    switch(sin){
        case 0:sin=0x00;break;//QD or SE ,default QD
        case 1:sin=0x01;break;//SE1
        case 2:sin=0x02;break;//SE2
        case 3:sin=0x03;break;//SE3
        case 4:sin=0x04;break;//QD or SE ,default SE
        case 5:sin=0x05;break;//mute
    }
    switch(s_gain){
        case 0 :s_gain=0x00;break;//0dB
        case 1 :s_gain=0x08;break;
        case 2 :s_gain=0x10;break;
        case 3 :s_gain=0x18;break;
        case 4 :s_gain=0x20;break;
        case 5 :s_gain=0x28;break;
        case 6 :s_gain=0x30;break;
        case 7 :s_gain=0x38;break;
        case 8 :s_gain=0x40;break;
        case 9 :s_gain=0x48;break;
        case 10:s_gain=0x50;break;
        case 11:s_gain=0x58;break;
        case 12:s_gain=0x60;break;
        case 13:s_gain=0x68;break;
        case 14:s_gain=0x70;break;
        case 15:s_gain=0x78;break;//15dB
    }
    switch(sel_in){
        case 0:sel_in=0x00;break;//main source
        case 1:sel_in=0x80;break;//second source
    }
    int sd_sett=sin+s_gain+sel_in;
    writeWire(SD_SOURCE,sd_sett);
}
void TDA7419::setLoud(int att,int cf){
    switch(att){
        case 15:att=0x00;break;//0dB
        case 14:att=0x01;break;
        case 13:att=0x02;break;
        case 12:att=0x03;break;
        case 11:att=0x04;break;
        case 10:att=0x05;break;
        case  9:att=0x06;break;
        case  8:att=0x07;break;
        case  7:att=0x08;break;
        case  6:att=0x09;break;
        case  5:att=0x0A;break;
        case  4:att=0x0B;break;
        case  3:att=0x0C;break;
        case  2:att=0x0D;break;
        case  1:att=0x0E;break;
        case  0:att=0x0F;break;//-15dB
    }
   
    switch(cf){
        case 0:cf=0x00;break;//flat
        case 1:cf=0x10;break;//400hz
        case 2:cf=0x20;break;//800hz
        case 3:cf=0x30;break;//2400hz
    }
    int loud_sett=att+cf+HIGH_BOOST+SOFT_STEP;
    writeWire(MAIN_LOUD,loud_sett);
}
void TDA7419::softMute(int mute){
    switch (mute)
    {
    case 0: mute=1;break; //mute off
    case 1: mute=0;break; //mute on
    }
    int mute_sett=mute+MUTE_IIC+MUTE_TIME+STEP_TIME+CLK_FST;
    writeWire(SOFT_MUTE,mute_sett);
}
void TDA7419::setVol(int vol){
    vol=vol-MAIN_VOL;
    if(vol<0){vol=abs(vol)+16;}
    writeWire(VOLUME,vol+SOFT_STEP);
}
void TDA7419::setFl(int fl){
    fl=TRIM_VOL-fl;
    writeWire(ATT_FL,fl+SOFT_STEP);
}
void TDA7419::setFr(int fr){
    fr=TRIM_VOL-fr;
    writeWire(ATT_FR,fr+SOFT_STEP);
}
void TDA7419::setRl(int rl){
    rl=TRIM_VOL-rl;
    writeWire(ATT_RL,rl+SOFT_STEP);
}
void TDA7419::setRr(int rr){
    rr=TRIM_VOL-rr;
    writeWire(ATT_RR,rr+SOFT_STEP);
}
void TDA7419::setSub(int sub){
    sub=TRIM_VOL-sub;
    writeWire(ATT_SUB,sub+SOFT_STEP);
}
void TDA7419::setMix(int mix){
    mix=TRIM_VOL-mix;
    writeWire(MX_LEVEL,mix+SOFT_STEP);
}
void TDA7419::setTreble(int treble,int tr_cn){
    switch(treble){
    case -15:treble=0x0F;break;//-15dB
    case -14:treble=0x0E;break;
    case -13:treble=0x0D;break;
    case -12:treble=0x0C;break;
    case -11:treble=0x0B;break;
    case -10:treble=0x0A;break;
    case -9 :treble=0x09;break;
    case -8 :treble=0x08;break;
    case -7 :treble=0x07;break;
    case -6 :treble=0x06;break;
    case -5 :treble=0x05;break;
    case -4 :treble=0x04;break;
    case -3 :treble=0x03;break;
    case -2 :treble=0x02;break;
    case -1 :treble=0x01;break;
    case 0  :treble=0x00;break;//0dB
    case 1  :treble=0x11;break;
    case 2  :treble=0x12;break;
    case 3  :treble=0x13;break;
    case 4  :treble=0x14;break;
    case 5  :treble=0x15;break;
    case 6  :treble=0x16;break;
    case 7  :treble=0x17;break;
    case 8  :treble=0x18;break;
    case 9  :treble=0x19;break;
    case 10 :treble=0x1A;break;
    case 11 :treble=0x1B;break;
    case 12 :treble=0x1C;break;
    case 13 :treble=0x1D;break;
    case 14 :treble=0x1E;break;
    case 15 :treble=0x1F;break;//+15dB
    }    
    switch(tr_cn){
        case 0:tr_cn=0x00;break;//10.0khz
        case 1:tr_cn=0x20;break;//12.5khz
        case 2:tr_cn=0x40;break;//15.0khz
        case 3:tr_cn=0x60;break;//17.5khz
    }
    int tr_sett=treble+tr_cn+V_REF;
    writeWire(TREBLE,tr_sett);
}
void TDA7419::setMid(int mid){
    switch(mid){
    case -15:mid=0x0F;break;//-15dB
    case -14:mid=0x0E;break;
    case -13:mid=0x0D;break;
    case -12:mid=0x0C;break;
    case -11:mid=0x0B;break;
    case -10:mid=0x0A;break;
    case -9 :mid=0x09;break;
    case -8 :mid=0x08;break;
    case -7 :mid=0x07;break;
    case -6 :mid=0x06;break;
    case -5 :mid=0x05;break;
    case -4 :mid=0x04;break;
    case -3 :mid=0x03;break;
    case -2 :mid=0x02;break;
    case -1 :mid=0x01;break;
    case 0  :mid=0x00;break;//0dB
    case 1  :mid=0x11;break;
    case 2  :mid=0x12;break;
    case 3  :mid=0x13;break;
    case 4  :mid=0x14;break;
    case 5  :mid=0x15;break;
    case 6  :mid=0x16;break;
    case 7  :mid=0x17;break;
    case 8  :mid=0x18;break;
    case 9  :mid=0x19;break;
    case 10 :mid=0x1A;break;
    case 11 :mid=0x1B;break;
    case 12 :mid=0x1C;break;
    case 13 :mid=0x1D;break;
    case 14 :mid=0x1E;break;
    case 15 :mid=0x1F;break;//+15dB
    }
    int mid_sett=mid+Q_FACTOR+SOFT_STEP;
    writeWire(MIDDLE,mid_sett);
}
void TDA7419::setBass(int bass){
    switch(bass){
    case -15:bass=0x0F;break;//-15dB
    case -14:bass=0x0E;break;
    case -13:bass=0x0D;break;
    case -12:bass=0x0C;break;
    case -11:bass=0x0B;break;
    case -10:bass=0x0A;break;
    case -9 :bass=0x09;break;
    case -8 :bass=0x08;break;
    case -7 :bass=0x07;break;
    case -6 :bass=0x06;break;
    case -5 :bass=0x05;break;
    case -4 :bass=0x04;break;
    case -3 :bass=0x03;break;
    case -2 :bass=0x02;break;
    case -1 :bass=0x01;break;
    case 0  :bass=0x00;break;//0dB
    case 1  :bass=0x11;break;
    case 2  :bass=0x12;break;
    case 3  :bass=0x13;break;
    case 4  :bass=0x14;break;
    case 5  :bass=0x15;break;
    case 6  :bass=0x16;break;
    case 7  :bass=0x17;break;
    case 8  :bass=0x18;break;
    case 9  :bass=0x19;break;
    case 10 :bass=0x1A;break;
    case 11 :bass=0x1B;break;
    case 12 :bass=0x1C;break;
    case 13 :bass=0x1D;break;
    case 14 :bass=0x1E;break;
    case 15 :bass=0x1F;break;//+15dB
    }
    int bass_sett=bass+Q_FACTOR+SOFT_STEP;
    writeWire(BASS,bass_sett);
}
void TDA7419::setFreq(int sub_c,int mid_c,int bass_c){
    //subwoofer cuttoff frequency
    switch(sub_c){
        case 0:sub_c=0x00;break;//flat
        case 1:sub_c=0x01;break;//80hz
        case 2:sub_c=0x02;break;//120hz
        case 3:sub_c=0x03;break;//160hz
    }
    //middle center frequency
    switch(mid_c){
        case 0:mid_c=0x00;break;//500hz
        case 1:mid_c=0x04;break;//1000hz
        case 2:mid_c=0x08;break;//1500hz
        case 3:mid_c=0x0C;break;//2500hz
    }
    //bass center frequency
    switch(bass_c){
        case 0:bass_c=0x00;break;//60hz
        case 1:bass_c=0x10;break;//80hz
        case 2:bass_c=0x20;break;//100hz
        case 3:bass_c=0x60;break;//200hz
    }
    int freq_sett=sub_c+mid_c+bass_c+DC_BASS+SM_FILTER;
    writeWire(SUB_M_BASS,freq_sett);
}
void TDA7419::mixFunc(int mx_l,int mx_r,int en_mx,int en_sub,int hpf){
    switch(mx_l){case 0:mx_l=0x00;break;case 1:mx_l=0x01;break;}//mixing to fl(0-on)
    switch(mx_r){case 0:mx_r=0x00;break;case 1:mx_r=0x02;break;}//mixing to fr(1-off)
    switch(en_mx){case 0:en_mx=0x00;break;case 1:en_mx=0x03;break;}//enable mixing
    switch(en_sub){case 0:en_sub=0x00;break;case 1:en_sub=0x04;break;}//enable subwoofer
    switch(hpf){
        case 0:hpf=0xA0;break;//0dB
        case 1:hpf=0x00;break;//4dB
        case 2:hpf=0x10;break;//6dB
        case 3:hpf=0x20;break;//8dB
        case 4:hpf=0x30;break;//10dB
        case 5:hpf=0x40;break;//12dB
        case 6:hpf=0x50;break;//14dB
        case 7:hpf=0x60;break;//16dB
        case 8:hpf=0x70;break;//18dB
        case 9:hpf=0x80;break;//20dB
        case 10:hpf=0x90;break;//22dB
    }
    int mix_sett=mx_l+mx_r+en_mx+en_sub+hpf;
    writeWire(MX_GAIN,mix_sett);
}
void TDA7419::spectrum(int sq,int rst_md,int s_src,int s_run,int rst,int s_clk,int cp_md){
    //spectrum analyzer q factor-3.5/1.75
    switch(sq){case 0:sq=0x00;break;case 1:sq=0x01;break;}
    //reset mode-iic/auto
    switch(rst_md){case 0:rst_md=0x00;break;case 1:rst_md=0x02;break;}
    //souce-bass/gain
    switch(s_src){case 0:s_src=0x00;break;case 1:s_src=0x04;break;}
    //spectrum analyzer run-on/off
    switch(s_run){case 0:s_run=0x00;break;case 1:s_run=0x08;break;}
    //reset-on/off
    switch(rst){case 0:rst=0x00;break;case 1:rst=0x10;break;}
    //clock source-internal/external
    switch(s_clk){case 0:s_clk=0x00;break;case 1:s_clk=0x20;break;}
    //coupling mode 
    switch(cp_md){
        case 0:cp_md=0x00;break;//dc coupling(without hpf)
        case 1:cp_md=0x40;break;//ac coupling after in gain
        case 2:cp_md=0x80;break;//dc coupling (with hpf)
        case 3:cp_md=0xC0;break;//ac coupling after bass
    }
    int spec_sett=sq+rst_md+s_src+s_run+rst+s_clk+cp_md;
    writeWire(SPEC_ANL,spec_sett);
}
void TDA7419::writeWire(char a, char b){
    Wire.beginTransmission(IC_ADDRESS);
    Wire.write(a);
    Wire.write(b);
    Wire.endTransmission();
}