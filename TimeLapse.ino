 #include <IRremote.h>
 #include <LiquidCrystal.h>
 #include <Stepper.h>

 
 int ir=0;
 int m1=6;
 int m2=7;
 int m3=8;
 int m4=9;
 int shutter=1;
 int focus=13;
 unsigned long int last=0,time=0;
 
 int speeed=0;
 int pause=0;
 int moving=0;
 int focustime=0;
 unsigned long moment;
 int shots=0;
 
 boolean FOCUS=false;
 boolean start=false;
 boolean conf=true;
 boolean reverse=false;
 boolean button=false;
 
 LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);
 IRrecv irrecv(ir);
 decode_results results;
 decode_results menu;
 
 boolean t=false;
 
 
 void shoot(int pause, int focustime)
 {
   shots++;
   if(focustime)
   {
   digitalWrite(focus,HIGH);
   delay(focustime);
   digitalWrite(focus,LOW);
   }
   digitalWrite(shutter,HIGH);
   delay(pause+100);
   digitalWrite(shutter,LOW); 
 }
 
 void ostanovka()
{
  if(pause) {
  digitalWrite(m1,LOW);
  digitalWrite(m2,LOW);
  digitalWrite(m3,LOW);
  digitalWrite(m4,LOW);
  if((millis()-time)>(moving)){time=0; delay(1000); shoot(pause,focustime); time = millis(); }}
}
void koncevik()
{
  if((analogRead(A3)==0) && (!button)) {start=false; conf=true; reverse=!reverse;button=true;}
}

void animation()
{
  lcd.setCursor(7,1);
 lcd.print(shots);
}

void setup()
{
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m4, OUTPUT); 
  pinMode(shutter, OUTPUT);
  pinMode(focus, OUTPUT);
  digitalWrite(shutter,LOW);
  digitalWrite(focus,LOW);
  
  irrecv.enableIRIn();
  lcd.begin(20, 2);// задаем размерность дисплея
  lcd.print("Time-lapse slider"); 
  delay(1000);
  lcd.clear();

}







void loop()
{    
  if (irrecv.decode(&results)) {
  if (results.value!=0xFFFFFFFF){delay(50); last=results.value;}
  else {results.value=last;}
irrecv.resume(); // Получить следующее значение
} 


 if(analogRead(A3)!=0) button=false;

if(results.value==0x8B7FA05)
{conf=false;
FOCUS=false;
start=true;
moment=millis();
shots=0;
delay(30);
results.value=0;
lcd.clear();
lcd.print("Process");
lcd.setCursor(0,1);
lcd.print("Shots:");
}

if(results.value==0x8B7609F)
{ FOCUS=true;
conf=false;
start=false;
results.value=0;
 lcd.clear();
    lcd.setCursor(0, 0);
lcd.print("Focus time:");
    
}
  
  if(FOCUS)
  {
   
if(focustime<10)
{lcd.setCursor(12, 0);
lcd.print(" ");}
if(focustime<100)
{lcd.setCursor(13, 0);
lcd.print(" ");}
if(focustime<1000 && focustime>=100)
{lcd.setCursor(14, 0);
lcd.print(" ");}
lcd.setCursor(11, 0);
lcd.print(focustime);  
if((results.value==0x8B73AC5)&&(FOCUS))
{focustime++;
results.value=0;}
if((results.value==0x8B71AE5)&&(FOCUS))
{focustime--;
results.value=0;}

if((results.value==0x8B77887)&&(FOCUS))
{focustime+=25;
results.value=0;}
if((results.value==0x8B758A7)&&(FOCUS))
{if(focustime<25) {focustime=0;}
else{
  focustime-=25;}
results.value=0;}

if((results.value==0x8B77A85)&&(FOCUS))
{focustime+=250;
results.value=0;}
if((results.value==0x8B7DA25)&&(FOCUS))
{if(focustime<250) {focustime=0;}
else{
  focustime-=250;}
results.value=0;}

  }
  
  
  
if(results.value==0x8B722DD)
{ conf=true;
FOCUS=false;
lcd.clear();
lcd.setCursor(5,0);
lcd.print("Config mod");
delay(1000);
lcd.clear();
start=false;
results.value=0;}
    
  
  if(conf)
  {
  digitalWrite(m1,LOW);
  digitalWrite(m2,LOW);
  digitalWrite(m3,LOW);
  digitalWrite(m4,LOW);
lcd.setCursor(0, 0);
lcd.print("Time of step: ");
if(speeed<10)
{lcd.setCursor(15, 0);
lcd.print(" ");}
if(speeed<100)
{lcd.setCursor(16, 0);
lcd.print(" ");}
if((speeed<1000)&&(speeed>=100))
{lcd.setCursor(17, 0);
lcd.print(" ");}
lcd.setCursor(14, 0);
lcd.print(speeed);

lcd.setCursor(0, 1);
lcd.print("Pause:");
if(pause<10)
{lcd.setCursor(7, 1);
lcd.print(" ");}
if(pause<100)
{lcd.setCursor(8, 1);
lcd.print(" ");}
if(pause<1000 && pause>=100)
{lcd.setCursor(9, 1);
lcd.print(" ");}
if(pause<10000 && pause>=1000)
{lcd.setCursor(10, 1);
lcd.print(" ");}


lcd.setCursor(6, 1);
lcd.print(pause);

lcd.setCursor(11, 1);
lcd.print("->:");
if(moving<10)
{lcd.setCursor(15, 1);
lcd.print(" ");}
if(moving<100)
{lcd.setCursor(16, 1);
lcd.print(" ");}
if(moving<1000 && moving>=100)
{lcd.setCursor(17, 1);
lcd.print(" ");}
if(moving<10000 && moving>=1000)
{lcd.setCursor(18, 1);
lcd.print(" ");}
lcd.setCursor(14, 1);
lcd.print(moving);

if(!reverse)
{lcd.setCursor(19,1);
lcd.print(" ");
lcd.setCursor(19,0);
lcd.print("R");
}
else
{
lcd.setCursor(19,0);
lcd.print(" ");
lcd.setCursor(19,1);
lcd.print("L");
}

if(results.value==0x8B718E7)
{pause=0; moving=0; speeed=0;
lcd.clear();
results.value=0;}


if((results.value==0x8B73AC5)&&(pause!=60000)&&(conf))
{pause++;
results.value=0;}
if((results.value==0x8B71AE5)&&(pause!=0)&&(conf))
{pause--;
results.value=0;}

if((results.value==0x8B77887)&&(pause!=60000)&&(conf))
{pause+=25;
results.value=0;}
if((results.value==0x8B758A7)&&(pause!=0)&&(conf))
{if(pause<25) {pause=0;}
else{
  pause-=25;}
results.value=0;}

if((results.value==0x8B77A85)&&(pause!=60000)&&(conf))
{pause+=250;
results.value=0;}
if((results.value==0x8B7DA25)&&(pause!=0)&&(conf))
{if(pause<250) {pause=0;}
else{
  pause-=250;}
results.value=0;}







if((results.value==0x8B7A05F)&&(moving!=99000))
{moving+=100;
results.value=0;}
if((results.value==0x8B7E21D)&&(moving!=0))
{moving-=100;
results.value=0;}

if((results.value==0x8B750AF)&&(speeed!=1000))
{speeed++;results.value=0;}
if((results.value==0x8B7906F)&&(speeed!=0))
{speeed--;results.value=0;}



 if(results.value==0x8B740BF)
 {reverse=!reverse;
 delay(50);
 results.value=0;}

 }
 

 
 if(start)
{
  if(!time) time = millis();

  if(!reverse)
  {
  // step 1 
digitalWrite(m1, 1);
digitalWrite(m2, 0);
digitalWrite(m3, 1);
digitalWrite(m4, 0);
  delay (speeed);
  ostanovka();
  animation();
  koncevik();
    // step 2
digitalWrite(m1, 1);
digitalWrite(m2, 0);
digitalWrite(m3, 0);
digitalWrite(m4, 1);
  delay ( speeed);
  ostanovka();
  animation();
  koncevik();
    // step 3
digitalWrite(m1, 0);
digitalWrite(m2, 1);
digitalWrite(m3, 0);
digitalWrite(m4, 1);
  delay ( speeed);
  ostanovka();
  animation();
  koncevik();
    // step 4
digitalWrite(m1, 0);
digitalWrite(m2, 1);
digitalWrite(m3, 1);
digitalWrite(m4, 0);
  delay ( speeed);
  ostanovka();
  animation();
  koncevik();
  }
  else
  { 
    // step 8
digitalWrite(m1, 0);
digitalWrite(m2, 1);
digitalWrite(m3, 1);
digitalWrite(m4, 0);
  delay ( speeed);
  ostanovka();
  animation();
  koncevik();
    // step 7
digitalWrite(m1,0);
digitalWrite(m2, 1);
digitalWrite(m3, 0);
digitalWrite(m4, 1);
  delay ( speeed);
  ostanovka();
  animation();
  koncevik();
    // step 6
digitalWrite(m1, 1);
digitalWrite(m2, 0);
digitalWrite(m3, 0);
digitalWrite(m4, 1);
  delay ( speeed);
  ostanovka();
  animation();
  koncevik();
    // step 5
digitalWrite(m1,1);
digitalWrite(m2, 0);
digitalWrite(m3, 1);
digitalWrite(m4, 0);
  delay ( speeed);
  ostanovka();
  animation();
  koncevik();
  }
 }
}

