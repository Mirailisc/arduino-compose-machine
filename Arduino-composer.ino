#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

int LEDD0 = D0; // LED 1
int LEDD1 = D1; // LED 2
int LEDD2 = D2; // LED 3

// WIFI
#define WIFI_SSID "Poolnoi Family_2.4G" 
#define WIFI_PASSWORD "0813790188"

// Firebase
#define FIREBASE_HOST "arduino-project-5c3ff-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "qxJUhhCi6iCZGEFqoyumUYGBbqLtConrKOmeecqG"
                                                       
void setup() {
  Serial.begin(9600);
  delay(1000);      

  pinMode(LEDD0, OUTPUT);  
  pinMode(LEDD1, OUTPUT);  
  pinMode(LEDD2, OUTPUT);  
                 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                      
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());
                                                       
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // Connect Arduino to Google Firebase
  Firebase.setString("status", "Stand by"); // Set status to Stand by
  Firebase.setBool("isSkip", false); // Set isSkip to false                                         
}

void loop() {
  String status = Firebase.getString("status"); // Get status data from Firebase
  bool isSkip = Firebase.getBool("isSkip"); // Get skip data from Firebase

  Serial.println(isSkip);
    
  if (status == "Stand by") {
    Serial.println("Machine is ready!");                       
    digitalWrite(LEDD0, HIGH);
    digitalWrite(LEDD1, LOW); 
    digitalWrite(LEDD2, LOW);                                                     
  } else if (status == "Working") {
    if (isSkip == false) {
      Serial.println("Machine is working!");
      digitalWrite(LEDD0, LOW);
      digitalWrite(LEDD1, HIGH); 
      digitalWrite(LEDD2, LOW);
    
      //Motor Working Clockwork
      // delay(1800000); //Working for 30 mins
      
      //Motor Working Clockwise
      // delay(1800000); //Working for 30 mins
      delay(10000);
      Firebase.setString("status", "Waiting"); // Set status to Waiting after finish Motor 
    } else if (isSkip == true) {
      Serial.println("Skipped Motor");
      delay(2000);
      Firebase.setString("status", "Waiting"); // Set status to Waiting without using a Motor
    }
    
  } else if(status == "Waiting") {
    Serial.println("Machine is waiting for water!"); 
    digitalWrite(LEDD0, LOW);
    digitalWrite(LEDD1, LOW); 
    digitalWrite(LEDD2, HIGH);   
  } else {
    Firebase.setString("status", "Stand by");
  }
}
