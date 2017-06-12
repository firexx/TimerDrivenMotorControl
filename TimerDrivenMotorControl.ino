#include <TimerOne.h>

int led = 11;

unsigned long currentPeriod ; // current period for call interrupt function
long deltaPeriod ; // linear acceleration this value will be substrated from current period every second call of interrupt
unsigned long minPeriod = 20000; // minimal allowed period (maximal speed)
unsigned long maxPeriod = 200000; // maximal allowed period (start and stop speed)
unsigned long cntStep; // steps to count down to zero. at zero a motor stops. 
                       // cntStep = preSteps initialized the liniar breaking of motor
                       // be sure the interrupts are disabled in the time of setting of this variable
unsigned long preSteps; // steps nees to accelarate and decelerate motor to/from maximal speed

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  digitalWrite(led,LOW);
  Serial.begin(115200);
}

int ledState;
void myTimerInterrupt(){
  ledState = digitalRead(led);
  
  if (ledState)
    digitalWrite(led,LOW);
  else
    digitalWrite(led,HIGH);
  
  if (ledState == 1){
    Timer1.detachInterrupt();
    cntStep--;
    
    // end of actions
    if (cntStep == 0)
      return; 
    
    
    // change the acceleration direction
    if (cntStep <= preSteps && deltaPeriod > 0)
      deltaPeriod= - deltaPeriod;
     
    currentPeriod -= deltaPeriod;
    
    // check for maximal speed range
    if (currentPeriod < minPeriod)
      currentPeriod = minPeriod;
    
    Timer1.initialize(currentPeriod);
    Timer1.attachInterrupt(myTimerInterrupt);
  }
}

void motor(unsigned int steps){
  Serial.println("start");
  currentPeriod = maxPeriod;
  deltaPeriod = 10000;
  cntStep = steps;
  unsigned long stepsToMaxSpeed = (unsigned long)((maxPeriod - minPeriod) / deltaPeriod);
  if (stepsToMaxSpeed * 2 > steps )
    preSteps = (unsigned long)(steps / 2) ;
  else
    preSteps = stepsToMaxSpeed;

  Timer1.initialize(currentPeriod);
  Timer1.attachInterrupt(myTimerInterrupt);
}

void loop() {
  motor(100);
  while(cntStep){
    //Serial.print(preSteps); Serial.print(" "); 
    //Serial.print(cntStep); Serial.print(" "); 
    //Serial.println(currentPeriod);
    delay((unsigned long)(currentPeriod/500));
  }
}

