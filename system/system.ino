
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
#define IR_RE   0xFFFFFFFF//ir_func
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
#define LOUD_P  0x807F6A95//loud++
#define LOUD_M  0x807F4AB5//loud--
#define SEL_FQ  0x807F12ED//change frequency's

//custom binary's
byte zuhl[4][8]={  { B11111,B10001,B11101,B10001,B10111,B10001,B11111,B00000},//z
                   { B11111,B10101,B10101,B10101,B10101,B10001,B11111,B00000},//u
                   { B11111,B10101,B10101,B10001,B10101,B10101,B11111,B00000},//h
                   { B11111,B10111,B10111,B10111,B10111,B10001,B11111,B00000},//l
                 };
byte DIG[4][8] = {
  { B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111},//Y block
  { B11111,B11111,B00000,B00000,B00000,B00000,B00000,B00000},//xl block
  { B00000,B00000,B00000,B00000,B00000,B00000,B11111,B11111},//xr block
  { B11111,B11111,B00000,B00000,B00000,B00000,B11111,B11111},//xlr blck
  };

                 
TDA7419 tda7419;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);// RS,E,D4,D5,D6,D7 connection LCD

unsigned long time,key_value;
int backlight=8;//lcd backlight connection
int IR_PIN=9;   //ir sensor out
//Rotary encoder pins
const byte DAT = 10;
const byte CLK = 11;
int BTN = 12;
int8_t state = 0;

IRrecv irrecv(IR_PIN);
decode_results ir;

int menu,mute,in,rom,pre_menu,s;
int mix,gain,att_loud,cf,t_mute,t_step,tr_cn,sub_c,mid_c,bass_c;
int vol,front,rear,sub,bass,mid,treble;
boolean light,sel;
String MENUS[]={"AUDIO-SYSTEM","INPUT","GAIN","VOLUME","FRONT","REAR","SUBOUT","MIXING","TREBLE","MIDDLE","BASS","LOUD","MUTE"};

void setup(){
  
  pinMode(backlight,OUTPUT);
  pinMode(DAT,INPUT);
  pinMode(CLK,INPUT);
  pinMode(BTN,INPUT_PULLUP);
   
  
  Wire.begin();
  Serial.begin(9600);
  lcd.begin(16,2);
  irrecv.enableIRIn();
  
lcd.createChar(0,DIG[0]);
lcd.createChar(1,DIG[1]);
lcd.createChar(2,DIG[2]);
lcd.createChar(3,DIG[3]);
lcd.createChar(4, zuhl[0]);
lcd.createChar(5, zuhl[1]);
lcd.createChar(6, zuhl[2]);
lcd.createChar(7, zuhl[3]);

  readRom(); audio();
  printDev();turnOnlcd(1);//on/off 0/1
    
}

void loop(){

  // Remote control menu selection 
  if(irrecv.decode(&ir)){
    if(ir.value==IR_RE){
      ir.value=key_value;
    }
    switch(ir.value){
      case IN_SW: menu=1;break;
      case GAIN_P: case GAIN_M:menu=2;break;
      case VOL_P:  case VOL_M:menu=3;break;
      case FR_P:   case FR_M:menu=4;break;
      case RR_P :  case RR_M:menu=5;break;
      case SUB_P : case SUB_M:menu=6;break;
      case MIX_P : case MIX_M:menu=7;break;
      case TR_P  : case TR_M :menu=8;break;
      case MID_P : case MID_M:menu=9;break;
      case BAS_P : case BAS_M:menu=10;break;
      case LOUD_P : case LOUD_M:menu=11;break;
      case MUTE_SW : menu=12; break;

    }
    key_value=ir.value;
    delay(100);
   irrecv.resume();
   Serial.println(ir.value,HEX);
 }

 //Rotary encoder menu slection
  if(digitalRead(BTN)==LOW){
    if(millis()-time>50){
      key_value=1;
      if(menu==6||menu==8||menu==9||menu==10||menu==11){sel=true;}//for rotary encoder frequency selection 
      else{menu++;}
      if(sel){s++;lcd.setCursor(0,1);lcd.print(">");if(s>1){s=0;sel=false;menu++;}}
      cl();if(menu>11){menu=1;}rom=1;
      }
     time=millis();
  }

//input
 if(menu==1){
  rotaryEncoder(state);
  if(ir.value==IN_SW||state==1 ){in++;cl();if(in>3){in=0;}audio();rom=1;time=millis();}
  printMenu();printValue(in+1);
 }
//input gain
  if(menu==2){
    rotaryEncoder(state);
    if(ir.value==GAIN_P||state== 1){gain++;cl();if(gain>15){gain=15;}audio();rom=1;time=millis();}
    if(ir.value==GAIN_M||state==-1){gain--;cl();if(gain<0){gain=0;}audio();rom=1;time=millis();}
    printMenu();printValue(gain);
    
  }
//main volume
  if(menu==3){
      rotaryEncoder(state);
      if(ir.value==VOL_P||state== 1){vol++;cl();if(vol>79){vol=79;}audio();rom=1;time=millis();}
      if(ir.value==VOL_M||state==-1){vol--;cl();if(vol<0){vol=0;}audio();rom=1;time=millis();}
      printMenu();printValue(vol);
    
  }
//front volume
  if(menu==4){
    rotaryEncoder(state);
    if(ir.value==FR_P||state== 1){front++;cl();if(front>15){front=15;}audio();rom=1;time=millis();}
    if(ir.value==FR_M||state==-1){front--;cl();if(front<0){front=0;}audio();rom=1;time=millis();}
    printMenu();printValue(front);
  }
//rear volume
  if(menu==5){
    rotaryEncoder(state);
    if(ir.value==RR_P||state== 1){rear++;cl();if(rear>15){rear=15;}audio();rom=1;time=millis();}
    if(ir.value==RR_M||state==-1){rear--;cl();if(rear<0){rear=0;}audio();rom=1;time=millis();}
    printMenu();printValue(rear);
  }
//subwoofer volume
  if(menu==6){
    rotaryEncoder(state);
    if(ir.value==SUB_P||state== 1&&!sel){sub++;cl();if(sub>15){sub=15;}audio();rom=1;time=millis();}
    if(ir.value==SUB_M||state==-1&&!sel){sub--;cl();if(sub<0){sub=0;}audio();rom=1;time=millis();}
    if(ir.value==SEL_FQ||sel&&state==1){sub_c++;cl();if(sub_c>3){sub_c=0;}audio();rom=1;time=millis();}
    printMenu();printValue(sub);
  }
//mixing
  if(menu==7){
    rotaryEncoder(state);
    if(ir.value==MIX_P||state== 1){mix++;cl();if(mix>15){mix=15;}audio();rom=1;time=millis();}
    if(ir.value==MIX_M||state==-1){mix--;cl();if(mix<0){mix=0;}audio();rom=1;time=millis();}
    printMenu();printValue(mix);
  }
//treble
  if(menu==8){
    rotaryEncoder(state);
    if(ir.value==TR_P||state== 1&&!sel){treble++;cl();if(treble>15){treble=15;}audio();rom=1;time=millis();}
    if(ir.value==TR_M||state==-1&&!sel){treble--;cl();if(treble<-15){treble=-15;}audio();rom=1;time=millis();}
    if(ir.value==SEL_FQ||sel&&state==1){tr_cn++;cl();if(tr_cn>3){tr_cn=0;}audio();rom=1;time=millis();}
    printMenu();printValue(treble);
  }
//mid
if(menu==9){
    rotaryEncoder(state);
    if(ir.value==MID_P||state== 1&&!sel){mid++;cl();if(mid>15){mid=15;}audio();rom=1;time=millis();}
    if(ir.value==MID_M||state==-1&&!sel){mid--;cl();if(mid<-15){mid=-15;};audio();rom=1;time=millis();}
    if(ir.value==SEL_FQ||sel&&state==1){mid_c++;cl();if(mid_c>3){mid_c=0;}audio();rom=1;time=millis();}
    printMenu();printValue(mid);
  }
//bass
if(menu==10){
    rotaryEncoder(state);
    if(ir.value==BAS_P||state== 1&&!sel){bass++;cl();if(bass>15){bass=15;}audio();rom=1;time=millis();}
    if(ir.value==BAS_M||state==-1&&!sel){bass--;cl();if(bass<-15){bass=-15;}audio();rom=1;time=millis();}
    if(ir.value==SEL_FQ||sel&&state==1){bass_c++;cl();if(bass_c>3){bass_c=0;}audio();rom=1;time=millis();}
    printMenu();printValue(bass);
  }
//loudness
if(menu==11){
  rotaryEncoder(state);
    if(ir.value==LOUD_P||state== 1&&!sel){att_loud++;cl();if(att_loud>15){att_loud=15;}audio();rom=1;time=millis();}
    if(ir.value==LOUD_M||state==-1&&!sel){att_loud--;cl();if(att_loud<0){att_loud=0;}audio();rom=1;time=millis();}
    if(ir.value==SEL_FQ||sel&&state==1){cf++;cl();if(cf>3){cf=0;}audio();rom=1;time=millis();}
    printMenu();printValue(att_loud);
}
//mute
if(menu==12){
   if(mute==0&&ir.value==MUTE_SW){cl();mute=1;audio();}
   if(mute==1&&ir.value==MUTE_SW){cl();mute=0;audio();}
   printMute();
}

pre_menu=menu;//previews menu

//5sec after updating rom and displaying main menu
  if(millis()-time>5000&&rom==1){
    updateRom();cl();key_value=0;rom=0;
  if(mute==1){
    menu=12;cl();
   }else{
    menu=0;cl();printDev();
   }
  }
  //1minuts after off the display light
  if(millis()-time>60000){turnOnlcd(0);light=false;}//turn off backlight 1s
  if(key_value&&!light){turnOnlcd(1);light=true;}//turn on backlight from inputs 

}
//rotary encoder control 
bool rotaryEncoder(int8_t &delta) {
   delta = 0;
  enum {STATE_LOCKED, STATE_TURN_RIGHT_START, 
  STATE_TURN_RIGHT_MIDDLE, STATE_TURN_RIGHT_END, 
  STATE_TURN_LEFT_START, STATE_TURN_LEFT_MIDDLE, 
  STATE_TURN_LEFT_END, STATE_UNDECIDED};
  
  static uint8_t encoderState = STATE_LOCKED;
  bool a = !digitalRead(DAT);
  bool b = !digitalRead(CLK);
  
  switch (encoderState) {
    case STATE_LOCKED:
      if (a && b) {
        encoderState = STATE_UNDECIDED;
      }
      else if (!a && b) {
        encoderState = STATE_TURN_LEFT_START;
      }
      else if (a && !b) {
        encoderState = STATE_TURN_RIGHT_START;
      }
      else {
        encoderState = STATE_LOCKED;
      };
      break;
    case STATE_TURN_RIGHT_START:
      if (a && b) {
        encoderState = STATE_TURN_RIGHT_MIDDLE;
      }
      else if (!a && b) {
        encoderState = STATE_TURN_RIGHT_END;
      }
      else if (a && !b) {
        encoderState = STATE_TURN_RIGHT_START;
      }
      else {
        encoderState = STATE_LOCKED;
      };
      break;
    case STATE_TURN_RIGHT_MIDDLE:
    case STATE_TURN_RIGHT_END:
      if (a && b) {
        encoderState = STATE_TURN_RIGHT_MIDDLE;
      }
      else if (!a && b) {
        encoderState = STATE_TURN_RIGHT_END;
      }
      else if (a && !b) {
        encoderState = STATE_TURN_RIGHT_START;
      }
      else {
        encoderState = STATE_LOCKED;
        delta = -1;
      };
      break;
    case STATE_TURN_LEFT_START:
      if (a && b) {
        encoderState = STATE_TURN_LEFT_MIDDLE;
      }
      else if (!a && b) {
        encoderState = STATE_TURN_LEFT_START;
      }
      else if (a && !b) {
        encoderState = STATE_TURN_LEFT_END;
      }
      else {
        encoderState = STATE_LOCKED;
      };
      break;
    case STATE_TURN_LEFT_MIDDLE:
    case STATE_TURN_LEFT_END:
      if (a && b) {
        encoderState = STATE_TURN_LEFT_MIDDLE;
      }
      else if (!a && b) {
        encoderState = STATE_TURN_LEFT_START;
      }
      else if (a && !b) {
        encoderState = STATE_TURN_LEFT_END;
      }
      else {
        encoderState = STATE_LOCKED;
        delta = 1;
      };
      break;
    case STATE_UNDECIDED:
      if (a && b) {
        encoderState = STATE_UNDECIDED;
      }
      else if (!a && b) {
        encoderState = STATE_TURN_RIGHT_END;
      }
      else if (a && !b) {
        encoderState = STATE_TURN_LEFT_END;
      }
      else {
        encoderState = STATE_LOCKED;
      };
      break;
  }
}
//display and remote hex value clearing
void cl(){
  if(pre_menu!=menu){
    ir.value=0;lcd.clear();
  }else{
    ir.value=0;
  }
}
//lcd on/off function
void turnOnlcd(int a){
  switch (a){
    case 0:digitalWrite(backlight,LOW);break;
    case 1:digitalWrite(backlight,HIGH);break;
  }
}
//reading values from eeprom
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
//updating values of eeprom
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
//main audio function
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
  tda7419.spectrum(1,1,1,0,0,0,0);
  
}
//print mute menu
void printMute(){
  switch(mute){
    case 1:lcd.setCursor(6,0);lcd.print(MENUS[menu]);delay(500);
           lcd.setCursor(6,0);lcd.print("    ");delay(500);break;
    case 0:lcd.clear();menu=3;rom=1;time=millis();
  }
}
//print main menu's and frequency's
void printMenu(){
  lcd.setCursor(1,0);
  lcd.print(MENUS[menu]);
    
  if(menu==6){
    lcd.setCursor(1,1);
    switch(sub_c){
        case 0:lcd.print("FLAT ");break;
        case 1:lcd.print("80Hz ");break;
        case 2:lcd.print("120Hz");break;
        case 3:lcd.print("160Hz");break;
        }
  }
  if(menu==8){
  lcd.setCursor(1,1);
  switch(tr_cn){
    case 0:lcd.print("10KHz");break;
    case 1:lcd.print("12KHz");break;
    case 2:lcd.print("15KHz");break;
    case 3:lcd.print("17KHz");break;
     }
 }
 if(menu==9){
  lcd.setCursor(1,1);
   switch(mid_c){
    case 0:lcd.print("500Hz ");break;
    case 1:lcd.print("1000Hz");break;
    case 2:lcd.print("1500Hz");break;
    case 3:lcd.print("2500Hz");break;
    }
 }
 if(menu==10){
  lcd.setCursor(1,1);
  switch(bass_c){
     case 0:lcd.print("60Hz ");break;
     case 1:lcd.print("80Hz ");break;
     case 2:lcd.print("100Hz");break;
     case 3:lcd.print("200Hz");break;
    }
 }
 if(menu==11){
  lcd.setCursor(1,1);
  switch(cf){
     case 0:lcd.print("FLAT  ");break;
     case 1:lcd.print("400Hz ");break;
     case 2:lcd.print("800Hz ");break;
     case 3:lcd.print("2400Hz");break;
    }
 }
}
//print developer
void printDev(){
  lcd.clear();
  lcd.setCursor(6,0);lcd.write(byte(4));lcd.write(byte(5));
  lcd.write(byte(6));lcd.write(byte(7));
  lcd.setCursor(2,1);lcd.print(MENUS[menu]);
}
//print digitis
void printValue(int val){
int col=9;
  if(val<0){val=abs(val);lcd.setCursor(7,1);lcd.print("-");}
  else{lcd.setCursor(7,1);lcd.print(" ");}
  
  printDigits(val/10,col);
  printDigits(val%10,col+4);
  
}

void printDigits(int digits, int x){
  switch (digits) {
  case 0: digit_0(x);break;
  case 1: digit_1(x);break;
  case 2: digit_2(x);break;
  case 3: digit_3(x);break;
  case 4: digit_4(x);break;
  case 5: digit_5(x);break;
  case 6: digit_6(x);break;
  case 7: digit_7(x);break;
  case 8: digit_8(x);break;
  case 9: digit_9(x);break;
  }
}

void digit_0(int x){
  lcd.setCursor(x,0);
  lcd.write((byte)0); 
  lcd.write(byte(1)); 
  lcd.write(byte(0));
  lcd.setCursor(x, 1);
  lcd.write(byte(0)); 
  lcd.write(byte(2)); 
  lcd.write(byte(0));
}
void digit_1(int x){
  lcd.setCursor(x,0);
  lcd.write(byte(1));
  lcd.write(byte(0));
  lcd.print(" ");
  lcd.setCursor(x,1);
  lcd.write(byte(2));
  lcd.write(byte(0));
  lcd.write(byte(2));
}
void digit_2(int x){
  lcd.setCursor(x,0);
  lcd.write(byte(3));
  lcd.write(byte(3));
  lcd.write(byte(0));
  lcd.setCursor(x, 1);
  lcd.write(byte(0));
  lcd.write(byte(2));
  lcd.write(byte(2));
}
void digit_3(int x){
  lcd.setCursor(x,0);
  lcd.write(byte(3));
  lcd.write(byte(3));
  lcd.write(byte(0));
  lcd.setCursor(x, 1);
  lcd.write(byte(2));
  lcd.write(byte(2));
  lcd.write(byte(0));
}
void digit_4(int x){
  lcd.setCursor(x,0);
  lcd.write(byte(0));
  lcd.write(byte(2));
  lcd.write(byte(0));
  lcd.setCursor(x, 1);
  lcd.print("  ");
  lcd.write(byte(0));
}
void digit_5(int x){
  lcd.setCursor(x,0);
  lcd.write(byte(0));
  lcd.write(byte(3));
  lcd.write(byte(3));
  lcd.setCursor(x, 1);
  lcd.write(byte(2));
  lcd.write(byte(2));
  lcd.write(byte(0));
}
void digit_6(int x){
  lcd.setCursor(x,0);
  lcd.write((byte)0);
  lcd.write(byte(3));
  lcd.write(byte(3));
  lcd.setCursor(x, 1);
  lcd.write(byte(0));
  lcd.write(byte(2));
  lcd.write(byte(0));
}
void digit_7(int x){
  lcd.setCursor(x,0);
  lcd.write(byte(1));
  lcd.write(byte(1));
  lcd.write(byte(0));
  lcd.setCursor(x, 1);
  lcd.print(" ");
  lcd.print(" ");
  lcd.write(byte(0));
}
void digit_8(int x){
  lcd.setCursor(x,0);
  lcd.write(byte(0));
  lcd.write(byte(3));
  lcd.write(byte(0));
  lcd.setCursor(x, 1);
  lcd.write(byte(0));
  lcd.write(byte(2));
  lcd.write(byte(0));
}
void digit_9(int x){
  lcd.setCursor(x,0);
  lcd.write(byte(0));
  lcd.write(byte(3));
  lcd.write(byte(0));
  lcd.setCursor(x, 1);
  lcd.print(" ");
  lcd.print(" ");
  lcd.write(byte(0));
}
