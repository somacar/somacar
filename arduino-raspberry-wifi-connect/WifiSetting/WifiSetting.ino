#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11);

char BLEMessage;
int i;
String data = "";
String APData = "";
bool ReceiveMode = false;
bool ReceiveComplete_AP = true;

// -- Send usage
// recommands send length 10 > text 
// 1. Set AP Name
// send : ap_set_your-AP_
// 2. Set your AP Password
// send : your-pass^

// example)
// ap_set_exampleAP_    // Send
// password^            // Send 

String getValue(String data, char separator, int index)
{
 int found = 0;
  int strIndex[] = {0, -1  };
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
  if(data.charAt(i)==separator || i==maxIndex){
  found++;
  strIndex[0] = strIndex[1]+1;
  strIndex[1] = (i == maxIndex) ? i+1 : i;
  }
 }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup() {
  pinMode(13, OUTPUT);

  BT.begin(9600);
  Serial.begin(9600);
  BT.println("[BT] Started bluetooth communication");
  BT.println("[BT] Init completed");
  data = "";
  APData = "";
  ReceiveMode = false;
  ReceiveComplete_AP = true;
}

void loop() {
  
  if (Serial.available()) {
    r_funclist(Serial.read() - '0');
  }
  if(BT.available()) {
    BLEMessage = (BT.read());
    //Serial.println(BLEMessage);
    
    if(BLEMessage == '%') {
      data = "";
      APData = "";
      ReceiveMode = false;
      ReceiveComplete_AP = true;
      BT.println("[BT] Init completed");
    }
    
    if (ReceiveComplete_AP == false) {
      setAP(BLEMessage);  
    }
    
    if (BLEMessage == 'a' || ReceiveMode == true) {
      //Serial.println("detected " + a);
      data = data + BLEMessage;
      if (data == "ap_set_") {
        // ap_set_RainC4Lab_password
        // ap_set_<APName>_<APPassword>^
        BT.println("ap set process( ^=fin ) ");
        ReceiveComplete_AP = false;
        
      }
      ReceiveMode = true;
    }
    
    if (BLEMessage == '^') { // End of Message
      ReceiveComplete_AP = true;
      
      String AP = getValue(APData, '_', 0);
      String PW_str = getValue(APData, '_', 1);
      String PW = PW_str.substring(0, PW_str.length() - 1);
      char char_APName[20];
      char char_APPass[20];
      AP.toCharArray(char_APName, AP.length() + 1);
      PW.toCharArray(char_APPass, PW.length() + 1);
      BT.println("APName : " + AP);
      BT.println("AP Password : " + PW);
      Serial.println("AP_SEND");
      Serial.println(AP + "/" + PW);
      // send to Raspberry Pi
      
      APData = "";
      data = "";
      ReceiveMode = false;
      ReceiveComplete_AP = false;
    }
  }
}

void r_funclist(int number) {
   switch (number) {
      case 0:
        Serial.println("Received 0 from Raspi");
        break;
      case 1:
        BT.println("[BT] Wi-Fi Connected");
        break;
      case 2:
        BT.println("[BT] Failed to Conenct");
   }
}

void setAP(char onechar) { 
  APData = APData + onechar;
}
