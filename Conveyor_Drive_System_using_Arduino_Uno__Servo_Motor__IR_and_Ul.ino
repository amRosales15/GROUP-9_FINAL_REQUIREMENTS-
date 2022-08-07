/*
Group9_ FINAL_REQUIREMENTS

Title: Conveyor Drive System Using Arduino Uno,
        Servo Motor, IR and Ultrasonic Sensor.
    

Coded by: Ariel M. Rosales
Created on: August 05,2022 
Last edit on: August 07,2022
---------------------------------------------------------

 This circuit can explain the  simple operation of a conveyor drive 
--------------------------------------------------------- 
 
 
Components Equivalent in Real Conveyor Drive:

2 DC Motor =      represents Conveyor Motor  
2 LEDs =          represents Single Acting Cylinder Spring Return
2 Servo Motor =   represents base of Robotic Manipulator (e.g.,SCARA RRP)


INSTRUCTIONS & CONDITIONS:


NOTE: The Whole Process must Starts from left to right

KINDLY READ CAREFULLY AND THOROUGHLY TO AVOID MIS-INTERPRETATION.

----------------------------------------------------------------------------------------
1.Pressing button "|<<" in  IR remote will  make the LEFT SERVO to move with 30 deg increments 
    until it reaches the assigned angle which is 0 deg.(clockwise  direction).
    
2. When instruction #1 happen, the red LED  will turn on for 2 seconds.

3. After LEFT SERVO reaches its assigned angle, it will back to its initial position leaving the 
  LEFT DC motor energized while RIGHT DC motor is off.  
  
-----------------------------------------------------------------------------------------   
4. Pressing button ">>|" in IR remote will make the RIGHT SERVO to move with  30 deg increments
     until it reaches the assigned angle which is 180 deg.(counter clockwise direction).
     
5. When instruction #4 happen, the green  LED will turn on for 2 seconds.    

6. After RIGHT SERVO reaches its assigned angle, it will back to its initial position leaving the 
  RIGHT DC motor energized and turn off LEFT DC motor.
  
-------------------------------------------------------------------------------------------
7. Instruction 2&5 symbolizes that the Cylinder had already been actuated.

8. For the left and right DC motor, it will run when the distance measured by Ultrasonic sensor
       is greater than 60(left) and 120(right), Thus, make sure that the measured value is above 120 or 
       else both DC motor will not be energized.
       
 (note: For the instruction 2, 5 and 8 to occur, the assigned angle in instruction 1&4 must be reached.) 
 
9. All the resulted output will be display in lcd screens.




------------------START OF PROGRAM--------------------------*/


//libraries and variables
#include <Wire.h>
#include <Servo.h>
int servoPin= 5;
int servoPin2=6;
Servo myServo;
Servo myServo2;

int angle1 =90;    // initial angle  for servo
int angle2 =90;
int angle3 =90;
int angle4 =90;

int angleStep1 =30; // angle increments
int angleStep2 =30;
int angleStep3 =30;
int angleStep4 =30;


int d1=60;//distance for relayPin1 to activate
int d2=120;//distance fro relaypin2 to activate

//LCDs
#include <LiquidCrystal_I2C.h>  
LiquidCrystal_I2C lcd1(32,16,2);
LiquidCrystal_I2C lcd2(33,16,2);

//LEDs
int LED1=8,LED2=7;

//relays
int relayPin1=4,relayPin2=2;

// ultrasonic
int trigPin=12,echoPin=10;
long duration;
int distance;


#include <IRremote.h>
int IRpin=11;
const boolean PCB = 0;
const String LEFT="|<<";  //for left servo
const String RIGHT=">>|"; //for right servo

IRrecv irrecv(IRpin);
unsigned  irRemote[] ={ 
             0xFD20DF, //prev button
             0xFD609F, //next button
             0xFD08F7, //1
             0xFD8877 //2
                  };


String IRremoteKey[]= {
       "|<<",//left
       ">>|",//right
       "1",
       "2"
       };
decode_results results;


void setup()
{
  pinMode(servoPin, OUTPUT); //for servo 
  pinMode(servoPin2, OUTPUT);
  myServo.attach(servoPin);
  myServo2.attach(servoPin2);
  
  pinMode(LED1, OUTPUT); //for led
  pinMode(LED2, OUTPUT);
  
  
  pinMode(relayPin1, OUTPUT);//for relaypins 
  pinMode(relayPin2, OUTPUT);
  
  pinMode(trigPin,OUTPUT);//for ultrasonic
  pinMode(echoPin,INPUT);
  
  
  pinMode(IRpin, INPUT);//for ir sensor
  irrecv.enableIRIn();
  
   
  Serial.begin(9600); //for serial monitor and lcd display
 
  //lcd1
  lcd1.begin(16,2);
  lcd1.init();
  lcd1.backlight();
  
  //lcd2
  lcd2.begin(16,2);
  lcd2.init();
  lcd2.backlight();
}//end of void setup

void ServoAct(String value){
 
  while(value == LEFT){
    if (angle1> 0 && angle1 <= 180) {
      angle1 = angle1 - angleStep1;
       if(angle1 < 0){
        angle1 = 0;
         
      }else{ 
      myServo.write(angle1); // move the servo to desired angle
      lcd2.setCursor(0,1);   
      lcd2.print("Moved to: ");
      lcd2.setCursor(10,1);   
      lcd2.print(angle1);   // print the angle
      lcd2.setCursor(13,1);    
      lcd2.print("deg");
       }
      if (angle1==0){
           digitalWrite(LED1,HIGH);
      if(distance>=d1){
           digitalWrite(relayPin1,HIGH);
       } 
 
      delay(2000);
      myServo.write(90);
      digitalWrite(LED1,LOW);
      digitalWrite(relayPin2,LOW);
      angle1 = angle3;
      angle3=angle3+angleStep3;
       }
    }// if
    value =".";
    delay(100);
  }// while for LEFT
  
  
  while(value == RIGHT){ 
    if (angle2 >= 0 && angle2 <= 180) {
      angle2 = angle2 + angleStep2;
      if(angle2 >180){
        angle2 =180;
        
      }else{
      myServo2.write(angle2); // move the servo to desired angle 
      lcd2.setCursor(0,1);   
      lcd2.print("Moved to: ");
      lcd2.setCursor(10,1);   
      lcd2.print(angle2);   // print the angle
      lcd2.setCursor(13,1);    
      lcd2.print("deg");
         }
       if (angle2==180){
            digitalWrite(LED2,HIGH);
       if(distance>=d2){
            digitalWrite(relayPin2,HIGH);
         }
      delay(2000);
      myServo2.write(90);
      digitalWrite(LED2,LOW);
      digitalWrite(relayPin1,LOW);
      angle2 = angle4;
      angle4=angle4+angleStep4;
         }
    }//if
    value =".";
    delay(100);
  }// while for RIGHT  
  
}//end of ServoAct

void ValidateCode(int cd){
   int found=0;
      for(int i=0; i< sizeof(irRemote)/sizeof(int); i++){
        if(irRemote[i] ==cd){
          lcd2.clear();
          lcd2.setCursor(0,0);
          lcd2.print("Key pressed:");
          lcd2.setCursor(13,0);
          lcd2.print(IRremoteKey[i]);
          ServoAct(IRremoteKey[i]);// take action
          found=1;
        }// if matched
      }
  if(!found){
    if(cd !=0xFFFFFFFF){
    Serial.println("Key unkown");
      }
   }// found
}// ValidateCode end
 
void Conveyor(){ //for dc motor
      lcd1.setCursor(0,0);
      lcd1.print("Distance= ");
      lcd1.setCursor(10,0);
      lcd1.print(distance);
      lcd1.setCursor(14,0);
      lcd1.print("cm");
      digitalWrite(trigPin, LOW); 
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance= (duration*0.034)/2;  
      }//end of void conveyor

void loop(){    
   Conveyor();
   if (irrecv.decode(&results)){
   ValidateCode(results.value);
    irrecv.resume(); // Receive the next value
    }   
}//loop





 




//------------------------END OF PROGRAM-----------------------
//coded by: Ariel M. Rosales
