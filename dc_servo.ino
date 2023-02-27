#include <Encoder.h>

#define encoderYPinA 3  // interrupt 4
#define encoderYPinB 4
#define stepYpin 2  //yellow
#define dirYpin 5   //green
#define hbridgeYpwmL 10
#define hbridgeYpwmR 11

void forewardY(void);
void backwardY(void);
void stopMotorY(void);
void pid();
void doYstep();

long oldPosition  = -999;

int motorDirectionY = 0;
int overRunLimit = 4;//margin of error in line count
int oscilationDeadzoneL = 4;
int oscilationDeadzoneR = -4;
int motorMaximumSpeedY = 255;
int motorMinimumSpeedY = 110;
long differenceY = 0;
long currentPositionY = 0;
int target_Y = 0;

Encoder myEnc(encoderYPinA, encoderYPinB);

void doYstep(){
  if (digitalRead(dirYpin) == HIGH){ 
    target_Y--;
  }else{ 
    target_Y++;
  }
}

void setup() {
  pinMode(stepYpin, INPUT);
  pinMode(dirYpin, INPUT);
  pinMode(hbridgeYpwmL, OUTPUT);
  pinMode(hbridgeYpwmR, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(stepYpin), doYstep, RISING);  // pin 2 interrupt Y step 
}

void loop(){
  currentPositionY = myEnc.read();
    
  if(target_Y == currentPositionY){
    stopMotorY();
  }else{
    differenceY = target_Y - currentPositionY;
    
    if(target_Y > currentPositionY){
      if(differenceY > oscilationDeadzoneL){
        forewardY();
      }else{
        stopMotorY();
      }
    }else{
      if(differenceY < oscilationDeadzoneR){
        backwardY();
      }else{
        stopMotorY();
      }
    }
  }  
}

void forewardY(void){
	if (motorDirectionY != 1) {
		motorDirectionY = 1;
		analogWrite(hbridgeYpwmR, 0);
	}
	
	analogWrite(hbridgeYpwmL, motorMinimumSpeedY + (differenceY/10));
}

void backwardY(void){
	if (motorDirectionY != -1) {
		motorDirectionY = -1;
		analogWrite(hbridgeYpwmL, 0);
	}

	analogWrite(hbridgeYpwmR, motorMinimumSpeedY + (differenceY/10));
}

void stopMotorY(void) {
  analogWrite(hbridgeYpwmL, 0);
  analogWrite(hbridgeYpwmR, 0);

  motorDirectionY = 0;
}

void pid(){
  currentPositionY = myEnc.read();
    
  if(target_Y == currentPositionY){
    stopMotorY();
  }else{
    differenceY = target_Y - currentPositionY;
    
    if(target_Y > currentPositionY){
      if(differenceY > oscilationDeadzoneL){
        forewardY();
      }else{
        stopMotorY();
      }
    }else{
      if(differenceY > oscilationDeadzoneR){
        backwardY();
      }else{
        stopMotorY();
      }
    }
  }
}
