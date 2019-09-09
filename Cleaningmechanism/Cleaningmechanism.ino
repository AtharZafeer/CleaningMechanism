#define trigPin 10 //for ultrasonic sensor  (trig pin of ultrasonic sensor to arduino pin 10)
#define echoPin 11  //for ultrasonic sensor (echo pin of ultrasonic sensor to arduino pin 11)
#define sig1 8    //signal pin from the resistor circuit(wattman signal 12v-5v converter) to pin 8 and is for cleaning one panel
#define sig2 9    //signal pin from the resistor circuit(wattman signal 12v-5v converter) to pin 9 and is for cleaning both panels
#define solenoid 6  //arduino pin 6 to  relay pin in5 for solenoid
#define pump 7      //arduino pin 7 to relay pin in6 for pump
const int R[4] = {2,3,4,5}; //arduino pin 2,3,4,5 to relay pin in1,in2,in3,in4 respectively for motors
int distancecm,i,signal1 =0, signal2=0;  //for distance and signal recording
unsigned long int duration; //this stores the duration of the pulse
int distance();   //function declaration of the distance function which returns distance when called
void cleaning(int); //this is the cleaning function, this is called to start the cleaning
int temp = 0;  
void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);     //trig is output as it outputs a pulse
  pinMode(echoPin,INPUT);   //echopin is input as it measures the duration of the return pulse
  pinMode(sig1, INPUT);     // signal1 is the input signal from wattmon(only wattmon not particle)
  pinMode(sig2, INPUT);     //signal1 is the input signal from wattmon(only wattmon not particle)
  pinMode(solenoid, OUTPUT);  //solenoid is output for relay pin
  for(i = 0;i < 4; i++) {     //loop to initialize the relay pins that controls the relay
    pinMode(R[i],OUTPUT);       
    digitalWrite(R[i],HIGH);  //relay use active low to switch on and active high to switch off
   }
  pinMode(pump, OUTPUT);    //pump is output for relay pin
  digitalWrite(solenoid, HIGH); //Solenoid pin OFF
  digitalWrite(pump, HIGH);     //Solenoid pin OFF        
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  signal1 = digitalRead(sig1);    //looks for signal 1 from wattmon
  delay(100);
  //Serial.println("signal1");     
  //Serial.println(signal1);
  signal2 = digitalRead(sig2); // looks for signal 2 from wattman
  /*It is to be noted that there is no delay between two signals for detecting them, so only signal must be given from 
  wattmon at a time*/
  
  delay(100);  
  //Serial.println("signal2");
  //Serial.println(signal2);
  if(signal1 == HIGH && signal2 == LOW) {   //if statement that proceeds only when signal 1 is high (signal1 from wattmon)
    //  Serial.println("I am in signal1");
    
    cleaning(2);  //the value two is minimum distance that the bot will go near the reference point
    /*passing the value of 2 mean, its will go in forward direction until the distance between bot and the reference point is 2 cm */
    
    signal1 = LOW; //make the signal low after cleaning is done, so that it doesn't trigger again
    
    delay(2000); //delay of 2 seconds so that loop rest and wait for the wattmon to cut the signal off (wattmon is slow)
      }
   if(signal1 == LOW && signal2 == HIGH) {   //if statement that proceeds only when signal 2 is high (signal1 from wattmon)
      //Serial.println("I am  in signal2");
      
      cleaning(2); //signal 2 is for cleaning both the panels
      
      signal2 = LOW; //make the signal low after cleaning is done so that it doesnt trigger cleaning again
      delay(2000); //delay of 2 seconds so that loop rest and wait for the wattmon to cut the signal off (wattmon is slow)
    }
}

int distance() {      //this function return distance is uses ultrasound sensor (this is the eyes of the bot)
    distancecm = 0;     //initialization
    duration = 0;       //" "
    digitalWrite(trigPin, LOW); //trigPin is held LOW for 4 microsecond
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH); //then HIGH for 10 microseconds
    delayMicroseconds(10);    
    digitalWrite(trigPin, LOW); //then it is pulled LOW 
    /* Doing the above, gives the sensor the signal to send the ultrasound wave (this is as per the data sheet of the sensor
    */
    duration = pulseIn(echoPin, HIGH);  //this will measure the pulse duration from the echo pin
    /*Once the signal that is sent is received by the sensor, the echo pin will go high for a duration. This duration will depend 
    on the time taken by the ultrasound signal sent by the sensor to reach back after getting reflected from a object. So, measuring this duration
    gives us the time taken by the ultrasound to go, hit the object and comeback to the sensor*/
    distancecm = ((int)duration *0.034)/2;  //distance can be calulated from time and speed of sound
    //Serial.println(distancecm);
    return distancecm;    //returns the calculated distance
}

void forwards() { //this function will move the motor forward
    digitalWrite(R[3],HIGH);  
    digitalWrite(R[2], LOW);
    digitalWrite(R[0],LOW);
    digitalWrite(R[1], HIGH);
  }

void reverse() {  //this function will move the motor backward
    digitalWrite(R[0], HIGH);
    digitalWrite(R[1], LOW);
    digitalWrite(R[2],HIGH);
    digitalWrite(R[3], LOW); 
  }

void stopfunc() { //this function stops the motor
    digitalWrite(R[0],HIGH);
    digitalWrite(R[1], HIGH);
    digitalWrite(R[2],HIGH);
    digitalWrite(R[3], HIGH);    
  } 
/*
  the below cleaning functions does the following the the order
      It will switch on the pump
      wait for 3 seconds
      it will switch on the solenoid

      now, first, it will move forward, 
          it will check the distance()
          if the distance is greater than or equal to passed value
          (lets say the passed value is 2cm), the bot will go forward until the distance is 2 cm
          then the bot will stop for 1 second
          now the bot will start to go in reverse,(lets say the end distance is 100cm from the reference block),
          it will go reverse until it reached the 100 cm
      then it switches of the solenoid and pumps 
          
*/
void cleaning (int panel) { //this function is the heart of the bot
    digitalWrite(pump, LOW);    //this switches on the pump 
    delay(3000);    //waits for 3 seconds so that water starts pumping
    digitalWrite(solenoid, LOW);  //then switches on the solenoid
    delay(1000);  //after one second delay it start the cleaning
    do {
      forwards();
      temp = distance();
      delay(20);
    }while(!(temp >= panel-1 && temp <= panel+1) ); 
    stopfunc();
    delay(1000);
    do{
      reverse(); 
      temp = distance();
      delay(20);
     }while(!(temp >=34 && temp <=36 ));
     stopfunc();
    digitalWrite(pump, HIGH); // switch off pump after cleaning
    delay(1000); //
    digitalWrite(solenoid, HIGH);
    
 }
  
