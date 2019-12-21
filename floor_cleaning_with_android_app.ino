

#include <ESP8266WiFi.h>

WiFiClient client;
WiFiServer server(80);

#include <SoftwareSerial.h>
SoftwareSerial s(D7, D8);



int IRPin = D4; // declaring pin A0 to read digital input

int value; //save the digital value sent by ir led

/* WIFI settings */
const char* ssid = "iPhone";
const char* password = "12345678";

/* data received from application */
String  data = "";

int L298N_A_In1_pin = D5;
int L298N_A_In2_pin = D6;

int L298N_B_In3_pin = D3;
int L298N_B_In4_pin = D2;


int relay_pin1 = D1;
int relay_pin2 =  D0;

int Auto_Mode = 0;

String readString; //main captured String

String frontL; //data String
String frontR;
String left;
String right;

int ind1; // , locations
int ind2;
int ind3;
int ind4;

int F_L;
int F_R;
int L;
int R;




void connectWiFi()
{
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    delay(300);
    Serial.print("..");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("NodeMCU Local IP is : ");
  Serial.print((WiFi.localIP()));
  delay(5000);
}
void setup()
{
  Serial.begin(115200);
  pinMode(IRPin, INPUT);
  s.begin(115200);
  pinMode(relay_pin1, OUTPUT);
  pinMode(relay_pin2, OUTPUT);
  digitalWrite(relay_pin1, HIGH);
  digitalWrite(relay_pin2, HIGH);

  pinMode(L298N_A_In1_pin, OUTPUT);
  pinMode(L298N_A_In2_pin, OUTPUT);
  pinMode(L298N_B_In3_pin, OUTPUT);
  pinMode(L298N_B_In4_pin, OUTPUT);

  digitalWrite(L298N_A_In1_pin, LOW);
  digitalWrite(L298N_A_In2_pin, LOW);
  digitalWrite(L298N_B_In3_pin, LOW);
  digitalWrite(L298N_B_In4_pin, LOW);

  /* start server communication */
  connectWiFi();
  server.begin();
}


void loop()
{

  if (Auto_Mode == 1 )
  {

    if (s.available()) {
      char c = s.read(); //gets one byte from serial buffer
      if (c == '*') {
        //do stuff
        //Serial.print("Recieveing String is :");
        //Serial.print(readString); //prints string to serial port out

        ind1 = readString.indexOf(',');                 //finds location of first ,
        frontL = readString.substring(0, ind1);        //captures first data String
        ind2 = readString.indexOf(',', ind1 + 1 );     //finds location of second ,
        frontR = readString.substring(ind1 + 1, ind2 + 1); //captures second data String
        ind3 = readString.indexOf(',', ind2 + 1 );
        left = readString.substring(ind2 + 1, ind3 + 1);
        ind4 = readString.indexOf(',', ind3 + 1 );
        right = readString.substring(ind3 + 1); //captures remain part of data after last ,

        F_L = frontL.toInt();
        F_R = frontR.toInt();
        L   = left.toInt();
        R   = right.toInt();

        Serial.print(F_L);
        Serial.print("   ");
        Serial.print(F_R);
        Serial.print("   ");
        Serial.print(L);
        Serial.print("   ");
        Serial.print(R);
        Serial.print("   ");
        Serial.println();
        delay(10);

        if ( F_L == 0 && F_R == 0 && L == 0  && R == 0 )
        {
          Serial.println("Forward");
          digitalWrite(L298N_A_In1_pin, LOW);
          digitalWrite(L298N_A_In2_pin, HIGH);
          digitalWrite(L298N_B_In3_pin, LOW);
          digitalWrite(L298N_B_In4_pin, HIGH);
        }

        if ( F_L == 0 && F_R == 0 && L == 1 && R == 1 )
        {
          Serial.println("Forward");
          digitalWrite(L298N_A_In1_pin, LOW);
          digitalWrite(L298N_A_In2_pin, HIGH);
          digitalWrite(L298N_B_In3_pin, LOW);
          digitalWrite(L298N_B_In4_pin, HIGH);
        }

        if ( F_L == 1 && F_R == 1 && L == 0 && R == 0 )
        {
          Serial.println("right");
          digitalWrite(L298N_A_In1_pin, HIGH);
          digitalWrite(L298N_A_In2_pin, LOW);
          digitalWrite(L298N_B_In3_pin, LOW);
          digitalWrite(L298N_B_In4_pin, HIGH);
          delay(100);
        }

        if ( F_L == 1 && F_R == 1 && L == 1 && R == 0 )
        {
          Serial.println("right");
          digitalWrite(L298N_A_In1_pin, HIGH);
          digitalWrite(L298N_A_In2_pin, LOW);
          digitalWrite(L298N_B_In3_pin, LOW);
          digitalWrite(L298N_B_In4_pin, HIGH);
          delay(5000);
        }

        if ( F_L == 1 && F_R == 1 && L == 0 && R == 1 )
        {
          Serial.println("left");
          digitalWrite(L298N_A_In1_pin, LOW);
          digitalWrite(L298N_A_In2_pin, HIGH);
          digitalWrite(L298N_B_In3_pin, HIGH);
          digitalWrite(L298N_B_In4_pin, LOW);
          delay(5000);
        }

        if ( F_L == 1 && F_R == 1 && L == 1 && R == 1 )
        {
          Serial.println("Backward");
          digitalWrite(L298N_A_In1_pin, HIGH);
          digitalWrite(L298N_A_In2_pin, LOW);
          digitalWrite(L298N_B_In3_pin, HIGH);
          digitalWrite(L298N_B_In4_pin, LOW);
          delay(5000);
        }


        readString = ""; //clears variable for new input
        frontL = "";
        frontR = "";
        left = "";
        right = "";

      }
      else {
        readString += c; //makes the string readString
      }
    }
  }





  floorcheck();
  /* If the server available, run the "checkClient" function */
  client = server.available();
  if (!client) return;
  data = checkClient ();

  /************************ Run function according to incoming data from application *************************/

  if      (data == "vaccumon")        VaccumOn();
  else if (data == "vaccumoff")       VaccumOff();
  else if (data == "moppingon")  MoppingOn();
  else if (data == "moppingoff") MoppingOff();
  else if (data == "automatic")  Auto();
  else if (data == "manual")     Manual();




  if (Auto_Mode == 0)
  {
    if      (data == "forward")    MotorForward();
    else if (data == "backward")   MotorBackward();
    else if (data == "left")       TurnLeft();
    else if (data == "right")      TurnRight();
    else if (data == "stop")      MotorStop();
  }


}

void Auto(void)
{
  delay(100);
  Serial.println("Automatic");
  Auto_Mode = 1;

}


void Manual(void)
{
  Auto_Mode = 0;
  delay(100);
  Serial.println("Manual");
}







/********************************** RECEIVE DATA FROM the APP ******************************************/
String checkClient (void)
{
  while (!client.available()) delay(1);
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length() - 9, 9);
  return request;
}


void floorcheck()
{
  value = digitalRead(IRPin);         //Read and save digital value from ir sensor
  // Serial.println(value);               //Print value

  if (value == 0)
  {
    //  Serial.println("Floor detected");

  }
  else
  {
    Serial.println("Floor not detected");
    MotorStop();
  }
}


/********************************************* BACKWARD *****************************************************/
void MotorBackward(void)
{
  Serial.println("Backward");
  digitalWrite(L298N_A_In1_pin, HIGH);
  digitalWrite(L298N_A_In2_pin, LOW);
  digitalWrite(L298N_B_In3_pin, HIGH);
  digitalWrite(L298N_B_In4_pin, LOW);

}

/********************************************* TURN LEFT *****************************************************/
void TurnLeft(void)
{
  Serial.println("left");
  digitalWrite(L298N_A_In1_pin, HIGH);
  digitalWrite(L298N_A_In2_pin, LOW);
  digitalWrite(L298N_B_In3_pin, LOW);
  digitalWrite(L298N_B_In4_pin, HIGH);
}

/********************************************* TURN RIGHT *****************************************************/
void TurnRight(void)
{
  Serial.println("right");
  digitalWrite(L298N_A_In1_pin, LOW);
  digitalWrite(L298N_A_In2_pin, HIGH);
  digitalWrite(L298N_B_In3_pin, HIGH);
  digitalWrite(L298N_B_In4_pin, LOW);

}

/********************************************* FORWARD *****************************************************/
void MotorForward(void)
{
  Serial.println("Forward");
  digitalWrite(L298N_A_In1_pin, LOW);
  digitalWrite(L298N_A_In2_pin, HIGH);
  digitalWrite(L298N_B_In3_pin, LOW);
  digitalWrite(L298N_B_In4_pin, HIGH);
}

/********************************************* STOP *****************************************************/
void MotorStop(void)
{
  Serial.println("Stop");
  digitalWrite(L298N_A_In1_pin, LOW);
  digitalWrite(L298N_A_In2_pin, LOW);
  digitalWrite(L298N_B_In3_pin, LOW);
  digitalWrite(L298N_B_In4_pin, LOW);

}



void VaccumOn(void)
{
  Serial.println("Vaccume on");
  digitalWrite(relay_pin1, LOW);
}
void VaccumOff(void)
{
  Serial.println("Vaccume off");
  digitalWrite(relay_pin1, HIGH);

}

void MoppingOn(void)
{
  Serial.println("Mopping on");
  digitalWrite(relay_pin2, LOW);

}
void MoppingOff(void)
{
  Serial.println("Mopping off");
  digitalWrite(relay_pin2, HIGH);
}
