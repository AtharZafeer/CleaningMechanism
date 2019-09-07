#define trigPin 10 
#define echoPin 11
#define sig1 8
#define sig2 9
#define solenoid 6
#define pump 7
const int R[4] = {2,3,4,5};
int distancecm,i,signal1 =0, signal2=0;
unsigned long int duration;
int distance();
void cleaning(int);
int temp = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(sig1, INPUT);
  pinMode(sig2, INPUT); 
  pinMode(solenoid, OUTPUT); 
  for(i = 0;i < 4; i++) {
    pinMode(R[i],OUTPUT);
    digitalWrite(R[i],HIGH);
   }
  pinMode(pump, OUTPUT);
  digitalWrite(solenoid, HIGH);
  digitalWrite(pump, HIGH);                               
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  signal1 = digitalRead(sig1);
  delay(100);
  Serial.println("signal1");
  Serial.println(signal1);
  signal2 = digitalRead(sig2);
  delay(100);
  Serial.println("signal2");
  Serial.println(signal2);
  if(signal1 == HIGH && signal2 == LOW) {
      Serial.println("I am in signal1");
      digitalWrite(pump, LOW);
      delay(3000);
      digitalWrite(solenoid, LOW);
      delay(1000);
      cleaning(2);
        digitalWrite(pump, HIGH);
      delay(1000);
      digitalWrite(solenoid, HIGH);
      signal1 = LOW;
      }
   if(signal1 == LOW && signal2 == HIGH) {
      Serial.println("I am  in signal2");
      
      cleaning(2);
      signal2 = LOW;
      digitalWrite(solenoid, HIGH);
      digitalWrite(pump, HIGH);
    }
}

int distance() {
    distancecm = 0;
    duration = 0;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distancecm = ((int)duration *0.034)/2;
    Serial.println(distancecm);
    return distancecm;
}

void forwards() {
    digitalWrite(R[3],HIGH);
    digitalWrite(R[2], LOW);
    digitalWrite(R[0],LOW);
    digitalWrite(R[1], HIGH);
  }

void reverse() {
    digitalWrite(R[0], HIGH);
    digitalWrite(R[1], LOW);
    digitalWrite(R[2],HIGH);
    digitalWrite(R[3], LOW); 
  }

void stopfunc() {    
    digitalWrite(R[0],HIGH);
    digitalWrite(R[1], HIGH);
    digitalWrite(R[2],HIGH);
    digitalWrite(R[3], HIGH);    
  } 

void cleaning (int panel) {
    do {
      forwards();
      temp = distance();
      delay(50);
    }while(!(temp >= panel-1 && temp <= panel+1) ); 
    stopfunc();
    delay(1000);
    do{
      reverse(); 
      temp = distance();
      delay(50);
     }while(!(temp >=34 && temp <=36 ));
     stopfunc();
    
 }
  
