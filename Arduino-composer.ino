#include <IOXhop_FirebaseESP32.h>
#include <WiFi.h>
#include <Stepper.h>

#define LED1 23
#define LED2 22
#define LED3 21

// WIFI
#define WIFI_SSID "Poolnoi Family_2.4G"
#define WIFI_PASSWORD "0813790188"

// Firebase
#define FIREBASE_HOST "arduino-project-5c3ff-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "qxJUhhCi6iCZGEFqoyumUYGBbqLtConrKOmeecqG"


//Stepper Motor
#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 17

const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  //LED
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

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

  //Stepper
  myStepper.setSpeed(15 );
}

void loop() {
  String status = Firebase.getString("status"); // Get status data from Firebase
  bool isSkip = Firebase.getBool("isSkip"); // Get skip data from Firebase

  if (status == "Ready") {
    Serial.println("Machine is ready!");
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
  } else if (status == "Working") {
    if (isSkip == false) {
      Serial.println("Machine is working!");
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      // check current stepper motor position to invert direction

      for (int i = 0; i <= 10; i++) {
        Serial.println("clockwise");
        myStepper.step(stepsPerRevolution);
        for (int i = 0; i <= 1000; i++) {
          delay(10);
        }
      
        // step one revolution in the other direction:
        Serial.println("counterclockwise");
        myStepper.step(-stepsPerRevolution);
        for (int i = 0; i <= 1000; i++) {
          delay(10);
        }
      }
      Firebase.setString("status", "Need Water");
    } else if (isSkip == true) {
      Serial.println("Skipped Motor");
      delay(2000);
      Firebase.setString("status", "Need Water"); // Set status to Waiting without using a Motor
    }

  } else if (status == "Need Water") {
    Serial.println("Machine is waiting for water!");
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
  } else {
    Firebase.setString("status", "Ready"); // Set status back to Ready when status is not in condition
  }
}
