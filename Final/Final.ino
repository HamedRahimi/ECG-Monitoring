#include "SoftwareSerial.h"
const int num = 100;
const int numtoint = 20;
String data[num / numtoint] ;
byte ECG_PULSE[num] = {0};

String ssid = "shirazunifi";
String password = "shirazunifi#1395";

String server = "appgozar.com"; // www.example.com
String uri = "/a.php";// our example is /esppost.php


SoftwareSerial esp(7, 6); // RX, TX

void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  esp.begin(115200);
 // reset();
 // connectWifi();
  pinMode(10, INPUT); // Setup for leads off detection LO +
  pinMode(11, INPUT); // Setup for leads off detection LO -

}

void loop() {

  Sampling();
  Quantizing();
/*
  for (int k = 0; k < num / numtoint; k++)
  {
    httppost (data[k]);
    delay(200);
  }
*/
  delay(5000);
}

////////////////////////////////////////////////////////////////////
/////////////////////////////Reseting///////////////////////////////
////////////////////////////////////////////////////////////////////
void reset() {

  esp.println("AT+RST");
  delay(1000);
  Serial.println("Module Reset");
}
//////////////////////////////////////////////////////////////////////////
///////////////////////Connecting/////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void connectWifi() {
  String cmd = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\"";
  esp.println(cmd);
  delay(4000);
  Serial.println("wifi connected");
}
///////////////////////////////////////////////////////////////////////////
////////////////////////////////Posting////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
void httppost (String Data) {
  esp.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.
  delay(100);
  String postRequest =
    "POST " + uri + " HTTP/1.0\r\n" +
    "Host: " + server + "\r\n" +
    "Accept: *" + "/" + "*\r\n" +
    "Content-Length: " + Data.length() + "\r\n" +
    "Content-Type: application/x-www-form-urlencoded\r\n" +
    "\r\n" + Data;
  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.
  esp.print(sendCmd);
  esp.println(postRequest.length() );
  delay(500);
  Serial.println("Sending..");
  esp.print(postRequest);
  Serial.println(postRequest);
  Serial.println("Packet sent");
  delay(1500);
  // close the connection
  esp.println("AT+CIPCLOSE");
}
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////Sampling////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void Sampling() {
  for (int i = 0; i < num; i++) {
    if ((digitalRead(10) == 1) || (digitalRead(11) == 1)) {
      ///Serial.println(255);
      ECG_PULSE[i] = 123;
    }
    else {
      //Serial.println(analogRead(A0)/2);
      ECG_PULSE[i] = analogRead(A0) / 2;
    }
    delay(10);
  }
}
/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////Quantize//////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void Quantizing() {
  // String data[(num/numtoint)];
  for (int j = 0; j < (num / numtoint) ; j++) {
    data[j] = "a"+String(j)+"=";
    for (int i = 0; i < numtoint; i++) {
      data[j] = data[j] + "E" + String(ECG_PULSE[i + (j * (num / numtoint))]);
    }
    Serial.println(data[j]);
  }
}
