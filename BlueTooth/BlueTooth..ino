
  #include <SoftwareSerial.h>

int ledPin = 13;
int state = 0;
int flag=0;


SoftwareSerial myConnection(10,11);

// the delay between each screen display (ms)
int SpeedDelay = 12;


//total string lenght. The first message is 25 characters long (including spaces)
int messageLength=25;


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
        }
        else if (Command[0]=='0'){
          digitalWrite(13,LOW);
          Serial.println("led: OFF");
        }
        
        myConnection.println("Command Recieved");
        
    }
}
