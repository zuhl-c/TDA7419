
/*This program was written by zuhail-pm/
  <november-8-2021>
  <devloper.zuhail@gmial.com>
  <github.com/zuhl-c*/

#include <LiquidCrystal.h>
#include <Wire.h>
#include <EEPROM.h>
#include <IRremote.h>
#include <TDA7419.h>

//IR Hex codes
#define DEC_IR  0xFFFFFFFF//ir_func
#define POW     0x807FBA45//power
#define IN_SW   0x807F3AC5//input
#define MUTE_SW 0x807FFA05//mute
#define VOL_P   0x807F32CD//vol ++
#define VOL_M   0x807F52AD//vol --
#define FR_P    0x807F08F7//front ++
#define FR_M    0x807F8877//front --
#define RR_P    0x807F18E7//rear++
#define RR_M    0x807F9867//rear--
#define MIX_P   0x807F28D7//mix++
#define MIX_M   0x807FA857//mix--
#define SUB_P   0x807F30CF//sub++
#define SUB_M   0x807FB04F//sub--
#define BAS_P   0x807F9A65//bass++
#define BAS_M   0x807FDA25//bass--
#define MID_P   0x807F1AE5//mid++
#define MID_M   0x807FAA55//mid--
#define TR_P    0x807F5AA5//treble++
#define TR_M    0x807F2AD5//treble--
#define GAIN_P  0x807FD22D//gain++
#define GAIN_M  0x807F926D//gain--
#define SEL_FQ  0x807F12ED//select frequency options
#define LOUD_P  0x807F6A95//loud++
#define LOUD_M  0x807F4AB5//loud--

byte _zuhl[4][8]={ { B11111,B10001,B11101,B10001,B10111,B10001,B11111,B00000},//z
                   { B11111,B10101,B10101,B10101,B10101,B10001,B11111,B00000},//u
                   { B11111,B10101,B10101,B10001,B10101,B10101,B11111,B00000},//h
                   { B11111,B10111,B10111,B10111,B10111,B10001,B11111,B00000},//l
                 };
byte spkr[8]={ B00000,B00001,B00011,B01111,B01111,B01111,B00011,B00001};
byte cx[8]={  B00000,B00000,B01010,B00100,B01010,B00000,B00000,B00000};

TDA7419 tda7419;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);// RS,E,D4,D5,D6,D7 connection LCD
unsigned long time;
int backlight=8;//lcd backlight connection
int IR_PIN=9;   //ir sensor out
IRrecv irrecv(IR_PIN);
decode_results ir;

int menu,mute,in,rom;
int mix,gain,att_loud,cf,t_mute,t_step,tr_cn,sub_c,mid_c,bass_c;
int vol,front,rear,sub,bass,mid,treble;
boolean inc,dec;
boolean light,disable;

String MENUS[]={"INPUT","GAIN","VOLUME","FRONT","REAR","SUBOUT","MIXING","TREBLE","MIDDLE","BASS","LOUD"};

void setup(){
  
  pinMode(backlight,OUTPUT);
  
  Wire.begin();
  Serial.begin(9600);
  lcd.begin(16,2);
  irrecv.enableIRIn();


  //custom charecter//
  lcd.createChar(0,_zuhl[0]);
  lcd.createChar(1,_zuhl[1]);
  lcd.createChar(2,_zuhl[2]);
  lcd.createChar(3,_zuhl[3]);
  lcd.createChar(4,spkr);
  lcd.createChar(5,cx);
  
  turnOnlcd(1);//on/off 0/1
  readRom(); audio();
    
}

void loop(){
  
  if(irrecv.decode(&ir)){
    switch(ir.value){
      case IN_SW:  case GAIN_P: case GAIN_M:menu=1;break;
      case VOL_P:  case VOL_M:menu=2;break;
      case FR_P:   case FR_M:menu=3;break;
      case RR_P :  case RR_M:menu=4;break;
      case SUB_P : case SUB_M:menu=5;break;
      case MIX_P : case MIX_M:menu=6;break;
      case TR_P  : case TR_M :menu=7;break;
      case MID_P : case MID_M:menu=8;break;
      case BAS_P : case BAS_M:menu=9;break;
      case LOUD_P : case LOUD_M:menu=10;break;

    }
    Serial.println(ir.value,HEX);
    delay(100);
   irrecv.resume();
 }
 //mute menu//
 if(mute==0&&ir.value==MUTE_SW){cl();mute=1;inc=false;dec=false;audio();}
 if(mute==1&&ir.value==MUTE_SW){cl();mute=0;inc=false;dec=false;audio();lcd.clear();menu=menu;}
 if(mute==1){
  if(menu==0){lcd.setCursor(14,0);lcd.write(byte(4));lcd.write(byte(5));}
  else{lcd.setCursor(12,1);lcd.write(byte(4));lcd.write(byte(5));}
  }

 //main menmu//

 if(menu==0){printDev();}
  if(menu==1){
    if(ir.value==IN_SW){in++;clr();if(in>3){in=1;}inc=false;dec=false;audio();rom=1;time=millis();}
    if(ir.value==GAIN_P){gain++;clr();if(gain>15){gain=15;}inc=true;dec=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && inc){gain++;cl();if(gain>15){gain=15;}audio();rom=1;time=millis();}
    if(ir.value==GAIN_M){gain--;clr();if(gain<0){gain=0;}dec=true;inc=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && dec){gain--;cl();if(gain<0){gain=0;}audio();rom=1;time=millis();}
    printMenu(gain,false);
    
  }
  if(menu==2){
      if(ir.value==VOL_P){vol++;clr();if(vol>79){vol=79;}inc=true;dec=false;audio();rom=1;time=millis();}
      if(ir.value==DEC_IR && inc){vol++;cl();if(vol>79){vol=79;}audio();rom=1;time=millis();}
      if(ir.value==VOL_M){vol--;clr();if(vol<0){vol=0;}dec=true;inc=false;audio();rom=1;time=millis();}
      if(ir.value==DEC_IR && dec){vol--;cl();if(vol<0){vol=0;}audio();rom=1;time=millis();}
      printMenu(vol,false);
    
  }
  if(menu==3){
    if(ir.value==FR_P){front++;clr();if(front>15){front=15;}inc=true;dec=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && inc){front++;cl();if(front>15){front=15;}audio();rom=1;time=millis();}
    if(ir.value==FR_M){front--;clr();if(front<0){front=0;}dec=true;inc=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && dec){front--;cl();if(front<0){front=0;}audio();rom=1;time=millis();}
    printMenu(front,false);
  }
  if(menu==4){
    if(ir.value==RR_P){rear++;clr();if(rear>15){rear=15;}inc=true;dec=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && inc){rear++;cl();if(rear>15){rear=15;}audio();rom=1;time=millis();}
    if(ir.value==RR_M){rear--;clr();if(rear<0){rear=0;}dec=true;inc=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && dec){rear--;cl();if(rear<0){rear=0;}audio();rom=1;time=millis();}
    printMenu(rear,false);
  }
  if(menu==5){
    if(ir.value==SUB_P){sub++;clr();if(sub>15){sub=15;}inc=true;dec=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && inc){sub++;cl();if(sub>15){sub=15;}audio();rom=1;time=millis();}
    if(ir.value==SUB_M){sub--;clr();if(sub<0){sub=0;}dec=true;inc=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && dec){sub--;cl();if(sub<0){sub=0;}audio();rom=1;time=millis();}
    if(ir.value==SEL_FQ){inc=false;dec=false;sub_c++;clr();if(sub_c>3){sub_c=0;}audio();rom=1;time=millis();}
    printMenu(sub,sub_c);
  }
  if(menu==6){
    if(ir.value==MIX_P){mix++;clr();if(mix>15){mix=15;}inc=true;dec=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && inc){mix++;cl();if(mix>15){mix=15;}audio();rom=1;time=millis();}
    if(ir.value==MIX_M){mix--;clr();if(mix<0){mix=0;}dec=true;inc=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && dec){mix--;cl();if(mix<0){mix=0;}audio();rom=1;time=millis();}
    printMenu(mix,false);
  }
  if(menu==7){
    if(ir.value==TR_P){treble++;clr();if(treble>15){treble=15;}inc=true;dec=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && inc){treble++;cl();if(treble>15){treble=15;}audio();rom=1;time=millis();}
    if(ir.value==TR_M){treble--;clr();if(treble<-15){treble=-15;}dec=true;inc=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && dec){treble--;cl();if(treble<-15){treble=-15;}audio();rom=1;time=millis();}
    if(ir.value==SEL_FQ){inc=false;dec=false;tr_cn++;clr();if(tr_cn>3){tr_cn=0;}audio();rom=1;time=millis();}
    printMenu(treble,tr_cn);
  }
if(menu==8){
    if(ir.value==MID_P){mid++;clr();if(mid>15){mid=15;}inc=true;dec=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && inc){mid++;cl();if(mid>15){mid=15;}audio();rom=1;time=millis();}
    if(ir.value==MID_M){mid--;clr();if(mid<-15){mid=-15;}dec=true;inc=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && dec){mid--;cl();if(mid<-15){mid=-15;}audio();rom=1;time=millis();}
    if(ir.value==SEL_FQ){inc=false;dec=false;mid_c++;clr();if(mid_c>3){mid_c=0;}audio();rom=1;time=millis();}
    printMenu(mid,mid_c);
  }
if(menu==9){
    if(ir.value==BAS_P){bass++;clr();if(bass>15){bass=15;}inc=true;dec=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && inc){bass++;cl();if(bass>15){bass=15;}audio();rom=1;time=millis();}
    if(ir.value==BAS_M){bass--;clr();if(bass<-15){bass=-15;}dec=true;inc=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && dec){bass--;cl();if(bass<-15){bass=-15;}audio();rom=1;time=millis();}
    if(ir.value==SEL_FQ){inc=false;dec=false;bass_c++;clr();if(bass_c>3){bass_c=0;}audio();rom=1;time=millis();}
    printMenu(bass,bass_c);
  }
if(menu==10){
    if(ir.value==LOUD_P){att_loud++;clr();if(att_loud>15){att_loud=15;}inc=true;dec=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && inc){att_loud++;cl();if(att_loud>15){att_loud=15;}audio();rom=1;time=millis();}
    if(ir.value==LOUD_M){att_loud--;clr();if(att_loud<0){att_loud=0;}dec=true;inc=false;audio();rom=1;time=millis();}
    if(ir.value==DEC_IR && dec){att_loud--;cl();if(att_loud<0){att_loud=0;}audio();rom=1;time=millis();}
    if(ir.value==SEL_FQ){inc=false;dec=false;cf++;clr();if(cf>3){cf=0;}audio();rom=1;time=millis();}
    printMenu(att_loud,cf);
}
//5sec after updating rom
  if(millis()-time>5000&&rom==1){updateRom();clr();menu=0;rom=0;inc=false;dec=false;}
  if(millis()-time>60000){turnOnlcd(0);light=false;}//turn off backlight 1s
  if(ir.value&&!light){turnOnlcd(1);light=true;}//turn on backlight from inputs 
}
void clr(){
  ir.value=0;lcd.clear();
  }
void cl(){
  ir.value=0;
}
void turnOnlcd(int a){
  switch (a){
    case 0:digitalWrite(backlight,LOW);break;
    case 1:digitalWrite(backlight,HIGH);break;
  }
}
void readRom(){
  in=EEPROM.read(0);
  vol=EEPROM.read(1);
  bass=EEPROM.read(2);
  mid=EEPROM.read(3);
  treble=EEPROM.read(4);
  front=EEPROM.read(5);
  rear=EEPROM.read(6);
  sub=EEPROM.read(7);
  gain=EEPROM.read(8);
  att_loud=EEPROM.read(9);
  cf=EEPROM.read(10);
  tr_cn=EEPROM.read(11);
  sub_c=EEPROM.read(12);
  mid_c=EEPROM.read(12);
  bass_c=EEPROM.read(14);
  mix=EEPROM.read(15);
}
void updateRom(){
    EEPROM.update(0,in);
    EEPROM.update(1,vol);
    EEPROM.update(2,bass);
    EEPROM.update(3,mid);
    EEPROM.update(4,treble);
    EEPROM.update(5,front);
    EEPROM.update(6,rear);
    EEPROM.update(7,sub);
    EEPROM.update(8,gain);
    EEPROM.update(9,att_loud);
    EEPROM.update(10,cf);
    EEPROM.update(11,tr_cn);
    EEPROM.update(12,sub_c);
    EEPROM.update(13,mid_c);
    EEPROM.update(14,bass_c);
    EEPROM.update(15,mix);
}
void audio(){
  tda7419.input(in,gain);//Serial.print("in : ");Serial.println(in);
  tda7419.sInput(0,0,0);
  tda7419.setLoud(att_loud,cf);//Serial.print("loud : ");Serial.println(att_loud);
  tda7419.softMute(mute);//Serial.print("mute : ");Serial.println(mute);
  tda7419.setVol(vol);//Serial.print("volume : ");Serial.println(vol);
  tda7419.setFl(front);//Serial.print("front : ");Serial.println(front);
  tda7419.setFr(front);//Serial.print("rear : ");Serial.println(rear);
  tda7419.setRl(rear);//Serial.print("sub : ");Serial.println(sub);
  tda7419.setRr(rear);//Serial.print("mix : ");Serial.println(mix);
  tda7419.setSub(sub);//Serial.print("treble : ");Serial.println(treble);
  tda7419.setMix(mix);//Serial.print("treble cn : ");Serial.println(tr_cn);
  tda7419.setTreble(treble,tr_cn);//Serial.print("mid : ");Serial.println(mid);
  tda7419.setMid(mid);//Serial.print("bass : ");Serial.println(bass);
  tda7419.setBass(bass);//Serial.print("sub cn : ");Serial.println(sub_c);
  tda7419.setFreq(sub_c,mid_c,bass_c);//Serial.print("mid cn : ");Serial.println(mid_c);
  tda7419.mixFunc(0,0,0,0,10);//Serial.print("bass cn : ");Serial.println(bass_c);
  tda7419.spectrum(0,0,0,1,0,0,0);
  
}
void printMenu(int val_1,int val_2){
    String FQ;
    
    lcd.setCursor(1,0);lcd.print("< ");lcd.print(MENUS[menu]);lcd.setCursor(9,0);lcd.print(" >");
    if(val_1<10){lcd.setCursor(12,0);lcd.print("0");lcd.print(val_1);}
    else{lcd.setCursor(12,0);lcd.print(val_1);}
    
      lcd.setCursor(1,1);lcd.print("<");
      lcd.setCursor(10,1);lcd.print(">");
    
    lcd.setCursor(3,1);
      if(menu==5){
        switch(val_2){
          case 0:FQ="FLAT";break;
          case 1:FQ="80Hz";break;
          case 2:FQ="120Hz";break;
          case 3:FQ="160Hz";break;
          }
          lcd.print(FQ);
       }
       else if(menu==7){
        switch(val_2){
          case 0:FQ="10KHz";break;
          case 1:FQ="12KHz";break;
          case 2:FQ="15KHz";break;
          case 3:FQ="17KHz";break;
        }
        lcd.print(FQ);
       }
       else if(menu==8){
        switch(val_2){
          case 0:FQ="500Hz";break;
          case 1:FQ="1000Hz";break;
          case 2:FQ="1500Hz";break;
          case 3:FQ="2500Hz";break;
        }
        lcd.print(FQ);
       }
       else if(menu==9){
        switch(val_2){
          case 0:FQ="60Hz";break;
          case 1:FQ="80Hz";break;
          case 2:FQ="100Hz";break;
          case 3:FQ="200Hz";break;
        }
        lcd.print(FQ);
       }
       else if(menu==10){
        switch(val_2){
          case 0:FQ="FLAT";break;
          case 1:FQ="400Hz";break;
          case 2:FQ="800Hz";break;
          case 3:FQ="2400Hz";break;
        }
        lcd.print(FQ);
       }
       else{
        lcd.print(MENUS[0]+" ");lcd.print(in);
       }
}

void printDev(){
    lcd.setCursor(6,0);lcd.write(byte(0));lcd.write(byte(1));lcd.write(byte(2));lcd.write(byte(3));
    lcd.setCursor(2,1);lcd.print("AUDIO-SYSTEM");
}
