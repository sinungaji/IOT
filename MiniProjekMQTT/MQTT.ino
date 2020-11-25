// This example uses an ESP32 Development Board
// to connect to shiftr.io.
//
// You can check on your device after a successful
// connection here: https://shiftr.io/try.
//
// by Joël Gähwiler
// https://github.com/256dpi/arduino-mqtt

#include <WiFi.h>
#include <MQTT.h>
#include <Servo.h>
#include <HCSR04.h>

const char ssid[] = "BERSINAR"; //usernmae wifi
const char pass[] = "tanyajeki"; //password

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

#define ldr 23 //pin cahaya ldr
#define servo  21 //pin servo
#define ledJarak 27 //pin ledJarak
#define ledLDR 14 //pin ledLDR
#define ledServo 12 //pin ledServo

const int TRIG_PIN = 18; //pin trig HCSR04
const int ECHO_PIN = 19; //pin echo HCSR04
String adrLDR;
String newLDR;

Servo microservo;


void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("mqttprojek", "sinungmqtt30", "12345678")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/servo");
  client.subscribe("/jarak");
  client.subscribe("/ldr");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
  String inputservogerak = payload;
  int tampung = inputservogerak.toInt();
  int tampungservo = 0;

  //ambil nilai dari LDR dan kirim ke MQTT
  adrLDR = analogRead(ldr);
  newLDR = adrLDR;

  Serial.print("LDR: ");
  Serial.println(adrLDR);
  client.publish("ldr",newLDR);
  digitalWrite(ledLDR,LOW);
  delay(100);
  digitalWrite(ledLDR,HIGH);
  delay(100);
  
  //ambil nilai dari sensor ultarsonic dan kirim ke MQTT
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration/29/2;

  
  if(duration<=0){
   Serial.println("Warning: no pulse from sensor");
   } 
  else{
      Serial.print("Jarak:");
      Serial.println(distance);
      client.publish("jarak",newLDR);
      digitalWrite(ledJarak,LOW);
      delay(100);
      digitalWrite(ledJarak,HIGH);
      delay(100);
  }
  delay(100);

  //ambil nilai dari servo dan dapet nilai dr MQTT
  if (tampung >= tampungservo ) {
    for (int i = tampungservo ; i <= tampung; i++) {
      microservo.write(i);
      delay(25);
    }
    tampungservo = tampung;
    Serial.println("Servo: ");
    Serial.println(tampungservo);
    digitalWrite(ledServo,LOW);
    delay(100);
    digitalWrite(ledServo,HIGH);
    delay(100);
  } else if (tampung < tampungservo && tampung >= 0) {
    for (int j = tampungservo ; j >= tampung ; j--) {
      microservo.write(j);
      delay(25);
    }
    tampungservo = tampung; 
    Serial.println("Servo: ");
    Serial.println(tampungservo);
    digitalWrite(ledServo,LOW);
    delay(100);
    digitalWrite(ledServo,HIGH);
    delay(100);
  } else if (tampungservo == tampung) {
    microservo.write(tampung); //stand still
  }
  else {}
}

void setup() {
  Serial.begin(115200);
 
  pinMode(ldr,INPUT); 
  microservo.attach(servo);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ledLDR, OUTPUT);
  pinMode(ledJarak, OUTPUT);
  pinMode(ledServo, OUTPUT);

   WiFi.begin(ssid, pass);
  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

}
