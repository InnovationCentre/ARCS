//ARCS software v3.0

#include <Servo.h>

Servo myservo;

int RXCH[6]; 
volatile int RXSG[6];
int RXOK[6];
int PWMSG[6];
const int Apin = 5;
int previous = 90;
int pos = 90;

void setup() {

  //Start communication to serial port
  Serial.begin(115200); 
  pinMode(Apin, OUTPUT); 

  myservo.attach(8);

  //Assign PPM input pins. The receiver output pins are conected as below to non-PWM Digital connectors:
  RXCH[1] = 4;  //Throttle
  RXCH[2] = 6;  //Aile / Yaw
  //RXCH[3] = 5;  //Elev. / Pitch
  //RXCH[4] = 2;  //Rudd. / Roll
  //RXCH[5] = 7;  //Gear
  //RXCH[6] = 8;  //Aux / Flt Mode

  for (int i = 1; i < 7; i++){
    pinMode(RXCH[i], INPUT);
  }
  pinMode(dirPin, OUTPUT);
  pinMode(stepperPin, OUTPUT);
}

void loop() {

  // Read RX values for Throttle
  for (int i = 1; i < 3; i++){                                        //for each of the channels: 
    RXSG[i] = pulseIn(RXCH[i], HIGH, 20000);                            //read the receiver signal
    if (RXSG[i] == 0) {
      RXSG[i] = RXOK[i];
    } 
    else {
      RXOK[i] = RXSG[i];
    }    //if the signal is good then use it, else use the previous signal
    PWMSG[1] = map(RXSG[1], 1060, 1980, 0, 255);                        //substitute the high values to a value between 0 and 255 for PWM
    constrain (PWMSG[1], 0, 255);                                       //make sure that the value stays within the disired boundries
    PWMSG[2] = map(RXSG[2], 1060, 1980, 0, 600);
    constrain (PWMSG[2], 0, 600);
    // Print RX values for Throttle
    Serial.print(" ||   Ch: ");
    Serial.print(2);
    Serial.print(" / PWMSG: ");
    analogWrite(Apin,PWMSG[1]);
  }

  //Servo
  if (PWMSG[1] < 100 || PWMSG [1] > 155) {
    if (PWMSG[2] > 260 && PWMSG[2] < 300) {
      PWMSG[2] = 280;
    }
    Serial.print(PWMSG[2]);
    Serial.print(" / Previous: ");
    Serial.print(previous);
    for(PWMSG[2] > 480 && pos = 90; pos <= 180; pos += 1){
      myservo.write(pos);
      delay(15)
        Serial.print("  / Step1: ");
      Serial.print(200);
    }
    for(PWMSG[2] == 280 && pos = 180; pos >= 90; pos -= 1){
      myservo.write(pos);
      delay(15)
        Serial.print("  / Step1: ");
      Serial.print(200);
    }
    for(PWMSG[2] < 80 && pos = 90; pos >= 0; pos -= 1){
      myservo.write(pos);
      delay(15)
        Serial.print("  / Step3: -");
      Serial.print(200);
      previous = 80;
    }
    for(PWMSG[2] == 280 && pos = 0; pos <= 90; pos += 1){
      myservo.write(pos);
      delay(15)
        Serial.print("  / Step4: ");
      Serial.print(200);
      previous = 280;
    }
  }

  //Serial.print(" / RXSG: ");  
  //Serial.print(RXSG[i]);
  delay(5);  


  Serial.println();
}








