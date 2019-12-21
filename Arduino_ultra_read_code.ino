#include <SoftwareSerial.h>
SoftwareSerial s(10,11);

#define trigPin1 2               //ultrasonic 1
#define echoPin1 3
#define trigPin2 4               //ultrasonic 2
#define echoPin2 5
#define trigPin3 8               //ultrasonic 3
#define echoPin3 9
#define trigPin4 6               //ultrasonic 4
#define echoPin4 7

long duration1, distance_left; // Duration used to calculate distance
long duration2, distance_front_L;
long duration3, distance_front_R;
long duration4, distance_right;

int left;
int right;
int frontL;
int frontR;
String str;

void setup() {
  s.begin(115200);
  Serial.begin(115200);
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(trigPin4, OUTPUT);
  
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  pinMode(echoPin3, INPUT);
  pinMode(echoPin4, INPUT);
  Serial.print(char(169));
}

void loop() {
 
  digitalWrite(trigPin1, HIGH);
 delay(1); 
 digitalWrite(trigPin1, LOW);
 duration1 = pulseIn(echoPin1, HIGH);

 digitalWrite(trigPin2, HIGH);
 delay(1); 
 digitalWrite(trigPin2, LOW);
 duration2 = pulseIn(echoPin2, HIGH);
 
 digitalWrite(trigPin3, HIGH);
 delay(1); 
 digitalWrite(trigPin3, LOW);
 duration3 = pulseIn(echoPin3, HIGH);

 digitalWrite(trigPin4, HIGH);
 delay(1); 
 digitalWrite(trigPin4, LOW);
 duration4 = pulseIn(echoPin4, HIGH);

 
 distance_left= duration1*0.034/2;
 distance_front_L= duration2*0.034/2;
 distance_front_R= duration3*0.034/2;
 distance_right= duration4*0.034/2;
delay(50);

if (distance_front_L > 20   )
{ 
  frontL = 0;
  delay(10);
}
else 
 { 
  frontL =1;
  delay(10);
 }
 
if (distance_front_R > 20   )
{ 
  frontR = 0;
  delay(10);
}
else 
 { 
  frontR =1;
  delay(10);
 }


 if (distance_left > 20 )
{ left = 0;
delay(10);
}
else 
 { 
  left =1;
  delay(10);
 }

 if (distance_right > 20 )
{ right = 0;
  delay(10);
}

else 
 { 
  right =1;
  delay(10);
 }

  Serial.println("=======================");
  Serial.print("frontL: ");  Serial.println(frontL);
  Serial.print("frontR: ");  Serial.println(frontR);
  Serial.print("left : ");  Serial.println(left);
  Serial.print("Right: "); Serial.println(right);
  Serial.println("=======================");
  delay(50);
//Serial.print("Distance_left:     "); Serial.println(distance_left);  //  distance = (high level timevelocity of sound (340M/S) / 2, 
//Serial.print("Distance_front_L:  "); Serial.println(distance_front_L);
//Serial.print("Distance_front_R:  "); Serial.println(distance_front_R);
//Serial.print("Distance_right:   "); Serial.println(distance_right);
  str =String(frontL)+String(",")+String(frontR)+String(",")+String(left)+String(",")+String(right)+String("*");
  Serial.println(str);
  s.println(str);
  delay(100);
  
}
