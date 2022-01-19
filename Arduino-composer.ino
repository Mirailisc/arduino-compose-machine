#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <AccelStepper.h>

#define LEDD0 D0 // LED 1
#define LEDD1 D1 // LED 2
#define LEDD2 D2 // LED 3

// WIFI
#define WIFI_SSID "Poolnoi Family_2.4G"
#define WIFI_PASSWORD "0813790188"

// Firebase
#define FIREBASE_HOST "arduino-project-5c3ff-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "qxJUhhCi6iCZGEFqoyumUYGBbqLtConrKOmeecqG"


//Stepper Motor
#define IN1 D5 // IN1 is connected to NodeMCU pin D1 (GPIO5)
#define IN2 D6 // IN2 is connected to NodeMCU pin D2 (GPIO4)
#define IN3 D7 // IN3 is connected to NodeMCU pin D3 (GPIO0)
#define IN4 D8  // IN4 is connected to NodeMCU pin D4 (GPIO2)

const int stepsPerRevolution = 2048;
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  //LED
  pinMode(LEDD0, OUTPUT);
  pinMode(LEDD1, OUTPUT);
  pinMode(LEDD2, OUTPUT);

  //Wifi
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

  //Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // Connect Arduino to Google Firebase
  Serial.println("Connected to Firebase");
  Firebase.setString("status", "Ready"); // Set status to Stand by
  Firebase.setBool("isSkip", false); // Set isSkip to false
}

void loop() {
  String status = Firebase.getString("status"); // Get status data from Firebase
  bool isSkip = Firebase.getBool("isSkip"); // Get skip data from Firebase
  int Times = 0;

  if (status == "Ready") {
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
      // check current stepper motor position to invert direction
//      while(Times < 10) {
//        
//        if (stepper.distanceToGo() == 0){
//          stepper.moveTo(-stepper.currentPosition());
//          Serial.println("Changing direction");
//        }
//        // move the stepper motor (one step at a time)
//        stepper.run();
//        Times += 1;
//        Serial.println(Times);
//      }
      Firebase.setString("status", "Need Water"); // Set status to Waiting after finish Motor
    } else if (isSkip == true) {
      Serial.println("Skipped Motor");
      delay(2000);
      Firebase.setString("status", "Need Water"); // Set status to Waiting without using a Motor
    }

  } else if (status == "Need Water") {
    Serial.println("Machine is waiting for water!");
    digitalWrite(LEDD0, LOW);
    digitalWrite(LEDD1, LOW);
    digitalWrite(LEDD2, HIGH);
  } else {
    Firebase.setString("status", "Ready"); // Set status back to Ready when status is not in condition
  }
}
