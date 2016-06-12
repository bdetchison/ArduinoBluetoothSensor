
  #include <SoftwareSerial.h>


SoftwareSerial myConnection(10,11);

// Make sure these two variables are correct for your setup
int scale = 3; // 3 (±3g) for ADXL337, 200 (±200g) for ADXL377
boolean micro_is_5V = true; // Set to true if using a 5V microcontroller such as the Arduino Uno, false if using a 3.3V microcontroller, this affects the interpretation of the sensor data

//total string lenght. The first message is 25 characters long (including spaces)
int messageLength=25;
boolean ledIsOn = false;


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
  
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() {
    if (ledIsOn){
      String measurement = GetVibrationMeasurement();
      SendVibrationData(measurement);
    }
  
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
}

void SendVibrationData(String data){
  String message = "VIB:";
  String fullMessage = message + data;
  Serial.println(fullMessage);
  myConnection.println(fullMessage);
}

String GetVibrationMeasurement(){
  // Get raw accelerometer data for each axis
  int rawX = analogRead(A0);
  int rawY = analogRead(A1);
  int rawZ = analogRead(A2);
  
    //zero_G is the reading we expect from the sensor when it detects
  //no acceleration.  Subtract this value from the sensor reading to
  //get a shifted sensor reading.
  float zero_G = 512.0; 

  //scale is the number of units we expect the sensor reading to
  //change when the acceleration along an axis changes by 1G.
  //Divide the shifted sensor reading by scale to get acceleration in Gs.
  float scale = 106; 
 
  float scaledX = ((float)rawX - zero_G)/scale;
  Serial.print(scaledX);
  Serial.print("\t");
  
  float scaledY = ((float)rawY - zero_G)/scale;
  Serial.print(scaledY);
  Serial.print("\t");
  
  float scaledZ = ((float)rawZ - zero_G)/scale;
  Serial.print(scaledZ);
  Serial.print("\n");
  
  
  char temp[10];
  String scaledXasString;
  String scaledYasString;
  String scaledZasString;
  
  dtostrf(scaledX,1,4,temp);
  scaledXasString = String(temp);
 
  dtostrf(scaledY,1,4,temp);
  scaledYasString = String(temp);
 
  dtostrf(scaledZ,1,4,temp);
  scaledZasString = String(temp); 
  
  return scaledXasString + ":" + scaledYasString + ":" + scaledZasString;
}
