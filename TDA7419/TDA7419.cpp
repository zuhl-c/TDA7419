
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
    //Serial.println("in :");Serial.println(in_sett);
    writeWire(SOURCE,in_sett);
}
void TDA7419::setLoud(int att,int cf){
    if(att>15){att=15;}if(att<0){att=0;}
    switch(cf){
        case 0:cf=0x00;break;//flat
        case 1:cf=0x10;break;//400hz
        case 2:cf=0x20;break;//800hz
        case 3:cf=0x30;break;//2400hz
    }
    int loud_sett=att+cf+HIGH_BOOST+SOFT_STEP;
    //Serial.println("loud :");Serial.println(loud_sett);
    writeWire(MAIN_LOUD,loud_sett);
}
void TDA7419::softMute(int mute){
    switch (mute)
    {
    case 0: mute=1;break; //mute off
    case 1: mute=0;break; //mute on
    }
    int mute_sett=mute+MUTE_IIC+MUTE_TIME+STEP_TIME+CLK_FST;
    //Serial.println("mute");Serial.println(mute_sett);
    writeWire(SOFT_MUTE,mute_sett);
}
void TDA7419::setVol(int vol){
    if(vol<0){vol=abs(vol)+16;}
    //Serial.println("vol :");Serial.println(vol);
    writeWire(VOLUME,vol+SOFT_STEP);
}
void TDA7419::setFl(int fl){
    fl= 174-fl;
    //Serial.println("fl :");Serial.println(fl);
    writeWire(ATT_FL,fl+SOFT_STEP);
}
void TDA7419::setFr(int fr){
    fr= 174-fr;
    //Serial.println("fr :");Serial.println(fr);
    writeWire(ATT_FR,fr+SOFT_STEP);
}
void TDA7419::setRl(int rl){
    rl=174-rl;
    writeWire(ATT_RL,rl+SOFT_STEP);
}
void TDA7419::setRr(int rr){
    rr=174-rr;
    writeWire(ATT_RR,rr+SOFT_STEP);
}
void TDA7419::setSub(int sub){
    sub=174-sub;
    writeWire(ATT_SUB,sub);
}
void TDA7419::setMix(int mix){
    mix=174-mix;
    writeWire(MX_LEVEL,mix);
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
    //Serial.println("treble");Serial.println(tr_sett);
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
    //Serial.println("mid :");Serial.println(mid);
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
    //Serial.println("bass :");Serial.println(bass_sett);
    writeWire(BASS,bass_sett);
}
void TDA7419::sdInput(int src,int sel){
    int sd_sett=src+SD_GAIN+sel;
    //Serial.println("sd_in :");Serial.println(sd_sett);;
    writeWire(SD_SOURCE,sd_sett);
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
    //Serial.println("freq :");Serial.println(freq_sett);
    writeWire(SUB_M_BASS,freq_sett);
}
void TDA7419::setMixGain(int ml,int mr,int en_mx,int en_sub,int gain_ef){
    int gain_sett=ml+mr+en_mx+en_sub+gain_ef;
    //Serial.println("mix_gain :");Serial.println(gain_sett);
    writeWire(MX_GAIN,gain_sett);
}
void TDA7419::specAnalyse(int spc_filter,int rst_mode,int spc_src, int spc_run, int rst, int clk, int ac){
    int spec_sett= spc_filter+rst_mode+spc_src+spc_run+rst+clk+ac;
    //Serial.println("spec :");Serial.println(spec_sett);
    writeWire(SPEC_ANL,spec_sett);
}
void TDA7419::writeWire(char a, char b){
    Wire.beginTransmission(IC_ADDRESS);
    Wire.write(a);
    Wire.write(b);
    Wire.endTransmission();
}