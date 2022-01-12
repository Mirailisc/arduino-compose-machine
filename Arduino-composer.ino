#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

int LEDD0 = D0; // ขา D0
int LEDD1 = D1; // ขา D1
int LEDD2 = D2; // ขา D2

// WIFI
#define WIFI_SSID "ssid" 
#define WIFI_PASSWORD "password"

// Firebase
#define FIREBASE_HOST "host"
#define FIREBASE_AUTH "auth"
                                                       
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
                                                       
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                                         
}

void loop() {
  loadStatus();
}

void loadStatus() {
  String status = Firebase.getString("status");  
  if (status == "Standby") {
    Serial.println("Machine is ready!");                       
    digitalWrite(LEDD0, HIGH);
    digitalWrite(LEDD1, LOW); 
    digitalWrite(LEDD2, LOW);                                                     
  }
  if (status == "Working") {
    Serial.println("Machine is working!");
    digitalWrite(LEDD0, LOW);
    digitalWrite(LEDD1, HIGH); 
    digitalWrite(LEDD2, LOW);
    //Motor Working...
    delay(10000); 
    Firebase.setString("status", "Waiting");                                                   
  }
  if(status == "Waiting") {
    Serial.println("Machine is waiting for water!"); 
    digitalWrite(LEDD0, LOW);
    digitalWrite(LEDD1, LOW); 
    digitalWrite(LEDD2, HIGH);   
  }
}
