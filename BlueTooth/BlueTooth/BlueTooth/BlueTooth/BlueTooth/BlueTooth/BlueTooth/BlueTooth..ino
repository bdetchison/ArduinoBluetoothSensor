
#include <SoftwareSerial.h>

SoftwareSerial myConnection(10,11);

// Make sure these two variables are correct for your setup
int scale = 3; // 3 (±3g) for ADXL337, 200 (±200g) for ADXL377
boolean micro_is_5V = true; // Set to true if using a 5V microcontroller such as the Arduino Uno, false if using a 3.3V microcontroller, this affects the interpretation of the sensor data

//total string lenght. The first message is 25 characters long (including spaces)
int messageLength=25;
boolean ledIsOn = false;


// Accelerometer calibrate
const int XPin = A0;
const int YPin = A1;
const int ZPin = A2;

// Raw analog values for -1g and +1g.  Position carefully and look for min and max values.
// These are from an accelerometer running on 3.3V measured agains a 5V analog reference.
// You would expect (3.3V/2-0.33V)*1024/5V = 270 for -1g and (3.3V/2+0.33V)*1024/5V = 405 for +1g.
// As you can see, my accelerometer does not match the nominal values.
const int XMin = 422;
const int XMax = 655;
const int YMin = 452;
const int YMax = 657;
const int ZMin = 452;
const int ZMax = 656;

char incomingByte; //incoming data from the BT link


// the two pixels arrays
char Buffer[200];   
char Buffer_inverse[200]; 

// the message array
char Command[50];
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myConnection.begin(9600);
  
  analogReference(EXTERNAL);
  
  pinMode(13, OUTPUT);
 // pinMode(A0, INPUT);
 // pinMode(A1, INPUT);
 // pinMode(A2, INPUT);
  digitalWrite(13, LOW);
}

void loop() {
    if (myConnection.available() > 0) {
        //myConnection.println("Connection is avail");
        messageLength = myConnection.available(); //incoming string lenght

        for  (int i = 0; i < messageLength; i++) {

            // read the incoming byte:
            incomingByte = myConnection.read();

            Command[i] = incomingByte; ///store the character into the string array

        }

        //debug features, to check :
        Serial.print("Command: ");
        Serial.println(Command);
        Serial.print("Length : ");
        Serial.println(messageLength);
        
        
        if (Command[0] =='1'){
          Serial.println("led: ON");
          digitalWrite(13,HIGH);
          ledIsOn = true;
        }
        else if (Command[0]=='0'){
          digitalWrite(13,LOW);
          Serial.println("led: OFF");
          ledIsOn = false;
          myConnection.println("Done");
        }
    }
    
     if (ledIsOn){
      String measurement = GetVibrationMeasurement();
      SendVibrationData(measurement);
      delay(500);
    }
}

void SendVibrationData(String data){
  String message = "VIB:";
  String fullMessage = message + data;
  Serial.println(fullMessage);
  myConnection.println(fullMessage);
}

String GetVibrationMeasurement(){
  unsigned long XSum = 0;
  unsigned long YSum = 0;
  unsigned long ZSum = 0;
  
  Serial.print("X raw=");
  Serial.print(analogRead(XPin));
  Serial.print("Y raw=");
  Serial.print(analogRead(YPin));
  Serial.print("Z raw=");
  Serial.print(analogRead(ZPin));
  
  for (int i=0; i<100; i++) {
    XSum += analogRead(XPin);
    YSum += analogRead(YPin);
    ZSum += analogRead(ZPin);
  }
  
  String scaledXasString;
  String scaledYasString;
  String scaledZasString;
  
  char temp[10];
  float scaledX = map(XSum/100, XMin, XMax, -1000, 1000);
  float scaledY = map(YSum/100, YMin, YMax, -1000, 1000);
  float scaledZ = map(ZSum/100, ZMin, ZMax, -1000, 1000);
  
  dtostrf(scaledX / 1000,1,4,temp);
  scaledXasString = String(temp);
 
  dtostrf(scaledY / 1000,1,4,temp);
  scaledYasString = String(temp);
 
  dtostrf(scaledZ / 1000,1,4,temp);
  scaledZasString = String(temp); 
  
  Serial.print("X=");
  Serial.print(scaledXasString);
    
  Serial.print(", Y=");
  Serial.print(scaledYasString);

  Serial.print(", Z=");
  Serial.println(scaledZasString);
  
  return scaledXasString + ":" + scaledYasString + ":" + scaledZasString;
}
