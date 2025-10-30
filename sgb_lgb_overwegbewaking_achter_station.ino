/*
Overwegbomen LGB-baan achter station
Herschreven en getest 21 juli 2025
Pieter van der Pijl 
*/

const int LEDPIN1=3;
const int LEDPIN2=4;
int LedState1=LOW;
int LedState2=LOW;
int StartKnipper=5;
int Motor1Pin1 = 8;  //blauw-pin1
int Motor1Pin2 = 9;  //pink-pin2
int Motor1Pin3 = 10; //geel-pin3
int Motor1Pin4 = 11; //oranje-pin4
                     //rood-pin5 = VCC
int MotorSpeed=10;
int Count=0;
int TreinDetectie=6;
int OverwegBoom=2; //ReedContact OverwegBoom open terugkoppeling
int VolleSlag=96; //Beweging OverwegBoom
int I=7; //Open/Dicht richting
int LookUp [8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};
unsigned long VorigeMillis = 0;
unsigned long Interval = 400000;


void setup () 
{
  pinMode (Motor1Pin1,OUTPUT);
  pinMode (Motor1Pin2,OUTPUT);
  pinMode (Motor1Pin3,OUTPUT);
  pinMode (Motor1Pin4,OUTPUT);
  pinMode (OverwegBoom,INPUT_PULLUP); //Pin2 - Hoog bij open overweg
  pinMode (TreinDetectie,INPUT_PULLUP); //Pin6 - Laag bij trein nadering
  pinMode (LEDPIN1,OUTPUT); //Pin3
  pinMode (LEDPIN2,OUTPUT); //Pin4
  pinMode (StartKnipper,INPUT_PULLUP); //Pin5
  digitalWrite (LEDPIN1,LOW);
  digitalWrite (LEDPIN2,LOW);
  Serial.begin (9600);

  Count = 0;
}

void knipper()
{
  unsigned long VerlopenMillis=micros();
  if (VerlopenMillis-VorigeMillis>=Interval)
  {
    VorigeMillis=VerlopenMillis;
    if (LedState1==LOW)
    {
      LedState1=HIGH;
      LedState2=LOW;
    }
    else
    {
      LedState1=LOW;
      LedState2=HIGH;
    }
    digitalWrite(LEDPIN1,LedState1);
    digitalWrite(LEDPIN2,LedState2);
  }
}
void clockwise () 
{
  for (I = 0; I<8; I++)
  {
    setOutput1 (I);
    delay (MotorSpeed);
  }
    Count++;
}

void anticlockwise ()
{
  for(I=7; I>=0; I--)
  {
    setOutput1 (I);
    delay (MotorSpeed);
  }
    Count--;
}
void setOutput1 (int out)
{
  digitalWrite(Motor1Pin1,bitRead(LookUp[out],0));
  digitalWrite(Motor1Pin2,bitRead(LookUp[out],1));
  digitalWrite(Motor1Pin3,bitRead(LookUp[out],2));
  digitalWrite(Motor1Pin4,bitRead(LookUp[out],3));
}
void loop ()
{
  if ((digitalRead(TreinDetectie)==LOW)||(digitalRead(OverwegBoom)==LOW)) //Overweg lichten aan bij treindetectie of boom niet omhoog.
  {
    knipper(); //start knipperlicht routine
  }
  else
  {
    Count=0;
    LedState1=LOW;
    LedState2=LOW;
  }
  {
   digitalWrite(LEDPIN1,LedState1);
   digitalWrite(LEDPIN2,LedState2);
  }
  if((digitalRead(TreinDetectie)==LOW)&&(Count<=VolleSlag)) //Sluit OverwegBoom wanneer Pin6==LOW tot Volleslag==96
  {
    clockwise(); //start clockwise routine
  }
  if((digitalRead(TreinDetectie)==HIGH)&&(digitalRead(OverwegBoom)==LOW)) //Open OverwegBoom wanneer Pin6==High && Pin2==LOW tot Pin2==HIGH
  {
    anticlockwise(); //start anticlockwise routine
    Count=0;
  }
}
